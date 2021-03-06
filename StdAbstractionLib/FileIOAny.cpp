#include "PreHeader.hpp"
#include "FileIO.hpp"

namespace StdLib
{
    namespace FileIO
    {
        namespace Private
        {
            bool WriteToFile( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written );
            bool ReadFromFile( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
            bool CancelCachedRead( CFileBasis *file );
        }
    }
}

static void FORCEINLINE WriteToBuffer( FileIO::Private::CFileBasis *file, const void *what, ui32 howMuch )
{
    _MemCpy( file->internalBuffer.get() + file->bufferPos, what, howMuch );
    FILEIO_STAT( file->stats.bytesToBufferWritten += howMuch; )
    FILEIO_STAT( ++file->stats.writesToBufferCount; )
    file->bufferPos += howMuch;
}

static void FORCEINLINE ReadFromBuffer( FileIO::Private::CFileBasis *file, void *target, ui32 howMuch )
{
    _MemCpy( target, file->internalBuffer.get() + file->bufferPos, howMuch );
    FILEIO_STAT( file->stats.bytesFromBufferReaded += howMuch; )
    FILEIO_STAT( ++file->stats.readsFromBufferCount; )
    file->bufferPos += howMuch;
}

void FileIO::Private::FileIO_Initialize( CFileBasis *file )
{
    file->handle = fileHandle_undefined;
	file->internalBuffer.reset();
	file->bufferSize = 0;
	#ifdef WINDOWS
		file->pnn = FilePath();
	#endif
}

void FileIO::Private::FileIO_Destroy( CFileBasis *file )
{
    FileIO_Close( file );
	file->internalBuffer.reset();
	#ifdef WINDOWS
		file->pnn = FilePath();
	#endif
}

NOINLINE bool FileIO::Private::FileIO_Write( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written )
{
    ASSUME( FileIO_IsValid( file ) && (cp_source || len == 0) && !!(file->procMode & FileProcMode::Write) );
	DSA( written, 0 );
    if( file->bufferSize )
    {
        FILEIO_STAT( ++file->stats.bufferedWrites; )
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
                return WriteToFile( file, cp_source, len, written );
            }
        }
        WriteToBuffer( file, cp_source, len );
		DSA( written, len );
        return true;
    }
    FILEIO_STAT( ++file->stats.unbufferedWrites; )
    return WriteToFile( file, cp_source, len, written );
}

NOINLINE bool FileIO::Private::FileIO_Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed )
{
    ASSUME( FileIO_IsValid( file ) && (p_target || len == 0) && !!(file->procMode & FileProcMode::Read) );
    DSA( p_readed, 0 );
    if( file->bufferSize )
    {
        FILEIO_STAT( ++file->stats.bufferedReads; )
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
            if( !ReadFromFile( file, file->internalBuffer.get(), file->bufferSize, &file->readBufferCurrentSize ) )
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
    FILEIO_STAT( ++file->stats.unbufferedReads; )
    return ReadFromFile( file, p_target, len, p_readed );
}

NOINLINE bool FileIO::Private::FileIO_BufferSet( CFileBasis *file, ui32 size, decltype(CFileBasis::internalBuffer) buffer )
{
    ASSUME( FileIO_IsValid( file ) );
	ASSUME( size || buffer.get() == nullptr );

	if( buffer.get() == file->internalBuffer.get() && buffer.get_deleter() == file->internalBuffer.get_deleter() && size == file->bufferSize )
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
		file->internalBuffer = std::move( buffer );
    }
    else
    {
		if( size == 0 )
		{
			file->internalBuffer.reset();
		}
		else
		{
			file->internalBuffer = decltype(file->internalBuffer)( (byte *)malloc( size ), []( byte *ptr ) { free( ptr ); } );
		}
    }
    file->bufferPos = 0;
    file->readBufferCurrentSize = 0;
    file->bufferSize = size;
    return true;
}

ui32 FileIO::Private::FileIO_BufferSizeGet( const CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) );
    return file->bufferSize;
}
		
const void *FileIO::Private::FileIO_BufferGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->internalBuffer.get();
}

NOINLINE bool FileIO::Private::FileIO_Flush( CFileBasis *file )
{
    ASSUME( FileIO_IsValid( file ) && file->bufferPos <= file->bufferSize );
    if( !file->is_reading && file->bufferPos )
    {
        bool result = WriteToFile( file, file->internalBuffer.get(), file->bufferPos, 0 );
        if( result )
        {
            file->bufferPos = 0;
        }
        return result;
    }
    return true;
}

#ifdef ENABLE_FILEIO_STATS
	void FileIO::Private::FileIO_StatsGet( const CFileBasis *file, SStats *po_stats )
	{
		ASSUME( FileIO_IsValid( file ) && po_stats );
		_MemCpy( po_stats, &file->stats, sizeof(SStats) );
	}

	void FileIO::Private::FileIO_StatsReset( CFileBasis *file )
	{
		ASSUME( FileIO_IsValid( file ) );
		Funcs::ClearPod( &file->stats );
	}
#endif

FileOpenMode FileIO::Private::FileIO_OpenModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->openMode;
}

FileProcMode FileIO::Private::FileIO_ProcModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->procMode;
}

FileCacheMode FileIO::Private::FileIO_CacheModeGet( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->cacheMode;
}

fileHandle FileIO::Private::FileIO_FileHandle( const CFileBasis *file )
{
	ASSUME( FileIO_IsValid( file ) );
	return file->handle;
}