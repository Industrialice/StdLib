#ifndef __BASIC_DEFINES_HPP__
#define __BASIC_DEFINES_HPP__

#define BIT( bit ) (1 << (bit))
#define TOSTR( code ) #code
#define CONCAT( first, second ) first##second

#if WORD_SIZE == 32
    #define WSC( word32func, word64func ) word32func
#else
    #define WSC( word32func, word64func ) word64func
#endif

#ifdef COUNTER_SUPPORTED
    #define MAKE_UNIQUE( name ) CONCAT( name, __COUNTER__ )
#else
    #define MAKE_UNIQUE( name ) CONCAT( name, __LINE__ )  /*  not really unique  */
#endif

#define CONSTS_OPS( name ) \
    inline name operator |( name left, name right ) \
    { \
        return (name)((uiw)left | (uiw)right); \
    } \
    inline name operator &( name left, name right ) \
    { \
        return (name)((uiw)left & (uiw)right); \
    } \
    inline name operator ~( name operand ) \
    { \
        return (name)(~(uiw)operand); \
    } \
    inline name operator |=( name &left, name right ) \
    { \
        left = (name)((uiw)left | (uiw)right); \
        return left; \
    } \
    inline name operator &=( name &left, name right ) \
    { \
        left = (name)((uiw)left & (uiw)right); \
        return left; \
    } \
	inline bool operator !( name value ) \
	{ \
		return value == (name)0; \
	}

#ifdef DEBUG
    #define DBGCODE( ... ) __VA_ARGS__
    #define RELCODE( ... )
#else
    #define DBGCODE( ... )
    #define RELCODE( ... ) __VA_ARGS__
#endif

#define NOT_IMPLEMENTED SOFTBREAK

#if defined(STATIC_ASSERTION_SUPPORTED) && !defined(DISALLOW_NATIVE_STATIC_ASSERTION)
    #define STATIC_CHECK( what, str ) static_assert( what, str )
#else
    #ifdef ZERO_LENGTH_ARRAYS_SUPPORTED
        #error no static check for this condition
    #else
        #ifdef COUNTER_SUPPORTED
            #define STATIC_CHECK( what, str ) typedef char CONCAT( __a, __COUNTER__ )[ (what) != 0 ]
        #else
            //#error no static check for this condition
            #define STATIC_CHECK( what, str )  /*  TODO: real check  */
        #endif
    #endif
#endif

/*  if you need more capacity, just use inserted TERSWITCH'es  */
#define TERSWITCH1( case0check, code0, defaultcode ) ((case0check) ? (code0) : (defaultcode))
#define TERSWITCH2( case0check, code0, case1check, code1, defaultcode ) ((case0check) ? (code0) : ((case1check) ? (code1) : (defaultcode)))
#define TERSWITCH3( case0check, code0, case1check, code1, case2check, code2, defaultcode ) ((case0check) ? (code0) : ((case1check) ? (code1) : ((case2check) ? (code2) : (defaultcode))))

#define DSA( pointer, assigning ) if( pointer ) *(pointer) = (assigning)

#endif
