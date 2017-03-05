#include "PreHeader.hpp"

#ifdef POSIX

#include <sys/mman.h>

namespace
{
    const int ca_PageProtectMapping[] =
    {
        0,  //  0 - Unused
        0,  //  1 - Write
        PROT_READ,  //  2 - Read
        PROT_WRITE | PROT_READ,  //  3 - Write + Read
        PROT_EXEC,  //  4 - Execute
        0,  //  5 - Execute + Write
        PROT_EXEC | PROT_READ,  //  6 - Execute + Read
        PROT_EXEC | PROT_WRITE | PROT_READ  //  7 - Execute + Write + Read
    };

    bool is_Initialized;

    class __MiscData
    {
        ui32 _cpuCoresCount;

    public:
        void Initialize( ui32 cpuCoresCount )
        {
            _cpuCoresCount = cpuCoresCount;
            is_Initialized = true;
        }

        ui32 CpuCoresCount() const
        {
            ASSUME( is_Initialized );
            return _cpuCoresCount;
        }
    } MiscData;
}

//  VirtualMem

void *VirtualMem::VM_Reserve( uiw size )
{
    ASSUME( size );
    return ::mmap( 0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

bool VirtualMem::VM_Commit( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        SOFTBREAK;
        return false;
    }
    return ::mprotect( p_mem, size, prot ) == 0;
}

void *VirtualMem::VM_Alloc( uiw size, PageMode::PageMode_t mode )
{
    ASSUME( size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        SOFTBREAK;
        return 0;
    }
    return ::mmap( 0, size, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

bool VirtualMem::VM_Free( void *p_mem )
{
    ASSUME( p_mem );
    return ::munmap( p_mem, 0 ) == 0;  //  TODO: munmap with 0 len isn't correct!
}

ui32 VirtualMem::VM_PageSize()
{
    return PAGESIZE;
}

VirtualMem::PageMode::PageMode_t VirtualMem::VM_ProtectGet( const void *p_mem, uiw size, CError *po_error )
{
    DSA( po_error, Error::UnsupportedFeature() );
    return PageMode::Error;
}

bool VirtualMem::VM_ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        SOFTBREAK;
        return false;
    }
    return ::mprotect( p_mem, size, prot ) == 0;
}

void VirtualMemory_Initialize()
{}

#endif