#include "PreHeader.hpp"
#include "StdHelperLib.hpp"
#include "VirtualMemory.hpp"
#include "CMutex.hpp"
#include "FileIO.hpp"
#include "TimeMoment.hpp"

struct MutexInitializer : private CMutex
{
	static void Initialize()
	{
		CMutex::Initialize();
	}
};

struct TimeMomentInitializer : private TimeMoment
{
	static void Initialize()
	{
		TimeMoment::Initialize();
	}
};

EXTERNALS void StdAbstractionLib_Initialize()
{
	static bool is_initialized;
	if( is_initialized )
	{
		return;
	}

	void VirtualMemory_Initialize();
	void FileIO_InitializeFileIOSystem();
	void SystemInfo_Initialize();

    StdHelperLib_Initialize();
	MutexInitializer::Initialize();
	TimeMomentInitializer::Initialize();
	FileIO_InitializeFileIOSystem();
    VirtualMemory_Initialize();
	SystemInfo_Initialize();
	is_initialized = true;
}

#if defined(WINDOWS) && defined(STDLIB_DYNAMIC)
BOOL WINAPI DllMain( HINSTANCE h_inst, DWORD reason, PVOID )
{
	switch( reason )
	{
	case DLL_PROCESS_ATTACH:
	#if defined(_DLL)  //  dynamically linked CRT must define _DLL
		::DisableThreadLibraryCalls( h_inst );  //  we can't call it if CRT has been statically linked
	#endif
		StdAbstractionLib_Initialize();
		break;
	}
	return TRUE;
}
#endif