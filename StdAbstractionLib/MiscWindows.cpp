#include "PreHeader.hpp"

#ifdef WINDOWS

#include "WinCEFuncs.hpp"
#include "Misc.hpp"
#include "Funcs.hpp"

const char *const *VirtualMem::Private::GetErrorsDesc()
{
    static const char *const errors[] = { "INCONSISTENT_PROTECTION" };
    return errors;
}

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

    ui32 MemPageSizeInit()
    {
        SYSTEM_INFO o_si;
        ::GetSystemInfo( &o_si );
        return o_si.dwPageSize;
    }
    ui32 MemPageSize = MemPageSizeInit();

    struct SFreq
    {
        f32 freqMult32;
        f64 freqMult64;
        ui64 freqDivU64;

        SFreq()
        {
            LARGE_INTEGER o_freq;
            BOOL freqRes = ::QueryPerformanceFrequency( &o_freq );
            ASSUME( freqRes );
            freqMult32 = 1.f / o_freq.QuadPart;
            freqMult64 = 1.0 / o_freq.QuadPart;
            freqDivU64 = o_freq.QuadPart;
        }
    } o_Freq;
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
    return MemPageSize;
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
        o_error = Error::GetOther( VirtualMemError::InconsistentProtection, Private::GetErrorsDesc() );
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
        DBGCODE( _is_seted = false );
    }
}

void CTC::Set()
{
    ::QueryPerformanceCounter( &_tc );
    DBGCODE( _is_seted = true );
}

f32 CTC::Get32() const
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return (o_count.QuadPart - _tc.QuadPart) * o_Freq.freqMult32;
}

f64 CTC::Get64() const
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return (o_count.QuadPart - _tc.QuadPart) * o_Freq.freqMult64;
}

ui64 CTC::GetUSec64() const
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    return ((o_count.QuadPart - _tc.QuadPart) * (1000ULL * 1000ULL * 1000ULL)) / o_Freq.freqDivU64;
}

f32 CTC::Get32Set()
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    f32 dt = (o_count.QuadPart - _tc.QuadPart) * o_Freq.freqMult32;
    _tc = o_count;
    return dt;
}

f64 CTC::Get64Set()
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    f64 dt = (o_count.QuadPart - _tc.QuadPart) * o_Freq.freqMult64;
    _tc = o_count;
    return dt;
}

ui64 CTC::GetUSec64Set()
{
    CHECK( _is_seted );
    LARGE_INTEGER o_count;
    ::QueryPerformanceCounter( &o_count );
    ui64 dt = ((o_count.QuadPart - _tc.QuadPart) * (1000ULL * 1000ULL * 1000ULL)) / o_Freq.freqDivU64;
    _tc = o_count;
    return dt;
}

f32 CTC::Compare32( const CTC &second ) const
{
    CHECK( _is_seted && second._is_seted );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return delta * o_Freq.freqMult32;
}

f64 CTC::Compare64( const CTC &second ) const
{
    CHECK( _is_seted && second._is_seted );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return delta * o_Freq.freqMult64;
}

ui64 CTC::CompareUSec64( const CTC &second ) const
{
    CHECK( _is_seted && second._is_seted );
    LONGLONG delta = _tc.QuadPart - second.TCSGet().QuadPart;
    return (delta * (1000ULL * 1000ULL * 1000ULL)) / o_Freq.freqDivU64;
}

const tcs &CTC::TCSGet() const
{
    CHECK( _is_seted );
    return _tc;
}

#endif WINDOWS
