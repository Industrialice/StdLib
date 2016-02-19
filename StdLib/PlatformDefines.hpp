#ifndef __PLATFORM_DEFINES_HPP__
#define __PLATFORM_DEFINES_HPP__

#ifdef WINDOWS

    #define NOMINMAX

    #include <Windows.h>

    #if defined(DEBUG) || defined(ALLOW_ASSERTIONS)
        #define WHRCHECK( a ) if( FAILED( a ) ) __debugbreak()
        #define CHECK( a ) if( !(a) ) __debugbreak()
        #define DBGBREAK __debugbreak()
        #ifdef DEFINE_VARARGS_SUPPORTED
            #define DBGMSG( ... ) ::fprintf( stderr, __VA_ARGS__ )
            #define DBGMSGBREAK( ... ) ::fprintf( stderr, __VA_ARGS__ ); __debugbreak()
        #else
            #define DBGMSG( arg ) ::fprintf arg
            #define DBGMSGBREAK( arg ) ::fprintf arg; __debugbreak()
        #endif
    #else
        #define WHRCHECK( a )
        #define CHECK( a )
        #define DBGBREAK UNREACHABLE
        #ifdef DEFINE_VARARGS_SUPPORTED
            #define DBGMSG( ... )
            #define DBGMSGBREAK( ... )
        #else
            #define DBGMSG
            #define DBGMSGBREAK
        #endif
    #endif

#elif defined(POSIX)

    #define MAX_PATH 256

    #if defined(DEBUG) || defined(ALLOW_ASSERTIONS)
        #ifdef ANDROID
            #include <android/log.h>
            #define CHECK( a ) if( !(a) ) __android_log_print( ANDROID_LOG_ERROR , "check failed", "%s %s %u", #a, __FILE__, __LINE__ )
            #define DBGBREAK __android_log_print( ANDROID_LOG_ERROR , "debug break", "%s %u", __FILE__, __LINE__)
            #define DBGMSG( ... ) __android_log_print( ANDROID_LOG_ERROR , "debug message", __VA_ARGS__ )
            #define DBGMSGBREAK( ... ) __android_log_print( ANDROID_LOG_ERROR , "debug break", __VA_ARGS__ ); ::raise( SIGINT )
        #else
            #include <signal.h>
            #define CHECK( a ) if( !(a) ) ::raise( SIGINT )
            #define DBGBREAK ::raise( SIGINT )
            #ifdef DEFINE_VARARGS_SUPPORTED
                #define DBGMSG( ... ) ::fprintf( stderr, __VA_ARGS__ )
                #define DBGMSGBREAK( ... ) ::fprintf( stderr, __VA_ARGS__ ); ::raise( SIGINT )
            #else
                #define DBGMSG( arg ) ::fprintf arg
                #define DBGMSGBREAK( arg ) ::fprintf arg; ::raise( SIGINT )
            #endif
        #endif
    #else
        #define CHECK( a )
        #define DBGBREAK UNREACHABLE
        #ifdef DEFINE_VARARGS_SUPPORTED
            #define DBGMSG( ... )
            #define DBGMSGBREAK( ... )
        #else
            #define DBGMSG
            #define DBGMSGBREAK
        #endif
    #endif

#else

    #error unknown platform

#endif

#ifdef min
    #undef min
#endif

#ifdef max
    #undef max
#endif

#endif
