#ifndef __MISC_HPP__
#define __MISC_HPP__

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
    EXTERNALD void *VM_Reserve( uiw size );
    EXTERNALD bln VM_Commit( void *p_mem, uiw size, PageMode::PageMode_t mode );
    EXTERNALD void *VM_Alloc( uiw size, PageMode::PageMode_t mode );
    EXTERNALD bln VM_Free( void *p_mem );
    EXTERNALD PageMode::PageMode_t VM_ProtectGet( const void *p_mem, uiw size, CError *po_error );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
    EXTERNALD bln VM_ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode );  //  can commit uncommited memory
    EXTERNALS ui32 VM_PageSize();

	inline CError Error_InconsistentProtection() { return CError( Error::_MaxDefaultError + 0, "VIRTUAL_MEM", "INCONSISTENT_PROTECTION" ); }
}

namespace CPU
{
    EXTERNALS ui32 CPUCoresNum();
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
