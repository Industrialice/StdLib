#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FileIO.hpp"
#include "Files.hpp"

CError<> StdLib_FileError();  //  from FilesWindows.cpp

namespace StdLib
{
    namespace FileIO
    {
        namespace Private
        {
            bln WriteToFile( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written );
            bln ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
            bln CancelCachedRead( CFileBasis *file );
        }
    }
}

namespace
{
	DWORD( WINAPI *StdLib_GetFinalPathNameByHandleW )(HANDLE hFile, LPWSTR lpszFilePath, DWORD cchFilePath, DWORD dwFlags);
}

NOINLINE auto FileIO::Private::FileIO_Open( CFileBasis *file, const FilePath &pnn, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode ) -> fileError
{
    ASSUME( file );

    file->handle = INVALID_HANDLE_VALUE;
	file->offsetToStart = 0;

	DWORD dwDesiredAccess = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = 0;

	if( !pnn.IsValid() )
	{
		return fileError( Error::InvalidArgument(), "Path is invalid" );
	}

	if( StdLib_GetFinalPathNameByHandleW == 0 )
	{
		file->pnn = pnn;
		file->pnn.MakeAbsolute();
	}

    if( !(procMode & (FileProcMode::Read | FileProcMode::Write)) )
    {
		return fileError( Error::InvalidArgument(), "No read or write was requested" );
    }

    if( !!(procMode & FileProcMode::Write) )
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if( !!(procMode & FileProcMode::Read) )
    {
        dwDesiredAccess |= GENERIC_READ;
    }

    if( openMode == FileOpenMode::CreateIfDoesNotExist )
    {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else if( openMode == FileOpenMode::CreateAlways || openMode == FileOpenMode::CreateNew )
    {
		if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
		{
			return fileError( Error::InvalidArgument(), "FileProcMode::WriteAppend can't be used with OpenMode::CreateAlways or OpenMode::CreateNew" );
		}

		if( openMode == FileOpenMode::CreateAlways )
		{
			dwCreationDisposition = CREATE_ALWAYS;
		}
		else
		{
			dwCreationDisposition = CREATE_NEW;
		}
    }
    else  //  if( openMode == OpenMode::OpenExisting )
    {
        ASSUME( openMode == FileOpenMode::OpenExisting );
        dwCreationDisposition = OPEN_EXISTING;
    }

    if( !!(cacheMode & (FileCacheMode::LinearRead | FileCacheMode::RandomRead)) )
    {
		if( (cacheMode & (FileCacheMode::LinearRead | FileCacheMode::RandomRead)) == (FileCacheMode::LinearRead | FileCacheMode::RandomRead) )
		{
			return fileError( Error::InvalidArgument(), "Both FileCacheMode::LinearRead and FileCacheMode::RandomRead are specified" );
		}

		if( !(procMode & FileProcMode::Read) )
		{
			return fileError( Error::InvalidArgument(), "FileCacheMode::LinearRead or FileCacheMode::RandomRead can be used only when FileProcMode::Read is specified" );
		}

		if( !!(cacheMode & FileCacheMode::LinearRead) )
		{
			dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
		}
		else
		{
			dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
		}
    }

	if( !!(cacheMode & FileCacheMode::DisableSystemReadCache) )
	{
		if( !(procMode & FileProcMode::Read) )
		{
			return fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemReadCache can be used only when FileProcMode::Read is specified" );
		}

		dwFlagsAndAttributes |= FILE_FLAG_NO_BUFFERING;
	}
	
	if( !!(cacheMode & FileCacheMode::DisableSystemWriteCache) )
	{
		if( !(procMode & FileProcMode::Write) )
		{
			return fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemWriteCache can be used only when FileProcMode::Write is specified" );
		}

		dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
	}

    HANDLE h_file = ::CreateFileW( pnn.PlatformPath(), dwDesiredAccess, FILE_SHARE_READ, 0, dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( h_file == INVALID_HANDLE_VALUE )
    {
		return fileError( StdLib_FileError(), "failed to open system file" );
    }

    if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
    {
		LARGE_INTEGER curPos = {};
        if( !::SetFilePointerEx( h_file, LARGE_INTEGER(), &curPos, FILE_END ) )
        {
            BOOL result = ::CloseHandle( h_file );
            ASSUME( result );
            return fileError( Error::UnknownError(), "Failed to set file pointer to the end of the file( append )" );
        }
		file->offsetToStart = curPos.QuadPart;
    }

    file->openMode = openMode;
    file->procMode = procMode;
	file->cacheMode = cacheMode;
    file->handle = h_file;
    file->bufferPos = 0;
	FILEIO_STAT( Funcs::ClearPod( &file->stats ); )
	file->readBufferCurrentSize = 0;
	file->is_reading = false;
	file->is_shouldCloseFileHandle = true;

	return Error::Ok();
}
        
NOINLINE auto FileIO::Private::FileIO_OpenFromDescriptor( fileHandle osFileDescriptor, bln is_shouldCloseFileHandle ) -> fileError
{
	return Error::Unimplemented();
}

NOINLINE void FileIO::Private::FileIO_Close( CFileBasis *file )
{
    ASSUME( file );
    if( file->handle == INVALID_HANDLE_VALUE )
    {
        return;
    }
	FileIO_Flush( file );
	if( file->is_shouldCloseFileHandle )
	{
		BOOL result = ::CloseHandle( file->handle );
		ASSUME( result );
	}
    file->handle = INVALID_HANDLE_VALUE;
}

bln FileIO::Private::FileIO_IsValid( const CFileBasis *file )
{
    ASSUME( file );
    return file->handle != INVALID_HANDLE_VALUE;
}

CResult < i64 > FileIO::Private::FileIO_OffsetGet( CFileBasis *file, FileOffsetMode mode )
{
    ASSUME( FileIO_IsValid( file ) );

	using returnType = CResult < i64 >;

	if( mode == FileOffsetMode::FromEnd )
	{
		if( !CancelCachedRead( file ) || !FileIO_Flush( file ) )
		{
			return returnType( 0, Error::UnknownError() );
		}
	}
	
	LARGE_INTEGER o_pos;
	if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_pos, FILE_CURRENT ) )
	{
		return returnType( 0, StdLib_FileError() );
	}
	ASSUME( o_pos.QuadPart >= (i64)file->offsetToStart );
	LONGLONG offsetFromBegin = o_pos.QuadPart - file->offsetToStart;

	if( mode == FileOffsetMode::FromBegin )
	{
		ASSUME( file->bufferPos == 0 || file->internalBuffer.get() );
		if( file->is_reading )
		{
			offsetFromBegin -= (LONGLONG)file->bufferPos;
		}
		else
		{
			offsetFromBegin += (LONGLONG)file->bufferPos;
		}

		return offsetFromBegin;
	}
	else if( mode == FileOffsetMode::FromCurrent )
	{
		return 0;
	}
	else
	{
		ASSUME( mode == FileOffsetMode::FromEnd );

		CResult < ui64 > fileSize = FileIO_SizeGet( file );
		if( false == fileSize.Ok() )
		{
			return returnType( 0, fileSize.UnwrapError() );
		}

		return offsetFromBegin - fileSize.Unwrap();
	}
}

NOINLINE CResult < i64 > FileIO::Private::FileIO_OffsetSet( CFileBasis *file, FileOffsetMode mode, i64 offset )
{
    ASSUME( FileIO_IsValid( file ) );
    LARGE_INTEGER o_move;
    DWORD moveMethod;

	using returnType = CResult < i64 >;

    if( !CancelCachedRead( file ) )
    {
        return returnType( 0, Error::UnknownError() );
    }
    if( !FileIO_Flush( file ) )
    {
        return returnType( 0, Error::UnknownError() );
    }

    if( mode == FileOffsetMode::FromBegin )
    {
        moveMethod = FILE_BEGIN;
		offset += file->offsetToStart;
    }
    else if( mode == FileOffsetMode::FromCurrent )
    {
        moveMethod = FILE_CURRENT;
    }
    else if( mode == FileOffsetMode::FromEnd )
    {
        moveMethod = FILE_END;
    }
    else
    {
        return returnType( 0, Error::InvalidArgument() );
    }
    o_move.QuadPart = offset;

    if( !::SetFilePointerEx( file->handle, o_move, &o_move, moveMethod ) )
    {
        return returnType( 0, Error::UnknownError() );
    }

	if( (file->procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
	{
		if( mode != FileOffsetMode::FromBegin )
		{
			if( o_move.QuadPart < (i64)file->offsetToStart )
			{
				o_move.QuadPart = file->offsetToStart;
				if( !::SetFilePointerEx( file->handle, o_move, &o_move, FILE_BEGIN ) )
				{
					return returnType( 0, StdLib_FileError() );
				}
			}
		}
		ASSUME( o_move.QuadPart >= (i64)file->offsetToStart );
		o_move.QuadPart -= file->offsetToStart;
	}

    return o_move.QuadPart;
}

CResult < ui64 > FileIO::Private::FileIO_SizeGet( const CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    LARGE_INTEGER o_size;
    if( !::GetFileSizeEx( file->handle, &o_size ) )
    {
		return CResult < ui64 >( 0, Error::UnknownError() );
    }
	ASSUME( o_size.QuadPart >= (i64)file->offsetToStart );
	o_size.QuadPart -= file->offsetToStart;
    return o_size.QuadPart;
}

NOINLINE CError<> FileIO::Private::FileIO_SizeSet( CFileBasis *file, ui64 newSize )  //  TODO: error procing
{
    ASSUME( FileIO_IsValid( file ) );

	if( !CancelCachedRead( file ) )
	{
		return Error::UnknownError();
	}
	if( !FileIO_Flush( file ) )
	{
		return Error::UnknownError();
	}

	newSize += file->offsetToStart;
	if( newSize < file->offsetToStart )  //  overflow
	{
		newSize = ui64_max;
	}

	LARGE_INTEGER currentOffset;
    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &currentOffset, FILE_CURRENT ) )  //  getting current offset
    {
		return StdLib_FileError();
    }
    ASSUME( currentOffset.QuadPart >= (i64)file->offsetToStart );

    if( currentOffset.QuadPart != newSize )
    {
        LARGE_INTEGER o_newOffset;
        o_newOffset.QuadPart = newSize;
        if( !::SetFilePointerEx( file->handle, o_newOffset, 0, FILE_BEGIN ) )
        {
			return StdLib_FileError();
        }
    }

    if( !::SetEndOfFile( file->handle ) )
    {
		return StdLib_FileError();
    }

    if( !::SetFilePointerEx( file->handle, currentOffset, 0, FILE_BEGIN ) )  //  return the old offset
    {
		return StdLib_FileError();
    }

	return Error::Ok();
}

NOINLINE FilePath FileIO::Private::FileIO_PNNGet( const CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );

	if( StdLib_GetFinalPathNameByHandleW )
	{
		wchar_t tempBuf[ MAX_PATH_LENGTH ];
		DWORD result = StdLib_GetFinalPathNameByHandleW( file->handle, tempBuf, MAX_PATH_LENGTH - 1, FILE_NAME_NORMALIZED );
		if( result < 2 || result >= MAX_PATH_LENGTH )
		{
			return FilePath();
		}
		return FilePath( tempBuf );
	}
	else
	{
		return file->pnn;
	}
}

bln FileIO::Private::WriteToFile( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written )
{
    ASSUME( FileIO_IsValid( file ) && (cp_source || len == 0) );
    FILEIO_STAT( ++file->stats.writesToFileCount; )
    if( !len )
    {
		DSA( written, 0 );
        return true;
    }
	DWORD dwritten;
    if( !::WriteFile( file->handle, cp_source, len, &dwritten, 0 ) )
    {
		DSA( written, 0 );
        return false;
    }
    FILEIO_STAT( file->stats.bytesToFileWritten += len; )
	DSA( written, len );
    return true;
}

bln FileIO::Private::ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( FileIO_IsValid( file ) && (p_target || len == 0) );
    DWORD readed = 0;
    FILEIO_STAT( ++file->stats.readsFromFileCount; )
    if( len != 0 )
    {
        if( !::ReadFile( file->handle, p_target, len, &readed, 0 ) )
        {
            return false;
        }
    }
    FILEIO_STAT( file->stats.bytesFromFileReaded += readed; )
    if( p_readed )
    {
        *p_readed += readed;
    }
    return true;
}

NOINLINE bln FileIO::Private::CancelCachedRead( CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    if( !file->is_reading || file->bufferPos == file->readBufferCurrentSize )
    {
        return true;
    }
    int move = (int)file->bufferPos - (int)file->readBufferCurrentSize;
	ASSUME( move <= 0 );
    LARGE_INTEGER o_move;
    o_move.QuadPart = move;
    BOOL result = ::SetFilePointerEx( file->handle, o_move, 0, FILE_CURRENT );
    file->bufferPos = file->readBufferCurrentSize = file->bufferSize;
    return result != FALSE;
}

void FileIO_InitializeFileIOSystem()
{
	HMODULE k32 = GetModuleHandleA( "kernel32.dll" );
	if( !k32 )
	{
		FatalAppExitA( 1, "StdLib: failed to acquire kernel32.dll handle, can't initialize FileIO" );
		return;
	}
	*(uiw *)&StdLib_GetFinalPathNameByHandleW = (uiw)GetProcAddress( k32, "GetFinalPathNameByHandleW" );
}

#endif
