#ifndef __ATOMIC_OPS_HPP__
#define __ATOMIC_OPS_HPP__

#include <StdCoreLib.hpp>

#if defined(WINDOWS) && !defined(_WIN32_WCE)
    #include <intrin.h>
#endif

namespace StdLib
{
    namespace AtomicOps
    {
        #if defined(WINDOWS)

            #if defined(_M_IX86) || defined(_M_AMD64)

                typedef volatile LONG atomic32;

                FORCEINLINE atomic32 Acquire_Load( const volatile atomic32 *value )
                {
                    return *value;
                }

                FORCEINLINE atomic32 Increment( volatile atomic32 *value )
                {
                    return _InterlockedIncrement( value );
                }

                FORCEINLINE atomic32 Decrement( volatile atomic32 *value )
                {
                    return _InterlockedDecrement( value );
                }

                FORCEINLINE atomic32 Exchange( volatile atomic32 *value, atomic32 exchange )
                {
                    return _InterlockedExchange( value, exchange );
                }

                FORCEINLINE void Set( volatile atomic32 *value, atomic32 set )
                {
                    _InterlockedExchange( value, set );
                }

                FORCEINLINE atomic32 CompareExchange( volatile atomic32 *value, atomic32 exchange, atomic32 comparand )
                {
                    return _InterlockedCompareExchange( value, exchange, comparand );
                }

            #else  //  defined(_M_IX86) || defined(_M_AMD64)

                #define NO_ATOMIC_OPS

            #endif

        #elif defined(POSIX)  //  TODO: there should be a CPU type check

            typedef volatile int atomic32;

            FORCEINLINE atomic32 Acquire_Load( const volatile atomic32 *value )
            {
                atomic32 ret = *value;
                //__sync_synchronize();  following Epic Games
                return ret;
            }

            FORCEINLINE atomic32 Increment( volatile atomic32 *value )
            {
                return __sync_add_and_fetch( value, 1 );
            }

            FORCEINLINE atomic32 Decrement( volatile atomic32 *value )
            {
                return __sync_add_and_fetch( value, -1 );
            }

            FORCEINLINE atomic32 Exchange( volatile atomic32 *value, atomic32 exchange )
            {
                atomic32 oldVal;
                do
                {
                    oldVal = Acquire_Load( value );
                } while ( !__sync_bool_compare_and_swap( value, oldVal, exchange ) );
                return oldVal;
            }

            FORCEINLINE void Set( volatile atomic32 *value, atomic32 set )
            {
                atomic32 oldVal;
                do
                {
                    oldVal = Acquire_Load( value );
                } while ( !__sync_bool_compare_and_swap( value, oldVal, set ) );
            }

            FORCEINLINE atomic32 CompareExchange( volatile atomic32 *value, atomic32 exchange, atomic32 comparand )
            {
                return __sync_val_compare_and_swap( value, comparand, exchange );
            }

        #else

            #define NO_ATOMIC_OPS

        #endif
    }

    #ifdef NO_ATOMIC_OPS

        //#error NO_ATOMIC_OPS

    #endif
}

#endif __ATOMIC_OPS_HPP__