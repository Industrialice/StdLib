#include "PreHeader.hpp"

#ifdef POSIX

#include "SystemInfo.hpp"

using namespace StdLib;

namespace
{
    ui32 CpuCores;
	bool is_Initialized;
}

ui32 SystemInfo::CPUCoresCount()
{
	ASSUME( is_Initialized );
	return CpuCores;
}

void SystemInfo_Initialize()
{
	int cpuCores = sysconf( _SC_NPROCESSORS_ONLN );
	if( cpuCores <= 0 )
	{
		//  TODO: error
	}
	CpuCores = (ui32)cpuCores;
	
	is_Initialized = true;
}

#endif