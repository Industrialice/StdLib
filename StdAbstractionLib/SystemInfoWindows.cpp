#include "PreHeader.hpp"

#ifdef WINDOWS

#include "SystemInfo.hpp"

using namespace StdLib;

namespace
{
	ui32 CpuCores;
	bln is_Initialized;
}

ui32 SystemInfo::CPUCoresCount()
{
	ASSUME( is_Initialized );
	return CpuCores;
}

void SystemInfo_Initialize()
{
	SYSTEM_INFO sysinfo;
	::GetSystemInfo( &sysinfo );
	CpuCores = sysinfo.dwNumberOfProcessors;

	is_Initialized = true;
}

#endif