#include "PreHeader.hpp"

#ifdef POSIX

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "FileIO.hpp"
#include "Files.hpp"

namespace StdLib
{
    namespace FileIO
    {
        namespace Private
        {
            NOINLINE bln WriteToFile( CFileBasis *file, const void *cp_source, ui32 len );
            NOINLINE bln ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
            NOINLINE bln CancelCachedRead( CFileBasis *file );
        }
    }
}

NOINLINE bln FileIO::Private::Open( CFileBasis *file, const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, CacheMode::CacheMode_t cacheMode, fileError *po_error )
{
    ASSUME( cp_pnn && file );

    file->handle = -1;
    file->offsetToStart = 0;

    fileError o_error;
    int fileHandle;
    mode_t process_mask;
    int flags = 0;
    int seekResult;

    if( (procMode & (ProcMode::Read | ProcMode::Write)) == 0 )
    {
		o_error = fileError( Error::InvalidArgument(), "No read or write was requested" );
        goto toExit;
    }

    if( (procMode & ProcMode::Read) && (procMode & ProcMode::Write) )
    {
        flags |= O_RDWR;
    }
    else if( procMode & ProcMode::Read )
    {
        if( procMode & ProcMode::Append )
        {
            o_error = fileError( Error::InvalidArgument(), "ProcMode::Append was requested without ProcMode::Write" );
            goto toExit;
        }
        flags |= O_RDONLY;
    }
    else if( procMode & ProcMode::Write )
    {
        flags |= O_WRONLY;
    }
    else
    {
		o_error = fileError( Error::InvalidArgument(), "No read or write was requested" );
        goto toExit;
    }

    if( openMode == OpenMode::CreateIfDoesNotExist )
    {
        flags |= O_CREAT;
    }
    else if( openMode == OpenMode::CreateAlways || openMode == OpenMode::CreateNew )
    {
		if( procMode & ProcMode::Append )
		{
			o_error = fileError( Error::InvalidArgument(), "ProcMode::Append can't be used with OpenMode::CreateAlways or OpenMode::CreateNew" );
			goto toExit;
		}

		if( openMode == OpenMode::CreateAlways )
		{
            if( (procMode & ProcMode::Write) == 0 )  //  we need to truncate file, but we can't do it if we can't write to it, so we just remove it
            {
                if( !Files::RemoveFile( cp_pnn, &o_error ) )
                {
                    goto toExit;
                }
                flags |= O_CREAT;
            }
            else
            {
                flags |= O_CREAT | O_TRUNC;
            }
        }
        else
        {
            flags |= O_CREAT | O_EXCL;
        }
    }
    else  //  if( openMode == OpenMode::OpenExisting )
    {
        ASSUME( openMode == OpenMode::OpenExisting );
    }

    if( cacheMode & (CacheMode::LinearRead | CacheMode::RandomRead) )
    {
		if( (cacheMode & (CacheMode::LinearRead | CacheMode::RandomRead)) == (CacheMode::LinearRead | CacheMode::RandomRead) )
		{
			o_error = fileError( Error::InvalidArgument(), "Both CacheMode::LinearRead and CacheMode::RandomRead are specified" );
			goto toExit;
		}

		if( (procMode & ProcMode::Read) == 0 )
		{
			o_error = fileError( Error::InvalidArgument(), "CacheMode::LinearRead or CacheMode::RandomRead must be used only when ProcMode::Read is specified" );
			goto toExit;
		}

		if( cacheMode & CacheMode::LinearRead )
		{
			//  TODO:
		}
		else
		{
			//  TODO:
		}
    }
	if( cacheMode & CacheMode::DisableSystemWriteCache )
	{
		if( (procMode & ProcMode::Write) == 0 )
		{
			o_error = fileError( Error::InvalidArgument(), "CacheMode::DisableSystemWriteCache must be used only when ProcMode::Write is specified" );
			goto toExit;
		}

		//  TODO:
	}

    process_mask = ::umask( 0 );
    fileHandle = ::open( cp_pnn, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
    ::umask( process_mask );
    if( fileHandle == -1 )
    {
		switch( errno )
		{
		case EACCES:
		case EFAULT:
		case EROFS:
		case ETXTBSY:
			o_error = Error::NoAccess();
			break;
		case EEXIST:
			o_error = Error::AlreadyExists();
			break;
		case EINTR:
			o_error = Error::Interrupted();
			break;
		case ENOENT:
			o_error = Error::DoesNotExist();
			break;
		case ENOMEM:
		case ENFILE:
		case EMFILE:
		case ENOSPC:
			o_error = Error::OutOfMemory();
			break;
		default:
			o_error = Error::CannotOpenFile();
		}
        goto toExit;
    }

    if( procMode & ProcMode::Append )
    {
        seekResult = ::lseek64( fileHandle, 0, SEEK_END );
        if( seekResult == -1 )
        {
            o_error = Error::Unknown();
            goto toExit;
        }
        file->offsetToStart = seekResult;
    }

	_MemZero( &file->stats, sizeof(SStats) );
    file->openMode = openMode;
    file->procMode = procMode;
	file->cacheMode = cacheMode;
    file->handle = fileHandle;
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;

    o_error = Error::Ok();

toExit:
    DSA( po_error, o_error );
    return file->handle != -1;
}

NOINLINE void FileIO::Private::Close( CFileBasis *file )
{
    ASSUME( file );
    if( file->handle == -1 )
    {
        return;
    }
    Flush( file );
    int result = ::close( file->handle );
    ASSUME( result == 0 );
    file->handle = -1;
}

bln FileIO::Private::IsValid( const CFileBasis *file )
{
    ASSUME( file );
    return file->handle != -1;
}

i64 FileIO::Private::OffsetGet( CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    off64_t offset = ::lseek64( file->handle, 0, SEEK_CUR );
    ASSUME( offset != -1 );
    offset += file->offsetToStart;
    return offset + (off64_t)file->bufferPos - (off64_t)file->bufferPos;
}

NOINLINE i64 FileIO::Private::OffsetSet( CFileBasis *file, OffsetMode::OffsetMode_t mode, i64 offset, CError *po_error )
{
    ASSUME( IsValid( file ) );
    CError o_error = Error::Ok();
    off64_t result = -1;
    int whence;

    if( !CancelCachedRead( file ) )
    {
        o_error = Error::Unknown();
        return false;
    }
    if( !Flush( file ) )
    {
        o_error = Error::Unknown();
        return false;
    }

    if( mode == OffsetMode::FromBegin )
    {
        offset += file->offsetToStart;
        whence = SEEK_SET;
    }
    else if( mode == OffsetMode::FromCurrent )
    {
        whence = SEEK_CUR;
    }
    else if( mode == OffsetMode::FromEnd )
    {
        whence = SEEK_END;
    }
    else
    {
        o_error = Error::InvalidArgument();
        goto toExit;
    }

    result = ::lseek64( file->handle, offset, whence );
    if( result == -1 )
    {
        o_error = Error::Unknown();
        goto toExit;
    }

	if( file->procMode & ProcMode::Append )
	{
		if( mode != OffsetMode::FromBegin )
		{
			if( result < file->offsetToStart )
			{
				result = file->offsetToStart;
				result = ::lseek64( file->handle, file->offsetToStart, SEEK_SET );
				if( result == -1 )
				{
					o_error = Error::Unknown();
					goto toExit;
				}
			}
		}
		ASSUME( result >= file->offsetToStart );
		result -= file->offsetToStart;
	}

toExit:
    DSA( po_error, o_error );
    return result;
}

ui64 FileIO::Private::SizeGet( CFileBasis *file, CError *error )
{
    ASSUME( IsValid( file ) );
    struct stat64 o_stat;
    int result = ::fstat64( file->handle, &o_stat );
    if( result == -1 )
    {
        DSA( error, Error::Unknown() );
        return 0;
    }
    ASSUME( o_stat.st_size >= file->offsetToStart );
    DSA( error, Error::Ok() );
    return o_stat.st_size - file->offsetToStart;
}

bln FileIO::Private::SizeSet( CFileBasis *file, ui64 newSize )
{
    DBGBREAK;  //  TODO:
    return false;
}

NOINLINE ui32 FileIO::Private::PNNGet( const CFileBasis *file, char *p_buf )
{
    ASSUME( IsValid( file ) );
    char a_proc[ 512 ] = "/proc/self/fd/";
    Funcs::IntToStrDec( file->handle, a_proc + sizeof("/proc/self/fd/") - 1 );
    char a_buf[ 4096 ];
    ssize_t len = ::readlink( a_proc, a_buf, sizeof(a_buf) - 1 );
    if( len == -1 )
    {
        if( p_buf )
        {
            *p_buf = '\0';
        }
        return 0;
    }
    if( p_buf )
    {
        _MemCpy( p_buf, a_buf, len );
        p_buf[ len ] = '\0';
    }
    return len;
}

NOINLINE bln FileIO::Private::WriteToFile( CFileBasis *file, const void *cp_source, ui32 len )
{
    ASSUME( IsValid( file ) && (cp_source || len == 0) );
    ++file->stats.writesToFileCount;
    if( !len )
    {
        return true;
    }
    ssize_t written = ::write( file->handle, cp_source, len );
    if( written == -1 )
    {
        return false;
    }
    file->stats.bytesToFileWritten += len;
    return true;
}

NOINLINE bln FileIO::Private::ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( IsValid( file ) && (p_target || len == 0) );
    ++file->stats.readsFromFileCount;
    ssize_t readed = ::read( file->handle, p_target, len );
    if( readed == -1 )
    {
        return false;
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
    if( file->bufferPos == file->readBufferCurrentSize )
    {
        return true;
    }
    i32 move = (i32)file->bufferPos - (i32)file->readBufferCurrentSize;
    off64_t result = ::lseek64( file->handle, move, SEEK_CUR );
    file->bufferPos = file->readBufferCurrentSize = file->bufferSize;
    return result != -1;
}

void FileIO::Private::Initialize()
{}

#endif
