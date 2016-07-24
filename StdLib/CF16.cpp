#include "PreHeader.hpp"

static ui16 GetOverflowed( Private::OverflowAction_t action, bln is_negative )
{
	if( action == Private::Flow_Clamp )
	{
		return is_negative ? f16_min : f16_max;
	}
	else
	{
		return is_negative ? f16_inf_neg : f16_inf_pos;
	}
}

NOINLINE f32 Private::f16_ToF32( ui16 value )
{
    ui32 s = value & f16_sign_mask;
    if( (value & f16_exp_mask) == 0 && (value & f16_frac_mask) == 0 )
    {
        return s ? -0.f : 0.f;
    }
    if( (value | f16_exp_mask) == value )
    {
        if( value & f16_frac_mask )
        {
            return f32_nan;
        }
        return s ? f32_inf_neg : f32_inf_pos;
    }
    ui32 em = ((ui32)(value & ~f16_sign_mask) << 13) + (112 << 23);
    union  //  TODO: UB
    {
        f32 f;
        ui32 i;
    } u;
    u.i = (s << 16) | em;
    return u.f;
}

NOINLINE f64 Private::f16_ToF64( ui16 value )
{
    ui64 s = value & f16_sign_mask;
    if( (value & f16_exp_mask) == 0 && (value & f16_frac_mask) == 0 )
    {
        return s ? -0.0 : 0.0;
    }
    if( (value | f16_exp_mask) == value )
    {
        if( value & f16_frac_mask )
        {
            return f64_nan;
        }
        return s ? f64_inf_neg : f64_inf_pos;
    }
    ui64 em = ((ui64)(value & ~f16_sign_mask) << 42) + (1008ULL << 52);
    union  //  TODO: UB
    {
        f64 f;
        ui64 i;
    } u;
    u.i = (s << 48) | em;
    return u.f;
}

NOINLINE i32 Private::f16_ToI32( ui16 value )  //  TODO: facepalm
{
    if( (value & f16_exp_mask) == 0 )
    {
        return 0;
    }
    if( (value | f16_exp_mask) == value )
    {
        return 0;
    }
    ui32 s = (value & f16_sign_mask) << 16;
    ui32 em = ((ui32)(value & ~f16_sign_mask) << 13) + (112 << 23);
    union  //  TODO: UB
    {
        f32 f;
        ui32 i;
    } u;
    u.i = s | em;
    return (i32)(u.f + 0.00001f);
}

NOINLINE ui16 Private::f16_FromF32( f32 source, OverflowAction_t flowAction )
{
	ui16 value;

	ui32 isource = *(ui32 *)&source;
	ui32 s = isource & f32_sign_mask;
	ui32 e = isource & f32_exp_mask;
	if( e <= ((127 - 15) << 23) )  //  underflow, zero of subnormal
	{
		if( e < ((127 - 15) << 23) )  //  underflow or zero, assign a zero
		{
			value = s >> 16;
			return value;
		}
	}
	else if( e >= ((127 + 16) << 23) )  //  overflow, NaN or infinity
	{
		if( e == (255 << 23) && (isource & f32_significand_mask) )
		{
			value = f16_nan;
		}
		else
		{
			value = GetOverflowed( flowAction, s != 0 );
		}
		return value;
	}
	isource -= ((127 - 15) << 23);
	value = (isource & ~f32_sign_mask) >> 13;
	value |= s >> 16;

	return value;
}

NOINLINE ui16 Private::f16_FromF64( f64 source, OverflowAction_t flowAction )
{
	ui16 value;

	ui64 isource = *(ui64 *)&source;
	ui64 s = isource & f64_sign_mask;
	ui64 e = isource & f64_exp_mask;
	if( e <= ((1023ULL - 15) << 52) )  //  underflow, zero of subnormal
	{
		if( e < ((1023ULL - 15) << 52) )  //  underflow or zero, assign a zero
		{
			value = s >> 16;
			return value;
		}
	}
	else if( e >= ((1023ULL + 16) << 52) )  //  overflow, NaN or infinity
	{
		if( e == (2048ULL << 52) && (isource & f32_significand_mask) )
		{
			value = f16_nan;
		}
		else
		{
			value = GetOverflowed( flowAction, s != 0 );
		}
		return value;
	}
	isource -= ((1023ULL - 15) << 52);
	value = (isource & ~f64_sign_mask) >> 42;
	value |= s >> 48;

	return value;
}

NOINLINE ui16 Private::f16_FromUI32Clamp( ui32 source )
{
    if( source > f16_int_max )
    {
        return f16_max;
    }
    else
    {
        f32 fsource = (f32)source;  //  TODO: facepalm
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        return (ifsource & ~f32_sign_mask) >> 13;
    }
}

NOINLINE ui16 Private::f16_FromI32Clamp( i32 source )
{
    if( (ui32)source > f16_int_max )
    {
        return source > 0 ? f16_max : f16_min;
    }
    else
    {
        f32 fsource = (f32)source;  //  TODO: facepalm
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        ui16 value = (ifsource & ~f32_sign_mask) >> 13;
        value |= (ifsource & f32_sign_mask) >> 16;
		return value;
    }
}

bln Private::f16_IsNaN( ui16 value )
{
    if( (value | f16_exp_mask) != value )
    {
        return false;
    }
    return value & f16_frac_mask;
}

bln Private::f16_IsInf( ui16 value )
{
    if( (value | f16_exp_mask) != value )
    {
        return false;
    }
    return (value & f16_frac_mask) == 0;
}

bln Private::f16_IsInfPos( ui16 value )
{
    if( !f16_IsInf( value ) )
    {
        return false;
    }
    return (value & f16_sign_mask) == 0;
}

bln Private::f16_IsInfNeg( ui16 value )
{
    if( !f16_IsInf( value ) )
    {
        return false;
    }
    return (value & f16_sign_mask) != 0;
}