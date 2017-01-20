#ifndef __TIME_MOMENT_HPP__
#define __TIME_MOMENT_HPP__

namespace StdLib
{
	//  note that TimeMoment is undefined after the default contructor had been called
	//  using undefined TimeMoment is an error and will trigger an exception in debug mode
	//  use TimeMoment( TimeMoment::Current() ) to create a TimeMoment for the current moment
	class TimeMoment
	{
		tcs _tc;

	public:
		/*TimeMoment( const TimeMoment &source ) = default;
		TimeMoment &operator = ( const TimeMoment &source ) = default;
		TimeMoment( TimeMoment && ) = default;
		TimeMoment &operator = ( TimeMoment && ) = default;*/

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
		bln operator <= ( const TimeMoment &other ) const;
		bln operator > ( const TimeMoment &other ) const;
		bln operator >= ( const TimeMoment &other ) const;
		bln operator == ( const TimeMoment &other ) const;
		bln operator != ( const TimeMoment &other ) const;

		bln IsEmpty() const;  //  return true if the TimeMoment hasn't been initialized with anything. you can't use an empty TimeMoment for comparisons

		const tcs &TCSGet() const;

		static TimeMoment CreateCurrent();
		static TimeMoment CreateShiftedSec( const TimeMoment &ref, f32 delta );
		static TimeMoment CreateShiftedSec( const TimeMoment &ref, f64 delta );
		static TimeMoment CreateShiftedMSec( const TimeMoment &ref, f32 delta );
		static TimeMoment CreateShiftedMSec( const TimeMoment &ref, f64 delta );
		static TimeMoment CreateShiftedUSec( const TimeMoment &ref, f32 delta );
		static TimeMoment CreateShiftedUSec( const TimeMoment &ref, f64 delta );

	protected:
		static void Initialize();
	};
}

#endif