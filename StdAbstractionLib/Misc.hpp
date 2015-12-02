#ifndef __MISC_HPP__
#define __MISC_HPP__

#include <StdCoreLib.hpp>
#include <Error.hpp>

namespace StdLib {

namespace VirtualMem
{
    namespace PageMode
    {
        CONSTS_OPED( PageMode_t,
                Error = 0,
                Write = BIT( 0 ),
                Read = BIT( 1 ),
                Execute = BIT( 2 ) )
    }
    EXTERNAL void *Reserve( size_t size );
    EXTERNAL bool Commit( void *p_mem, size_t size, PageMode::PageMode_t mode );
    EXTERNAL void *Alloc( size_t size, PageMode::PageMode_t mode );
    EXTERNAL bool Free( void *p_mem );
    EXTERNAL PageMode::PageMode_t ProtectGet( const void *p_mem, size_t size, SError *po_error );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
    EXTERNAL bool ProtectSet( void *p_mem, size_t size, PageMode::PageMode_t mode );  //  can commit uncommited memory
    EXTERNAL unsigned int PageSize();

    namespace VirtualMemError
    {
        const unsigned int InconsistentProtection = Error::Private::MaxDefaultError + 0;
    }
}

namespace CPU
{
    EXTERNAL unsigned int CoresNum();
}

class CTC
{
    tcs _tc;
    DBGCODE( bool _is_set; );

public:
    CTC( bool is_set = false );
    void Set();
    float Get32() const;
    double Get64() const;
    ui64 GetUSec64() const;
    float Get32Set();
    double Get64Set();
    ui64 GetUSec64Set();
    float Compare32( const CTC &second ) const;    //  ...
    double Compare64( const CTC &second ) const;    //  ...
    ui64 CompareUSec64( const CTC &second ) const;  //  will subtract the second from the first
    const tcs &TCSGet() const;
};

namespace Misc
{
    namespace Private
    {
        EXTERNAL const char *const *GetErrorsDesc();
        void Initialize();
    }
}

}  //  namespace StdLib

#endif __MISC_HPP__
