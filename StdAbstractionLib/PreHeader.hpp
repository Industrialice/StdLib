#include <StdHelperLib.hpp>
#include "WinCEFuncs.hpp"

#ifdef STDLIB_DYNAMIC
	#pragma comment( lib, "StdCoreLib_Dynamic.lib" )
	#pragma comment( lib, "StdCoreLib_Static.lib" )
	//#pragma comment( lib, "StdHelperLib_Dynamic.lib" )
	#pragma comment( lib, "StdHelperLib_Static.lib" )
#endif

using namespace StdLib;