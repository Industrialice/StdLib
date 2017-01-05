#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include "PlatformTypes.hpp"

namespace StdLib {

/*  use template versions instead  */
#define MAX( a, b ) TERSWITCH1( (a) > (b), a, b )
#define MIN( a, b ) TERSWITCH1( (a) > (b), b, a )
#define CLAMP( value, min, max ) TERSWITCH2( (value) > (max), max, \
                                             (value) < (min), min, \
                                             (value) );

#define MEMALIGN( mem, alignment ) (void *)((((uiw)mem) + (alignment - 1)) & ~(alignment - 1))

#define MEMREMAP( mem, oldBase, newBase ) (void *)((uiw)mem + ((uiw)newBase - (uiw)oldBase))

namespace Funcs
{
    #define _PowF32( fp, power ) ::powf( fp, (i32)power )
    #define _PowF64( fp, power ) ::pow( fp, (i32)power )
    EXTERNALS bln IsF32Equal( f32 first, f32 second, f32 epsilon );
    #define _IsF32Equal( first, second, epsilon ) (::fabsf( first - second ) < epsilon)
    EXTERNALS bln IsF64Equal( f64 first, f64 second, f64 epsilon );
    #define _IsF64Equal( first, second, epsilon ) (::fabs( first - second ) < epsilon)
    EXTERNALS f32 CeilF32Precise( f32 val, ui32 precise );
    EXTERNALS f64 CeilF64Precise( f64 val, ui32 precise );
    EXTERNALS f32 SaturateF32( f32 val );
    EXTERNALS f64 SaturateF64( f64 val );
    EXTERNALS f32 F32NormalizeRadian( f32 rad );
	EXTERNALS f32 LerpF32( f32 left, f32 right, f32 value );
	EXTERNALS f64 LerpF64( f64 left, f64 right, f64 value );
    EXTERNALS f32 F32FracPart( f32 val );  //  will preserve sign
    EXTERNALS f64 F64FracPart( f64 val );  //  will preserve sign
    EXTERNALS f32 F32IntegerPart( f32 val );  //  will preserve sign
    EXTERNALS f64 F64IntegerPart( f64 val );  //  will preserve sign
    EXTERNALS bln IsF32NaN( f32 val );
    EXTERNALS bln IsF64NaN( f64 val );
    EXTERNALS bln IsF32Inf( f32 val );
    EXTERNALS bln IsF32InfPos( f32 val );
    EXTERNALS bln IsF32InfNeg( f32 val );
    EXTERNALS bln IsF64Inf( f64 val );
    EXTERNALS bln IsF64InfPos( f64 val );
    EXTERNALS bln IsF64InfNeg( f64 val );
    EXTERNALS f32 MakeF32ByBits( ui32 sign1, ui32 exp8, ui32 significand23 );
    EXTERNALS f64 MakeF64ByBits( uiw sign1, uiw exp11, ui64 significand52 );
    EXTERNALS f32 MakeF32ByND( i32 numerator, i32 denominator );
    EXTERNALS f64 MakeF64ByND( i32 numerator, i32 denominator );

    EXTERNALS ui32 FNV32Hash( const byte *source, uiw maxLen = uiw_max );
    EXTERNALS ui64 FNV64Hash( const byte *source, uiw maxLen = uiw_max );
    EXTERNALS uiw FNVWordHash( const byte *source, uiw maxLen = uiw_max );

    EXTERNALS uiw MemCpy( void *RSTR p_target, const void *cp_source, uiw size );
    #define _MemCpy( dest, source, size ) (::memcpy( dest, source, size ), size)
	EXTERNALS uiw MemMove( void *RSTR p_target, const void *cp_source, uiw size );
    #define _MemMove( dest, source, size ) (::memmove( dest, source, size ), size)
    EXTERNALS uiw MemZero( void *p_mem, uiw size );
    #define _MemZero( mem, size ) (::memset( mem, 0, size ), size)
    EXTERNALS uiw MemSet( void *p_mem, byte val, uiw size );
    #define _MemSet( mem, value, size ) (::memset( mem, (byte)value, size ), size)
    EXTERNALS bln MemTest( void *p_mem, byte val, uiw size );  //  true if size is 0
    EXTERNALS bln MemEquals( const void *cp_mem0, const void *cp_mem1, uiw size );  //  true if size is 0
    #define _MemEquals( mem0, mem1, size ) (!::memcmp( mem0, mem1, size ))
    EXTERNALS void *MemFindSeq( const void *cp_mem, const void *cp_seq, uiw memSize, uiw seqSize );  //  assume the output is const if the input was const

    template < typename X > bln IsPowerOf2( const X &val )
    {
        return val && (val & val - 1) == 0;
    }

    template < typename X, typename Y > X BinCopy( const Y &source )
    {
        ASSUME( sizeof(X) == sizeof(Y) );
        return *(X *)&source;
    }

    template < typename X, typename Y > X BinAssign( X *target, const Y &source )
    {
        ASSUME( target && sizeof(X) == sizeof(Y) );
        *target = *(X *)&source;
        return *target;
    }

    template < typename X > X MaxValue( ui32 bits )
    {
        if( bits > TypeDesc < X >::bits )
        {
            return TypeDesc < X >::max;
        }
        return TypeDesc < X >::max >> (TypeDesc < X >::bits - bits);  //  TODO: fix
    }

    //  overflow is undefined behavior here. TODO: safe variant required
    template < typename X > X PowInt( X val, i32 power )
    {
        X powered = 1;
        ui32 absPower = ::abs( power );
        for( ui32 index = 0; index < absPower; ++index )
        {
            powered *= val;
        }
        if( power < 0 )
        {
            return Funcs::RoundF64( 1.0 / powered );
        }
        return powered;
    }

    template < typename X > const X &Max( const X &first, const X &second )
    {
        return first > second ? first : second;
    }

    template < typename X > const X &Min( const X &first, const X &second )
    {
        return first < second ? first : second;
    }

    template < typename X > const X &Clamp( const X &val, const X &low, const X &high )
    {
        if( val < low )
        {
            return low;
        }
        if( val > high )
        {
            return high;
        }
        return val;
    }

    template < typename X, typename Y > void BytewiseCopy( X *p_dest, const Y &source )
    {
        _MemCpy( p_dest, &source, sizeof(Y) );
    }

    template < typename X > X ChangeEndianness( X val )
    {
        STATIC_CHECK( TypeDesc < X >::is_pod, "val is not a POD type in ChangeEndianness" );
        if( sizeof(val) == 8 )
        {
            return INT64_CHANGE_ENDIANNESS( val );
        }
        if( sizeof(val) == 4 )
        {
            return INT32_CHANGE_ENDIANNESS( val );
        }
        if( sizeof(val) == 2 )
        {
            return INT16_CHANGE_ENDIANNESS( val );
        }
        STATIC_CHECK( sizeof(val) == 1, "incorrect size of val in ChangeEndianness" );
        return val;
    }

    template < typename X > X NativeToLittleEndian( X val )
    {
        #ifdef BIG_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X NativeToBigEndian( X val )
    {
        #ifdef LITTLE_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X LittleEndianToNative( X val )
    {
        #ifdef BIG_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X BigEndianToNative( X val )
    {
        #ifdef LITTLE_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

	template < typename X > void ClearPod( X *pod )
	{
		ASSUME( TypeDesc < X >::is_pod );
		_MemSet( pod, 0, sizeof(X) );
	}
}

}  //  namespace StdLib

#endif
