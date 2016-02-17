#include "PreHeader.hpp"

#ifdef WINDOWS

#include "CMutex.hpp"

namespace
{
	BOOL (WINAPI *StdLib_InitializeCriticalSectionEx)( LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags );
	VOID (WINAPI *StdLib_InitializeCriticalSection)( LPCRITICAL_SECTION lpCriticalSection );
}

CMutex::~CMutex()
{
    ::DeleteCriticalSection( &_handle );
}

CMutex::CMutex( ui32 spinCount /* = 0 */ )
{
	DWORD flags = 0;
#ifndef DEBUG
	flags = CRITICAL_SECTION_NO_DEBUG_INFO;
#endif
	if( StdLib_InitializeCriticalSectionEx )
	{
		BOOL result = StdLib_InitializeCriticalSectionEx( &_handle, spinCount, flags );
		ASSUME( result );
	}
	else
	{
		ASSUME( StdLib_InitializeCriticalSection );
		StdLib_InitializeCriticalSection( &_handle );
	}
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

void CMutex::Initialize()
{
	HMODULE k32 = GetModuleHandleA( "kernel32.dll" );
	if( !k32 )
	{
		FatalAppExitA( 1, "StdLib: failed to acquire kernel32.dll handle, can't initialize mutex" );
		return;
	}
	*(uiw *)&StdLib_InitializeCriticalSectionEx = (uiw)GetProcAddress( k32, "InitializeCriticalSectionEx" );
	*(uiw *)&StdLib_InitializeCriticalSection = (uiw)GetProcAddress( k32, "InitializeCriticalSection" );
	if( !StdLib_InitializeCriticalSectionEx && !StdLib_InitializeCriticalSection )
	{
		FatalAppExitA( 1, "StdLib: failed to find neither InitializeCriticalSectionEx nor InitializeCriticalSection, can't initialize mutex" );
	}
}

#endif