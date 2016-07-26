#ifndef __FILE_IO_HPP__
#define __FILE_IO_HPP__

#include "FilePath.hpp"
#include <FileInterface.hpp>

namespace StdLib {

namespace FileIO
{
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

	typedef CTError < const char * > fileError;

    namespace Private
    {
        struct CFileBasis
        {
            fileHandle handle;

            FileIO::SStats stats;

			ui64 offsetToStart;  //  used only when you're using ProcMode::Append, the file will be opened as usual, then the offset will be added so you can't work with the existing part of the file

			UniquePtr < byte, MallocDeleter > internalBuffer;
			byte *bufferRef;
			ui32 bufferSize;
			ui32 bufferPos;
			ui32 readBufferCurrentSize;  //  can be lower than bufferSize if, for example, EOF is reached
            bln is_reading;

			FileOpenMode::mode_t openMode;
			FileProcMode::mode_t procMode;
			FileCacheMode::mode_t cacheMode;

			#ifdef WINDOWS
				UniquePtr < FilePath > pnn;  //  used only on WindowsXP where you can't get PNN from the file handle
			#endif
        };

        /*  Core Functions  */
        EXTERNALD void FileIO_Initialize( CFileBasis *file );
        EXTERNALD void FileIO_Destroy( CFileBasis *file );
        EXTERNALD bln FileIO_Open( CFileBasis *file, const FilePath &pnn, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode, fileError *po_error );
        EXTERNALD void FileIO_Close( CFileBasis *file );
		EXTERNALD bln FileIO_IsValid( const CFileBasis *file );
        EXTERNALD bln FileIO_Write( CFileBasis *file, const void *cp_source, ui32 len, ui32 *written );
        EXTERNALD bln FileIO_Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
        EXTERNALD bln FileIO_BufferSet( CFileBasis *file, ui32 size, void *buffer );  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
		EXTERNALD ui32 FileIO_BufferSizeGet( const CFileBasis *file );
		EXTERNALD const void *FileIO_BufferGet( const CFileBasis *file );
        EXTERNALD void FileIO_StatsGet( const CFileBasis *file, SStats *po_stats );
        EXTERNALD void FileIO_StatsReset( CFileBasis *file );
        EXTERNALD bln FileIO_Flush( CFileBasis *file );  //  false if writing to file failed to complete
        EXTERNALD i64 FileIO_OffsetGet( CFileBasis *file, FileOffsetMode::mode_t mode, CError *error );
        EXTERNALD i64 FileIO_OffsetSet( CFileBasis *file, FileOffsetMode::mode_t mode, i64 offset, CError *error );
		EXTERNALD ui64 FileIO_SizeGet( const CFileBasis *file, CError *error );  //  returns 0 on error
        EXTERNALD bln FileIO_SizeSet( CFileBasis *file, ui64 newSize, CError *error );
		EXTERNALD FileOpenMode::mode_t FileIO_OpenModeGet( const CFileBasis *file );
		EXTERNALD FileProcMode::mode_t FileIO_ProcModeGet( const CFileBasis *file );
		EXTERNALD FileCacheMode::mode_t FileIO_CacheModeGet( const CFileBasis *file );
        EXTERNALD FilePath FileIO_PNNGet( const CFileBasis *file );

		EXTERNALD void FileIO_InitializeFileIOSystem();
    }

    class CFile final : private Private::CFileBasis, public FileInterface
    {
		CFile( const CFile &source );
		CFile & operator = ( const CFile &source );

	public:
        ~CFile()
        {
            Private::FileIO_Destroy( this );
        }

        CFile()
        {
            Private::FileIO_Initialize( this );
        }

        CFile( const FilePath &pnn, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode = FileCacheMode::Default, fileError *po_error = 0 )
        {
            Private::FileIO_Initialize( this );
            Private::FileIO_Open( this, pnn, openMode, procMode, cacheMode, po_error );
        }

#ifdef MOVE_SUPPORTED
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
#endif

        void Open( const FilePath &pnn, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode = FileCacheMode::Default, fileError *po_error = 0 )
        {
            Close();
            Private::FileIO_Open( this, pnn, openMode, procMode, cacheMode, po_error );
        }

        void StatsGet( SStats *po_stats ) const
        {
            Private::FileIO_StatsGet( this, po_stats );
        }

        void StatsReset()
        {
            Private::FileIO_StatsReset( this );
        }

        FileOpenMode::mode_t OpenModeGet() const
        {
            return Private::FileIO_OpenModeGet( this );
        }

		FilePath PNNGet() const
        {
            return Private::FileIO_PNNGet( this );
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

        virtual bln BufferSet( ui32 size, void *buffer = 0 ) override
        {
            return Private::FileIO_BufferSet( this, size, buffer );
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

        virtual i64 OffsetGet( FileOffsetMode::mode_t mode = FileOffsetMode::FromBegin, CError *error = 0 ) override
        {
            return Private::FileIO_OffsetGet( this, mode, error );
        }

        virtual i64 OffsetSet( FileOffsetMode::mode_t mode, i64 offset, CError *error = 0 ) override
        {
            return Private::FileIO_OffsetSet( this, mode, offset, error );
        }

        virtual ui64 SizeGet( CError *error = 0) const override
        {
            return Private::FileIO_SizeGet( this, error );
        }

        virtual bln SizeSet( ui64 newSize, CError *error = 0 ) override
        {
            return Private::FileIO_SizeSet( this, newSize, error );
        }

        virtual FileProcMode::mode_t ProcModeGet() const override
        {
            return Private::FileIO_ProcModeGet( this );
        }

		virtual FileCacheMode::mode_t CacheModeGet() const override
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
			target->stats = this->stats;
			target->bufferRef = this->bufferRef;
			target->internalBuffer = this->internalBuffer.TakeAway();
			target->is_reading = this->is_reading;
			target->bufferSize = this->bufferSize;
			target->bufferPos = this->bufferPos;
			target->readBufferCurrentSize = this->readBufferCurrentSize;
			#ifdef WINDOWS
				target->pnn = this->pnn.TakeAway();
			#endif

			Private::FileIO_Initialize( this );
		}
        
#ifdef MOVE_SUPPORTED
    private:
        void Transfer( CFile &&source )
        {
            this->handle = source.handle;
			this->offsetToStart = source.offsetToStart;
            this->openMode = source.openMode;
            this->procMode = source.procMode;
			this->cacheMode = source.cacheMode;
            this->stats = source.stats;
			if( source.internalBuffer == source.bufferRef )
			{
				this->internalBuffer = std::move( source.internalBuffer );
				this->bufferRef = this->internalBuffer;
			}
			else
			{
				this->internalBuffer = 0;
				this->bufferRef = source.bufferRef;
			}
            this->is_reading = source.is_reading;
            this->bufferSize = source.bufferSize;
            this->bufferPos = source.bufferPos;
            this->readBufferCurrentSize = source.readBufferCurrentSize;
			#ifdef WINDOWS
				this->pnn = std::move( source.pnn );
			#endif

            Private::FileIO_Initialize( &source );
        }
#endif
    };
}

}  //  namespace StdLib

#endif
