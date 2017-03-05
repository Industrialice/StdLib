#include "PreHeader.hpp"
#include "VirtualMemory.hpp"
#include <RoundingFuncs.hpp>

#ifdef WINDOWS

using namespace StdLib;

namespace
{
    const DWORD ca_PageProtectMapping[] =
    {
        0,  //  0 - Unused
        0,  //  1 - Write
        PAGE_READONLY,  //  2 - Read
        PAGE_READWRITE,  //  3 - Write + Read
        PAGE_EXECUTE,  //  4 - Execute
        0,  //  5 - Execute + Write
        PAGE_EXECUTE_READ,  //  6 - Execute + Read
        PAGE_EXECUTE_READWRITE  //  7 - Execute + Write + Read
    };

	ui32 Static_PageSize;
	ui32 Static_LargePageSize;
	bool is_Initialized;
}

static DWORD SystemProtectionMode( VirtualMemory::PageMode mode )
{
    DWORD protect = ((i32)mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ (i32)mode ]);
	return protect;
}

void *VirtualMemory::Reserve( uiw size )
{
    return ::VirtualAlloc( 0, size, MEM_RESERVE, PAGE_NOACCESS );
}

bool VirtualMemory::Commit( void *p_mem, uiw size, PageMode mode )
{
    ASSUME( p_mem && size );
	DWORD protect = SystemProtectionMode( mode );
    if( !protect )
    {
        SOFTBREAK;
        return false;
    }
    return ::VirtualAlloc( p_mem, size, MEM_COMMIT, protect ) != 0;
}

void *VirtualMemory::Alloc( uiw size, PageMode mode )
{
    ASSUME( size );
	DWORD protect = SystemProtectionMode( mode );
    if( !protect )
    {
        SOFTBREAK;
        return false;
    }
    return ::VirtualAlloc( 0, size, MEM_RESERVE | MEM_COMMIT, protect );
}

bool VirtualMemory::Free( void *p_mem )
{
    ASSUME( p_mem );
    return ::VirtualFree( p_mem, 0, MEM_RELEASE ) != 0;
}

ui32 VirtualMemory::PageSize()
{
	ASSUME( is_Initialized );
    return Static_PageSize;
}

CResult < ui32 > VirtualMemory::LargePageSize()
{
	ASSUME( is_Initialized );
	if( Static_LargePageSize )
	{
		return CResult < ui32 >( Static_LargePageSize );
	}
	return CResult < ui32 >( 0, Error::UnsupportedFeature() );
}

NOINLINE auto VirtualMemory::ProtectGet( const void *p_mem, uiw size ) -> CResult < PageMode >
{
    MEMORY_BASIC_INFORMATION o_mbi;
    PageMode mode;
    CError<> error = Error::Ok();
    size = Funcs::RoundUIUpToStep( size, PageSize() );
    SIZE_T infSize = ::VirtualQuery( p_mem, &o_mbi, sizeof(o_mbi) );
    if( !infSize )
    {
		error = Error::UnknownError();
        goto toExit;
    }
    if( o_mbi.RegionSize < size )
    {
        error = Error_InconsistentProtection();
        goto toExit;
    }

    for( ui32 index = 0; index < COUNTOF( ca_PageProtectMapping ); ++index )
    {
        if( ca_PageProtectMapping[ index ] == o_mbi.Protect )
        {
            mode = (PageMode)index;
            break;
        }
    }

toExit:
    return CResult < PageMode >( mode, error );
}

bool VirtualMemory::ProtectSet( void *p_mem, uiw size, PageMode mode )
{
    ASSUME( p_mem && size );
    DWORD oldProtect;
    DWORD protect = SystemProtectionMode( mode );
    if( !protect )
    {
        SOFTBREAK;
        return false;
    }
    return ::VirtualProtect( p_mem, size, protect, &oldProtect ) != 0;
}

void VirtualMemory_Initialize()
{
	HMODULE k32 = GetModuleHandleA( "kernel32.dll" );
	if( !k32 )
	{
		FatalAppExitA( 1, "StdLib VirtualMemory_Initialize: failed to acquire kernel32.dll handle" );
		return;
	}
	SIZE_T (WINAPI *PtrToGetLargePageMinimum)(void);
	*(FARPROC *)&PtrToGetLargePageMinimum = GetProcAddress( k32, "GetLargePageMinimum" );  //  supported since Vista
	if (PtrToGetLargePageMinimum)
	{
		Static_LargePageSize = PtrToGetLargePageMinimum();
	}

	SYSTEM_INFO sysinfo;
	::GetSystemInfo( &sysinfo );
	Static_PageSize = sysinfo.dwPageSize;

	is_Initialized = true;
}

#endif