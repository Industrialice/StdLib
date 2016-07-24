#include "PreHeader.hpp"

static ui32 WideRandom()
{
	return rand() | (rand() << 15) | (rand() << 30);
}

i32 Funcs::RandomI32()
{
    return (i32)WideRandom();
}

ui32 Funcs::RandomUI32()
{
    return WideRandom();
}

f32 Funcs::RandomF32()
{
    return rand() / (f32)RAND_MAX;
}

i32 Funcs::RandomRangeI32( i32 from, i32 to )
{
    return (WideRandom() & 0x7FffFFff) % ((to - from) + 1) + from;
}

ui32 Funcs::RandomRangeUI32( ui32 from, ui32 to )
{
    return WideRandom() % ((to - from) + 1) + from;
}

f32 Funcs::RandomRangeF32( f32 from, f32 to )
{
    return from + (rand() / (f32)RAND_MAX) * (to - from);
}

ui32 Funcs::RandomUI32Limit( ui32 limit )
{
    return WideRandom() % limit;
}
	
i32 Funcs::RandomFluctuateI32( i32 target, i32 fluctuation )
{
	i32 distToMin = target - i32_min;
	i32 distToMax = i32_max - target;
	i32 from = target - (distToMin > fluctuation ? fluctuation : distToMin);
	i32 to = target + (distToMax > fluctuation ? fluctuation : distToMax);
	return RandomRangeI32( from, to );
}

ui32 Funcs::RandomFluctuateUI32( ui32 target, ui32 fluctuation )
{
	ui32 distToMin = target;
	ui32 distToMax = ui32_max - target;
	ui32 from = target - (distToMin > fluctuation ? fluctuation : distToMin);
	ui32 to = target + (distToMax > fluctuation ? fluctuation : distToMax);
	return RandomRangeUI32( from, to );
}

f32 Funcs::RandomFluctuateF32( f32 target, f32 fluctuation )
{
	return RandomRangeF32( target - fluctuation, target + fluctuation );
}

i32 Funcs::RoundF32( f32 val )  //  TODO: bullshit
{
    return (i32)(val < 0 ? val - 0.55555555555f : val + 0.55555555555f);
}

i32 Funcs::RoundF32WithPrecise( f32 val, ui32 precise )  //  TODO: bullshit
{
    f32 rounder = 0.f;
    f32 adder = 0.50001f;
    for( ; precise; --precise )
    {
        rounder += adder;
        adder *= 0.1f;
    }
    return (i32)(val < 0 ? val - rounder : val + rounder);
}

f32 Funcs::RoundF32DownToStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val );
    return step * val;
}

f32 Funcs::RoundF32UpToStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val );
    return step * (val + 1.f);
}

f32 Funcs::RoundF32ToNearestStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val + 0.5f );
    return step * val;
}

i32 Funcs::RoundF64( f64 val )  //  TODO: bullshit
{
    return (i32)(val < 0 ? val - 0.555555555555555 : val + 0.555555555555555);
}

i32 Funcs::RoundF64WithPrecise( f64 val, ui32 precise )  //  TODO: bullshit
{
    f64 rounder = 0.0;
    f64 adder = 0.5000001;
    for( ; precise; --precise )
    {
        rounder += adder;
        adder *= 0.1;
    }
    return (i32)(val < 0 ? val - rounder : val + rounder);
}

f64 Funcs::RoundF64DownToStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val );
    return step * val;
}

f64 Funcs::RoundF64UpToStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val );
    return step * (val + 1.f);
}

f64 Funcs::RoundF64ToNearestStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val + 0.5f );
    return step * val;
}

uiw Funcs::RoundUIDownToStep( uiw val, uiw step )
{
    return val - val % step;
}

uiw Funcs::RoundUIUpToStep( uiw val, uiw step )
{
    return val + (step - val % step);
}

uiw Funcs::RoundUIToNearestStep( uiw val, uiw step )
{
    uiw left = val % step;
    uiw low = val - left;
    uiw up = val + (step - left);
    return left >= step / 2 ? up : low;
}

iw Funcs::RoundIDownToStep( iw val, iw step )
{
	NOT_IMPLEMENTED;
    return 0;
}

iw Funcs::RoundIUpToStep( iw val, iw step )
{
	NOT_IMPLEMENTED;
    return 0;
}

iw Funcs::RoundIToNearestStep( iw val, iw step )
{
	NOT_IMPLEMENTED;
    return 0;
}

bln Funcs::IsF32Equal( f32 first, f32 second, f32 epsilon )
{
    return ::fabsf( first - second ) < epsilon;
}

bln Funcs::IsF64Equal( f64 first, f64 second, f64 epsilon )
{
    return ::fabs( first - second ) < epsilon;
}

f32 Funcs::CeilF32Precise( f32 val, ui32 precise )  //  TODO: rework
{
    f32 press = _PowF32( 10.f, precise );
    f32 multed = val * press;
    multed = ::floorf( multed );
    return multed / press;
}

f64 Funcs::CeilF64Precise( f64 val, ui32 precise )  //  TODO: rework
{
    f64 press = _PowF64( 10.0, precise );
    f64 multed = val * press;
    multed = ::floor( multed );
    return multed / press;
}

f32 Funcs::SaturateF32( f32 val )
{
    if( val < 0.f )
    {
        return 0.f;
    }
    if( val > 1.f )
    {
        return 1.f;
    }
    return val;
}

f64 Funcs::SaturateF64( f64 val )
{
    if( val < 0.0 )
    {
        return 0.0;
    }
    if( val > 1.0 )
    {
        return 1.0;
    }
    return val;
}

f32 Funcs::F32NormalizeRadian( f32 rad )
{
    rad = ::fabsf( rad );
    return Funcs::F32FracPart( rad / (f32_pi * 2) ) * (f32_pi * 2);
}
	
f32 Funcs::LerpF32( f32 left, f32 right, f32 value )
{
	ASSUME( value >= 0 && value <= 1 );
	return left + (right - left) * value;
}

f64 Funcs::LerpF64( f64 left, f64 right, f64 value )
{
	ASSUME( value >= 0 && value <= 1 );
	return left + (right - left) * value;
}

f32 Funcs::F32FracPart( f32 val )
{
    return val - ::floorf( val );
}

f64 Funcs::F64FracPart( f64 val )
{
    return val - ::floor( val );
}

bln Funcs::IsF32NaN( f32 val )
{
    return val != val;
}

bln Funcs::IsF64NaN( f64 val )
{
    return val != val;
}

bln Funcs::IsF32Inf( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( (ival | f32_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f32_significand_mask) == 0;
}

bln Funcs::IsF32InfPos( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) == 0;
}

bln Funcs::IsF32InfNeg( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) != 0;
}

bln Funcs::IsF64Inf( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( (ival | f64_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f64_significand_mask) == 0;
}

bln Funcs::IsF64InfPos( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( !Funcs::IsF64Inf( val ) )
    {
        return false;
    }
    return (ival & f64_sign_mask) == 0;
}

bln Funcs::IsF64InfNeg( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( !Funcs::IsF64Inf( val ) )
    {
        return false;
    }
    return (ival & f64_sign_mask) != 0;
}

f32 Funcs::MakeF32ByBits( ui32 sign1, ui32 exp8, ui32 significand23 )
{
    union
    {
        ui32 i;
        f32 f;
    } u;
    u.i = sign1 << 31;
    u.i |= exp8 << 23;
    u.i |= significand23;
    return u.f;
}

f64 Funcs::MakeF64ByBits( uiw sign1, uiw exp11, ui64 significand52 )
{
    union
    {
        ui64 i;
        f64 f;
    } u;
    u.i = (ui64)sign1 << 63;
    u.i |= (ui64)exp11 << 52;
    u.i |= significand52;
    return u.f;
}

f32 Funcs::MakeF32ByND( i32 numerator, i32 denominator )
{
    return (f32)numerator / denominator;
}

f64 Funcs::MakeF64ByND( i32 numerator, i32 denominator )
{
    return (f64)numerator / denominator;
}

ui32 Funcs::FNV32Hash( const byte *source, uiw maxLen )
{
	ui32 hash = 2166136261;
	for( uiw index = 0; index < maxLen && source[ index ]; ++index )
	{
		hash ^= source[ index ];
		hash *= 16777619;
	}
	return hash;
}

ui64 Funcs::FNV64Hash( const byte *source, uiw maxLen )
{
	ui64 hash = 14695981039346656037ULL;
	for( uiw index = 0; index < maxLen && source[ index ]; ++index )
	{
		hash ^= source[ index ];
		hash *= 1099511628211;
	}
	return hash;
}

uiw Funcs::FNVWordHash( const byte *source, uiw maxLen )
{
#if WORD_SIZE == 32
	return FNV32Hash( source, maxLen );
#else
	return FNV64Hash( source, maxLen );
#endif
}

uiw Funcs::MemCpy( void *RSTR p_dest, const void *cp_source, uiw size )
{
    ::memcpy( p_dest, cp_source, size );
    return size;
}

uiw Funcs::MemZero( void *p_mem, uiw size )
{
    ASSUME( p_mem );
    ::memset( p_mem, 0, size );
    return size;
}

uiw Funcs::MemSet( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem );
    ::memset( p_mem, val, size );
    return size;
}

bln Funcs::MemTest( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem );
    byte *p_memByte = (byte *)p_mem;
    while( size )
    {
        if( *p_memByte != val )
        {
            return false;
        }
        ++p_memByte;
        --size;
    }
    return true;
}

bln Funcs::MemEquals( const void *cp_mem0, const void *cp_mem1, uiw size )
{
    ASSUME( cp_mem0 && cp_mem1 );
    return !::memcmp( cp_mem0, cp_mem1, size );
}

void *Funcs::MemFindSeq( const void *cp_mem, const void *cp_seq, uiw memSize, uiw seqSize )
{
    const byte *cp_memByte = (byte *)cp_mem;
    for( ; memSize >= seqSize; --memSize, ++cp_memByte )
    {
        if( _MemEquals( cp_memByte, cp_seq, seqSize ) )
        {
            return (void *)cp_memByte;
        }
    }
    return 0;
}