#include "PreHeader.hpp"

#ifdef POSIX

#include "CEvent.hpp"

CEvent::~CEvent()
{
    int result = pthread_cond_destroy( &_handle.cond );
    ASSUME( result == 0 );
    result = pthread_mutex_destroy( &_handle.lock );
    ASSUME( result == 0 );
}

CEvent::CEvent( bool isInitiallySignaling /* = false */, bool isResetAfterWait /* = false */ )
{
    _handle.is_resetAfterWait = isResetAfterWait;
    _handle.is_set = isInitiallySignaling;

    int result = pthread_cond_init( &_handle.cond, NULL );
    ASSUME( result == 0 );
    result = pthread_mutex_init( &_handle.lock, NULL );
    ASSUME( result == 0 );
}

void CEvent::WaitFor( ui32 timeout )
{
	SOFTBREAK;  //  WTF?  //  TODO:
    int result = pthread_mutex_lock( &_handle.lock );
    ASSUME( result == 0 );

    if( !_handle.is_set )
    {
        result = pthread_cond_wait( &_handle.cond, &_handle.lock );
        ASSUME( result == 0 );
    }

    if( _handle.is_resetAfterWait )
    {
        _handle.is_set = false;
    }

    result = pthread_mutex_unlock( &_handle.lock );
    ASSUME( result == 0 );
}

void CEvent::Reset()
{
    int result = pthread_mutex_lock( &_handle.lock );
    ASSUME( result == 0 );
    _handle.is_set = false;
    result = pthread_mutex_unlock( &_handle.lock );
    ASSUME( result == 0 );
}

void CEvent::Raise()
{
    int result = pthread_mutex_lock( &_handle.lock );
    ASSUME( result == 0 );
    _handle.is_set = true;
    result = pthread_cond_broadcast( &_handle.cond );
    ASSUME( result == 0 );
    result = pthread_mutex_unlock( &_handle.lock );
    ASSUME( result == 0 );
}

#endif