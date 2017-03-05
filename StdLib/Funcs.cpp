#include "PreHeader.hpp"

bool Funcs::IsF32Equal( f32 first, f32 second, f32 epsilon )
{
    return ::fabsf( first - second ) < epsilon;
}

bool Funcs::IsF64Equal( f64 first, f64 second, f64 epsilon )
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

f32 Funcs::F32NormalizeRadian( f32 rad )  //  T
{
	if( rad >= 0.0f && rad < f32_pi2 )
	{
		return rad;
	}

	if( rad < 0.0f && rad >= -f32_pi2 )
	{
		return rad + f32_pi2;
	}

	if( rad >= f32_pi2 && rad < (f32_pi2 * 2) )
	{
		return rad - f32_pi2;
	}

	rad = Funcs::F32FracPart( rad / f32_pi2 ) * f32_pi2;
	if( rad < 0.0f )
	{
		rad += f32_pi2;
	}
	return rad;

	//  reference implementation
	//while( rad < 0.0f )
	//{
	//	rad += f32_pi * 2;
	//}

	//while( rad >= f32_pi * 2 )
	//{
	//	rad -= f32_pi * 2;
	//}

	//return rad;
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

f32 Funcs::F32FracPart( f32 val )  //  T
{
	return std::fmod( val, 1.0f );
}

f64 Funcs::F64FracPart( f64 val )  //  T
{
	return std::fmod( val, 1.0 );
}
    
f32 Funcs::F32IntegerPart( f32 val )  //  T
{
	if( val < 0.0f )
	{
		return std::ceilf( val );
	}
	return std::floorf( val );
}
    
f64 Funcs::F64IntegerPart( f64 val )  //  T
{
	if( val < 0.0 )
	{
		return std::ceil( val );
	}
	return std::floor( val );
}

bool Funcs::IsF32NaN( f32 val )
{
    return val != val;
}

bool Funcs::IsF64NaN( f64 val )
{
    return val != val;
}

bool Funcs::IsF32Inf( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( (ival | f32_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f32_significand_mask) == 0;
}

bool Funcs::IsF32InfPos( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) == 0;
}

bool Funcs::IsF32InfNeg( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) != 0;
}

bool Funcs::IsF64Inf( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( (ival | f64_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f64_significand_mask) == 0;
}

bool Funcs::IsF64InfPos( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( !Funcs::IsF64Inf( val ) )
    {
        return false;
    }
    return (ival & f64_sign_mask) == 0;
}

bool Funcs::IsF64InfNeg( f64 val )
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
    union  //  TODO: UB
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
    union  //  TODO: UB
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
	return WSC( FNV32Hash, FNV64Hash )( source, maxLen );
}

uiw Funcs::MemCpy( void *RSTR p_dest, const void *cp_source, uiw size )
{
	ASSUME( p_dest && cp_source || size == 0 );
    ::memcpy( p_dest, cp_source, size );
    return size;
}

uiw Funcs::MemMove( void *RSTR p_dest, const void *cp_source, uiw size )
{
	ASSUME( p_dest && cp_source || size == 0 );
    ::memmove( p_dest, cp_source, size );
    return size;
}

uiw Funcs::MemZero( void *p_mem, uiw size )
{
    ASSUME( p_mem || size == 0 );
    ::memset( p_mem, 0, size );
    return size;
}

uiw Funcs::MemSet( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem || size == 0 );
    ::memset( p_mem, val, size );
    return size;
}

bool Funcs::MemTest( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem || size == 0 );
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

bool Funcs::MemEquals( const void *cp_mem0, const void *cp_mem1, uiw size )
{
    ASSUME( cp_mem0 && cp_mem1 || size == 0 );
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