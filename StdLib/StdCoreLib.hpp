#ifndef __STD_LIB_HEADER_HPP__
#define __STD_LIB_HEADER_HPP__

#if !defined(WINDOWS) && !defined(POSIX)
    #if defined(_WIN32)
        #define WINDOWS
    #else
        #error you must define platform: WINDOWS or POSIX
    #endif
#endif

#if defined(WINDOWS) && defined(POSIX)
    #error both WINDOWS and POSIX are specified
#endif

#ifndef DATA_ALIGNMENT
    #define DATA_ALIGNMENT 8
#endif

#define EXTERN_TEMPLATES_ALLOWED

#define EXTERNAL

#include <stddef.h>
#include <new>
#include <math.h>
#include <stdlib.h>

#include "CompilerDefines.hpp"

#ifndef WORD_SIZE
    #error you must define machine word size in bits
#endif

#if WORD_SIZE != 32 && WORD_SIZE != 64
    #error incorrect WORD_SIZE
#endif

#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
    #error you must define endianness: LITTLE_ENDIAN or BIG_ENDIAN
#endif

#if defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
    #error both LITTLE_ENDIAN and BIG_ENDIAN are specified
#endif

#include "BasicDefines.hpp"
#include "PlatformDefines.hpp"
#include "BasicTypes.hpp"
#include "PlatformTypes.hpp"
#include "Funcs.hpp"
#include "CF16.hpp"
#include "CCustomF16.hpp"
#include "Error.hpp"

void StdCoreLib_Initialize();

#endif __STD_LIB_HEADER_HPP__
