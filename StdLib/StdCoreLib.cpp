#include "PreHeader.hpp"

EXTERNALS void StdCoreLib_Initialize()
{
	static bln is_initialized;
	if( is_initialized )
	{
		return;
	}
	is_initialized = true;
}

#if defined(WINDOWS) && defined(STDLIB_DYNAMIC)
BOOL WINAPI DllMain( HINSTANCE h_inst, DWORD reason, PVOID )
{
	switch( reason )
	{
	case DLL_PROCESS_ATTACH:
	#if defined(_DLL)  //  dynamically linked CRT must define _DLL
		::DisableThreadLibraryCalls( h_inst );  //  we can't call it if CRT was statically linked
	#endif
		StdCoreLib_Initialize();
		break;
	}
	return TRUE;
}
#endif