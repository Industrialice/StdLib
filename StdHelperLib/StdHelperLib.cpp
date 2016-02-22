#include "PreHeader.hpp"
#include "CString.hpp"

namespace StdLib
{
	template class TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
	template class TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
}

void StdHelperLib_Initialize()
{
    StdCoreLib_Initialize();
}