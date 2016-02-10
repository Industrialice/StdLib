#include "PreHeader.hpp"
#include "StdHelperLib.hpp"
#include "Misc.hpp"
#include "CMutex.hpp"

struct MutexInitializer : public CMutex
{
	static void Initialize()
	{
		CMutex::Initialize();
	}
};

void StdAbstractionLib_Initialize()
{
    StdHelperLib_Initialize();
	MutexInitializer::Initialize();
    Misc::Private::Initialize();
}