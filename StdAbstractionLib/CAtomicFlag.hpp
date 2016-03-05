#ifndef __CATOMIC_FLAG_HPP__
#define __CATOMIC_FLAG_HPP__

#include "AtomicOps.hpp"

#ifdef NO_ATOMIC_OPS
    #include "CMutex.hpp"
#endif

namespace StdLib
{
    #ifndef NO_ATOMIC_OPS

        class CAtomicFlag
        {
            AtomicOps::atomic32 flag;

        public:
            explicit FORCEINLINE CAtomicFlag( bln initialState = false )
            {
                flag = initialState;
            }

            FORCEINLINE bln IsSet() const
            {
                #ifdef LITTLE_ENDIAN
                    union
                    {
                        AtomicOps::atomic32 flag;
                        bln boolFlag;
                    } u;
                    u.flag = AtomicOps::Acquire_Load( &flag );  //  will return 0 or 1
                    return u.boolFlag;
                #else
                    return AtomicOps::Acquire_Load( &flag );  //  will return 0 or 1
                #endif
            }

            FORCEINLINE void Set()
            {
                AtomicOps::Set( &flag, 1 );
            }

            FORCEINLINE void Reset()
            {
                AtomicOps::Set( &flag, 0 );
            }

            FORCEINLINE void SetTo( bln is_set )
            {
                AtomicOps::Set( &flag, is_set );
            }
        };

    #else

        class CAtomicFlag
        {
            bln flag;
            mutable CMutex mutex;

        public:
            explicit FORCEINLINE CAtomicFlag( bln initialState = false )
            {
                CScopeLock <> lock( &mutex );
                flag = initialState;
            }

            FORCEINLINE bln IsSet() const
            {
                CScopeLock <> lock( &mutex );
                return flag;
            }

            FORCEINLINE void Set()
            {
                CScopeLock <> lock( &mutex );
                flag = true;
            }

            FORCEINLINE void Reset()
            {
                CScopeLock <> lock( &mutex );
                flag = false;
            }

            FORCEINLINE void SetTo( bln is_set )
            {
                CScopeLock <> lock( &mutex );
                flag = is_set;
            }
        };

    #endif
}

#endif