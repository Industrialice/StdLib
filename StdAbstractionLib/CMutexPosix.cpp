#include "PreHeader.hpp"

#ifdef POSIX

#include "CMutex.hpp"

/*  TODO: spin locks?  */

CMutex::~CMutex()
{
    int result = pthread_mutex_destroy( &_handle );
    ASSUME( result == 0 );
}

CMutex::CMutex( ui32 spinCount /* = 0 */ )
{
    pthread_mutexattr_t attr;

    int result = pthread_mutexattr_init( &attr );
    ASSUME( result == 0 );

    result = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
    ASSUME( result == 0 );

    result = pthread_mutex_init( &_handle, &attr );
    ASSUME( result == 0 );

    result = pthread_mutexattr_destroy( &attr );
    ASSUME( result == 0 );
}

void CMutex::Lock()
{
    int result = ::pthread_mutex_lock( &_handle );
    ASSUME( result == 0 );
}

void CMutex::Unlock()
{
    int result = ::pthread_mutex_unlock( &_handle );
    ASSUME( result == 0 );
}

bool CMutex::TryLock()
{
    return ::pthread_mutex_trylock( &_handle ) == 0;
}

void CMutex::Initialize()
{
}

#endif