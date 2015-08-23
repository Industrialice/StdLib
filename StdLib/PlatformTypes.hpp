#ifndef __PLATFORM_TYPES_HPP__
#define __PLATFORM_TYPES_HPP__

#include "BasicTypes.hpp"

#ifdef WINDOWS

#define NOMINMAX

#include <Windows.h>

typedef LARGE_INTEGER tcs;
typedef HANDLE fileEnumHandle;
static const fileEnumHandle fileEnumHandle_undefined = INVALID_HANDLE_VALUE;
typedef HANDLE threadHandle;
typedef HANDLE eventHandle;
typedef CRITICAL_SECTION mutexHandle;
typedef HANDLE fileHandle;
static const fileHandle fileHandle_undefined = INVALID_HANDLE_VALUE;
static const char FSPathSep = '\\';

#elif defined(POSIX)

#include <unistd.h>

typedef timespec tcs;
typedef DIR * fileEnumHandle;
static const fileEnumHandle fileEnumHandle_undefined = 0;
typedef pthread_t threadHandle;
typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    bln             is_set;
    bln             is_resetAfterWait;
} eventHandle;
typedef pthread_mutex_t mutexHandle;
typedef int fileHandle;
static const fileHandle fileHandle_undefined = -1;
static const char FSPathSep = '/';

#else

#error unknown platform

#endif

#endif __PLATFORM_TYPES_HPP__
