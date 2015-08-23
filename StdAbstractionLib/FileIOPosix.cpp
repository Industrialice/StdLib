#include "PreHeader.hpp"

#ifdef POSIX

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "FileIO.hpp"
#include "Funcs.hpp"

struct SFile
{
    int handle;
    FileIO::ProcMode::ProcMode_t attribs;

    union
    {
        struct
        {
            ui32 writesToBufferCount;
            ui32 writesToFileCount;
            ui32 readsFromBufferCount;
            ui32 readsFromFileCount;
            ui32 bytesFromBufferReaded;
            ui32 bytesFromFileReaded;
            ui32 bytesToBufferWritten;
            ui32 bytesToFileWritten;
            ui32 bufferedWrites;
            ui32 unbufferedWrites;
            ui32 bufferedReads;
            ui32 unbufferedReads;
        };
        FileIO::SStats stats;
    };

    byte *buffer;
    ui32 bufferSize;
    ui32 bufferPos;
    ui32 readBufferCurrentSize;  //  can be lower than bufferSize if, for example, EOF is reached
    ui32 bufferSize;
    ui32 bufferPos;
};

#define dis ((SFile *)file.GetVal())

namespace
{
    CVector < SFile * > o_Files;
}

namespace FileIO
{
    namespace Private
    {
        NOINLINE bln WriteToFile( filehdl file, const void *cp_source, ui32 len );
        NOINLINE bln ReadFromFile( filehdl file, void *p_target, ui32 len, ui32 *p_readed );
        NOINLINE bln CancelCachedRead( filehdl file );
    }
}

NOINLINE filehdl FileIO::Open( const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, SError *po_error )
{
    ASSUME( cp_pnn );
    SError o_error = Error::Get( Error::InvalidArgument );
    filehdl retHdl = 0;
    int file;
    SFile *po_file;
    mode_t process_mask;
    int flags = 0;
    int seekResult;

    if( (procMode & (ProcMode::Read | ProcMode::Write)) == 0 )
    {
        goto toExit;
    }
    if( procMode & ProcMode::Write )
    {
    	if( procMode & ProcMode::Read )
    	{
    		flags |= O_RDWR;
    	}
    	else
    	{
    		if( procMode & ProcMode::SequentialScan )
    		{
    		  	goto toExit;
    		}
    		flags |= O_WRONLY;
    	}
    }
    else
    {
    	if( procMode & ProcMode::Append )
    	{
    		goto toExit;
    	}
    	if( procMode & ProcMode::Read )
    	{
    		flags |= O_RDONLY;
    	}
		else
		{
			goto toExit;
		}
    }

    if( openMode == OpenMode::CreateIfNotExists )
    {
    	flags |= O_CREAT;
    }
    else if( openMode == OpenMode::CreateAlways )
    {
    	flags |= O_CREAT | O_TRUNC;
    }
    else  //  if( openMode == OpenMode::OpenExisting )
    {
        ASSUME( openMode == OpenMode::OpenExisting );
    }

    process_mask = ::umask( 0 );
    file = ::open( cp_pnn, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
    ::umask( process_mask );
    if( file == -1 )
    {
        o_error = Error::GetOther( FileError::CanNotOpenFile, Private::GetErrorsDesc() );
        goto toExit;
    }

    if( procMode & ProcMode::Append )
    {
    	seekResult = ::lseek64( file, 0, SEEK_END );
    	if( seekResult == -1 )
    	{
    	    o_error = Error::Get( Error::Unknown );
    	    goto toExit;
    	}
    }

    po_file = (SFile *)::malloc( sizeof(SFile) );
    _Clear( po_file );
    o_Files.PushBack( po_file );

    po_file->attribs = procMode;
    po_file->handle = file;

    retHdl = po_file;

    o_error = Error::Get( Error::Ok );

toExit:
    DSA( po_error, o_error );
    return retHdl;
}

NOINLINE void FileIO::Close( filehdl file )
{
    ASSUME( IsValid( dis ) );
    uiw index;
    bln is_finded = o_Files.Find( dis, &index );
    if( !is_finded )
    {
        DBGBREAK;
        return;
    }
    ASSUME( dis->handle != -1 );
    Flush( dis );
    int result = ::close( dis->handle );
    ASSUME( result == 0 );
    ::free( dis );
    o_Files.Erase( index, 1 );
}

bln FileIO::IsValid( filehdl file )
{
    bln is_valid = dis != 0 && o_Files.Find( dis, 0 );
    ASSUME( !is_valid || dis->handle != -1 );
    return is_valid;
}

#include "FileIOAny.priv"  //  FileIO platform independent code is placed here

i64 FileIO::OffsetGet( filehdl file )
{
    ASSUME( dis && dis->handle != -1 );
    off64_t offset = ::lseek64( dis->handle, 0, SEEK_CUR );
    ASSUME( offset != -1 );
    return offset + (off64_t)dis->bufferPos - (off64_t)dis->bufferPos;
}

NOINLINE i64 FileIO::OffsetSet( filehdl file, OffsetMode::OffsetMode_t mode, i64 offset, SError *po_error )
{
    ASSUME( dis && dis->handle != -1 );
    SError o_error = Error::Get( Error::Ok );
    off64_t result = -1;
    int whence;

    if( !CancelCachedRead( dis ) )
    {
        return false;
    }
    if( !Flush( dis ) )
    {
        return false;
    }

    if( mode == OffsetMode::FromBegin )
    {
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
        o_error = Error::Get( Error::InvalidArgument );
        goto toExit;
    }

    result = ::lseek64( dis->handle, offset, whence );
    if( result == -1 )
    {
        o_error = Error::Get( Error::Unknown );
        goto toExit;
    }

toExit:
    DSA( po_error, o_error );
    return result;
}

ui64 FileIO::SizeGet( filehdl file )
{
    ASSUME( dis && dis->handle != -1 );
    struct stat64 o_stat;
    int result = ::fstat64( dis->handle, &o_stat );
    ASSUME( result != -1 );
    return o_stat.st_size;
}

FileIO::ProcMode::ProcMode_t FileIO::ProcModeGet( filehdl file )
{
    ASSUME( dis && dis->handle != -1 );
    return dis->attribs;
}

NOINLINE ui32 FileIO::PNNGet( filehdl file, char *p_buf )
{
    ASSUME( dis && dis->handle != -1 );
    char a_proc[ 512 ] = "/proc/self/fd/";
    Funcs::IntToStrDec( dis->handle, a_proc + sizeof("/proc/self/fd/") - 1 );
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

NOINLINE bln FileIO::Private::CancelCachedRead( filehdl file )
{
    ASSUME( FileIO::IsValid( dis ) );
    if( dis->bufferPos == dis->readBufferCurrentSize )
    {
        return true;
    }
    i32 move = (i32)dis->bufferPos - (i32)dis->readBufferCurrentSize;
    off64_t result = ::lseek64( dis->handle, move, SEEK_CUR );
    dis->bufferPos = dis->readBufferCurrentSize = dis->bufferSize;
    return result != -1;
}

NOINLINE bln FileIO::Private::WriteToFile( filehdl file, const void *cp_source, ui32 len )
{
    ASSUME( FileIO::IsValid( dis ) && (cp_source || len == 0) );
    ++dis->writesToFileCount;
    if( !len )
    {
        return true;
    }
    ssize_t written = ::write( dis->handle, cp_source, len );
    if( written == -1 )
    {
        return false;
    }
    dis->bytesToFileWritten += len;
    return true;
}

NOINLINE bln FileIO::Private::ReadFromFile( filehdl file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( FileIO::IsValid( dis ) && p_target );
    DSA( p_readed, 0 );
    ++dis->readsFromFileCount;
    ssize_t readed = ::read( dis->handle, p_target, len );
    if( readed == -1 )
    {
        return false;
    }
    dis->bytesFromFileReaded += readed;
    DSA( p_readed, readed );
    return true;
}

#undef dis

#endif POSIX
