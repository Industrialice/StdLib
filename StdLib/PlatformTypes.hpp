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
typedef wchar_t pathChar;

#elif defined(POSIX)

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <pthread.h>
#include <dirent.h> 

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
typedef char pathChar;

#else

#error unknown platform

#endif

#endif
