#include "PreHeader.hpp"

#ifdef WINDOWS

#include "CMutex.hpp"

CMutex::~CMutex()
{
    ::DeleteCriticalSection( &_handle );
}

CMutex::CMutex( ui32 spinCount /* = 0 */ )
{
    #ifdef DEBUG
        BOOL result = ::InitializeCriticalSectionEx( &_handle, spinCount, 0 );
        ASSUME( result );
    #else
        ::InitializeCriticalSectionEx( &_handle, spinCount, CRITICAL_SECTION_NO_DEBUG_INFO );
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

bln CMutex::TryLock()
{
    return ::TryEnterCriticalSection( &_handle ) == TRUE;
}

#endif