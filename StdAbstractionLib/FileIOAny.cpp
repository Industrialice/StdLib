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
    _MemCpy( file->bufferRef + file->bufferPos, what, howMuch );
    file->stats.bytesToBufferWritten += howMuch;
    ++file->stats.writesToBufferCount;
    file->bufferPos += howMuch;
}

static void FORCEINLINE ReadFromBuffer( FileIO::Private::CFileBasis *file, void *target, ui32 howMuch )
{
    _MemCpy( target, file->bufferRef + file->bufferPos, howMuch );
    file->stats.bytesFromBufferReaded += howMuch;
    ++file->stats.readsFromBufferCount;
    file->bufferPos += howMuch;
}

void FileIO::Private::FileIO_Initialize( CFileBasis *file )
{
    file->handle = fileHandle_undefined;
	file->bufferRef = 0;
	file->bufferSize = 0;
}

void FileIO::Private::FileIO_Destroy( CFileBasis *file )
{
    FileIO_Close( file );
	file->internalBuffer = 0;
	#ifdef WINDOWS
		file->pnn = 0;
	#endif
}

NOINLINE bln FileIO::Private::FileIO_Write( CFileBasis *file, const void *cp_source, ui32 len )
{
    ASSUME( FileIO_IsValid( file ) && (cp_source || len == 0) );
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
            if( !FileIO_Flush( file ) )
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

NOINLINE bln FileIO::Private::FileIO_Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( FileIO_IsValid( file ) && (p_target || len == 0) );
    DSA( p_readed, 0 );
    if( file->bufferSize )
    {
        ++file->stats.bufferedReads;
        if( !FileIO_Flush( file ) )
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
            if( !ReadFromFile( file, file->bufferRef, file->bufferSize, &file->readBufferCurrentSize ) )
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

NOINLINE bln FileIO::Private::FileIO_BufferSet( CFileBasis *file, ui32 size, void *buffer )
{
    ASSUME( FileIO_IsValid( file ) );
	ASSUME( size || buffer == 0 );
	if( buffer == 0 && file->bufferRef == file->internalBuffer && size == file->bufferSize )
    {
        return true;
    }
    if( !FileIO_Flush( file ) )
    {
        return false;
    }
    if( !CancelCachedRead( file ) )
    {
        return false;
    }
    if( buffer )
    {
		file->internalBuffer = 0;
		file->bufferRef = (byte *)buffer;
    }
    else
    {
		if( size == 0 )
		{
			file->internalBuffer = 0;
		}
		else
		{
			file->internalBuffer = (byte *)::realloc( file->internalBuffer, size );
		}
		file->bufferRef = file->internalBuffer;
    }
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;
    file->bufferSize = size;
    return true;
}

ui32 FileIO::Private::FileIO_BufferSizeGet( CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    return file->bufferSize;
}

NOINLINE bln FileIO::Private::FileIO_Flush( CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) && file->bufferPos <= file->bufferSize );
    if( !file->is_reading && file->bufferPos )
    {
        bln result = WriteToFile( file, file->bufferRef, file->bufferPos );
        if( result )
        {
            file->bufferPos = 0;
        }
        return result;
    }
    return true;
}

void FileIO::Private::FileIO_StatsGet( const CFileBasis *file, SStats *po_stats )
{
    ASSUME( FileIO_IsValid( file ) && po_stats );
    _MemCpy( po_stats, &file->stats, sizeof(SStats) );
}

void FileIO::Private::FileIO_StatsReset( CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    _Clear( &file->stats );
}

FileIO::OpenMode::OpenMode_t FileIO::Private::FileIO_OpenModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->openMode;
}

FileIO::ProcMode::ProcMode_t FileIO::Private::FileIO_ProcModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->procMode;
}

FileIO::CacheMode::CacheMode_t FileIO::Private::FileIO_CacheModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->cacheMode;
}