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
    EXTERNALS void *Reserve( uiw size );
    EXTERNALS bln Commit( void *p_mem, uiw size, PageMode::PageMode_t mode );
    EXTERNALS void *Alloc( uiw size, PageMode::PageMode_t mode );
    EXTERNALS bln Free( void *p_mem );
    EXTERNALS PageMode::PageMode_t ProtectGet( const void *p_mem, uiw size, CError *po_error );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
    EXTERNALS bln ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode );  //  can commit uncommited memory
    EXTERNALS ui32 PageSize();

	inline CError Error_InconsistentProtection() { return CError( Error::_MaxDefaultError + 0, "VIRTUAL_MEM", "INCONSISTENT_PROTECTION" ); }
}

namespace CPU
{
    EXTERNALS ui32 CoresNum();
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
        void Initialize();
    }
}

}  //  namespace StdLib

#endif
