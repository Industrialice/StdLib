#include "PreHeader.hpp"

using namespace StdLib;

const ui16 *f16::GetRawPointer()
{
    return &_val;
}

ui16 f16::RawValueGet() const
{
    return _val;
}

void f16::RawValueSet( ui16 source )
{
    _val = source;
}

NOINLINE f32 f16::ToF32()
{
    ui32 s = _val & f16_sign_mask;
    if( (_val & f16_exp_mask) == 0 && (_val & f16_frac_mask) == 0 )
    {
        return s ? -0.f : 0.f;
    }
    if( (_val | f16_exp_mask) == _val )
    {
        if( _val & f16_frac_mask )
        {
            return f32_nan;
        }
        return s ? f32_inf_neg : f32_inf_pos;
    }
    ui32 em = ((ui32)(_val & ~f16_sign_mask) << 13) + (112 << 23);
    union
    {
        f32 f;
        ui32 i;
    } u;
    u.i = (s << 16) | em;
    return u.f;
}

NOINLINE f64 f16::ToF64()
{
    ui64 s = _val & f16_sign_mask;
    if( (_val & f16_exp_mask) == 0 && (_val & f16_frac_mask) == 0 )
    {
        return s ? -0.0 : 0.0;
    }
    if( (_val | f16_exp_mask) == _val )
    {
        if( _val & f16_frac_mask )
        {
            return f64_nan;
        }
        return s ? f64_inf_neg : f64_inf_pos;
    }
    ui64 em = ((ui64)(_val & ~f16_sign_mask) << 42) + (1008ULL << 52);
    union
    {
        f64 f;
        ui64 i;
    } u;
    u.i = (s << 48) | em;
    return u.f;
}

NOINLINE i32 f16::ToI32()  //  TODO: facepalm
{
    if( (_val & f16_exp_mask) == 0 )
    {
        return 0;
    }
    if( (_val | f16_exp_mask) == _val )
    {
        return 0;
    }
    ui32 s = (_val & f16_sign_mask) << 16;
    ui32 em = ((ui32)(_val & ~f16_sign_mask) << 13) + (112 << 23);
    union
    {
        f32 f;
        ui32 i;
    } u;
    u.i = s | em;
    return (i32)(u.f + 0.00001f);
}

f16 f16::FromF32IEEE( f32 source )
{
    FromF32( source, Flow_IEEE );
    return *this;
}

f16 f16::FromF32Clamp( f32 source )
{
    FromF32( source, Flow_Clamp );
    return *this;
}

f16 f16::FromF64IEEE( f64 source )
{
    FromF64( source, Flow_IEEE );
    return *this;
}

f16 f16::FromF64Clamp( f64 source )
{
    FromF64( source, Flow_Clamp );
    return *this;
}

NOINLINE f16 f16::FromUI32Clamp( ui32 source )
{
    if( source > f16_int_max )
    {
        _val = f16_max;
    }
    else
    {
        f32 fsource = (f32)source;  //  TODO: facepalm
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        _val = (ifsource & ~f32_sign_mask) >> 13;
    }

    return *this;
}

NOINLINE f16 f16::FromI32Clamp( i32 source )
{
    if( (ui32)source > f16_int_max )
    {
        _val = source > 0 ? f16_max : f16_min;
    }
    else
    {
        f32 fsource = (f32)source;  //  TODO: facepalm
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        _val = (ifsource & ~f32_sign_mask) >> 13;
        _val |= (ifsource & f32_sign_mask) >> 16;
    }

    return *this;
}

bln f16::IsNaN()
{
    if( (_val | f16_exp_mask) != _val )
    {
        return false;
    }
    return _val & f16_frac_mask;
}

bln f16::IsInf()
{
    if( (_val | f16_exp_mask) != _val )
    {
        return false;
    }
    return (_val & f16_frac_mask) == 0;
}

bln f16::IsInfPos()
{
    if( !IsInf() )
    {
        return false;
    }
    return (_val & f16_sign_mask) == 0;
}

bln f16::IsInfNeg()
{
    if( !IsInf() )
    {
        return false;
    }
    return (_val & f16_sign_mask) != 0;
}

NOINLINE void f16::FromF32( f32 source, OverflowAction flowAction )
{
    ui32 isource = *(ui32 *)&source;
    ui32 s = isource & f32_sign_mask;
    ui32 e = isource & f32_exp_mask;
    if( e <= ((127 - 15) << 23) )  //  underflow, zero of subnormal
    {
        if( e < ((127 - 15) << 23) )  //  underflow or zero, assign a zero
        {
            _val = s >> 16;
            return;
        }
    }
    else if( e >= ((127 + 16) << 23) )  //  overflow, NaN or infinity
    {
        if( e == (255 << 23) && (isource & f32_significand_mask) )
        {
            _val = f16_nan;
        }
        else
        {
            _val = GetOverflowed( flowAction, s != 0 );
        }
        return;
    }
    isource -= ((127 - 15) << 23);
    _val = (isource & ~f32_sign_mask) >> 13;
    _val |= s >> 16;
}

NOINLINE void f16::FromF64( f64 source, OverflowAction flowAction )
{
    ui64 isource = *(ui64 *)&source;
    ui64 s = isource & f64_sign_mask;
    ui64 e = isource & f64_exp_mask;
    if( e <= ((1023ULL - 15) << 52) )  //  underflow, zero of subnormal
    {
        if( e < ((1023ULL - 15) << 52) )  //  underflow or zero, assign a zero
        {
            _val = s >> 16;
            return;
        }
    }
    else if( e >= ((1023ULL + 16) << 52) )  //  overflow, NaN or infinity
    {
        if( e == (2048ULL << 52) && (isource & f32_significand_mask) )
        {
            _val = f16_nan;
        }
        else
        {
            _val = GetOverflowed( flowAction, s != 0 );
        }
        return;
    }
    isource -= ((1023ULL - 15) << 52);
    _val = (isource & ~f64_sign_mask) >> 42;
    _val |= s >> 48;
}

ui16 f16::GetOverflowed( OverflowAction action, bln is_negative )
{
    if( action == Flow_Clamp )
    {
        return is_negative ? f16_min : f16_max;
    }
    else
    {
        return is_negative ? f16_inf_neg : f16_inf_pos;
    }
}