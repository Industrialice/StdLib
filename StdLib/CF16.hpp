#ifndef __C_F16_HPP__
#define __C_F16_HPP__

namespace StdLib {

//  TODO: it's does not following IEEE 754-2008 and i probably should fix this
//  +/-inf and NaN conversion to integer is undefined behavior

namespace Private
{
	enum OverflowAction_t { Flow_IEEE, Flow_Clamp };

	EXTERNALD f32 f16_ToF32( ui16 value );
	EXTERNALD f64 f16_ToF64( ui16 value );
	EXTERNALD i32 f16_ToI32( ui16 value );
	EXTERNALD ui16 f16_FromF32( f32 source, OverflowAction_t action );
	EXTERNALD ui16 f16_FromF64( f64 source, OverflowAction_t action );
	EXTERNALD ui16 f16_FromUI32Clamp( ui32 source );
	EXTERNALD ui16 f16_FromI32Clamp( i32 source );
	EXTERNALS bln f16_IsNaN( ui16 value );
	EXTERNALS bln f16_IsInf( ui16 value );
	EXTERNALS bln f16_IsInfPos( ui16 value );
	EXTERNALS bln f16_IsInfNeg( ui16 value );
}

class f16
{
    ui16 _val;

public:
	f16()
	{
		_val = 0;
	}

    const ui16 *GetRawPointer() const
	{
		return &_val;
	}

    ui16 RawValueGet() const
	{
		return _val;
	}
    
	void RawValueSet( ui16 source )
	{
		_val = source;
	}

    f32 ToF32()
	{
		return Private::f16_ToF32( _val );
	}
    
	f64 ToF64()
	{
		return Private::f16_ToF64( _val );
	}
    
	i32 ToI32()
	{
		return Private::f16_ToI32( _val );
	}

    f16 FromF32IEEE( f32 source )
	{
		_val = Private::f16_FromF32( source, Private::Flow_IEEE );
		return *this;
	}
    
	f16 FromF32Clamp( f32 source )
	{
		_val = Private::f16_FromF32( source, Private::Flow_Clamp );
		return *this;
	}
    
	f16 FromF64IEEE( f64 source )
	{
		_val = Private::f16_FromF64( source, Private::Flow_IEEE );
		return *this;
	}

    f16 FromF64Clamp( f64 source )
	{
		_val = Private::f16_FromF64( source, Private::Flow_Clamp );
		return *this;
	}
    
	f16 FromUI32Clamp( ui32 source )
	{
		_val = Private::f16_FromUI32Clamp( source );
		return *this;
	}
    
	f16 FromI32Clamp( i32 source )
	{
		_val = Private::f16_FromI32Clamp( source );
		return *this;
	}

    bln IsNaN()
	{
		return Private::f16_IsNaN( _val );
	}
    
	bln IsInf()
	{
		return Private::f16_IsInf( _val );
	}
    
	bln IsInfPos()
	{
		return Private::f16_IsInfPos( _val );
	}
    
	bln IsInfNeg()
	{
		return Private::f16_IsInfNeg( _val );
	}
};

const ui16 f16_sign_mask = 0x8000;
const ui16 f16_exp_mask = 0x7C00;
const ui16 f16_frac_mask = 0x03FF;
const ui16 f16_nan = 0xFFFF;  //  one of the possible NaN values
const ui16 f16_inf = 0x7C00;
const ui16 f16_inf_pos = 0x7C00;
const ui16 f16_inf_neg = 0xFC00;
const ui16 f16_zero = 0x0000;
const ui16 f16_zero_pos = 0x0000;
const ui16 f16_zero_neg = 0x8000;
const ui16 f16_zero_test = 0x7FFF;
const ui16 f16_max = 0x7BFF;
const ui16 f16_min = 0xFBFF;
const i32 f16_int_max = 65504;
const i32 f16_int_min = -65504;

}  //  namespace StdLib

#endif