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
    EXTERNAL void *Reserve( uiw size );
    EXTERNAL bln Commit( void *p_mem, uiw size, PageMode::PageMode_t mode );
    EXTERNAL void *Alloc( uiw size, PageMode::PageMode_t mode );
    EXTERNAL bln Free( void *p_mem );
    EXTERNAL PageMode::PageMode_t ProtectGet( const void *p_mem, uiw size, SError *po_error );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
    EXTERNAL bln ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode );  //  can commit uncommited memory
    EXTERNAL ui32 PageSize();

    namespace VirtualMemError
    {
        const ui32 InconsistentProtection = Error::Private::MaxDefaultError + 0;
    }
}

namespace CPU
{
    EXTERNAL ui32 CoresNum();
}

class CTC
{
    tcs _tc;
    DBGCODE( bln _is_set; );

public:
    CTC( bln is_set = false );
    void Set();
    f32 Get32() const;
    f64 Get64() const;
    ui64 GetUSec64() const;
    f32 Get32Set();
    f64 Get64Set();
    ui64 GetUSec64Set();
    f32 Compare32( const CTC &second ) const;    //  ...
    f64 Compare64( const CTC &second ) const;    //  ...
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
