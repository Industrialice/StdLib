#include "PreHeader.hpp"
#include "FileIO.hpp"

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

static void FORCEINLINE WriteToBuffer( FileIO::Private::CFileBasis *file, const void *what, ui32 howMuch )
{
    _MemCpy( file->buffer + file->bufferPos, what, howMuch );
    file->stats.bytesToBufferWritten += howMuch;
    ++file->stats.writesToBufferCount;
    file->bufferPos += howMuch;
}

static void FORCEINLINE ReadFromBuffer( FileIO::Private::CFileBasis *file, void *target, ui32 howMuch )
{
    _MemCpy( target, file->buffer + file->bufferPos, howMuch );
    file->stats.bytesFromBufferReaded += howMuch;
    ++file->stats.readsFromBufferCount;
    file->bufferPos += howMuch;
}

void FileIO::Private::Initialize( CFileBasis *file )
{
    file->handle = fileHandle_undefined;
    _Clear( &file->stats );
    file->buffer = 0;
    file->is_customBuffer = false;
    file->bufferSize = 0;
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;
    file->is_reading = false;
}

void FileIO::Private::Destroy( CFileBasis *file )
{
    Close( file );
    if( file->is_customBuffer )
    {
        ::free( file->buffer );
    }
}

NOINLINE bln FileIO::Private::Write( CFileBasis *file, const void *cp_source, ui32 len )
{
    ASSUME( IsValid( file ) && (cp_source || len == 0) );
    if( file->bufferSize )
    {
        ++file->stats.bufferedWrites;
        if( !CancelCachedRead( file ) )
        {
            return false;
        }
        file->is_reading = false;

        if( file->bufferSize - file->bufferPos < len )
        {
            ui32 cpyLen = file->bufferSize - file->bufferPos;
            WriteToBuffer( file, cp_source, cpyLen );
            if( !Flush( file ) )
            {
                return false;
            }

            len -= cpyLen;
            cp_source = (byte *)cp_source + cpyLen;
            if( len >= file->bufferSize )
            {
                return WriteToFile( file, cp_source, len );
            }
        }
        WriteToBuffer( file, cp_source, len );
        return true;
    }
    ++file->stats.unbufferedWrites;
    return WriteToFile( file, cp_source, len );
}

NOINLINE bln FileIO::Private::Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( IsValid( file ) && (p_target || len == 0) );
    DSA( p_readed, 0 );
    if( file->bufferSize )
    {
        ++file->stats.bufferedReads;
        if( !Flush( file ) )
        {
            return false;
        }
        file->is_reading = true;

        if( file->readBufferCurrentSize - file->bufferPos < len )
        {
            ui32 cpyLen = file->readBufferCurrentSize - file->bufferPos;
            ReadFromBuffer( file, p_target, cpyLen );
            DSA( p_readed, cpyLen );
            len -= cpyLen;
            p_target = (byte *)p_target + cpyLen;

            if( len >= file->bufferSize )
            {
                return ReadFromFile( file, p_target, len, p_readed );
            }

            file->readBufferCurrentSize = 0;
            if( !ReadFromFile( file, file->buffer, file->bufferSize, &file->readBufferCurrentSize ) )
            {
                return false;
            }
            file->bufferPos = 0;
        }

        ui32 cpyLen = Funcs::Min < ui32 >( file->readBufferCurrentSize - file->bufferPos, len );
        ReadFromBuffer( file, p_target, cpyLen );
        if( p_readed )
        {
            *p_readed += cpyLen;
        }
        return true;
    }
    ++file->stats.unbufferedReads;
    return ReadFromFile( file, p_target, len, p_readed );
}

NOINLINE bln FileIO::Private::BufferSet( CFileBasis *file, ui32 size, void *buffer )
{
    ASSUME( IsValid( file ) );
    if( buffer == 0 && file->is_customBuffer && size == file->bufferSize )
    {
        return true;
    }
    if( !Flush( file ) )
    {
        return false;
    }
    if( !CancelCachedRead( file ) )
    {
        return false;
    }
    if( buffer )
    {
        if( file->is_customBuffer )
        {
            ::free( file->buffer );
            file->is_customBuffer = false;
        }
        file->buffer = (byte *)buffer;
    }
    else
    {
        if( file->is_customBuffer )
        {
            if( size == 0 )
            {
                ::free( file->buffer );
                file->buffer = 0;
            }
            else
            {
                file->buffer = (byte *)::realloc( file->buffer, size );
            }
        }
        else
        {
            if( size )
            {
                file->buffer = (byte *)::malloc( size );
                file->is_customBuffer = true;
            }
            else
            {
                file->buffer = 0;
            }
        }
    }
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;
    file->bufferSize = size;
    return true;
}

ui32 FileIO::Private::BufferSizeGet( CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    return file->bufferSize;
}

NOINLINE bln FileIO::Private::Flush( CFileBasis *file )
{
    ASSUME( IsValid( file ) && file->bufferPos <= file->bufferSize );
    if( !file->is_reading && file->bufferPos )
    {
        bln result = WriteToFile( file, file->buffer, file->bufferPos );
        if( result )
        {
            file->bufferPos = 0;
        }
        return result;
    }
    return true;
}

void FileIO::Private::StatsGet( const CFileBasis *file, SStats *po_stats )
{
    ASSUME( IsValid( file ) && po_stats );
    _MemCpy( po_stats, &file->stats, sizeof(SStats) );
}

void FileIO::Private::StatsReset( CFileBasis *file )
{
    ASSUME( IsValid( file ) );
    _Clear( &file->stats );
}