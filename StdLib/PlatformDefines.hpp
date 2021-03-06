#ifndef __PLATFORM_DEFINES_HPP__
#define __PLATFORM_DEFINES_HPP__

#include "BasicDefines.hpp"

#ifdef WINDOWS

    #define NOMINMAX

    #include <Windows.h>

	//  actual symbols count + null terminator
	#define MAX_PATH_LENGTH MAX_PATH
	#define MAX_FILENAME_LENGTH 256

    #if defined(DEBUG) || defined(ALLOW_ASSERTIONS)
        #define WHRCHECK( a ) if( FAILED( a ) ) __debugbreak()
        #define CHECK( a ) if( !(a) ) __debugbreak()
		#define SOFTBREAK __debugbreak()
        #define HARDBREAK __debugbreak()
        #define DBGMSG( ... ) ::fprintf( stderr, __VA_ARGS__ )
        #define DBGMSGBREAK( ... ) ::fprintf( stderr, __VA_ARGS__ ); __debugbreak()
    #else
        #define WHRCHECK( a )
        #define CHECK( a )
		#define SOFTBREAK
        #define HARDBREAK UNREACHABLE
        #define DBGMSG( ... )
        #define DBGMSGBREAK( ... )
    #endif

#elif defined(POSIX)

	//  actual symbols count + null terminator
    #define MAX_PATH_LENGTH PATH_MAX
	#define MAX_FILENAME_LENGTH (NAME_MAX + 1)

    #if defined(DEBUG) || defined(ALLOW_ASSERTIONS)
        #ifdef ANDROID
            #include <android/log.h>
            #define CHECK( a ) if( !(a) ) __android_log_print( ANDROID_LOG_ERROR , "check failed", "%s %s %u", #a, __FILE__, __LINE__ )
			#define SOFTBREAK __android_log_print( ANDROID_LOG_ERROR , "debug break", "%s %u", __FILE__, __LINE__)
            #define HARDBREAK __android_log_print( ANDROID_LOG_ERROR , "debug break", "%s %u", __FILE__, __LINE__)
            #define DBGMSG( ... ) __android_log_print( ANDROID_LOG_ERROR , "debug message", __VA_ARGS__ )
            #define DBGMSGBREAK( ... ) __android_log_print( ANDROID_LOG_ERROR , "debug break", __VA_ARGS__ ); ::raise( SIGINT )
        #else
            #include <signal.h>
            #define CHECK( a ) if( !(a) ) ::raise( SIGINT )
            #define SOFTBREAK ::raise( SIGINT )
            #define HARDBREAK ::raise( SIGINT )
            #define DBGMSG( ... ) ::fprintf( stderr, __VA_ARGS__ )
            #define DBGMSGBREAK( ... ) ::fprintf( stderr, __VA_ARGS__ ); ::raise( SIGINT )
        #endif
    #else
        #define CHECK( a )
		#define SOFTBREAK
        #define HARDBREAK UNREACHABLE
        #define DBGMSG( ... )
        #define DBGMSGBREAK( ... )
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
