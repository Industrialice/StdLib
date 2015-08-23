#ifndef __BASIC_TYPES_HPP__
#define __BASIC_TYPES_HPP__

#if defined(ISPOD_SUPPORTED)
    #include <type_traits>
#endif

STATIC_CHECK( sizeof(INT64_NUMBER)  == 8, "sizeof(long long) is unexpected" );
STATIC_CHECK( sizeof(int)        == 4, "sizeof(int) is unexpected" );
STATIC_CHECK( sizeof(short)      == 2, "sizeof(short) is unexpected" );
STATIC_CHECK( sizeof(char)       == 1, "sizeof(char) is unexpected" );
STATIC_CHECK( sizeof(bool)       == 1, "sizeof(bool) is unexpected" );
STATIC_CHECK( sizeof(float)      == 4, "sizeof(float) is unexpected" );
STATIC_CHECK( sizeof(double)     == 8, "sizeof(double) is unexpected" );

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
    #pragma warning( disable: 4056 )
#endif

const f32 f32_inf = f32_max * f32_max;
const f32 f32_inf_pos = f32_max * f32_max;
const f32 f32_inf_neg = -(f32_max * f32_max);
const f32 f32_nan = f32_inf * 0.f;  //  one of the possible NaN values
const ui32 f32_exp_mask = 0x7F800000;
const ui32 f32_significand_mask = 0x007FFFFF;
const ui32 f32_sign_mask = 0x80000000;
const f32 f32_pi = 3.14159265358979f;
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
const f64 f64_pi = 3.141592653589793238463;

#ifdef _MSC_VER
    #pragma warning( default: 4056 )
#endif

template < typename Candidate, typename Of > class IsDerivedFrom
{
	template < typename Candidate, typename Of > struct Converter
	{
		operator Of *() const;
		operator Candidate *();
	};

	template < typename X > static int Func( volatile Candidate *, X val );
	static char Func( volatile Of *, int val );

public:
	enum { value = sizeof(Func(Converter < Candidate, Of >(), int())) == sizeof(int) };
};

template < uiw numBits > struct TypeWithSizeAndAlignment
{
    ALIGNED_PRE( numBits / 8 ) struct type { ui8 u[ numBits / 8 ]; } ALIGNED_POST( numBits / 8 );

    STATIC_CHECK( sizeof(type) == (numBits / 8), "Error in TypeWithSizeAndAlignment" );
    STATIC_CHECK( ALIGNOF(type) == (numBits / 8), "Error in TypeWithSizeAndAlignment" );
};

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

template < typename X, size_t count = 1 > class AlignmentHelper
{
#ifdef NATIVE_ALIGNOF
	typedef typename TypeWithSizeAndAlignment < ALIGNOF(X) * 8 >::type alignedType;
#else
	typedef typename TypeWithSize < DATA_ALIGNMENT * 8 >::type alignedType;
#endif

	enum { bufferArraySize = sizeof(X[ count ]) / sizeof(alignedType) +
		/* precaution */ (sizeof(X[ count ]) % sizeof(alignedType) ? 1 : 0) };

public:
    struct type { alignedType store[ bufferArraySize ]; };

    STATIC_CHECK( sizeof(X) == sizeof(type), "Error in AlignmentHelper" );
    STATIC_CHECK( ALIGNOF(X) == ALIGNOF(type), "Error in AlignmentHelper" );
};

static const struct Nullv
{} nullv;

template < typename X > class Nullable
{
    typename AlignmentHelper < X >::type _object;
    bln _is_null;

    X &ToRef()
    {
        return *(X *)&_object;
    }

    const X &ToRef() const
    {
        return *(const X *)&_object;
    }

public:
    ~Nullable()
    {
        if( !_is_null )
        {
            ToRef().~X();
        }
    }

    Nullable()
    {
        _is_null = true;
    }

    Nullable( const X &source )
    {
        new (&_object) X( source );
        _is_null = false;
    }

    Nullable( Nullv )
    {
        _is_null = true;
    }

    Nullable &operator = ( const X &source )
    {
        if( _is_null )
        {
            new (&_object) X( source );
            _is_null = false;
        }
        else
        {
            ToRef() = source;
        }
        return *this;
    }

    Nullable &operator = ( Nullv )
    {
        if( !_is_null )
        {
            ToRef().~X();
            _is_null = true;
        }
        return *this;
    }

    bool operator == ( const Nullable &source ) const
    {
        return !_is_null && !source._is_null && ToRef() == source.ToRef();
    }

    bool operator == ( Nullv ) const
    {
        return _is_null;
    }

#ifdef NULLPTR_SUPPORTED
    Nullable &operator = ( std::nullptr_t )
    {
        return (*this = nullv);
    }
    
    bool operator == ( std::nullptr_t ) const
    {
        return _is_null;
    }
#endif

    bool IsNull() const
    {
        return _is_null;
    }

    operator const X & () const
    {
        ASSUME( _is_null == false );
        return ToRef();
    }
};

struct CharMovable
{};

struct CharPOD : CharMovable
{};

template < typename X > struct NewDeleter
{
    NewDeleter( void *something )
    {
        delete something;
    }
};

template < typename X > struct NewDeleter < X [] >
{
    NewDeleter( void *something )
    {
        delete[] something;
    }
};

template < typename X = void > struct MallocDeleter
{
    MallocDeleter( void *something )
    {
        ::free( something );
    }
};

template < typename X, typename Deleter = NewDeleter < X > > class UniquePtr
{
    UniquePtr( const UniquePtr &source );
    UniquePtr &operator = ( const UniquePtr &source );

protected:
    X *_ptr;

public:
    ~UniquePtr()
    {
        Deleter( (void *)_ptr );
    }

    UniquePtr() : _ptr( 0 )
    {}

    UniquePtr( X *ptr ) : _ptr( ptr )
    {}

    void Own( UniquePtr *source )
    {
        Deleter( (void *)_ptr );
        _ptr = source->_ptr;
        source->_ptr = 0;
    }

    void Release()
    {
        Deleter( (void *)_ptr );
        _ptr = 0;
    }

    UniquePtr &operator = ( X *ptr )
    {
        Deleter( (void *)_ptr );
        _ptr = ptr;
        return *this;
    }

    const X &operator * () const
    {
        ASSUME( _ptr );
        return *_ptr;
    }

    X &operator * ()
    {
        ASSUME( _ptr );
        return *_ptr;
    }

    const X *operator -> () const
    {
        ASSUME( _ptr );
        return _ptr;
    }

    X *operator -> ()
    {
        ASSUME( _ptr );
        return _ptr;
    }

    operator X * ()
    {
        ASSUME( _ptr );
        return _ptr;
    }

    operator const X * () const
    {
        ASSUME( _ptr );
        return _ptr;
    }
};

template < typename X, typename Deleter > class UniquePtr < X[], Deleter > : public UniquePtr < X, Deleter >
{
public:
    UniquePtr()
    {
        _ptr = 0;
    }

    UniquePtr( X *ptr )
    {
        _ptr = ptr;
    }
};

/*  volatile is not really supported  */

template < typename X > struct TypeDesc 
{
    static const bln is_array = false;
	static const bln is_const = false;
	static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	#ifdef ISPOD_SUPPORTED
		static const bln is_pod = std::is_pod < X >::value || IsDerivedFrom < X, CharPOD >::value;
	#else
		static const bln is_pod = IsDerivedFrom < X, CharPOD >::value;
	#endif
	static const bln is_movable = is_pod || IsDerivedFrom < X, CharMovable >::value;
    static const uiw bits = sizeof(X) * 8;
	typedef X & ref;
	typedef X * pointer;
};
template < typename X > struct TypeDesc < const X > : TypeDesc < X >
{
	static const bln is_const = true;
	typedef const X & ref;
	typedef const X * pointer;
};
template < typename X > struct TypeDesc < X & >
{
    static const bln is_array = false;
	static const bln is_const = false;
	static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = true;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = sizeof(X &) * 8;
	typedef X & ref;
	typedef X * pointer;
};
template < typename X > struct TypeDesc < const X & > : TypeDesc < X & >
{
	static const bln is_const = true;
	typedef const X & ref;
	typedef const X * pointer;
};
template < typename X > struct TypeDesc < X * >
{
    static const bln is_array = false;
	static const bln is_const = false;
	static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = true;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = sizeof(X *) * 8;
	typedef X & ref;
	typedef X * pointer;
};
template < typename X > struct TypeDesc < const X * > : TypeDesc < X * >
{
	static const bln is_const = true;
	typedef const X & ref;
	typedef const X * pointer;
};
template < typename X > struct TypeDesc < X [] >
{
    static const bln is_array = true;
	static const bln is_const = false;
	static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = sizeof(X) * 8;
	typedef X & ref;
	typedef X * pointer;
};
template < typename X > struct TypeDesc < const X [] > : TypeDesc < X [] >
{
	static const bln is_const = true;
	typedef const X & ref;
	typedef const X * pointer;
};
template < typename X, size_t size > struct TypeDesc < X [ size ] >
{
    static const bln is_array = true;
	static const bln is_const = false;
	static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = sizeof(X) * 8 * size;
	typedef X & ref;
	typedef X * pointer;
};
template < typename X, size_t size > struct TypeDesc < const X [ size ] > : TypeDesc < X [ size ] >
{
	static const bln is_const = true;
	typedef const X & ref;
	typedef const X * pointer;
};

template <> struct TypeDesc < i64 >
{
    typedef ui64 uint_variant;
    typedef i64 int_variant;
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 64;
    static const uiw decDigits = 19;
    static const uiw hexDigits = 16;
    static const i64 max = i64_max;
    static const i64 min = i64_min;
	typedef i64 & ref;
	typedef i64 * pointer;
};
template <> struct TypeDesc < const i64 > : TypeDesc < i64 >
{
	static const bln is_const = true;
	typedef const i64 & ref;
	typedef const i64 * pointer;
};
template <> struct TypeDesc < ui64 > : TypeDesc < i64 >
{
	static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 20;
    static const ui64 max = ui64_max;
    static const ui64 min = ui64_min;
	typedef ui64 & ref;
	typedef ui64 * pointer;
};
template <> struct TypeDesc < const ui64 > : TypeDesc < ui64 >
{
	static const bln is_const = true;
	typedef const ui64 & ref;
	typedef const ui64 * pointer;
};

template <> struct TypeDesc < i32 >
{
    typedef ui32 uint_variant;
    typedef i32 int_variant;
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 32;
    static const uiw decDigits = 10;
    static const uiw hexDigits = 8;
    static const i32 max = i32_max;
    static const i32 min = i32_min;
	typedef i32 & ref;
	typedef i32 * pointer;
};
template <> struct TypeDesc < const i32 > : TypeDesc < i32 >
{
	static const bln is_const = true;
	typedef const i32 & ref;
	typedef const i32 * pointer;
};
template <> struct TypeDesc < ui32 > : TypeDesc < i32 >
{
	static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 10;
    static const ui32 max = ui32_max;
    static const ui32 min = ui32_min;
	typedef ui32 & ref;
	typedef ui32 * pointer;
};
template <> struct TypeDesc < const ui32 > : TypeDesc < ui32 >
{
	static const bln is_const = false;
	typedef const ui32 & ref;
	typedef const ui32 * pointer;
};

template <> struct TypeDesc < i16 >
{
    typedef ui16 uint_variant;
    typedef i16 int_variant;
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 16;
    static const uiw decDigits = 5;
    static const uiw hexDigits = 4;
    static const i16 max = i16_max;
    static const i16 min = i16_min;
	typedef i16 & ref;
	typedef i16 * pointer;
};
template <> struct TypeDesc < const i16 > : TypeDesc < i16 >
{
	static const bln is_const = true;
	typedef const i16 & ref;
	typedef const i16 * pointer;
};
template <> struct TypeDesc < ui16 > : TypeDesc < i16 >
{
	static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 5;
    static const ui16 max = ui16_max;
    static const ui16 min = ui16_min;
	typedef ui16 & ref;
	typedef ui16 * pointer;
};
template <> struct TypeDesc < const ui16 > : TypeDesc < ui16 >
{
	static const bln is_const = true;
	typedef const ui16 & ref;
	typedef const ui16 * pointer;
};

template <> struct TypeDesc < i8 >
{
    typedef ui8 uint_variant;
    typedef i8 int_variant;
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 8;
    static const uiw decDigits = 3;
    static const uiw hexDigits = 2;
    static const i8 max = i8_max;
    static const i8 min = i8_min;
	typedef i8 & ref;
	typedef i8 * pointer;
};
template <> struct TypeDesc < const i8 > : TypeDesc < i8 >
{
	static const bln is_const = true;
	typedef const i8 & ref;
	typedef const i8 * pointer;
};
template <> struct TypeDesc < ui8 > : TypeDesc < i8 >
{
	static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 3;
    static const ui8 max = ui8_max;
    static const ui8 min = ui8_min;
	typedef ui8 & ref;
	typedef ui8 * pointer;
};
template <> struct TypeDesc < const ui8 > : TypeDesc < ui8 >
{
	static const bln is_const = true;
	typedef const ui8 & ref;
	typedef const ui8 * pointer;
};

template <> struct TypeDesc < f32 >
{
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = false;
	static const bln is_fp = true;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 32;
    static constexpr f32 max() { return f32_max; }
    static constexpr f32 min() { return f32_min; }
	typedef f32 & ref;
	typedef f32 * pointer;
};
template <> struct TypeDesc < const f32 > : TypeDesc < f32 >
{
	static const bln is_const = true;
	typedef const f32 & ref;
	typedef const f32 * pointer;
};

template <> struct TypeDesc < f64 >
{
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = false;
	static const bln is_fp = true;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 64;
    static constexpr f64 max() { return f64_max; }
    static constexpr f64 min() { return f64_min; }
	typedef f64 & ref;
	typedef f64 * pointer;
};
template <> struct TypeDesc < const f64 > : TypeDesc < f64 >
{
	static const bln is_const = true;
	typedef const f64 & ref;
	typedef const f64 * pointer;
};

template <> struct TypeDesc < bln >
{
    static const bln is_array = false;
	static const bln is_const = false;
    static const bln is_integer = false;
	static const bln is_fp = false;
	static const bln is_pointer = false;
	static const bln is_reference = false;
	static const bln is_pod = true;
	static const bln is_movable = true;
    static const uiw bits = 8;
    static const bln max = bln_max;
    static const bln min = bln_min;
	typedef bln & ref;
	typedef bln * pointer;
};
template <> struct TypeDesc < const bln > : TypeDesc < bln >
{
	static const bln is_const = true;
	typedef const bln & ref;
	typedef const bln * pointer;
};

class CNoInit {};

class CAny
{
public:
    CAny() {  /*  void  */ }
    template < typename X > CAny( X ) {  /*  void  */  }
};

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

#endif __BASIC_TYPES_HPP__
