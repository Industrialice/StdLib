#include "PreHeader.hpp"

#ifdef WINDOWS

#include "TimeMoment.hpp"

using namespace StdLib;

namespace
{
    bool is_Initialized;

    class CMisc
    {
	protected:
        ui32 _memPageSize;
        
        ui32 _cpuCoresCount;

        f32 _freqMultSec32;
        f64 _freqMultSec64;
		f32 _freqMultMSec32;
		f64 _freqMultMSec64;
		f32 _freqMultUSec32;
		f64 _freqMultUSec64;

		f32 _freqSec32;
		f64 _freqSec64;
		f32 _freqMSec32;
		f64 _freqMSec64;
		f32 _freqUSec32;
		f64 _freqUSec64;

        ui64 _freqDivU64;

    public:
        ui32 MemPageSize() const
        {
            ASSUME( is_Initialized );
            return _memPageSize;
        }

        ui32 CpuCoresCount() const
        {
            ASSUME( is_Initialized );
            return _cpuCoresCount;
        }

        ui64 FreqDivU64() const
        {
            ASSUME( is_Initialized );
            return _freqDivU64;
        }

		f32 FreqMultSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultSec32;
		}

		f64 FreqMultSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultSec64;
		}

		f32 FreqMultMSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultMSec32;
		}

		f64 FreqMultMSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultMSec64;
		}

		f32 FreqMultUSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultUSec32;
		}

		f64 FreqMultUSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqMultUSec64;
		}

		f32 FreqSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqSec32;
		}

		f64 FreqSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqSec64;
		}

		f32 FreqMSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqMSec32;
		}

		f64 FreqMSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqMSec64;
		}

		f32 FreqUSec( f32 ) const
		{
			ASSUME( is_Initialized );
			return _freqUSec32;
		}

		f64 FreqUSec( f64 ) const
		{
			ASSUME( is_Initialized );
			return _freqUSec64;
		}
    } MiscData;
}

enum TMPrec
{
	Sec, MSec, USec
};

template < typename Type, bool is_since, TMPrec prec > Type TMDiff( const tcs &_left, const tcs &_right )
{
	Type mult;
	if( prec == Sec ) mult = MiscData.FreqMultSec( Type() );
	else if( prec == MSec ) mult = MiscData.FreqMultMSec( Type() );
	else if( prec == USec ) mult = MiscData.FreqMultUSec( Type() );
	else HARDBREAK;

	return (is_since ? (_left.QuadPart - _right.QuadPart) : (_right.QuadPart - _left.QuadPart)) * mult;
}

TimeMoment::TimeMoment()
{
	_MemZero( &_tc, sizeof(_tc) );
}

TimeMoment::TimeMoment( const tcs &time )
{
	this->_tc = time;
}

f32 TimeMoment::SinceSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, true, Sec >( _tc, second._tc );
}

f64 TimeMoment::SinceSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, true, Sec >( _tc, second._tc );
}

f32 TimeMoment::SinceMSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, true, MSec >( _tc, second._tc );
}

f64 TimeMoment::SinceMSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, true, MSec >( _tc, second._tc );
}

f32 TimeMoment::SinceUSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, true, USec >( _tc, second._tc );
}

f64 TimeMoment::SinceUSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, true, USec >( _tc, second._tc );
}

f32 TimeMoment::BeforeSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, false, Sec >( _tc, second._tc );
}

f64 TimeMoment::BeforeSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, false, Sec >( _tc, second._tc );
}

f32 TimeMoment::BeforeMSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, false, MSec >( _tc, second._tc );
}

f64 TimeMoment::BeforeMSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, false, MSec >( _tc, second._tc );
}

f32 TimeMoment::BeforeUSec32( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f32, false, USec >( _tc, second._tc );
}

f64 TimeMoment::BeforeUSec64( const TimeMoment &second ) const
{
	CHECK( !this->IsEmpty() && !second.IsEmpty() );
	return TMDiff < f64, false, USec >( _tc, second._tc );
}

bool TimeMoment::operator < ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart < other._tc.QuadPart;
}

bool TimeMoment::operator <= ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart <= other._tc.QuadPart;
}

bool TimeMoment::operator > ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart > other._tc.QuadPart;
}

bool TimeMoment::operator >= ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart > other._tc.QuadPart;
}

bool TimeMoment::operator == ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart == other._tc.QuadPart;
}

bool TimeMoment::operator != ( const TimeMoment &other ) const
{
	CHECK( !this->IsEmpty() && !other.IsEmpty() );
	return _tc.QuadPart > other._tc.QuadPart;
}

bool TimeMoment::IsEmpty() const
{
	return _tc.QuadPart == 0;
}

const tcs &TimeMoment::TCSGet() const
{
	return _tc;
}

TimeMoment TimeMoment::CreateCurrent()
{
	LARGE_INTEGER current;
	BOOL result = QueryPerformanceCounter( &current );
	ASSUME( result );
	return current;
}

template < typename Type, TMPrec prec > tcs Shift( const TimeMoment &ref, Type delta )
{
	if( prec == Sec ) delta *= MiscData.FreqSec( Type() );
	else if( prec == MSec ) delta *= MiscData.FreqMSec( Type() );
	else if( prec == USec ) delta *= MiscData.FreqUSec( Type() );
	else HARDBREAK;
	
	LARGE_INTEGER current = ref.TCSGet();
	current.QuadPart += delta;
	return current;
}

TimeMoment TimeMoment::CreateShiftedSec( const TimeMoment &ref, f32 delta )
{
	return Shift < f32, Sec >( ref, delta );
}

TimeMoment TimeMoment::CreateShiftedSec( const TimeMoment &ref, f64 delta )
{
	return Shift < f64, Sec >( ref, delta );
}

TimeMoment TimeMoment::CreateShiftedMSec( const TimeMoment &ref, f32 delta )
{
	return Shift < f32, MSec >( ref, delta );
}

TimeMoment TimeMoment::CreateShiftedMSec( const TimeMoment &ref, f64 delta )
{
	return Shift < f64, MSec >( ref, delta );
}

TimeMoment TimeMoment::CreateShiftedUSec( const TimeMoment &ref, f32 delta )
{
	return Shift < f32, USec >( ref, delta );
}

TimeMoment TimeMoment::CreateShiftedUSec( const TimeMoment &ref, f64 delta )
{
	return Shift < f64, USec >( ref, delta );
}

void TimeMoment::Initialize()
{
	struct SMisc : private CMisc
	{
		void Initialize()
		{
			LARGE_INTEGER o_freq;
			BOOL freqRes = ::QueryPerformanceFrequency( &o_freq );
			ASSUME( freqRes );

			_freqMultSec32 = 1.f / o_freq.QuadPart;
			_freqMultSec64 = 1.0 / o_freq.QuadPart;

			_freqMultMSec32 = 1000.f / o_freq.QuadPart;
			_freqMultMSec64 = 1000.0 / o_freq.QuadPart;

			_freqMultUSec32 = 1000000.f / o_freq.QuadPart;
			_freqMultUSec64 = 1000000.0 / o_freq.QuadPart;

			_freqSec32 = (f32)o_freq.QuadPart;
			_freqSec64 = (f64)o_freq.QuadPart;

			_freqMSec32 = (f32)o_freq.QuadPart * 1000.f;
			_freqMSec64 = (f64)o_freq.QuadPart * 1000.0;

			_freqUSec32 = (f32)o_freq.QuadPart * 1000000.f;
			_freqUSec64 = (f64)o_freq.QuadPart * 1000000.0;

			_freqDivU64 = o_freq.QuadPart;
		}
	} *smisc = (SMisc *)&MiscData;

	smisc->Initialize();

	is_Initialized = true;
}

#endif