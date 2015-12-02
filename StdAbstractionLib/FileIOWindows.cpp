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
            bool WriteToFile( CFileBasis *file, const void *cp_source, unsigned int len );
            bool ReadFromFile( CFileBasis *file, void *p_target, unsigned int len, unsigned int *p_readed );
            bool CancelCachedRead( CFileBasis *file );
        }
    }
}

NOINLINE bool FileIO::Private::Open( CFileBasis *file, const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, SError *po_error )
{
    ASSUME( cp_pnn && file );

    file->handle = INVALID_HANDLE_VALUE;

    SError o_error = Error::Get( Error::InvalidArgument );
    DWORD dwFlagsAndAttributes = 0;
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition;
    HANDLE h_file;
    char a_absPath[ MAX_PATH ];
    size_t absPathLen;

    if( (procMode & (ProcMode::Read | ProcMode::Write)) == 0 )
    {
        goto toExit;
    }
    if( procMode & ProcMode::Write )
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }
    else if( procMode & ProcMode::Append )
    {
        goto toExit;
    }
    if( procMode & ProcMode::Read )
    {
        dwDesiredAccess |= GENERIC_READ;
    }
    else if( procMode & ProcMode::SequentialScan )
    {
        goto toExit;
    }

    if( openMode == OpenMode::CreateIfNotExists )
    {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else if( openMode == OpenMode::CreateAlways )
    {
        dwCreationDisposition = CREATE_ALWAYS;
    }
    else  //  if( openMode == OpenMode::OpenExisting )
    {
        ASSUME( openMode == OpenMode::OpenExisting );
        dwCreationDisposition = OPEN_EXISTING;
    }

    if( procMode & ProcMode::SequentialScan )
    {
        dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
    }

    h_file = ::CreateFileA( cp_pnn, dwDesiredAccess, FILE_SHARE_READ, 0, dwCreationDisposition, dwFlagsAndAttributes, 0 );
    if( h_file == INVALID_HANDLE_VALUE )
    {
        DWORD error = ::GetLastError();
        if( error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND )
        {
            o_error = Error::Get( Error::DoesNotExists );
        }
        else if( error == ERROR_ACCESS_DENIED || error == ERROR_WRITE_PROTECT || error == ERROR_SHARING_VIOLATION || error == ERROR_LOCK_VIOLATION )
        {
            o_error = Error::Get( Error::NoAccess );
        }
        else if( error == ERROR_NOT_ENOUGH_MEMORY || error == ERROR_OUTOFMEMORY )
        {
            o_error = Error::Get( Error::OutOfMemory );
        }
        else
        {
            o_error = Error::GetOther( FileError::CanNotOpenFile, Private::GetErrorsDesc() );
        }
        goto toExit;
    }

    if( (procMode & ProcMode::Append) && (openMode != OpenMode::CreateAlways) )
    {
        if( ::SetFilePointer( h_file, 0, 0, FILE_END ) == INVALID_SET_FILE_POINTER )
        {
            o_error = Error::Get( Error::Unknown );
            BOOL result = ::CloseHandle( h_file );
            ASSUME( result );
            goto toExit;
        }
    }

    absPathLen = Files::AbsolutePath( cp_pnn, a_absPath ) + 1;

    file->pnn.Assign( a_absPath, absPathLen );
    file->openMode = openMode;
    file->procMode = procMode;
    file->handle = h_file;
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;

    o_error = Error::Get( Error::Ok );

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

bool FileIO::Private::IsValid( const CFileBasis *file )
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

    if( file->is_reading )
    {
        return o_pos.QuadPart - (LONGLONG)file->bufferPos;
    }
    return o_pos.QuadPart + (LONGLONG)file->bufferPos;
}

NOINLINE i64 FileIO::Private::OffsetSet( CFileBasis *file, OffsetMode::OffsetMode_t mode, i64 offset, SError *po_error )
{
    ASSUME( IsValid( file ) );
    SError o_error = Error::Get( Error::Ok );
    i64 result = -1;
    LARGE_INTEGER o_move;
    DWORD moveMethod;

    if( !CancelCachedRead( file ) )
    {
        o_error = Error::Get( Error::Unknown );
        goto toExit;
    }
    if( !Flush( file ) )
    {
        o_error = Error::Get( Error::Unknown );
        goto toExit;
    }

    if( mode == OffsetMode::FromBegin )
    {
        moveMethod = FILE_BEGIN;
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
        o_error = Error::Get( Error::InvalidArgument );
        goto toExit;
    }
    o_move.QuadPart = offset;

    if( !::SetFilePointerEx( file->handle, o_move, &o_move, moveMethod ) )
    {
        o_error = Error::Get( Error::Unknown );
        goto toExit;
    }

    result = o_move.QuadPart;

toExit:
    DSA( po_error, o_error );
    return result;
}

ui64 FileIO::Private::SizeGet( CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    LARGE_INTEGER o_size;
    if( !::GetFileSizeEx( file->handle, &o_size ) )
    {
        return 0;
    }
    return o_size.QuadPart;
}

bool FileIO::Private::SizeSet( CFileBasis *file, ui64 newSize )
{
    ASSUME( IsValid( file ) );
    LARGE_INTEGER o_userOffset;
    if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_userOffset, FILE_BEGIN ) )
    {
        return false;
    }
    if( o_userOffset.QuadPart != newSize )
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
    ASSUME( o_userOffset.QuadPart >= 0 );
    if( o_userOffset.QuadPart < newSize )
    {
        if( !::SetFilePointerEx( file->handle, LARGE_INTEGER(), &o_userOffset, FILE_BEGIN ) )
        {
            return false;
        }
    }

    return true;
}

FileIO::OpenMode::OpenMode_t FileIO::Private::OpenModeGet( const CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    return file->openMode;
}

FileIO::ProcMode::ProcMode_t FileIO::Private::ProcModeGet( const CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    return file->procMode;
}

unsigned int FileIO::Private::PNNGet( const CFileBasis *file, char *p_buf )
{
    ASSUME( IsValid( file ) );
    if( p_buf )
    {
        _MemCpy( p_buf, file->pnn.Data(), file->pnn.Size() + 1 );
    }
    return file->pnn.Size();
}

NOINLINE bool FileIO::Private::WriteToFile( FileIO::Private::CFileBasis *file, const void *cp_source, unsigned int len )
{
    ASSUME( FileIO::Private::IsValid( file ) && (cp_source || len == 0) );
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

NOINLINE bool FileIO::Private::ReadFromFile( FileIO::Private::CFileBasis *file, void *p_target, unsigned int len, unsigned int *p_readed )
{
    ASSUME( FileIO::Private::IsValid( file ) && (p_target || len == 0) );
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

NOINLINE bool FileIO::Private::CancelCachedRead( FileIO::Private::CFileBasis *file )
{
    ASSUME( FileIO::Private::IsValid( file ) );
    if( !file->is_reading || file->bufferPos == file->readBufferCurrentSize )
    {
        return true;
    }
    int move = (int)file->bufferPos - (int)file->readBufferCurrentSize;
    LARGE_INTEGER o_move;
    o_move.QuadPart = move;
    BOOL result = ::SetFilePointerEx( file->handle, o_move, 0, FILE_CURRENT );
    file->bufferPos = file->readBufferCurrentSize = file->bufferSize;
    return result != FALSE;
}

#endif WINDOWS
