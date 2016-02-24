#ifndef __FILE_IO_HPP__
#define __FILE_IO_HPP__

#include <StdCoreLib.hpp>
#include <Error.hpp>
#include <CString.hpp>

namespace StdLib {

namespace FileIO
{
    namespace OpenMode
    {
        CONSTS( OpenMode_t,
                CreateIfDoesNotExist,
                CreateAlways,
				CreateNew,  //  creates a new file only if such file doesn't exist
                OpenExisting )
    }

    namespace ProcMode
    {
        CONSTS_OPED( ProcMode_t,
                Read = BIT( 1 ),
                Write = BIT( 2 ),
                Append = BIT( 3 ) )  //  requires Write, will set the pointer to the end of the file when opening, makes existing part of the file virtually invisible( isn't reported with Size calls, isn't accessible through offset sets )
    }

	namespace CacheMode
	{
		CONSTS_OPED( CacheMode_t,
			Default = 0,
			LinearRead = BIT( 1 ),  //  requires ProcMode::Read, can't be used with RandomRead
			RandomRead = BIT( 2 ),  //  requires ProcMode::Read, can't be used with LinearRead
			DisableSystemWriteCache = BIT( 3 ) )  //  requires ProcMode::Write
	}

    namespace OffsetMode
    {
        CONSTS( OffsetMode_t,
                FromBegin,
                FromCurrent,
                FromEnd )
    }

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
        struct CFileBasis : CharMovable
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

			OpenMode::OpenMode_t openMode;
			ProcMode::ProcMode_t procMode;
			CacheMode::CacheMode_t cacheMode;

			#ifdef WINDOWS
				UniquePtr < CStr > pnn;  //  used only on WindowsXP where you can't get PNN from the file handle
			#endif
        };

        /*  Core Functions  */
        EXTERNALS void Initialize( CFileBasis *file );
        EXTERNALS void Destroy( CFileBasis *file );
        EXTERNALS bln Open( CFileBasis *file, const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, CacheMode::CacheMode_t cacheMode, fileError *po_error );
        EXTERNALS void Close( CFileBasis *file );
        EXTERNALS bln IsValid( const CFileBasis *file );
        EXTERNALS bln Write( CFileBasis *file, const void *cp_source, ui32 len );
        EXTERNALS bln Read( CFileBasis *file, void *p_target, ui32 len, ui32 *p_readed );
        EXTERNALS bln BufferSet( CFileBasis *file, ui32 size, void *buffer = 0 );  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
        EXTERNALS ui32 BufferSizeGet( CFileBasis *file );
        EXTERNALS void StatsGet( const CFileBasis *file, SStats *po_stats );
        EXTERNALS void StatsReset( CFileBasis *file );
        EXTERNALS bln Flush( CFileBasis *file );  //  false if writing to file failed to complete
        EXTERNALS i64 OffsetGet( CFileBasis *file );  //  -1 on fail, current offset on success
        EXTERNALS i64 OffsetSet( CFileBasis *file, OffsetMode::OffsetMode_t mode, i64 offset, CError *po_error );  //  -1 on fail, current offset on success, will Flush buffers
        EXTERNALS ui64 SizeGet( CFileBasis *file, CError *error = 0 );  //  returns 0 on error
        EXTERNALS bln SizeSet( CFileBasis *file, ui64 newSize );
        EXTERNALS OpenMode::OpenMode_t OpenModeGet( const CFileBasis *file );
        EXTERNALS ProcMode::ProcMode_t ProcModeGet( const CFileBasis *file );
		EXTERNALS CacheMode::CacheMode_t CacheModeGet( const CFileBasis *file );
        EXTERNALS ui32 PNNGet( const CFileBasis *file, char *p_buf );  //  pass 0 as p_buf to get only len, returns 0 at failure

		EXTERNALS void Initialize();
    }

    class CFile : private Private::CFileBasis
    {
		CFile( const CFile &source );
		CFile & operator = ( const CFile &source );

	public:
        ~CFile()
        {
            Private::Destroy( this );
        }

        CFile()
        {
            Private::Initialize( this );
        }

        CFile( const char *pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, CacheMode::CacheMode_t cacheMode = CacheMode::Default, fileError *po_error = 0 )
        {
            Private::Initialize( this );
            Private::Open( this, pnn, openMode, procMode, cacheMode, po_error );
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

        void Open( const char *pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, CacheMode::CacheMode_t cacheMode = CacheMode::Default, fileError *po_error = 0 )
        {
            Close();
            Private::Open( this, pnn, openMode, procMode, cacheMode, po_error );
        }

        void Close()
        {
            Private::Close( this );
        }

        bln IsOpened() const
        {
            return Private::IsValid( this );
        }

        bln Write( const void *cp_source, ui32 len )
        {
            return Private::Write( this, cp_source, len );
        }

        bln Read( void *p_target, ui32 len, ui32 *p_readed )
        {
            return Private::Read( this, p_target, len, p_readed );
        }

        bln BufferSet( ui32 size, void *buffer = 0 )  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
        {
            return Private::BufferSet( this, size, buffer );
        }

        ui32 BufferSizeGet()
        {
            return Private::BufferSizeGet( this );
        }

        void StatsGet( SStats *po_stats ) const
        {
            Private::StatsGet( this, po_stats );
        }

        void StatsReset()
        {
            Private::StatsReset( this );
        }

        bln Flush()  //  false if writing to file failed to complete
        {
            return Private::Flush( this );
        }

        i64 OffsetGet()  //  -1 on fail, current offset on success
        {
            return Private::OffsetGet( this );
        }

        i64 OffsetSet( OffsetMode::OffsetMode_t mode, i64 offset, CError *po_error )  //  -1 on fail, current offset on success
        {
            return Private::OffsetSet( this, mode, offset, po_error );
        }

        ui64 SizeGet()
        {
            return Private::SizeGet( this );
        }

        bln SizeSet( ui64 newSize )
        {
            return Private::SizeSet( this, newSize );
        }

        OpenMode::OpenMode_t OpenModeGet() const
        {
            return Private::OpenModeGet( this );
        }

        ProcMode::ProcMode_t ProcModeGet() const
        {
            return Private::ProcModeGet( this );
        }

		CacheMode::CacheMode_t CacheModeGet() const
		{
			return Private::CacheModeGet( this );
		}

        ui32 PNNGet( char *p_buf ) const  //  pass 0 as p_buf to get only len, will return 0 on error
        {
            return Private::PNNGet( this, p_buf );
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

			Private::Initialize( this );
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

            Private::Initialize( &source );
        }
#endif
    };
}

}  //  namespace StdLib

#endif
