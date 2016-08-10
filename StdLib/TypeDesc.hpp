#ifndef __TYPE_DESC_HPP__
#define __TYPE_DESC_HPP__

#include "PlatformTypes.hpp"

#if defined(TYPETRAITS_SUPPORTED)
    #include <type_traits>
#endif

namespace StdLib {

//  volatile doesn't supported
//  TODO: ** pointers, references to pointers, references to arrays and other composite types
//  TODO: rvalues for C++11

template < typename X > struct TypeDesc
{
    static const bln is_array = false;
    static const bln is_const = false;
    static const bln is_integer = false;
    static const bln is_fp = false;
    static const bln is_pointer = false;
    static const bln is_reference = false;
    #ifdef TYPETRAITS_SUPPORTED
        static const bln is_pod = std::is_pod < X >::value || (IsDerivedFrom < X, CharPOD >::value && !IsDerivedFrom < X, CharMovable >::value && !IsDerivedFrom < X, CharStrict >::value);

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
};
template < typename X > struct TypeDesc < X * const > : TypeDesc < X * >
{
    static const bln is_const = true;
};
template < typename X > struct TypeDesc < const X * const > : TypeDesc < X * >
{
    static const bln is_const = true;
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

}

#endif