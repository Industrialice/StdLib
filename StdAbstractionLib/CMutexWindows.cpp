#include "PreHeader.hpp"

#ifdef WINDOWS

#include "CMutex.hpp"

CMutex::~CMutex()
{
    ::DeleteCriticalSection( &_handle );
}

CMutex::CMutex( unsigned int spinCount /* = 0 */ )
{
    #ifdef _WIN32_WINNT_VISTA
        #ifdef DEBUG
            BOOL result = ::InitializeCriticalSectionEx( &_handle, spinCount, 0 );
            ASSUME( result );
        #else
            ::InitializeCriticalSectionEx( &_handle, spinCount, CRITICAL_SECTION_NO_DEBUG_INFO );
        #endif
    #else
        ::InitializeCriticalSection( &_handle );
    #endif
}

void CMutex::Lock()
{
    ::EnterCriticalSection( &_handle );
}

void CMutex::Unlock()
{
    ::LeaveCriticalSection( &_handle );
}

bool CMutex::TryLock()
{
    return ::TryEnterCriticalSection( &_handle ) == TRUE;
}

#endif