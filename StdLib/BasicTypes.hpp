#ifndef __BASIC_TYPES_HPP__
#define __BASIC_TYPES_HPP__

#include "PlatformDefines.hpp"

STATIC_CHECK( sizeof(INT64_NUMBER) == 8, "sizeof(long long) is unexpected" );
STATIC_CHECK( sizeof(int)          == 4, "sizeof(int) is unexpected" );
STATIC_CHECK( sizeof(long) == 4 || sizeof(long) == 8, "sizeof(long) is unexpected" );
STATIC_CHECK( sizeof(short)        == 2, "sizeof(short) is unexpected" );
STATIC_CHECK( sizeof(char)         == 1, "sizeof(char) is unexpected" );
STATIC_CHECK( sizeof(bool)         == 1, "sizeof(bool) is unexpected" );
STATIC_CHECK( sizeof(float)        == 4, "sizeof(float) is unexpected" );
STATIC_CHECK( sizeof(double)       == 8, "sizeof(double) is unexpected" );

STATIC_CHECK( sizeof(void *) * 8 == WORD_SIZE, "incorrect WORD_SIZE" );

#if WORD_SIZE == 32
    typedef unsigned int uiw;
    typedef signed int iw;
#elif WORD_SIZE == 64
    typedef UINT64_NUMBER uiw;
    typedef INT64_NUMBER iw;
#else
    #error incorrect WORD_SIZE
#endif
typedef unsigned int uint;
typedef UINT64_NUMBER ui64;
typedef unsigned int ui32;
typedef unsigned short ui16;
typedef unsigned char ui8;
typedef unsigned char byte;
typedef float f32;
typedef double f64;
typedef INT64_NUMBER i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8;
typedef bool bln;

#if WORD_SIZE == 32
    const uiw uiw_max = 4294967295;  //  0xFFffFFff
    const uiw uiw_min = 0;  //  0x00000000
    const iw iw_max = 2147483647;  //  0x7FffFFff
    const iw iw_min = -2147483647 - 1;  //  0x80000000
#elif WORD_SIZE == 64
    const uiw uiw_max = ASUINT64( 18446744073709551615 );  //  0xFFffFFffFFffFFff
    const uiw uiw_min = 0;  //  0x0000000000000000
    const iw iw_max = ASINT64( 9223372036854775807 );  //  0x7FffFFffFFffFFff
    const iw iw_min = ASINT64( -9223372036854775807 ) - 1;  //  0x8000000000000000
#else
    #error incorrect WORD_SIZE
#endif
const ui64 ui64_max = ASUINT64( 18446744073709551615 );  //  0xFFffFFffFFffFFff
const ui64 ui64_min = 0;  //  0x0000000000000000
const ui32 ui32_max = 4294967295;  //  0xFFffFFff
const ui32 ui32_min = 0;  //  0x00000000
const ui16 ui16_max = 65535;  //  0xFFff
const ui16 ui16_min = 0;  //  0x0000
const ui8 ui8_max = 255;  //  0xFF
const ui8 ui8_min = 0;  //  0x00
const bln bln_max = true;
const bln bln_min = false;
const byte byte_max = 255;  //  0xFF
const byte byte_min = 0;  //  0x00
const f32 f32_max = 3.402823466e+38F;
const f32 f32_min = 1.175494351e-38F;
const f64 f64_max = 1.7976931348623158e+308;
const f64 f64_min = 2.2250738585072014e-308;
const i64 i64_max = ASINT64( 9223372036854775807 );  //  0x7FffFFffFFffFFff
const i64 i64_min = ASINT64( -9223372036854775807 ) - 1;  //  0x8000000000000000
const i32 i32_max = 2147483647;  //  0x7FffFFff
const i32 i32_min = -2147483647 - 1;  //  0x80000000
const i16 i16_max = 32767;  //  0x7Fff
const i16 i16_min = -32768;  //  0x8000
const i8 i8_max = 127;  //  0x7F
const i8 i8_min = -128;  //  0x80

#ifdef _MSC_VER
	//  warning C4056: overflow in floating-point constant arithmetic
    #pragma warning( disable: 4056 )
#endif

const f32 f32_inf = f32_max * f32_max;
const f32 f32_inf_pos = f32_max * f32_max;
const f32 f32_inf_neg = -(f32_max * f32_max);
const f32 f32_nan = f32_inf * 0.f;  //  one of the possible NaN values
const ui32 f32_exp_mask = 0x7F800000;
const ui32 f32_significand_mask = 0x007FFFFF;
const ui32 f32_sign_mask = 0x80000000;
const f32 f32_piHalf = 1.570796326794895f;
const f32 f32_pi = 3.14159265358979f;
const f32 f32_pi2 = 6.28318530717958f;
const ui32 f32_smallest_norm_int = 0x800000;
const f32 f32_smallest_norm = *(f32 *)&f32_smallest_norm_int;
const ui32 f32_smallest_denorm_int = 0x1;
const f32 f32_smallest_denorm = *(f32 *)&f32_smallest_denorm_int;

const f64 f64_inf = f64_max * f64_max;
const f64 f64_inf_pos = f64_max * f64_max;
const f64 f64_inf_neg = -(f64_max * f64_max);
const f64 f64_nan = f64_inf * 0.f;  //  one of the possible NaN values
const ui64 f64_exp_mask = ASUINT64( 0x7FF0000000000000 );
const ui64 f64_significand_mask = ASUINT64( 0x000FFFFFFFFFFFFF );
const ui64 f64_sign_mask = ASUINT64( 0x8000000000000000 );
const f32 f64_piHalf = 1.5707963267948966192315;
const f64 f64_pi = 3.141592653589793238463;
const f64 f64_pi2 = 6.283185307179586476926;

#ifdef _MSC_VER
    #pragma warning( default: 4056 )
#endif

template < typename Candidate, typename Of > class IsDerivedFrom
{
    template < typename Candidate2, typename Of2 > struct Converter
    {
        operator Of2 *() const;
        operator Candidate2 *();
    };

    template < typename X > static int Func( volatile Candidate *, X val );
    static char Func( volatile Of *, int val );

public:
    enum { value = sizeof(Func(Converter < Candidate, Of >(), int())) == sizeof(int) };
	operator bln() const
	{
		return value;
	}
	bln operator () () const
	{
		return value;
	}
};

template < typename A, typename B > struct AreTypesTheSame
{
	enum { value = false };
	operator bln() const
	{
		return false;
	}
	bln operator () () const
	{
		return false;
	}
};

template < typename T > struct AreTypesTheSame < T, T >
{
	enum { value = true };
	operator bln() const
	{
		return true;
	}
	bln operator () () const
	{
		return true;
	}
};

template < const uiw al > struct TypeWithSizeAndAlignment;

#define _TypeWithSizeAndAlignment_HelperDefine( align ) \
    template <> struct TypeWithSizeAndAlignment < align > \
    { ALIGNED_PRE( align ) struct type { ui8 u[ align ]; } ALIGNED_POST( align ); \
      STATIC_CHECK( sizeof(type) == (align), "Error in TypeWithSizeAndAlignment" ); \
      STATIC_CHECK( ALIGNOF(type) == (align), "Error in TypeWithSizeAndAlignment" ); \
    };

_TypeWithSizeAndAlignment_HelperDefine( 1 )
_TypeWithSizeAndAlignment_HelperDefine( 2 )
_TypeWithSizeAndAlignment_HelperDefine( 4 )
_TypeWithSizeAndAlignment_HelperDefine( 8 )
_TypeWithSizeAndAlignment_HelperDefine( 16 )
_TypeWithSizeAndAlignment_HelperDefine( 32 )
_TypeWithSizeAndAlignment_HelperDefine( 64 )
_TypeWithSizeAndAlignment_HelperDefine( 128 )

template < uiw numBits > struct IntWithSize{};
template <> struct IntWithSize < 8 >
{
    typedef ui8 uint_t;
    typedef i8 int_t;
};
template <> struct IntWithSize < 16 >
{
    typedef ui16 uint_t;
    typedef i16 int_t;
};
template <> struct IntWithSize < 32 >
{
    typedef ui32 uint_t;
    typedef i32 int_t;
};
template <> struct IntWithSize < 64 >
{
    typedef ui64 uint_t;
    typedef i64 int_t;
};

template < uiw numBits, bln is_signed > struct IntWithSizeAndSign{};
template <> struct IntWithSizeAndSign < 8, true >
{
    typedef i8 type_t;
};
template <> struct IntWithSizeAndSign < 8, false >
{
    typedef ui8 type_t;
};
template <> struct IntWithSizeAndSign < 16, true >
{
    typedef i16 type_t;
};
template <> struct IntWithSizeAndSign < 16, false >
{
    typedef ui16 type_t;
};
template <> struct IntWithSizeAndSign < 32, true >
{
    typedef i32 type_t;
};
template <> struct IntWithSizeAndSign < 32, false >
{
    typedef ui32 type_t;
};
template <> struct IntWithSizeAndSign < 64, true >
{
    typedef i64 type_t;
};
template <> struct IntWithSizeAndSign < 64, false >
{
    typedef ui64 type_t;
};

template < typename X, uiw count = 1 > class AlignmentHelper
{
#ifdef NATIVE_ALIGNOF
    typedef typename TypeWithSizeAndAlignment < ALIGNOF(X) >::type alignedType;
#else
    typedef typename TypeWithSize < DATA_ALIGNMENT * 8 >::type alignedType;
#endif

    enum { bufferArraySize = sizeof(X[ count ]) / sizeof(alignedType) +
        /* precaution */ (sizeof(X[ count ]) % sizeof(alignedType) ? 1 : 0) };

public:
    struct type { alignedType store[ bufferArraySize ]; };

    //STATIC_CHECK( sizeof(X) * count == sizeof(type), "Error in AlignmentHelper" );
    //STATIC_CHECK( ALIGNOF(X) == ALIGNOF(type), "Error in AlignmentHelper" );
};

static const struct Nullv
{
	Nullv() {}
} nullv;

struct CharStrict
{};

struct CharMovable
{};

struct CharPOD : CharMovable
{};

enum TypeSemantic_t
{
    Sem_POD,
    Sem_MovableAsPOD,
    Sem_Strict
};

template < typename X > struct NewDeleter
{
    NewDeleter( X *something )
    {
        delete something;
    }
};

template < typename X > struct NewDeleter < X [] >
{
    NewDeleter( X *something )
    {
        delete[] something;
    }
};

struct MallocDeleter
{
    MallocDeleter( void *something )
    {
        ::free( something );
    }
};

template < bln condition, typename X = void > struct EnableIf
{
    typedef X type;
};

template < typename X > struct EnableIf < false, X >
{};

class CNoInit {};

class CAny
{
public:
    CAny() {  /*  void  */ }
    template < typename X > CAny( X ) {  /*  void  */  }
};
	
#ifdef DEFINE_VARARGS_SUPPORTED
    #ifdef DEBUG
        typedef uiw va_return;
        const va_return va_return_whatever = uiw_max;
    #define VC( func, arg0, ... ) { CAny anys[] = { arg0 __VA_ARGS__ }; va_return ret = func( arg0 __VA_ARGS__ ); if( ret != va_return_whatever && ret != COUNTOF( anys ) ) DBGBREAK; }
    #else
        typedef void va_return;
        #define va_return_whatever
        #define VC( func, arg0, ... ) func( arg0 __VA_ARGS__ )
    #endif
#else
    typedef void va_return;
    #define va_return_whatever
    #define VC( func, arg ) func arg
#endif

template < typename ret > ret DummyFunc()
{
    return ret();
}

template < typename ret, typename a0 > ret DummyFunc( a0 )
{
    return ret();
}

template < typename ret, typename a0, typename a1 > ret DummyFunc( a0, a1 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2 > ret DummyFunc( a0, a1, a2 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3 > ret DummyFunc( a0, a1, a2, a3 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4 > ret DummyFunc( a0, a1, a2, a3, a4 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5 > ret DummyFunc( a0, a1, a2, a3, a4, a5 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10, typename a11 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10, typename a11, typename a12 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10, typename a11, typename a12, typename a13 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10, typename a11, typename a12, typename a13, typename a14 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14 )
{
    return ret();
}

template < typename ret, typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8, typename a9, typename a10, typename a11, typename a12, typename a13, typename a14, typename a15 > ret DummyFunc( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 )
{
    return ret();
}

#endif
