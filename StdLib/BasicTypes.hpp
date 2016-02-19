#ifndef __BASIC_TYPES_HPP__
#define __BASIC_TYPES_HPP__

#if defined(TYPETRAITS_SUPPORTED)
    #include <type_traits>
#endif

STATIC_CHECK( sizeof(INT64_NUMBER) == 8, "sizeof(long long) is unexpected" );
STATIC_CHECK( sizeof(int)        == 4, "sizeof(int) is unexpected" );
STATIC_CHECK( sizeof(long) == 4 || sizeof(long) == 8, "sizeof(long) is unexpected" );
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
    template < typename Candidate2, typename Of2 > struct Converter
    {
        operator Of2 *() const;
        operator Candidate2 *();
    };

    template < typename X > static int Func( volatile Candidate *, X val );
    static char Func( volatile Of *, int val );

public:
    enum { value = sizeof(Func(Converter < Candidate, Of >(), int())) == sizeof(int) };
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

/*  References aren't supported!  */
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

    bln operator == ( const Nullable &source ) const
    {
        return !_is_null && !source._is_null && ToRef() == source.ToRef();
    }

    bln operator == ( Nullv ) const
    {
        return _is_null;
    }

#ifdef NULLPTR_SUPPORTED
    Nullable &operator = ( std::nullptr_t )
    {
        return (*this = nullv);
    }

    bln operator == ( std::nullptr_t ) const
    {
        return _is_null;
    }
#endif

    bln IsNull() const
    {
        return _is_null;
    }

    operator const X & () const
    {
        ASSUME( _is_null == false );
        return ToRef();
    }

	X &Get()
    {
	    return ToRef();
    }

	const X &Get() const
    {
	    return ToRef();
    }
};

//  To prevent using Nullable as a holder for reference types
template < typename X > class Nullable < X & >
{
    Nullable();
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

template < typename X = void > struct MallocDeleter
{
    MallocDeleter( X *something )
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
        Deleter( (X *)_ptr );  //  this dummy C-cast will eliminate a syntax confusion between creating a local variable _ptr and calling a constructor with the argument _ptr
    }

    UniquePtr() : _ptr( 0 )
    {}

    UniquePtr( X *ptr ) : _ptr( ptr )
    {}

#ifdef MOVE_SUPPORTED
    UniquePtr( UniquePtr &&source )
    {
        _ptr = source._ptr;
        source._ptr = 0;
    }

    UniquePtr &operator = ( UniquePtr &&source )
    {
        ASSUME( this != &source );
        _ptr = source._ptr;
        source._ptr = 0;
        return *this;
    }
#endif

    void Own( UniquePtr *source )
    {
        ASSUME( (_ptr != source->_ptr) || (_ptr == 0) );
        Deleter( (X *)_ptr );
        _ptr = source->_ptr;
        source->_ptr = 0;
    }

    void Release()
    {
        Deleter( (X *)_ptr );
        _ptr = 0;
    }

    X *TakeAway()
    {
        X *ret = _ptr;
        _ptr = 0;
        return ret;
    }

    X *Get()
    {
        return _ptr;
    }

    const X *Get() const
    {
        return _ptr;
    }

    X *const *Addr()
    {
        return &_ptr;
    }

    X **AddrModifiable()
    {
        ASSUME( _ptr == 0 );
        return &_ptr;
    }

    X **AddrModifiableRelease()
    {
        Release();
        return &_ptr;
    }

    X **AddrModifiableUnsafe()
    {
        return &_ptr;
    }

    const X *const *Addr() const
    {
        return &_ptr;
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
        return _ptr;
    }

    operator const X * () const
    {
        return _ptr;
    }
};

template < typename X, typename Deleter > class UniquePtr < X[], Deleter > : public UniquePtr < X, Deleter >
{
public:
    UniquePtr()
    {
        this->_ptr = 0;
    }

    UniquePtr( X *ptr )
    {
        this->_ptr = ptr;
    }
};

template < bln condition, typename X = void > struct EnableIf
{
    typedef X type;
};

template < typename X > struct EnableIf < false, X >
{};

/*  volatile doesn't supported  */

template < typename X > struct TypeDesc
{
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_integer = false;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    #ifdef TYPETRAITS_SUPPORTED
        static const bln is_pod = std::is_pod < X >::value || (IsDerivedFrom < X, CharPOD >::value && !IsDerivedFrom < X, CharMovable >::value);

        static const bln is_movableAsPOD = is_pod || (IsDerivedFrom < X, CharMovable >::value && !IsDerivedFrom < X, CharStrict >::value) ||
                                           (std::is_trivially_constructible < X, const X & >::value && std::is_trivially_assignable < X, const X & >::value &&
                                           std::is_trivially_move_constructible < X >::value && std::is_trivially_move_assignable < X >::value);  //  that line is not necessary because you can't have a non-trivial move construct/assign with trivial copy/assign
    #else
        static const bln is_pod = IsDerivedFrom < X, CharPOD >::value && !IsDerivedFrom < X, CharStrict >::value;
        static const bln is_movableAsPOD = is_pod || (IsDerivedFrom < X, CharMovable >::value && !IsDerivedFrom < X, CharStrict >::value);
    #endif
    static const uiw bits = sizeof(X) * 8;
    typedef X type;
};
template < typename X > struct TypeDesc < const X > : TypeDesc < X >
{
    static const bln is_const = true;
    typedef const X type;
};
template < typename X > struct TypeDesc < X & >
{
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_integer = false;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = true;
    static const bln is_pod = false;
    static const bln is_movableAsPOD = false;
    static const uiw bits = sizeof(X &) * 8;
    typedef X type;
};
template < typename X > struct TypeDesc < const X & > : TypeDesc < X & >
{
    static const bln is_const = true;
    typedef const X type;
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
    static const bln is_movableAsPOD = true;
    static const uiw bits = sizeof(X *) * 8;
    typedef X type;
};
template < typename X > struct TypeDesc < const X * > : TypeDesc < X * >
{
    static const bln is_const = true;
    typedef const X type;
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
    static const bln is_pod = TypeDesc < X >::is_pod;
    static const bln is_movableAsPOD = TypeDesc < X >::is_movableAsPOD;
    static const uiw bits = sizeof(X) * 8;
    typedef X type;
};
template < typename X > struct TypeDesc < const X [] > : TypeDesc < X [] >
{
    static const bln is_const = true;
    typedef const X type;
};
template < typename X, uiw size > struct TypeDesc < X [ size ] >
{
    static const bln is_array = true;
    static const bln is_const = false;
    static const bln is_integer = false;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = TypeDesc < X >::is_pod;
    static const bln is_movableAsPOD = TypeDesc < X >::is_movableAsPOD;
    static const uiw bits = sizeof(X) * 8 * size;
    typedef X type;
};
template < typename X, uiw size > struct TypeDesc < const X [ size ] > : TypeDesc < X [ size ] >
{
    static const bln is_const = true;
    typedef const X type;
};

#ifdef LONGLONG_SUPPORTED
template <> struct TypeDesc < signed long long int >
{
    typedef unsigned long long int uint_variant;
    typedef signed long long int int_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 64;
    static const uiw decDigits = 19;
    static const uiw hexDigits = 16;
    static const signed long long int max = LLONG_MAX;
    static const signed long long int min = LLONG_MIN;
    typedef signed long long int type;
};
template <> struct TypeDesc < const signed long long int > : TypeDesc < signed long long int >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned long long int > : TypeDesc < signed long long int >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 20;
    static const unsigned long long int max = ULLONG_MAX;
    static const unsigned long long int min = 0;
    typedef unsigned long long int type;
};
template <> struct TypeDesc < const unsigned long long int > : TypeDesc < unsigned long long int >
{
    static const bln is_const = true;
};
#elif defined(_MSC_VER)
template <> struct TypeDesc < signed __int64 >
{
    typedef unsigned __int64 uint_variant;
    typedef signed __int64 int_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 64;
    static const uiw decDigits = 19;
    static const uiw hexDigits = 16;
    static const signed __int64 max = 0x7FFFFFFFFFFFFFFFI64;
    static const signed __int64 min = 0x8000000000000000I64;
    typedef signed __int64 type;
};
template <> struct TypeDesc < const signed __int64 > : TypeDesc < signed __int64 >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned __int64 > : TypeDesc < signed __int64 >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 20;
    static const unsigned __int64 max = 0xFFFFFFFFFFFFFFFFUI64;
    static const unsigned __int64 min = 0UI64;
    typedef unsigned __int64 type;
};
template <> struct TypeDesc < const unsigned __int64 > : TypeDesc < unsigned __int64 >
{
    static const bln is_const = true;
};
#endif

template <> struct TypeDesc < signed long int >
{
    typedef unsigned long int ulong_variant;
    typedef signed long int long_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 32;
    static const uiw decDigits = 10;
    static const uiw hexDigits = 8;
    static const signed long int max = LONG_MAX;
    static const signed long int min = LONG_MIN;
    typedef signed long int type;
};
template <> struct TypeDesc < const signed long int > : TypeDesc < signed long int >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned long int > : TypeDesc < signed long int >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 10;
    static const unsigned long int max = ULONG_MAX;
    static const unsigned long int min = 0;
    typedef unsigned long int type;
};
template <> struct TypeDesc < const unsigned long int > : TypeDesc < unsigned long int >
{
    static const bln is_const = false;
};

template <> struct TypeDesc < signed int >
{
    typedef unsigned int uint_variant;
    typedef signed int int_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 32;
    static const uiw decDigits = 10;
    static const uiw hexDigits = 8;
    static const signed int max = INT_MAX;
    static const signed int min = INT_MIN;
    typedef signed int type;
};
template <> struct TypeDesc < const signed int > : TypeDesc < signed int >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned int > : TypeDesc < signed int >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 10;
    static const unsigned int max = UINT_MAX;
    static const unsigned int min = 0;
    typedef unsigned int type;
};
template <> struct TypeDesc < const unsigned int > : TypeDesc < unsigned int >
{
    static const bln is_const = false;
};

template <> struct TypeDesc < signed short int >
{
    typedef unsigned short int uint_variant;
    typedef signed short int int_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 16;
    static const uiw decDigits = 5;
    static const uiw hexDigits = 4;
    static const signed short int max = SHRT_MAX;
    static const signed short int min = SHRT_MIN;
    typedef signed short int type;
};
template <> struct TypeDesc < const signed short int > : TypeDesc < signed short int >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned short int > : TypeDesc < signed short int >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 5;
    static const unsigned short int max = USHRT_MAX;
    static const unsigned short int min = 0;
    typedef unsigned short int type;
};
template <> struct TypeDesc < const unsigned short int > : TypeDesc < unsigned short int >
{
    static const bln is_const = true;
};

template <> struct TypeDesc < signed char >
{
    typedef unsigned char uint_variant;
    typedef signed char int_variant;
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_unsigned = false;
    static const bln is_signed = !is_unsigned;
    static const bln is_integer = true;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 8;
    static const uiw decDigits = 3;
    static const uiw hexDigits = 2;
    static const signed char max = 127;
    static const signed char min = -128;
    typedef signed char type;
};
template <> struct TypeDesc < const signed char > : TypeDesc < signed char >
{
    static const bln is_const = true;
};
template <> struct TypeDesc < unsigned char > : TypeDesc < signed char >
{
    static const bln is_const = false;
    static const bln is_unsigned = true;
    static const bln is_signed = !is_unsigned;
    static const uiw decDigits = 3;
    static const unsigned char max = UCHAR_MAX;
    static const unsigned char min = 0;
    typedef unsigned char type;
};
template <> struct TypeDesc < const unsigned char > : TypeDesc < unsigned char >
{
    static const bln is_const = true;
};

template <> struct TypeDesc < float >
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
    static const bln is_movableAsPOD = true;
    static const uiw bits = 32;
    static float max() { return f32_max; }
    static float min() { return f32_min; }
    typedef float type;
};
template <> struct TypeDesc < const float > : TypeDesc < float >
{
    static const bln is_const = true;
};

template <> struct TypeDesc < double >
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
    static const bln is_movableAsPOD = true;
    static const uiw bits = 64;
    static double max() { return f64_max; }
    static double min() { return f64_min; }
    typedef double type;
};
template <> struct TypeDesc < const double > : TypeDesc < double >
{
    static const bln is_const = true;
};

template <> struct TypeDesc < bool >
{
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_integer = false;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    static const bln is_pod = true;
    static const bln is_movableAsPOD = true;
    static const uiw bits = 8;
    static const bool max = true;
    static const bool min = false;
    typedef bool type;
};
template <> struct TypeDesc < const bool > : TypeDesc < bool >
{
    static const bln is_const = true;
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

#endif
