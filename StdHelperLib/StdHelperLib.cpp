#include "PreHeader.hpp"
#include "CString.hpp"

namespace StdLib
{
	template class TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
	template class TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
}

EXTERNALS void StdHelperLib_Initialize()
{
    StdCoreLib_Initialize();
}