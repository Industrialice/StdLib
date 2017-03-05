#include "PreHeader.hpp"
#include "CString.hpp"

namespace StdLib
{
	template class TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
	template class TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
}

EXTERNALS void StdHelperLib_Initialize()
{
	static bool is_initialized;
	if( is_initialized )
	{
		return;
	}
    StdCoreLib_Initialize();
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
		StdHelperLib_Initialize();
		break;
	}
	return TRUE;
}
#endif