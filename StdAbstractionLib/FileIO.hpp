#ifndef __FILE_IO_HPP__
#define __FILE_IO_HPP__

#include <StdCoreLib.hpp>
#include <Error.hpp>
#include <CString.hpp>

namespace StdLib {

namespace FileIO
{
    struct CFile;

    namespace OpenMode
    {
        CONSTS( OpenMode_t,
                CreateIfNotExists,
                CreateAlways,
                OpenExisting )
    }

    namespace ProcMode
    {
        CONSTS_OPED( ProcMode_t,
                Read = BIT( 1 ),
                Write = BIT( 2 ),
                SequentialScan = BIT( 3 ),  /*  required Read  */
                Append = BIT( 4 ) )  /*  required Write, will set the pointer to the end of the file when opening  */
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
        unsigned int writesToBufferCount;
        unsigned int writesToFileCount;
        unsigned int readsFromBufferCount;
        unsigned int readsFromFileCount;
        unsigned int bytesFromBufferReaded;
        unsigned int bytesFromFileReaded;
        unsigned int bytesToBufferWritten;
        unsigned int bytesToFileWritten;
        unsigned int bufferedWrites;
        unsigned int unbufferedWrites;
        unsigned int bufferedReads;
        unsigned int unbufferedReads;
    };

    namespace Private
    {
        struct CFileBasis : CharMovable
        {
            HANDLE handle;
            OpenMode::OpenMode_t openMode;
            FileIO::ProcMode::ProcMode_t procMode;

            FileIO::SStats stats;

            byte *buffer;
            bool is_customBuffer;
            bool is_reading;
            unsigned int bufferSize;
            unsigned int bufferPos;
            unsigned int readBufferCurrentSize;  //  can be lower than bufferSize if, for example, EOF is reached

            #ifdef WINDOWS
                CStr pnn;
            #endif
        };

        /*  Core Functions  */
        EXTERNAL void Initialize( CFileBasis *file );
        EXTERNAL void Destroy( CFileBasis *file );
        EXTERNAL bool Open( CFileBasis *file, const char *cp_pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, SError *po_error );
        EXTERNAL void Close( CFileBasis *file );
        EXTERNAL bool IsValid( const CFileBasis *file );
        EXTERNAL bool Write( CFileBasis *file, const void *cp_source, unsigned int len );
        EXTERNAL bool Read( CFileBasis *file, void *p_target, unsigned int len, unsigned int *p_readed );
        EXTERNAL bool BufferSet( CFileBasis *file, unsigned int size, void *buffer = 0 );  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
        EXTERNAL unsigned int BufferSizeGet( CFileBasis *file );
        EXTERNAL void StatsGet( const CFileBasis *file, SStats *po_stats );
        EXTERNAL void StatsReset( CFileBasis *file );
        EXTERNAL bool Flush( CFileBasis *file );  //  false if writing to file failed to complete
        EXTERNAL i64 OffsetGet( CFileBasis *file );  //  -1 on fail, current offset on success
        EXTERNAL i64 OffsetSet( CFileBasis *file, OffsetMode::OffsetMode_t mode, i64 offset, SError *po_error );  //  -1 on fail, current offset on success
        EXTERNAL ui64 SizeGet( CFileBasis *file );
        EXTERNAL bool SizeSet( CFileBasis *file, ui64 newSize );
        EXTERNAL OpenMode::OpenMode_t OpenModeGet( const CFileBasis *file );
        EXTERNAL ProcMode::ProcMode_t ProcModeGet( const CFileBasis *file );
        EXTERNAL unsigned int PNNGet( const CFileBasis *file, char *p_buf );  //  pass 0 as p_buf to get only len
    }

    struct CFile : private Private::CFileBasis
    {
        ~CFile()
        {
            Private::Destroy( this );
        }

        CFile()
        {
            Private::Initialize( this );
        }

        CFile( const char *pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, SError *po_error )
        {
            Private::Initialize( this );
            Private::Open( this, pnn, openMode, procMode, po_error );
        }

        CFile( const CFile &source )  //  bufferization will not be derived  TODO: file position?
        {
            Private::Initialize( this );
            char pnn[ MAX_PATH ];
            source.PNNGet( pnn );
            Private::Open( this, pnn, source.OpenModeGet(), source.ProcModeGet(), 0 );
        }

        CFile & operator = ( const CFile &source )  //  bufferization will not be derived  TODO: file position?
        {
            if( this != &source )
            {
                Close();
                char pnn[ MAX_PATH ];
                source.PNNGet( pnn );
                Private::Open( this, pnn, source.OpenModeGet(), source.ProcModeGet(), 0 );
            }
            return *this;
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

        void Open( const char *pnn, OpenMode::OpenMode_t openMode, ProcMode::ProcMode_t procMode, SError *po_error )
        {
            Close();
            Private::Open( this, pnn, openMode, procMode, po_error );
        }

        void Close()
        {
            Private::Close( this );
        }

        bool IsOpened() const
        {
            return Private::IsValid( this );
        }

        bool Write( const void *cp_source, unsigned int len )
        {
            return Private::Write( this, cp_source, len );
        }

        bool Read( void *p_target, unsigned int len, unsigned int *p_readed )
        {
            return Private::Read( this, p_target, len, p_readed );
        }

        bool BufferSet( unsigned int size, void *buffer = 0 )  //  pass null as buffer to use auto allocated buffer, pass 0 as size to disable buffering
        {
            return Private::BufferSet( this, size, buffer );
        }

        unsigned int BufferSizeGet()
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

        bool Flush()  //  false if writing to file failed to complete
        {
            return Private::Flush( this );
        }

        i64 OffsetGet()  //  -1 on fail, current offset on success
        {
            return Private::OffsetGet( this );
        }

        i64 OffsetSet( OffsetMode::OffsetMode_t mode, i64 offset, SError *po_error )  //  -1 on fail, current offset on success
        {
            return Private::OffsetSet( this, mode, offset, po_error );
        }

        ui64 SizeGet()
        {
            return Private::SizeGet( this );
        }

        bool SizeSet( ui64 newSize )
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

        unsigned int PNNGet( char *p_buf ) const  //  pass 0 as p_buf to get only len
        {
            return Private::PNNGet( this, p_buf );
        }
        
#ifdef MOVE_SUPPORTED
    private:
        void Transfer( CFile &&source )
        {
            this->handle = source.handle;
            this->openMode = source.openMode;
            this->procMode = source.procMode;
            this->stats = source.stats;
            this->buffer = source.buffer;
            this->is_customBuffer = source.is_customBuffer;
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

    namespace FileError
    {
        const unsigned int CanNotOpenFile = Error::Private::MaxDefaultError + 0;
    }

    namespace Private
    {
        EXTERNAL const char *const *GetErrorsDesc();
    }
}

}  //  namespace StdLib

#endif __FILE_IO_HPP__
