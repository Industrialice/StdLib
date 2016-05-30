#ifndef __BASIC_DEFINES_HPP__
#define __BASIC_DEFINES_HPP__

#define EXTC extern "C"
#define COUNTOF( a ) (sizeof(a) / sizeof(*(a)))
#define BIT( bit ) (1 << (bit))
#define TOSTR( code ) #code
#define CONCAT( first, second ) first##second

#if WORD_SIZE == 32
    #define WSC( word32func, word64func ) word32func
#else
    #define WSC( word32func, word64func ) word64func
#endif

#ifdef COUNTER_SUPPORTED
    #define MAKE_UNIC( name ) CONCAT( name, __COUNTER__ )
#else
    #define MAKE_UNIC( name ) CONCAT( name, __LINE__ )  /*  not really unique  */
#endif

#ifdef MOVE_SUPPORTED
	#define TRY_MOVE( arg ) std::move( arg )
#else
	#define TRY_MOVE( arg ) (arg)
#endif

#define AUTO_EXEC( code ) struct CONCAT( $__AUTO_EXEC_STRUCT__, __LINE__ ) { CONCAT( $__AUTO_EXEC_STRUCT__, __LINE__ )() \
        { \
            code; \
        } \
    } MAKE_UNIC( $__auto_exec_struct_object__ )

#define CONSTS_OPS( name ) \
    static name operator |( name left, name right ) \
    { \
        return (name)((uiw)left | (uiw)right); \
    } \
    static name operator &( name left, name right ) \
    { \
        return (name)((uiw)left & (uiw)right); \
    } \
    static name operator ~( name operand ) \
    { \
        return (name)(~(uiw)operand); \
    } \
    static name operator |=( name &left, name right ) \
    { \
        left = (name)((uiw)left | (uiw)right); \
        return left; \
    } \
    static name operator &=( name &left, name right ) \
    { \
        left = (name)((uiw)left & (uiw)right); \
        return left; \
    }

#ifdef DEFINE_VARARGS_SUPPORTED

    #define CONSTS( name, ... ) enum name { __VA_ARGS__ };

    #define CONSTS_OPED( name, ... ) CONSTS( name, __VA_ARGS__ ); \
        CONSTS_OPS( name )

#endif

#define CREATE_HDL( name ) class name \
{ \
    void *_void; \
public: \
    name() \
    { \
        _void = 0; \
    } \
    \
    name( void *p_val ) \
    { \
        _void = p_val; \
    } \
    \
    void *GetVal() const \
    { \
        return _void; \
    } \
    \
    bln operator == ( const name &test ) const \
    { \
        return this->_void == test._void; \
    } \
    \
    bln operator != ( const name &test ) const \
    { \
        return this->_void != test._void; \
    } \
    \
    bln IsValid() const \
    { \
        return this->_void != 0; \
    } \
};

#ifdef DEFINE_VARARGS_SUPPORTED
    #ifdef DEBUG
        #define DBGCODE( ... ) __VA_ARGS__
        #define RELCODE( ... )
    #else
        #define DBGCODE( ... )
        #define RELCODE( ... ) __VA_ARGS__
    #endif
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

#endif
