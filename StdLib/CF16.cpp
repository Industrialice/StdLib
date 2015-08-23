#include "PreHeader.hpp"

using namespace StdLib;

namespace
{
    enum { Flow_IEEE, Flow_Clamp };
    const ui16 ca_Overflows[ 2 ][ 2 ] = { f16_inf_pos, f16_inf_neg,
                                              f16_max,     f16_min };
}

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
    if( (_val & f16_exp_mask) == 0 )
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
    if( (_val & f16_exp_mask) == 0 )
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

NOINLINE i32 f16::ToI32()
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

void f16::FromF32IEEE( f32 source )
{
    FromF32( source, (EnumDecl)Flow_IEEE );
}

void f16::FromF32Clamp( f32 source )
{
    FromF32( source, (EnumDecl)Flow_Clamp );
}

void f16::FromF64IEEE( f64 source )
{
    FromF64( source, (EnumDecl)Flow_IEEE );
}

void f16::FromF64Clamp( f64 source )
{
    FromF64( source, (EnumDecl)Flow_Clamp );
}

NOINLINE void f16::FromUI32Clamp( ui32 source )
{
    if( source > f16_int_max )
    {
        _val = f16_max;
    }
    else
    {
        f32 fsource = (f32)source;
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        _val = (ifsource & ~f32_sign_mask) >> 13;
    }
}

NOINLINE void f16::FromI32Clamp( i32 source )
{
    if( (ui32)source > f16_int_max )
    {
        _val = source > 0 ? f16_max : f16_min;
    }
    else
    {
        f32 fsource = (f32)source;
        ui32 ifsource = *(ui32 *)&fsource;
        ifsource -= (112 << 23);
        _val = (ifsource & ~f32_sign_mask) >> 13;
        _val |= (ifsource & f32_sign_mask) >> 16;
    }
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

NOINLINE void f16::FromF32( f32 source, EnumDecl flowAction )
{
    ui32 isource = *(ui32 *)&source;
    ui32 s = isource & f32_sign_mask;
    ui32 e = isource & f32_exp_mask;
    if( e == 0 )
    {
        _val = s >> 16;
        return;
    }
    if( (ui32)((i32)(e >> 23) - 112) > 31 )
    {
        if( e == (255 << 23) && (isource & f32_significand_mask) )
        {
            _val = f16_nan;
        }
        else if( e > (143 << 23) )
        {
            _val = ca_Overflows[ flowAction ][ s != 0 ];
        }
        else  //  underflow becomes [-0;0]
        {
            ASSUME( e < (112 << 23) );
            _val = s >> 16;
        }
    }
    else
    {
        isource -= (112 << 23);
        _val = (isource & ~f32_sign_mask) >> 13;
        _val |= s >> 16;
    }
}

NOINLINE void f16::FromF64( f64 source, EnumDecl flowAction )
{
    ui64 isource = *(ui64 *)&source;
    ui64 s = isource & f64_sign_mask;
    ui64 e = isource & f64_exp_mask;
    if( e == 0 )
    {
        _val = s >> 48;
        return;
    }
    if( (ui64)((i64)(e >> 52) - 1008ULL) > 31 )
    {
        if( e == (2047ULL << 52) && (isource & f64_significand_mask) )
        {
            _val = f16_nan;
        }
        else if( e > (1038ULL << 52) )
        {
            _val = ca_Overflows[ flowAction ][ s != 0 ];
        }
        else  //  underflow becomes [-0;0]
        {
            ASSUME( e < (1008ULL << 52) );
            _val = s >> 48;
        }
    }
    else
    {
        isource -= (1008ULL << 52);
        _val = (isource & ~f64_sign_mask) >> 42;
        _val |= s >> 48;
    }
}