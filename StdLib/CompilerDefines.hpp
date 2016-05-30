#ifndef __COMPILER_DEFINES_HPP__
#define __COMPILER_DEFINES_HPP__

#ifdef _MSC_VER

    #define NOMINMAX

    /*
    MSVC++ 13.0 _MSC_VER == 1900 (Visual Studio 2015)
    MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
    MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
    MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
    MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
    MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
    MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
    MSVC++ 7.0  _MSC_VER == 1300
    MSVC++ 6.0  _MSC_VER == 1200
    MSVC++ 5.0  _MSC_VER == 1100
    */

    #if _MSC_VER >= 1600  /*  TODO: exact version(s)  */
        #include <xkeycheck.h>
    #endif

    #define REMOVE_COMMA
    #define ALIGNED_PRE( al ) __declspec(align(al))
    #define ALIGNED_POST( al )
    #define SYMBOLEXPORT __declspec(dllexport)
    #define SYMBOLIMPORT __declspec(dllimport)
    #define FUNCNAME __FUNCSIG__
    #define RSTR __restrict
    #define FORCEINLINE __forceinline
    #define NORETURN __declspec(noreturn)
    #define UNIQUEPTRRETURN __declspec(restrict)
    #define ALLOCA( size ) _alloca( size )
    #define INT16_CHANGE_ENDIANNESS( val ) _byteswap_ushort( val )
    #define INT32_CHANGE_ENDIANNESS( val ) _byteswap_ulong( val )
    #define INT64_CHANGE_ENDIANNESS( val ) _byteswap_uint64( val )
    #if defined(_M_IX86) || defined(_M_AMD64) || _MSC_VER >= 1700
        #define RETURN_ADDRESS _ReturnAddress()
    #else
        #define RETURN_ADDRESS STATIC_CHECK( 0, "RETURN_ADDRESS is not defined for this compiler-platform" )
    #endif

    #ifdef DEBUG
        #define UNREACHABLE HARDBREAK
        #define ASSUME( what ) CHECK( what )
    #else
        #define UNREACHABLE __assume( 0 )
        #define ASSUME( what ) __assume( what )
    #endif

    #if _MSC_VER >= 1310  //  Visual Studio 2003
        #define COUNTER_SUPPORTED
        #define SWAP_SUPPORTED
        #define ALIGNOF( what ) __alignof( what )
        #define NATIVE_ALIGNOF
        #define EXTERN_TEMPLATES_SUPPORTED
        #define LONGLONG_SUPPORTED
    #endif

    #if _MSC_VER >= 1400  //  Visual Studio 2005
        #define DEFINE_VARARGS_SUPPORTED
        #define OVERRIDE_SUPPORTED
        #define NOINLINE __declspec(noinline)
    #endif

    #if _MSC_VER >= 1500  //  Visual Studio 2008
        #define TYPETRAITS_SUPPORTED
    #endif

    #if _MSC_VER >= 1600  //  Visual Studio 2010
        #define STATIC_ASSERTION_SUPPORTED
        #define MOVE_SUPPORTED
    #endif

    #if _MSC_VER >= 1700  //  Visual Studio 2012
        #define FINAL_SUPPORTED
        #define DEFAULT_FUNC_PARAMS_SUPPORTED
        #define NULLPTR_SUPPORTED
        #define RANGE_BASED_FOR_SUPPORTED
    #endif

    #if _MSC_VER >= 1800  //  Visual Studio 2013
        #define VAR_TEMPLATES_SUPPORTED
        #define INITIALIZER_LISTS_SUPPORTED
    #endif

    #if _MSC_VER >= 1900  //  Visual Studio 2015
        #define NOEXCEPT noexcept
        #define CONSTEXPR_SUPPORTED
    #endif

    #define INT64_NUMBER __int64
    #define UINT64_NUMBER unsigned __int64
    #define ASINT64( number ) number##I64
    #define ASUINT64( number ) number##UI64

    #if defined(_M_IX86) || defined(_M_AMD64)
        #ifndef LITTLE_ENDIAN
            #define LITTLE_ENDIAN
        #endif

        #ifdef BIG_ENDIAN
            #error your CPU isn not BIG_ENDIAN
        #endif
    #endif

    #if defined(_M_IX86)
        #ifndef WORD_SIZE
            #define WORD_SIZE 32
        #elif WORD_SIZE != 32
            #error incorrect WORD_SIZE
        #endif
    #elif defined(_M_AMD64)
        #ifndef WORD_SIZE
            #define WORD_SIZE 64
        #elif WORD_SIZE != 64
            #error incorrect WORD_SIZE
        #endif
    #endif

    #if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM) || defined(_M_ARMT)

        #if _MSC_VER >= 1700
            #include <intrin.h>
        #endif

        #define ROTATE32R( val, shift ) _rotr( val, shift )
        #define ROTATE32L( val, shift ) _rotl( val, shift )
        #if defined(_M_IX86) || defined(_M_AMD64) || _MSC_VER >= 1700
            #define ROTATE64R( val, shift ) _rotr64( val, shift )
            #define ROTATE64L( val, shift ) _rotl64( val, shift )
            #define ROTATE16R( val, shift ) _rotr16( val, shift )
            #define ROTATE16L( val, shift ) _rotl16( val, shift )
            #define ROTATE8R( val, shift ) _rotr8( val, shift )
            #define ROTATE8L( val, shift ) _rotl8( val, shift )
        #endif

        #if defined(_M_IX86) || defined(_M_AMD64) || _MSC_VER >= 1700
            #define MSNZB32( tosearch, result ) do { ASSUME( tosearch != 0 ); unsigned long r; _BitScanReverse( &r, tosearch ); *result = r; } while( 0 )
            #define LSNZB32( tosearch, result ) do { ASSUME( tosearch != 0 ); unsigned long r; _BitScanForward( &r, tosearch ); *result = r; } while( 0 )

            #define MSNZB64( tosearch, result ) do { ASSUME( tosearch != 0 ); unsigned long r; _BitScanReverse64( &r, tosearch ); *result = r; } while( 0 )
            #define LSNZB64( tosearch, result ) do { ASSUME( tosearch != 0 ); unsigned long r; _BitScanForward64( &r, tosearch ); *result = r; } while( 0 )
        #endif
    #endif

#elif defined(__GNUC__)

    //#define REMOVE_COMMA ##
    #define ALIGNED_PRE( al )
    #define ALIGNED_POST( al ) __attribute__((aligned(al)))
    #ifdef WINDOWS
        #define SYMBOLEXPORT __attribute__(dllexport)
        #define SYMBOLIMPORT __attribute__(dllimport)
    #else
        #define SYMBOLEXPORT __attribute__(visibility("default"))
        #define SYMBOLIMPORT __attribute__(visibility("default"))
    #endif
    #define FUNCNAME __PRETTY_FUNCTION__
    #define RSTR __restrict__
    #define NOINLINE __attribute__((noinline))
    #define FORCEINLINE __attribute__((always_inline))
    #define NORETURN __attribute__(noreturn)
    #define UNIQUEPTRRETURN  /*  there is must be something  */
    #define ALLOCA( size ) __builtin_alloca( size )
    #define INT16_CHANGE_ENDIANNESS( val ) __builtin_bswap16( val )
    #define INT32_CHANGE_ENDIANNESS( val ) __builtin_bswap32( val )
    #define INT64_CHANGE_ENDIANNESS( val ) __builtin_bswap64( val )
    #define RETURN_ADDRESS __builtin_extract_return_addr( __builtin_return_address() )

	/*  TODO: actual checks  */
	#define MOVE_SUPPORTED
	#define VAR_TEMPLATES_SUPPORTED
	#define DEFAULT_FUNC_PARAMS_SUPPORTED
	#define DEFINE_VARARGS_SUPPORTED
	#define EXTERN_TEMPLATES_SUPPORTED
	#define NOEXCEPT noexcept
	#define INITIALIZER_LISTS_SUPPORTED
	#define RANGE_BASED_FOR_SUPPORTED
	#define LONGLONG_SUPPORTED
	#define NATIVE_ALIGNOF
	#define TYPETRAITS_SUPPORTED

	#ifndef __clang__
		#define NULLPTR_SUPPORTED

		#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

		#if GCC_VERSION >= 40300
			#define STATIC_ASSERTION_SUPPORTED
			#define COUNTER_SUPPORTED
		#endif

		#if GCC_VERSION >= 40400
			#define TYPETRAITS_SUPPORTED  /*  TODO: 4.4 is not for sure  */
		#endif

		#if GCC_VERSION >= 40500
			#ifdef DEBUG
				#define UNREACHABLE HARDBREAK
				#define ASSUME( what ) CHECK( what )
			#else
				#define UNREACHABLE __builtin_unreachable()
				#define ASSUME( what ) do { if( !(what) ) { __builtin_unreachable(); } } while( 0 )
			#endif
		#else
			#define UNREACHABLE HARDBREAK
			#define ASSUME( what ) CHECK( what )
		#endif

		#if GCC_VERSION >= 40600
			#define CONSTEXPR_SUPPORTED
		#endif

		#if GCC_VERSION >= 40700
			#define OVERRIDE_SUPPORTED
			#define FINAL_SUPPORTED
		#endif
	#else
		#ifndef __has_feature
			#error something is wrong
		#endif

		#if __has_feature(cxx_static_assert)
			#define STATIC_ASSERTION_SUPPORTED
		#endif

		#if __has_feature(cxx_nullptr)
			#define NULLPTR_SUPPORTED
		#endif

		#if __has_feature(cxx_alignof)
			#define NATIVE_ALIGNOF
		#endif

		#if __has_feature(cxx_constexpr)
			#define CONSTEXPR_SUPPORTED
		#endif

		#if __has_feature(cxx_override_control)
			#define OVERRIDE_SUPPORTED
		#endif

		#if __has_feature(cxx_range_for)
			#define RANGE_BASED_FOR_SUPPORTED
		#endif

		#if __has_feature(cxx_variadic_templates)
			#define VAR_TEMPLATES_SUPPORTED
		#endif

		#if __has_feature(cxx_noexcept)
			#define NOEXCEPT noexcept
		#endif

		#if __has_feature(cxx_default_function_template_args)
			#define DEFAULT_FUNC_PARAMS_SUPPORTED
		#endif

		#ifdef DEBUG
			#define UNREACHABLE HARDBREAK
			#define ASSUME( what ) CHECK( what )
		#else
			#define UNREACHABLE __builtin_unreachable()
			#define ASSUME( what ) do { if( !(what) ) { __builtin_unreachable(); } } while( 0 )
		#endif
	#endif

    #define INT64_NUMBER long long
    #define UINT64_NUMBER unsigned long long
    #define ASINT64( number ) number##LL
    #define ASUINT64( number ) number##ULL

    #define ZERO_LENGTH_ARRAYS_SUPPORTED

    /*  not sure about minimal versions  */
    /*  most significant non-zero bit and least significant  */
    #define MSNZB32( tosearch, result ) ASSUME( tosearch != 0 ); *result = (31 - __builtin_clz( tosearch ))
    #define LSNZB32( tosearch, result ) ASSUME( tosearch != 0 ); *result = __builtin_ctz( tosearch )

    #define MSNZB64( tosearch, result ) ASSUME( tosearch != 0 ); *result = (63 - __builtin_clzll( tosearch ))
    #define LSNZB64( tosearch, result ) ASSUME( tosearch != 0 ); *result = __builtin_clzll( tosearch )

#else

    #error unknown compiler

#endif

#if !defined(OVERRIDE_SUPPORTED)
    #define override
#endif

#if !defined(FINAL_SUPPORTED)
    #define final
#endif

#if !defined(CONSTEXPR_SUPPORTED)
    #define constexpr
#endif

#ifndef ROTATE64R
    #define ROTATE64R( val, shift ) ((ui64)((ui64)(val) >> (shift)) | (ui64)((ui64)(val) << 64 - (shift)))
#endif
#ifndef ROTATE64L
    #define ROTATE64L( val, shift ) ((ui64)((ui64)(val) << (shift)) | (ui64)((ui64)(val) >> 64 - (shift)))
#endif
#ifndef ROTATE32R
    #define ROTATE32R( val, shift ) ((ui32)((ui32)(val) >> (shift)) | (ui32)((ui32)(val) << 32 - (shift)))
#endif
#ifndef ROTATE32L
    #define ROTATE32L( val, shift ) ((ui32)((ui32)(val) << (shift)) | (ui32)((ui32)(val) >> 32 - (shift)))
#endif
#ifndef ROTATE16R
    #define ROTATE16R( val, shift ) ((ui16)((ui16)(val) >> (shift)) | (ui16)((ui16)(val) << 16 - (shift)))
#endif
#ifndef ROTATE16L
    #define ROTATE16L( val, shift ) ((ui16)((ui16)(val) << (shift)) | (ui16)((ui16)(val) >> 16 - (shift)))
#endif
#ifndef ROTATE8R
    #define ROTATE8R( val, shift ) ((ui8)((ui8)(val) >> (shift)) | (ui8)((ui8)(val) << 8 - (shift)))
#endif
#ifndef ROTATE8L
    #define ROTATE8L( val, shift ) ((ui8)((ui8)(val) << (shift)) | (ui8)((ui8)(val) >> 8 - (shift)))
#endif

#ifndef MSNZB32
    #define MSNZB32( tosearch, result ) for( ui32 index = 31; ; --index ) { ASSUME( tosearch != 0 ); if( tosearch & 1 << index ) return index; }
#endif
#ifndef LSNZB32
    #define LSNZB32( tosearch, result ) for( ui32 index = 0; ; ++index ) { ASSUME( tosearch != 0 ); if( tosearch & 1 << index ) return index; }
#endif
#ifndef MSNZB64
    #define MSNZB64( tosearch, result ) for( ui32 index = 63; ; --index ) { ASSUME( tosearch != 0 ); if( tosearch & 1ULL << index ) return index; }
#endif
#ifndef LSNZB64
    #define LSNZB64( tosearch, result ) for( ui32 index = 0; ; ++index ) { ASSUME( tosearch != 0 ); if( tosearch & 1ULL << index ) return index; }
#endif

#ifndef ALIGNOF
    template < typename type > struct __AlignmentCheck
    {
        char c;
        type t;
    };
    #define ALIGNOF( type ) offsetof(__AlignmentCheck<type>, t)
#endif

#ifndef NOEXCEPT
    #define NOEXCEPT
#endif

/*  TODO: wtf  */
#if defined(TYPETRAITS_SUPPORTED) && defined(_WIN32_WCE)
    #undef TYPETRAITS_SUPPORTED
#endif

#ifndef NOINLINE
    #define NOINLINE
#endif

#endif
