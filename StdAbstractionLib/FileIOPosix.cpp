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
            NOINLINE bln WriteToFile( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written );
            NOINLINE bln ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
            NOINLINE bln CancelCachedRead( CFileBasis *file );
        }
    }
}

NOINLINE bln FileIO::Private::FileIO_Open( CFileBasis *file, const FilePath &pnn, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode, fileError *po_error )
{
    ASSUME( file );

    file->handle = -1;
    file->offsetToStart = 0;

    fileError o_error;
    int fileHandle;
    mode_t process_mask;
    int flags = 0;
    int seekResult;

    if( !pnn.IsValid() )
    {
        o_error = fileError( Error::InvalidArgument(), "Path is invalid" );
        goto toExit;
    }

    if( (procMode & FileProcMode::Read) && (procMode & FileProcMode::Write) )
    {
        flags |= O_RDWR;
    }
    else if( procMode & FileProcMode::Read )
    {
        flags |= O_RDONLY;
    }
    else if( procMode & FileProcMode::Write )
    {
        flags |= O_WRONLY;
    }
    else
    {
		o_error = fileError( Error::InvalidArgument(), "No read or write was requested" );
        goto toExit;
    }

    if( openMode == FileOpenMode::CreateIfDoesNotExist )
    {
        flags |= O_CREAT;
    }
    else if( openMode == FileOpenMode::CreateAlways || openMode == FileOpenMode::CreateNew )
    {
		if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
		{
			o_error = fileError( Error::InvalidArgument(), "FileProcMode::Append can't be used with FileOpenMode::CreateAlways or FileOpenMode::CreateNew" );
			goto toExit;
		}

		if( openMode == FileOpenMode::CreateAlways )
		{
            if( (procMode & FileProcMode::Write) == 0 )  //  we need to truncate file, but we can't do it if we can't write to it, so we just remove it
            {
                if( !Files::RemoveFile( pnn, &o_error ) )
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
    else  //  if( openMode == FileOpenMode::OpenExisting )
    {
        ASSUME( openMode == FileOpenMode::OpenExisting );
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
			//  TODO:
		}
		else
		{
			//  TODO:
		}
    }
	if( cacheMode & FileCacheMode::DisableSystemWriteCache )
	{
		if( (procMode & FileProcMode::Write) == 0 )
		{
			o_error = fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemWriteCache must be used only when FileProcMode::Write is specified" );
			goto toExit;
		}

		//  TODO:
	}

    process_mask = ::umask( 0 );
    fileHandle = ::open( pnn.PlatformPath(), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
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

    if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
    {
        seekResult = ::lseek64( fileHandle, 0, SEEK_END );
        if( seekResult == -1 )
        {
            o_error = Error::UnknownError();
            goto toExit;
        }
        file->offsetToStart = seekResult;
    }

	FILEIO_STAT( Funcs::ClearPod( &file->stats ); )
    file->openMode = openMode;
    file->procMode = procMode;
	file->cacheMode = cacheMode;
    file->handle = fileHandle;
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;

toExit:
    DSA( po_error, o_error );
    return file->handle != -1;
}

NOINLINE void FileIO::Private::FileIO_Close( CFileBasis *file )
{
    ASSUME( file );
    if( file->handle == -1 )
    {
        return;
    }
    FileIO_Flush( file );
    int result = ::close( file->handle );
    ASSUME( result == 0 );
    file->handle = -1;
}

bln FileIO::Private::FileIO_IsValid( const CFileBasis *file )
{
    ASSUME( file );
    return file->handle != -1;
}

i64 FileIO::Private::FileIO_OffsetGet( CFileBasis *file, FileOffsetMode::mode_t mode, CError *error )  //  TODO: processing offset mode
{
    ASSUME( FileIO_IsValid( file ) );
	DSA( error, Error::Ok() );
    off64_t offset = ::lseek64( file->handle, 0, SEEK_CUR );
    ASSUME( offset != -1 );
    offset += file->offsetToStart;
    return offset + (off64_t)file->bufferPos - (off64_t)file->bufferPos;
}

NOINLINE i64 FileIO::Private::FileIO_OffsetSet( CFileBasis *file, FileOffsetMode::mode_t mode, i64 offset, CError *po_error )
{
    ASSUME( FileIO_IsValid( file ) );
    CError o_error = Error::Ok();
    off64_t result = -1;
    int whence;

    if( !CancelCachedRead( file ) )
    {
        o_error = Error::UnknownError();
        return false;
    }
    if( !FileIO_Flush( file ) )
    {
        o_error = Error::UnknownError();
        return false;
    }

    if( mode == FileOffsetMode::FromBegin )
    {
        offset += file->offsetToStart;
        whence = SEEK_SET;
    }
    else if( mode == FileOffsetMode::FromCurrent )
    {
        whence = SEEK_CUR;
    }
    else if( mode == FileOffsetMode::FromEnd )
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
        o_error = Error::UnknownError();
        goto toExit;
    }

	if( (file->procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
	{
		if( mode != FileOffsetMode::FromBegin )
		{
			if( result < file->offsetToStart )
			{
				result = file->offsetToStart;
				result = ::lseek64( file->handle, file->offsetToStart, SEEK_SET );
				if( result == -1 )
				{
					o_error = Error::UnknownError();
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

ui64 FileIO::Private::FileIO_SizeGet( const CFileBasis *file, CError *error )
{
    ASSUME( FileIO_IsValid( file ) );
    struct stat64 o_stat;
    int result = ::fstat64( file->handle, &o_stat );
    if( result == -1 )
    {
        DSA( error, Error::UnknownError() );
        return 0;
    }
    ASSUME( o_stat.st_size >= file->offsetToStart );
    DSA( error, Error::Ok() );
    return o_stat.st_size - file->offsetToStart;
}

bln FileIO::Private::FileIO_SizeSet( CFileBasis *file, ui64 newSize, CError *error )
{
    SOFTBREAK;  //  TODO:
	DSA( error, Error::Unimplemented() );
    return false;
}

NOINLINE FilePath FileIO::Private::FileIO_PNNGet( const CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    char a_proc[ MAX_PATH_LENGTH ] = "/proc/self/fd/";
    Funcs::IntToStrDec( file->handle, a_proc + sizeof("/proc/self/fd/") - 1 );
    char a_buf[ MAX_PATH_LENGTH ];
    ssize_t len = ::readlink( a_proc, a_buf, sizeof(a_buf) - 1 );
    if( len == -1 )
    {
        return FilePath();
    }
    return FilePath( a_buf );
}

NOINLINE bln FileIO::Private::WriteToFile( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written )
{
    ASSUME( FileIO_IsValid( file ) && (cp_source || len == 0) );
    FILEIO_STAT( ++file->stats.writesToFileCount; )
    if( !len )
    {
		DSA( written, 0 );
        return true;
    }
    ssize_t sswritten = ::write( file->handle, cp_source, len );
    if( sswritten == -1 )
    {
		DSA( written, 0 );
        return false;
    }
	DSA( written, sswritten );
    FILEIO_STAT( file->stats.bytesToFileWritten += sswritten; )
    return true;
}

NOINLINE bln FileIO::Private::ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( FileIO_IsValid( file ) && (p_target || len == 0) );
    FILEIO_STAT( ++file->stats.readsFromFileCount; )
    ssize_t readed = ::read( file->handle, p_target, len );
    if( readed == -1 )
    {
        return false;
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
    if( file->bufferPos == file->readBufferCurrentSize )
    {
        return true;
    }
    i32 move = (i32)file->bufferPos - (i32)file->readBufferCurrentSize;
    off64_t result = ::lseek64( file->handle, move, SEEK_CUR );
    file->bufferPos = file->readBufferCurrentSize = file->bufferSize;
    return result != -1;
}

void FileIO::Private::FileIO_InitializeFileIOSystem()
{}

#endif
