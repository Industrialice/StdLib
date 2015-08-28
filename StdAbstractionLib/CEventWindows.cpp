#include "PreHeader.hpp"

#ifdef WINDOWS

#include "CEvent.hpp"

CEvent::~CEvent()
{
    BOOL result = ::CloseHandle( _handle );
    ASSUME( result );
}

CEvent::CEvent( bln isInitiallySignaling /* = false */, bln isResetAfterWait /* = false */ )
{
    _handle = ::CreateEventW( NULL, isResetAfterWait ? FALSE : TRUE, isInitiallySignaling ? TRUE : FALSE, NULL );
    ASSUME( _handle != NULL );
}

void CEvent::WaitFor( ui32 timeout )
{
    DWORD result = ::WaitForSingleObject( _handle, timeout );
    ASSUME( result != WAIT_FAILED );
}

void CEvent::Reset()
{
    BOOL result = ::ResetEvent( _handle );
    ASSUME( result );
}

void CEvent::Raise()
{
    BOOL result = ::SetEvent( _handle );
    ASSUME( result );
}

#endif