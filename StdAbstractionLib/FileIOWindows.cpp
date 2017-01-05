#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FileIO.hpp"
#include "Files.hpp"

CError StdLib_FileError();  //  from FilesWindows.cpp

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

NOINLINE bln FileIO::Private::FileIO_Open( CFileBasis *file, const FilePath &pnn, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode, fileError *error )
{
    ASSUME( file );

    file->handle = INVALID_HANDLE_VALUE;
	file->offsetToStart = 0;

    fileError o_error;
    DWORD dwFlagsAndAttributes = 0;
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition;
    HANDLE h_file;
	LARGE_INTEGER curPos;

	if( !pnn.IsValid() )
	{
		o_error = fileError( Error::InvalidArgument(), "Path is invalid" );
		goto toExit;
	}

	if( StdLib_GetFinalPathNameByHandleW == 0 )
	{
		file->pnn = new FilePath( pnn );
		file->pnn->MakeAbsolute();
	}

    if( (procMode & (FileProcMode::Read | FileProcMode::Write)) == 0 )
    {
		o_error = fileError( Error::InvalidArgument(), "No read or write was requested" );
        goto toExit;
    }

    if( procMode & FileProcMode::Write )
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if( procMode & FileProcMode::Read )
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
			o_error = fileError( Error::InvalidArgument(), "FileProcMode::WriteAppend can't be used with OpenMode::CreateAlways or OpenMode::CreateNew" );
			goto toExit;
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

    if( cacheMode & (FileCacheMode::LinearRead | FileCacheMode::RandomRead) )
    {
		if( (cacheMode & (FileCacheMode::LinearRead | FileCacheMode::RandomRead)) == (FileCacheMode::LinearRead | FileCacheMode::RandomRead) )
		{
			o_error = fileError( Error::InvalidArgument(), "Both FileCacheMode::LinearRead and FileCacheMode::RandomRead are specified" );
			goto toExit;
		}

		if( (procMode & FileProcMode::Read) == 0 )
		{
			o_error = fileError( Error::InvalidArgument(), "FileCacheMode::LinearRead or FileCacheMode::RandomRead must be used only when FileProcMode::Read is specified" );
			goto toExit;
		}

		if( cacheMode & FileCacheMode::LinearRead )
		{
			dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
		}
		else
		{
			dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
		}
    }
	if( cacheMode & FileCacheMode::DisableSystemWriteCache )
	{
		if( (procMode & FileProcMode::Write) == 0 )
		{
			o_error = fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemWriteCache must be used only when FileProcMode::Write is specified" );
			goto toExit;
		}

		dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
	}

    h_file = ::CreateFileW( pnn.PlatformPath(), dwDesiredAccess, FILE_SHARE_READ, 0, dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( h_file == INVALID_HANDLE_VALUE )
    {
		o_error = StdLib_FileError();
        goto toExit;
    }

    if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
    {
        if( !::SetFilePointerEx( h_file, LARGE_INTEGER(), &curPos, FILE_END ) )
        {
            o_error = fileError( Error::UnknownError(), "Failed to set file pointer to the end of the file( append )" );
            BOOL result = ::CloseHandle( h_file );
            ASSUME( result );
            goto toExit;
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

toExit:
    DSA( error, o_error );
    return file->handle != INVALID_HANDLE_VALUE;
}
        
NOINLINE bln FileIO::Private::FileIO_OpenFromDescriptor( fileHandle osFileDescriptor, bln is_shouldCloseFileHandle, fileError *error )
{
	DSA( error, Error::Unimplemented() );
	return false;
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

i64 FileIO::Private::FileIO_OffsetGet( CFileBasis *file, FileOffsetMode::mode_t mode, CError *error )
{
    ASSUME( FileIO_IsValid( file ) );

	DSA( error, Error::Ok() );

	if( mode == FileOffsetMode::FromEnd )
	{
		if( !CancelCachedRead( file ) || !FileIO_Flush( file ) )
		{
			DSA( error, Error::UnknownError() );
			return -1;
		}
	}
	
	LARGE_INTEGER o_pos;
	if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_pos, FILE_CURRENT ) )
	{
		DSA( error, StdLib_FileError() );
		return -1;
	}
	ASSUME( o_pos.QuadPart >= file->offsetToStart );
	LONGLONG offsetFromBegin = o_pos.QuadPart - file->offsetToStart;

	if( mode == FileOffsetMode::FromBegin )
	{
		ASSUME( file->bufferPos == 0 || file->bufferRef );
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

		CError sizeError;
		ui64 fileSize = FileIO_SizeGet( file, &sizeError );
		if( sizeError != Error::Ok() )
		{
			DSA( error, sizeError );
			return -1;
		}

		return offsetFromBegin - fileSize;
	}
}

NOINLINE i64 FileIO::Private::FileIO_OffsetSet( CFileBasis *file, FileOffsetMode::mode_t mode, i64 offset, CError *error )
{
    ASSUME( FileIO_IsValid( file ) );
    CError o_error = Error::Ok();
    i64 result = -1;
    LARGE_INTEGER o_move;
    DWORD moveMethod;

    if( !CancelCachedRead( file ) )
    {
        o_error = Error::UnknownError();
        goto toExit;
    }
    if( !FileIO_Flush( file ) )
    {
        o_error = Error::UnknownError();
        goto toExit;
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
        o_error = Error::InvalidArgument();
        goto toExit;
    }
    o_move.QuadPart = offset;

    if( !::SetFilePointerEx( file->handle, o_move, &o_move, moveMethod ) )
    {
        o_error = Error::UnknownError();
        goto toExit;
    }

	if( (file->procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
	{
		if( mode != FileOffsetMode::FromBegin )
		{
			if( o_move.QuadPart < file->offsetToStart )
			{
				o_move.QuadPart = file->offsetToStart;
				if( !::SetFilePointerEx( file->handle, o_move, &o_move, FILE_BEGIN ) )
				{
					o_error = StdLib_FileError();
					goto toExit;
				}
			}
		}
		ASSUME( o_move.QuadPart >= file->offsetToStart );
		o_move.QuadPart -= file->offsetToStart;
	}

    result = o_move.QuadPart;

toExit:
    DSA( error, o_error );
    return result;
}

ui64 FileIO::Private::FileIO_SizeGet( const CFileBasis *file, CError *error )
{
    ASSUME( FileIO_IsValid( file ) );
    LARGE_INTEGER o_size;
    if( !::GetFileSizeEx( file->handle, &o_size ) )
    {
		DSA( error, Error::UnknownError() );
        return 0;
    }
	ASSUME( o_size.QuadPart >= file->offsetToStart );
	o_size.QuadPart -= file->offsetToStart;
	DSA( error, Error::Ok() );
    return o_size.QuadPart;
}

NOINLINE bln FileIO::Private::FileIO_SizeSet( CFileBasis *file, ui64 newSize, CError *error )  //  TODO: error procing
{
    ASSUME( FileIO_IsValid( file ) );

	DSA( error, Error::UnknownError() );

	if( !CancelCachedRead( file ) )
	{
		return false;
	}
	if( !FileIO_Flush( file ) )
	{
		return false;
	}

	newSize += file->offsetToStart;
	if( newSize < file->offsetToStart )  //  overflow
	{
		newSize = ui64_max;
	}

	LARGE_INTEGER currentOffset;
    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &currentOffset, FILE_CURRENT ) )  //  getting current offset
    {
		DSA( error, StdLib_FileError() );
        return false;
    }
    ASSUME( currentOffset.QuadPart >= file->offsetToStart );

    if( currentOffset.QuadPart != newSize )
    {
        LARGE_INTEGER o_newOffset;
        o_newOffset.QuadPart = newSize;
        if( !::SetFilePointerEx( file->handle, o_newOffset, 0, FILE_BEGIN ) )
        {
			DSA( error, StdLib_FileError() );
            return false;
        }
    }

    if( !::SetEndOfFile( file->handle ) )
    {
		DSA( error, StdLib_FileError() );
        return false;
    }

    if( !::SetFilePointerEx( file->handle, currentOffset, 0, FILE_BEGIN ) )  //  return the old offset
    {
		DSA( error, StdLib_FileError() );
        return false;
    }

	DSA( error, Error::Ok() );
    return true;
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
		ASSUME( file->pnn );
		return *file->pnn;
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

void FileIO::Private::FileIO_InitializeFileIOSystem()
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
