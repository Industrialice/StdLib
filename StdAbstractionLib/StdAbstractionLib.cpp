#include "PreHeader.hpp"
#include "Misc.hpp"

void StdAbstractionLib_Initialize()
{
    StdHelperLib_Initialize();
    Misc::Private::Initialize();
}