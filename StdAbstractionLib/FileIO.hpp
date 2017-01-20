#ifndef __FILE_IO_HPP__
#define __FILE_IO_HPP__

#define ENABLE_FILEIO_STATS

#ifdef ENABLE_FILEIO_STATS
	#define FILEIO_STAT( ... ) __VA_ARGS__
#else
	#define FILEIO_STAT( ... )
#endif

#include "FilePath.hpp"
#include <FileInterface.hpp>

namespace StdLib {

namespace FileIO
{
#ifdef ENABLE_FILEIO_STATS
    struct SStats
    {
		typedef ui64 counter_t;
		counter_t writesToBufferCount;
		counter_t writesToFileCount;
		counter_t readsFromBufferCount;
		counter_t readsFromFileCount;
		counter_t bytesFromBufferReaded;
		counter_t bytesFromFileReaded;
		counter_t bytesToBufferWritten;
		counter_t bytesToFileWritten;
		counter_t bufferedWrites;
		counter_t unbufferedWrites;
		counter_t bufferedReads;
		counter_t unbufferedReads;
    };
#endif

	typedef CError < const char * > fileError;

    namespace Private
    {
        struct CFileBasis
        {
            fileHandle handle;

            FILEIO_STAT( FileIO::SStats stats; )

			ui64 offsetToStart;  //  used only when you're using ProcMode::Append, the file will be opened as usual, then the offset will be added so you can't work with the existing part of the file

			std::unique_ptr < byte, void(*)(byte *) > internalBuffer = std::unique_ptr < byte, void(*)(byte *) >( nullptr, [](byte *){} );
			ui32 bufferSize;
			ui32 bufferPos;
			ui32 readBufferCurrentSize;  //  can be lower than bufferSize if, for example, EOF is reached
            bln is_reading;

			bln is_shouldCloseFileHandle;

			FileOpenMode openMode;
			FileProcMode procMode;
			FileCacheMode cacheMode;

			#ifdef WINDOWS
				FilePath pnn;  //  used only on WindowsXP where you can't get PNN from the file handle
			#endif
        };

        /*  Core Functions  */
        EXTERNALD void FileIO_Initialize( CFileBasis *file );
        EXTERNALD void FileIO_Destroy( CFileBasis *file );
        EXTERNALD fileError FileIO_Open( CFileBasis *file, const FilePath &pnn, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode );
        EXTERNALD fileError FileIO_OpenFromDescriptor( fileHandle osFileDescriptor, bln is_shouldCloseFileHandle );
        EXTERNALD void FileIO_Close( CFileBasis *file );
		EXTERNALD bln FileIO_IsValid( const CFileBasis *file );
        EXTERNALD bln FileIO_Write( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written );
        EXTERNALD bln FileIO_Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
        EXTERNALD bln FileIO_BufferSet( CFileBasis *file, ui32 size, decltype(CFileBasis::internalBuffer) buffer );  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
		EXTERNALD ui32 FileIO_BufferSizeGet( const CFileBasis *file );
		EXTERNALD const void *FileIO_BufferGet( const CFileBasis *file );
        FILEIO_STAT( EXTERNALD void FileIO_StatsGet( const CFileBasis *file, SStats *po_stats ); )
        FILEIO_STAT( EXTERNALD void FileIO_StatsReset( CFileBasis *file ); )
        EXTERNALD bln FileIO_Flush( CFileBasis *file );  //  false if writing to file failed to complete
        EXTERNALD CResult < i64 > FileIO_OffsetGet( CFileBasis *file, FileOffsetMode mode );
        EXTERNALD CResult < i64 > FileIO_OffsetSet( CFileBasis *file, FileOffsetMode mode, i64 offset );
		EXTERNALD CResult < ui64 > FileIO_SizeGet( const CFileBasis *file );  //  returns 0 on error
        EXTERNALD CError<> FileIO_SizeSet( CFileBasis *file, ui64 newSize );
		EXTERNALD FileOpenMode FileIO_OpenModeGet( const CFileBasis *file );
		EXTERNALD FileProcMode FileIO_ProcModeGet( const CFileBasis *file );
		EXTERNALD FileCacheMode FileIO_CacheModeGet( const CFileBasis *file );
        EXTERNALD FilePath FileIO_PNNGet( const CFileBasis *file );
		EXTERNALD fileHandle FileIO_FileHandle( const CFileBasis *file );
    }

    class CFile final : private Private::CFileBasis, public FileInterface
    {
		CFile( const CFile &source ) = delete;
		CFile & operator = ( const CFile &source ) = delete;

	public:
        ~CFile()
        {
            Private::FileIO_Destroy( this );
        }

        CFile()
        {
            Private::FileIO_Initialize( this );
        }

        CFile( const FilePath &pnn, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode = FileCacheMode::Default, fileError *error = 0 )
        {
			fileError junkError;
			if( error == nullptr )
			{
				error = &junkError;
			}
            Private::FileIO_Initialize( this );
            *error = Private::FileIO_Open( this, pnn, openMode, procMode, cacheMode );
        }

        CFile( fileHandle osFileDescriptor, bln is_shouldCloseFileHandle, fileError *error = 0 )
        {
			fileError junkError;
			if( error == nullptr )
			{
				error = &junkError;
			}
            Private::FileIO_Initialize( this );
            *error = Private::FileIO_OpenFromDescriptor( osFileDescriptor, is_shouldCloseFileHandle );
        }

        CFile( CFile &&source )
        {
            Transfer( std::move( source ) );
        }

        CFile &operator = ( CFile &&source )
        {
            ASSUME( this != &source );
            Transfer( std::move( source ) );
            return *this;
        }

        fileError Open( const FilePath &pnn, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode = FileCacheMode::Default )
        {
            Close();
            return Private::FileIO_Open( this, pnn, openMode, procMode, cacheMode );
        }

        fileError Open( fileHandle osFileDescriptor, bln is_shouldCloseFileHandle )
        {
            Close();
            return Private::FileIO_OpenFromDescriptor( osFileDescriptor, is_shouldCloseFileHandle );
        }

	#ifdef ENABLE_FILEIO_STATS
        void StatsGet( SStats *po_stats ) const
        {
            Private::FileIO_StatsGet( this, po_stats );
        }

        void StatsReset()
        {
            Private::FileIO_StatsReset( this );
        }
	#endif

        FileOpenMode OpenModeGet() const
        {
            return Private::FileIO_OpenModeGet( this );
        }

		FilePath PNNGet() const
        {
            return Private::FileIO_PNNGet( this );
        }

		fileHandle FileHandle() const
		{
			return Private::FileIO_FileHandle( this );
		}

		fileHandle CloseAndGetFileHandle()
		{
			this->is_shouldCloseFileHandle = false;
			fileHandle handle = Private::FileIO_FileHandle( this );
			Private::FileIO_Close( this );
			return handle;
		}

        virtual void Close() override
        {
            Private::FileIO_Close( this );
        }

        virtual bln IsOpened() const override
        {
            return Private::FileIO_IsValid( this );
        }

        virtual bln Write( const void *cp_source, ui32 len, ui32 *written = 0 ) override
        {
            return Private::FileIO_Write( this, cp_source, len, written );
        }

        virtual bln Read( void *p_target, ui32 len, ui32 *p_readed = 0 ) override
        {
            return Private::FileIO_Read( this, p_target, len, p_readed );
        }

		virtual bln IsBufferingSupported() const override
		{
			return true;
		}

        virtual bln BufferSet( ui32 size, std::unique_ptr < byte, void(*)(byte *) > &&buffer = std::unique_ptr < byte, void(*)(byte *) >( nullptr, [](byte *){} ) ) override
        {
            return Private::FileIO_BufferSet( this, size, std::move( buffer ) );
        }

        virtual ui32 BufferSizeGet() const override
        {
            return Private::FileIO_BufferSizeGet( this );
        }

		virtual const void *BufferGet() const override
		{
			return Private::FileIO_BufferGet( this );
		}

        virtual bln Flush() override  //  false if writing to the file has failed to complete
        {
            return Private::FileIO_Flush( this );
        }

		virtual bln IsSeekSupported() const override
		{
			return true;
		}

        virtual CResult < i64 > OffsetGet( FileOffsetMode mode = FileOffsetMode::FromBegin ) override
        {
            return Private::FileIO_OffsetGet( this, mode );
        }

        virtual CResult < i64 > OffsetSet( FileOffsetMode mode, i64 offset ) override
        {
            return Private::FileIO_OffsetSet( this, mode, offset );
        }

        virtual CResult < ui64 > SizeGet() const override
        {
            return Private::FileIO_SizeGet( this );
        }

        virtual CError<> SizeSet( ui64 newSize ) override
        {
            return Private::FileIO_SizeSet( this, newSize );
        }

        virtual FileProcMode ProcModeGet() const override
        {
            return Private::FileIO_ProcModeGet( this );
        }

		virtual FileCacheMode CacheModeGet() const override
		{
			return Private::FileIO_CacheModeGet( this );
		}

		void TransferTo( CFile *target )  //  use this method if std::move isn't supported
		{
			target->Close();

			target->handle = this->handle;
			target->offsetToStart = this->offsetToStart;
			target->openMode = this->openMode;
			target->procMode = this->procMode;
			target->cacheMode = this->cacheMode;
			FILEIO_STAT( target->stats = this->stats; )
			target->internalBuffer = decltype(target->internalBuffer)( this->internalBuffer.release(), this->internalBuffer.get_deleter() );
			target->is_reading = this->is_reading;
			target->is_shouldCloseFileHandle = this->is_shouldCloseFileHandle;
			target->bufferSize = this->bufferSize;
			target->bufferPos = this->bufferPos;
			target->readBufferCurrentSize = this->readBufferCurrentSize;
			#ifdef WINDOWS
				target->pnn = this->pnn;
			#endif

			Private::FileIO_Initialize( this );
		}
        
    private:
        void Transfer( CFile &&source )
        {
            this->handle = source.handle;
			this->offsetToStart = source.offsetToStart;
            this->openMode = source.openMode;
            this->procMode = source.procMode;
			this->cacheMode = source.cacheMode;
            FILEIO_STAT( this->stats = source.stats; )
			this->internalBuffer = std::move( source.internalBuffer );
            this->is_reading = source.is_reading;
			this->is_shouldCloseFileHandle = source.is_shouldCloseFileHandle;
            this->bufferSize = source.bufferSize;
            this->bufferPos = source.bufferPos;
            this->readBufferCurrentSize = source.readBufferCurrentSize;
			#ifdef WINDOWS
				this->pnn = std::move( source.pnn );
			#endif

            Private::FileIO_Initialize( &source );
        }
    };
}

}  //  namespace StdLib

#endif
