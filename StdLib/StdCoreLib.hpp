#ifndef __STD_LIB_HEADER_HPP__
#define __STD_LIB_HEADER_HPP__

#if !defined(WINDOWS) && !defined(POSIX)
    #if defined(_WIN32) || defined(_WIN64)
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

#include <cstddef>
#include <new>
#include <cmath>
#include <cstdlib>
#include <cstdarg>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <memory>

#include "CompilerDefines.hpp"
	
//  TODO: temporary commented out because it confuses MSVS's intellisense
#define EXTERNALS /*extern "C"*/

#ifdef STDLIB_DYNAMIC
	#ifdef STDLIB_EXPORT
		#define EXTERNALD extern "C" SYMBOLEXPORT
	#else
		#define EXTERNALD extern "C" SYMBOLIMPORT
	#endif
#else
	#define EXTERNALD
#endif

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
    //#error both LITTLE_ENDIAN and BIG_ENDIAN are specified
#endif

#include "TypeDesc.hpp"
#include "Nullable.hpp"
#include "Funcs.hpp"
#include "StringFuncs.hpp"
#include "FormattedPrint.hpp"
#include "CF16.hpp"
#include "CCustomF16.hpp"
#include "Result.hpp"
#include "RandomizingFuncs.hpp"

EXTERNALS void StdCoreLib_Initialize();

#endif
