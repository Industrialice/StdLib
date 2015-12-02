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
        
    bool is_Initialized;

    class
    {
        unsigned int _memPageSize;
        
        unsigned int _cpuCoresCount;

        float _freqMult32;
        double _freqMult64;
        ui64 _freqDivU64;

    public:
        void Initialize( unsigned int memPageSize, unsigned int cpuCoresCount, float freqMult32, double freqMult64, ui64 freqDivU64 )
        {
            _memPageSize = memPageSize;
            _cpuCoresCount = cpuCoresCount;
            _freqMult32 = freqMult32;
            _freqMult64 = freqMult64;
            _freqDivU64 = freqDivU64;
            is_Initialized = true;
        }

        unsigned int MemPageSize() const
        {
            ASSUME( is_Initialized );
            return _memPageSize;
        }

        unsigned int CpuCoresCount() const
        {
            ASSUME( is_Initialized );
            return _cpuCoresCount;
        }

        float FreqMult32() const
        {
            ASSUME( is_Initialized );
            return _freqMult32;
        }

        double FreqMult64() const
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

void *VirtualMem::Reserve( size_t size )
{
    return ::VirtualAlloc( 0, size, MEM_RESERVE, PAGE_NOACCESS );
}

bool VirtualMem::Commit( void *p_mem, size_t size, PageMode::PageMode_t mode )
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

void *VirtualMem::Alloc( size_t size, PageMode::PageMode_t mode )
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

bool VirtualMem::Free( void *p_mem )
{
    ASSUME( p_mem );
    return ::VirtualFree( p_mem, 0, MEM_RELEASE ) != 0;
}

unsigned int VirtualMem::PageSize()
{
    return MiscData.MemPageSize();
}

NOINLINE VirtualMem::PageMode::PageMode_t VirtualMem::ProtectGet( const void *p_mem, size_t size, SError *po_error )
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

    for( unsigned int index = 0; index < COUNTOF( ca_PageProtectMapping ); ++index )
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

bool VirtualMem::ProtectSet( void *p_mem, size_t size, PageMode::PageMode_t mode )
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

unsigned int CPU::CoresNum()
{
    return MiscData.CpuCoresCount();
}

//  CTC

CTC::CTC( bool is_set /* = false */ )
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

float CTC::Get32() const
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult32();
}

double CTC::Get64() const
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

float CTC::Get32Set()
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    float dt = (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult32();
    _tc = o_count;
    return dt;
}

double CTC::Get64Set()
{
    CHECK( _is_set );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    double dt = (o_count.QuadPart - _tc.QuadPart) * MiscData.FreqMult64();
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

float CTC::Compare32( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return delta * MiscData.FreqMult32();
}

double CTC::Compare64( const CTC &second ) const
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
    LARGE_INTEGER o_freq;
    BOOL freqRes = ::QueryPerformanceFrequency( &o_freq );
    ASSUME( freqRes );
    float freqMult32 = 1.f / o_freq.QuadPart;
    double freqMult64 = 1.0 / o_freq.QuadPart;
    ui64 freqDivU64 = o_freq.QuadPart;
    
    SYSTEM_INFO sysinfo;
    ::GetSystemInfo( &sysinfo );
    unsigned int memPageSize = sysinfo.dwPageSize;
    unsigned int cpuCoresCount = sysinfo.dwNumberOfProcessors;

    MiscData.Initialize( memPageSize, cpuCoresCount, freqMult32, freqMult64, freqDivU64 );
}

#endif WINDOWS
