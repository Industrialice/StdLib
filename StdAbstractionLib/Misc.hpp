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

//  note that TimeMoment is undefined after the default contructor had been called
//  using undefined TimeMoment is an error and will trigger an exception in debug mode
//  use TimeMoment( TimeMoment::Current() ) to create a TimeMoment for the current moment
class TimeMoment
{
    tcs _tc;

public:
	TimeMoment( const TimeMoment &source ) : _tc( source._tc )
	{}

	TimeMoment &operator = ( const TimeMoment &source )
	{
		this->_tc = source._tc;
		return *this;
	}

#ifdef MOVE_SUPPORTED
	TimeMoment( TimeMoment && ) = default;
	TimeMoment &operator = ( TimeMoment && ) = default;
#endif

    TimeMoment();
	TimeMoment( const tcs &time );

	//  returned value is positive if second follows before the current TimeMoment
    f32 SinceSec32( const TimeMoment &second ) const;
    f64 SinceSec64( const TimeMoment &second ) const;
    f32 SinceMSec32( const TimeMoment &second ) const;
    f64 SinceMSec64( const TimeMoment &second ) const;
    f32 SinceUSec32( const TimeMoment &second ) const;
    f64 SinceUSec64( const TimeMoment &second ) const;

	//  returned value is positive if second follows after the current TimeMoment
    f32 BeforeSec32( const TimeMoment &second ) const;
    f64 BeforeSec64( const TimeMoment &second ) const;
    f32 BeforeMSec32( const TimeMoment &second ) const;
    f64 BeforeMSec64( const TimeMoment &second ) const;
    f32 BeforeUSec32( const TimeMoment &second ) const;
    f64 BeforeUSec64( const TimeMoment &second ) const;

	bln operator < ( const TimeMoment &other ) const;
	bln operator > ( const TimeMoment &other ) const;

	bln IsEmpty() const;  //  return true if the TimeMoment hasn't been initialized with anything. you can't use an empty TimeMoment for comparisons

    const tcs &TCSGet() const;

	static TimeMoment CreateCurrent();
	static TimeMoment CreateShiftedSec( const TimeMoment &ref, f32 delta );
	static TimeMoment CreateShiftedSec( const TimeMoment &ref, f64 delta );
	static TimeMoment CreateShiftedMSec( const TimeMoment &ref, f32 delta );
	static TimeMoment CreateShiftedMSec( const TimeMoment &ref, f64 delta );
	static TimeMoment CreateShiftedUSec( const TimeMoment &ref, f32 delta );
	static TimeMoment CreateShiftedUSec( const TimeMoment &ref, f64 delta );
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
