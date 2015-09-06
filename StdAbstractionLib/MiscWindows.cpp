#include "PreHeader.hpp"

#ifdef WINDOWS

#include "WinCEFuncs.hpp"
#include "Misc.hpp"
#include "Funcs.hpp"

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
        
    bln is_Initialized;

    class
    {
        ui32 _memPageSize;
        
        ui32 _cpuCoresCount;

        f32 _freqMult32;
        f64 _freqMult64;
        ui64 _freqDivU64;

    public:
        void Initialize( ui32 memPageSize, ui32 cpuCoresCount, f32 freqMult32, f64 freqMult64, ui64 freqDivU64 )
        {
            _memPageSize = memPageSize;
            _cpuCoresCount = cpuCoresCount;
            _freqMult32 = freqMult32;
            _freqMult64 = freqMult64;
            _freqDivU64 = freqDivU64;
            is_Initialized = true;
        }

        ui32 MemPageSize() const
        {
            ASSUME( is_Initialized );
            return _memPageSize;
        }

        ui32 CpuCoresCount() const
        {
            ASSUME( is_Initialized );
            return _cpuCoresCount;
        }

        f32 FreqMult32() const
        {
            ASSUME( is_Initialized );
            return _freqMult32;
        }

        f64 FreqMult64() const
        {
            ASSUME( is_Initialized );
            return _freqMult64;
        }

        ui64 FreqDivU64() const
        {
            ASSUME( is_Initialized );
            return _freqDivU64;
        }
    } MiscData;
}

//  VirtualMem

void *VirtualMem::Reserve( uiw size )
{
    return ::VirtualAlloc( 0, size, MEM_RESERVE, PAGE_NOACCESS );
}

bln VirtualMem::Commit( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    DWORD protect = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !protect )
    {
        DBGBREAK;
        return false;
    }
    return ::VirtualAlloc( p_mem, size, MEM_COMMIT, protect ) != 0;
}

void *VirtualMem::Alloc( uiw size, PageMode::PageMode_t mode )
{
    ASSUME( size && mode );
    DWORD protect = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !protect )
    {
        DBGBREAK;
        return false;
    }
    return ::VirtualAlloc( 0, size, MEM_RESERVE | MEM_COMMIT, protect );
}

bln VirtualMem::Free( void *p_mem )
{
    ASSUME( p_mem );
    return ::VirtualFree( p_mem, 0, MEM_RELEASE ) != 0;
}

ui32 VirtualMem::PageSize()
{
    return MiscData.MemPageSize();
}

NOINLINE VirtualMem::PageMode::PageMode_t VirtualMem::ProtectGet( const void *p_mem, uiw size, SError *po_error )
{
    MEMORY_BASIC_INFORMATION o_mbi;
    PageMode::PageMode_t mode = PageMode::Error;
    SError o_error = Error::Get( Error::Ok );
    size = Funcs::RoundUIUpToStep( size, PageSize() );
    SIZE_T infSize = ::VirtualQuery( p_mem, &o_mbi, sizeof(o_mbi) );
    if( !infSize )
    {
        o_error = Error::Get( Error::Unknown );
        goto toExit;
    }
    if( o_mbi.RegionSize < size )
    {
        o_error = Error::GetOther( VirtualMemError::InconsistentProtection, Misc::Private::GetErrorsDesc() );
        goto toExit;
    }

    for( ui32 index = 0; index < COUNTOF( ca_PageProtectMapping ); ++index )
    {
        if( ca_PageProtectMapping[ index ] == o_mbi.Protect )
        {
            mode = (PageMode::PageMode_t)index;
            break;
        }
    }

    ASSUME( mode != PageMode::Error );

toExit:
    DSA( po_error, o_error );
    return mode;
}

bln VirtualMem::ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    DWORD oldProtect;
    DWORD protect = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !protect )
    {
        DBGBREAK;
        return false;
    }
    return ::VirtualProtect( p_mem, size, protect, &oldProtect ) != 0;
}

//  CPU

ui32 CPU::CoresNum()
{
    return MiscData.CpuCoresCount();
}

//  CTC

CTC::CTC( bln is_set /* = false */ )
{
    if( is_set )
    {
        Set();
    }
    else
    {
        _MemSet( &_tc, 0, sizeof(_tc) );
        DBGCODE( _is_set = false );
    }
}

void CTC::Set()
{
    ::QueryPerformanceCounter( &_tc );
    DBGCODE( _is_set = true );
}

f32 CTC::Get32() const
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult32();
}

f64 CTC::Get64() const
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult64();
}

ui64 CTC::GetUSec64() const
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return ((o_count.QuadPart - _tc.QuadPart) * (1000ULL * 1000ULL * 1000ULL)) / MiscData.FreqDivU64();
}

f32 CTC::Get32Set()
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    f32 dt = (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult32();
    _tc = o_count;
    return dt;
}

f64 CTC::Get64Set()
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    f64 dt = (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult64();
    _tc = o_count;
    return dt;
}

ui64 CTC::GetUSec64Set()
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    ui64 dt = ((o_count.QuadPart - _tc.QuadPart) * (1000ULL * 1000ULL * 1000ULL)) / MiscData.FreqDivU64();
    _tc = o_count;
    return dt;
}

f32 CTC::Compare32( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return delta * MiscData.FreqMult32();
}

f64 CTC::Compare64( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return delta * MiscData.FreqMult64();
}

ui64 CTC::CompareUSec64( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return (delta * (1000ULL * 1000ULL * 1000ULL)) / MiscData.FreqDivU64();
}

const tcs &CTC::TCSGet() const
{
    CHECK( _is_set );
    return _tc;
}

const char *const *Misc::Private::GetErrorsDesc()
{
    static const char *const errors[] = { "INCONSISTENT_PROTECTION" };
    return errors;
}

void Misc::Private::Initialize()
{
    SYSTEM_INFO o_si;
    ::GetSystemInfo( &o_si );
    ui32 memPageSize = o_si.dwPageSize;

    LARGE_INTEGER o_freq;
    BOOL freqRes = ::QueryPerformanceFrequency( &o_freq );
    ASSUME( freqRes );
    f32 freqMult32 = 1.f / o_freq.QuadPart;
    f64 freqMult64 = 1.0 / o_freq.QuadPart;
    ui64 freqDivU64 = o_freq.QuadPart;
    
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    ui32 cpuCoresCount = sysinfo.dwNumberOfProcessors;

    MiscData.Initialize( memPageSize, cpuCoresCount, freqMult32, freqMult64, freqDivU64 );
}

#endif WINDOWS
