#include "PreHeader.hpp"
#include "StdHelperLib.hpp"
#include "Misc.hpp"
#include "CMutex.hpp"
#include "FileIO.hpp"

struct MutexInitializer : public CMutex
{
	static void Initialize()
	{
		CMutex::Initialize();
	}
};

EXTERNALS void StdAbstractionLib_Initialize()
{
	static bool is_initialized;
	if( is_initialized )
	{
		return;
	}
    StdHelperLib_Initialize();
	MutexInitializer::Initialize();
	FileIO::Private::FileIO_InitializeFileIOSystem();
    Misc::Private::Initialize();
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
		StdAbstractionLib_Initialize();
		break;
	}
	return TRUE;
}
#endif