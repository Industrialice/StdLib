#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FileIO.hpp"
#include "Files.hpp"

namespace StdLib
{
    namespace FileIO
    {
        namespace Private
        {
            bln WriteToFile( CFileBasis *file, const void *cp_source, ui32 len );
            bln ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
            bln CancelCachedRead( CFileBasis *file );
        }
    }
}

NOINLINE bln FileIO::Private::Open( CFileBasis *file, const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, CacheMode::CacheMode_t cacheMode, CTError < CStr > *po_error )
{
    ASSUME( cp_pnn && file );

    file->handle = INVALID_HANDLE_VALUE;
	file->offsetToStart = 0;

    CTError < CStr > o_error;
    DWORD dwFlagsAndAttributes = 0;
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition;
    HANDLE h_file;
    char a_absPath[ MAX_PATH ];
    uiw absPathLen;
	LARGE_INTEGER curPos;

    if( (procMode & (ProcMode::Read | ProcMode::Write)) == 0 )
    {
		o_error = CTError < CStr >( Error::InvalidArgument(), "No read or write was requested" );
        goto toExit;
    }

    if( procMode & ProcMode::Write )
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }
	else if( procMode & ProcMode::Append )
	{
		o_error = CTError < CStr >( Error::InvalidArgument(), "ProcMode::Append was requested without ProcMode::Write" );
		goto toExit;
	}

    if( procMode & ProcMode::Read )
    {
        dwDesiredAccess |= GENERIC_READ;
    }

    if( openMode == OpenMode::CreateIfDoesNotExist )
    {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else if( openMode == OpenMode::CreateAlways || openMode == OpenMode::CreateNew )
    {
		if( procMode & ProcMode::Append )
		{
			o_error = CTError < CStr >( Error::InvalidArgument(), "ProcMode::Append can't be used with OpenMode::CreateAlways or OpenMode::CreateNew" );
			goto toExit;
		}

		if( openMode == OpenMode::CreateAlways )
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
        ASSUME( openMode == OpenMode::OpenExisting );
        dwCreationDisposition = OPEN_EXISTING;
    }

    if( cacheMode & (CacheMode::LinearRead | CacheMode::RandomRead) )
    {
		if( (cacheMode & (CacheMode::LinearRead | CacheMode::RandomRead)) == (CacheMode::LinearRead | CacheMode::RandomRead) )
		{
			o_error = CTError < CStr >( Error::InvalidArgument(), "Both CacheMode::LinearRead and CacheMode::RandomRead are specified" );
			goto toExit;
		}

		if( (procMode & ProcMode::Read) == 0 )
		{
			o_error = CTError < CStr >( Error::InvalidArgument(), "CacheMode::LinearRead or CacheMode::RandomRead must be used only when ProcMode::Read is specified" );
			goto toExit;
		}

		if( cacheMode & CacheMode::LinearRead )
		{
			dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
		}
		else
		{
			dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
		}
    }
	if( cacheMode & CacheMode::DisableSystemWriteCache )
	{
		if( (procMode & ProcMode::Write) == 0 )
		{
			o_error = CTError < CStr >( Error::InvalidArgument(), "CacheMode::DisableSystemWriteCache must be used only when ProcMode::Write is specified" );
			goto toExit;
		}

		dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
	}

    h_file = ::CreateFileA( cp_pnn, dwDesiredAccess, FILE_SHARE_READ, 0, dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( h_file == INVALID_HANDLE_VALUE )
    {
		switch( ::GetLastError() )
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			o_error = Error::DoesNotExist();
			break;
		case ERROR_ACCESS_DENIED:
		case ERROR_WRITE_PROTECT:
		case ERROR_SHARING_VIOLATION:
		case ERROR_LOCK_VIOLATION:
			o_error = Error::NoAccess();
			break;
		case ERROR_NOT_ENOUGH_MEMORY:
		case ERROR_OUTOFMEMORY:
			o_error = Error::OutOfMemory();
			break;
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			o_error = Error::AlreadyExists();
			break;
		default:
			o_error = Error::CannotOpenFile();
		}
        goto toExit;
    }

    if( procMode & ProcMode::Append )
    {
        if( !::SetFilePointerEx( h_file, LARGE_INTEGER(), &curPos, FILE_END ) )
        {
            o_error = CTError < CStr >( Error::Unknown(), "Failed to set file pointer to the end of the file( append )" );
            BOOL result = ::CloseHandle( h_file );
            ASSUME( result );
            goto toExit;
        }
		file->offsetToStart = curPos.QuadPart;
    }

    absPathLen = Files::AbsolutePath( cp_pnn, a_absPath ) + 1;

	_MemZero( &file->stats, sizeof(SStats) );
    file->pnn.Assign( a_absPath, absPathLen );
    file->openMode = openMode;
    file->procMode = procMode;
	file->cacheMode = cacheMode;
    file->handle = h_file;
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;

toExit:
    DSA( po_error, o_error );
    return file->handle != INVALID_HANDLE_VALUE;
}

NOINLINE void FileIO::Private::Close( CFileBasis *file )
{
    ASSUME( file );
    if( file->handle == INVALID_HANDLE_VALUE )
    {
        return;
    }
    Flush( file );
    BOOL result = ::CloseHandle( file->handle );
    ASSUME( result );
    file->handle = INVALID_HANDLE_VALUE;
}

bln FileIO::Private::IsValid( const CFileBasis *file )
{
    ASSUME( file );
    return file->handle != INVALID_HANDLE_VALUE;
}

i64 FileIO::Private::OffsetGet( CFileBasis *file )
{
    ASSUME( IsValid( file ) );

    LARGE_INTEGER o_pos;
    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_pos, FILE_CURRENT ) )
    {
        return -1;
    }

	ASSUME( o_pos.QuadPart >= file->offsetToStart );
	o_pos.QuadPart -= file->offsetToStart;

    if( file->is_reading )
    {
        return o_pos.QuadPart - (LONGLONG)file->bufferPos;
    }
    return o_pos.QuadPart + (LONGLONG)file->bufferPos;
}

NOINLINE i64 FileIO::Private::OffsetSet( CFileBasis *file, OffsetMode::OffsetMode_t mode, i64 offset, CError *po_error )
{
    ASSUME( IsValid( file ) );
    CError o_error = Error::Ok();
    i64 result = -1;
    LARGE_INTEGER o_move;
    DWORD moveMethod;

    if( !CancelCachedRead( file ) )
    {
        o_error = Error::Unknown();
        goto toExit;
    }
    if( !Flush( file ) )
    {
        o_error = Error::Unknown();
        goto toExit;
    }

    if( mode == OffsetMode::FromBegin )
    {
        moveMethod = FILE_BEGIN;
		offset += file->offsetToStart;
    }
    else if( mode == OffsetMode::FromCurrent )
    {
        moveMethod = FILE_CURRENT;
    }
    else if( mode == OffsetMode::FromEnd )
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
        o_error = Error::Unknown();
        goto toExit;
    }

	if( file->procMode & ProcMode::Append )
	{
		if( mode != OffsetMode::FromBegin )
		{
			if( o_move.QuadPart < file->offsetToStart )
			{
				o_move.QuadPart = file->offsetToStart;
				if( !::SetFilePointerEx( file->handle, o_move, &o_move, FILE_BEGIN ) )
				{
					o_error = Error::Unknown();
					goto toExit;
				}
			}
		}
		ASSUME( o_move.QuadPart >= file->offsetToStart );
		o_move.QuadPart -= file->offsetToStart;
	}

    result = o_move.QuadPart;

toExit:
    DSA( po_error, o_error );
    return result;
}

ui64 FileIO::Private::SizeGet( CFileBasis *file, CError *error )
{
    ASSUME( IsValid( file ) );
    LARGE_INTEGER o_size;
    if( !::GetFileSizeEx( file->handle, &o_size ) )
    {
		DSA( error, Error::Unknown() );
        return 0;
    }
	ASSUME( o_size.QuadPart >= file->offsetToStart );
	o_size.QuadPart -= file->offsetToStart;
	DSA( error, Error::Ok() );
    return o_size.QuadPart;
}

NOINLINE bln FileIO::Private::SizeSet( CFileBasis *file, ui64 newSize )
{
    ASSUME( IsValid( file ) );

	if( !CancelCachedRead( file ) )
	{
		return false;
	}
	if( !Flush( file ) )
	{
		return false;
	}

	newSize += file->offsetToStart;

    LARGE_INTEGER currentOffset;
    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &currentOffset, FILE_BEGIN ) )  //  getting current offset
    {
        return false;
    }

    if( currentOffset.QuadPart != newSize )
    {
        LARGE_INTEGER o_newOffset;
        o_newOffset.QuadPart = newSize;
        if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_newOffset, FILE_BEGIN ) )
        {
            return false;
        }
    }

    if( !::SetEndOfFile( file->handle ) )
    {
        return false;
    }
    ASSUME( currentOffset.QuadPart >= file->offsetToStart );

    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &currentOffset, FILE_BEGIN ) )  //  reset offset back
    {
        return false;
    }

    return true;
}

ui32 FileIO::Private::PNNGet( const CFileBasis *file, char *p_buf )
{
    ASSUME( IsValid( file ) );
    if( p_buf )
    {
        _MemCpy( p_buf, file->pnn.Data(), file->pnn.Size() + 1 );
    }
    return file->pnn.Size();
}

bln FileIO::Private::WriteToFile( CFileBasis *file, const void *cp_source, ui32 len )
{
    ASSUME( IsValid( file ) && (cp_source || len == 0) );
    ++file->stats.writesToFileCount;
    if( !len )
    {
        return true;
    }
    DWORD written;
    if( !::WriteFile( file->handle, cp_source, len, &written, 0 ) )
    {
        return false;
    }
    file->stats.bytesToFileWritten += len;
    return true;
}

bln FileIO::Private::ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( IsValid( file ) && (p_target || len == 0) );
    DWORD readed = 0;
    ++file->stats.readsFromFileCount;
    if( len != 0 )
    {
        if( !::ReadFile( file->handle, p_target, len, &readed, 0 ) )
        {
            return false;
        }
    }
    file->stats.bytesFromFileReaded += readed;
    if( p_readed )
    {
        *p_readed += readed;
    }
    return true;
}

NOINLINE bln FileIO::Private::CancelCachedRead( CFileBasis *file )
{
    ASSUME( IsValid( file ) );
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

#endif
