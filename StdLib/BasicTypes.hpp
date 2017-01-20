#ifndef __BASIC_TYPES_HPP__
#define __BASIC_TYPES_HPP__

#include "PlatformDefines.hpp"

STATIC_CHECK( sizeof(void *) * 8 == WORD_SIZE, "incorrect WORD_SIZE" );

#if WORD_SIZE == 32
    typedef uint32_t uiw;
    typedef int32_t iw;
#elif WORD_SIZE == 64
    typedef uint64_t uiw;
    typedef int32_t iw;
#else
    #error incorrect WORD_SIZE
#endif
typedef uint64_t ui64;
typedef uint32_t ui32;
typedef uint16_t ui16;
typedef uint8_t ui8;
typedef uint8_t byte;
typedef float f32;
typedef double f64;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef bool bln;

const ui64 ui64_max = 18446744073709551615ULL;  //  0xFFffFFffFFffFFff
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
const i64 i64_max = 9223372036854775807LL;  //  0x7FffFFffFFffFFff
const i64 i64_min = -9223372036854775807LL - 1;  //  0x8000000000000000
const i32 i32_max = 2147483647;  //  0x7FffFFff
const i32 i32_min = -2147483647 - 1;  //  0x80000000
const i16 i16_max = 32767;  //  0x7Fff
const i16 i16_min = -32768;  //  0x8000
const i8 i8_max = 127;  //  0x7F
const i8 i8_min = -128;  //  0x80

#if WORD_SIZE == 32
    const uiw uiw_max = ui32_max;
    const uiw uiw_min = 0;
    const iw iw_max = i32_max;
    const iw iw_min = i32_min;
#elif WORD_SIZE == 64
    const uiw uiw_max = ui64_max;
    const uiw uiw_min = 0;
    const iw iw_max = i64_max;
    const iw iw_min = i64_min;
#else
    #error incorrect WORD_SIZE
#endif

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
const ui64 f64_exp_mask = 0x7FF0000000000000ULL;
const ui64 f64_significand_mask = 0x000FFFFFFFFFFFFFULL;
const ui64 f64_sign_mask = 0x8000000000000000ULL;
const f64 f64_piHalf = 1.5707963267948966192315;
const f64 f64_pi = 3.141592653589793238463;
const f64 f64_pi2 = 6.283185307179586476926;

#ifdef _MSC_VER
    #pragma warning( default: 4056 )
#endif

/*template < typename Candidate, typename Of > class IsDerivedFrom
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
};*/

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
    void operator()( X *something )
    {
        delete something;
    }
};

template < typename X > struct NewDeleter < X [] >
{
    void operator()( X *something )
    {
        delete[] something;
    }
};

struct MallocDeleter
{
    void operator()( void *something )
    {
        ::free( something );
    }
};

class CNoInit {};

class CAny
{
public:
    CAny() {  /*  void  */ }
    template < typename X > CAny( X ) {  /*  void  */  }
};
	
#ifdef DEBUG
    typedef uiw va_return;
    const va_return va_return_whatever = uiw_max;
#define VC( func, arg0, ... ) { CAny anys[] = { arg0 __VA_ARGS__ }; va_return ret = func( arg0 __VA_ARGS__ ); if( ret != va_return_whatever && ret != COUNTOF( anys ) ) DBGBREAK; }
#else
    typedef void va_return;
    #define va_return_whatever
    #define VC( func, arg0, ... ) func( arg0 __VA_ARGS__ )
#endif

#endif
