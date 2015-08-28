#include "PreHeader.hpp"
#include "Misc.hpp"

#ifdef POSIX

#include <sys/mman.h>
#include <time.h>

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
}

//  VirtualMem

void *VirtualMem::Reserve( uiw size )
{
    ASSUME( size );
    return ::mmap( 0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

bln VirtualMem::Commit( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        DBGBREAK;
        return false;
    }
    return ::mprotect( p_mem, size, prot ) == 0;
}

void *VirtualMem::Alloc( uiw size, PageMode::PageMode_t mode )
{
    ASSUME( size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        DBGBREAK;
        return false;
    }
    return ::mmap( 0, size, prot, MAP_PRIVATE | MAP_ANON, -1, 0);
}

bln VirtualMem::Free( void *p_mem )
{
    ASSUME( p_mem );
    return ::munmap( p_mem, 0 ) == 0;
}

ui32 VirtualMem::PageSize()
{
    return PAGESIZE;
}

VirtualMem::PageMode::PageMode_t VirtualMem::ProtectGet( const void *p_mem, uiw size, SError *po_error )
{
    DSA( po_error, Error::Get( Error::Unsupported ) );
    return PageMode::Error;
}

bln VirtualMem::ProtectSet( void *p_mem, uiw size, PageMode::PageMode_t mode )
{
    ASSUME( p_mem && size && mode );
    int prot = (mode >= COUNTOF( ca_PageProtectMapping )) ? (0) : (ca_PageProtectMapping[ mode ]);
    if( !prot )
    {
        DBGBREAK;
        return false;
    }
    return ::mprotect( p_mem, size, prot ) == 0;
}

//  CPU

ui32 CPU::CoresNum()
{
#error
}

//  TC

CTC::CTC( bln is_set = false )
{
    if( is_set )
    {
        Set();
    }
    else
    {
        _MemSet( &tc, 0, sizeof(tc) );
    }
}

void CTC::Set()
{
    ::clock_gettime( CLOCK_MONOTONIC, &_tc );
    DBGCODE( _is_set = true );
}

NOINLINE f32 CTC::Get32() const
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    return ((((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0) / 1000.0 / 1000.0;
}

NOINLINE f64 CTC::Get64() const
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    return ((((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0) / 1000.0 / 1000.0;
}

ui64 CTC::GetUSec64() const
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    return (((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0;
}

NOINLINE f32 CTC::Get32Set()
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    f32 dt = ((((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0) / 1000.0 / 1000.0;
    _tc = o_count;
    return dt;
}

NOINLINE f64 CTC::Get64Set()
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    f64 dt = ((((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0) / 1000.0 / 1000.0;
    _tc = o_count;
    return dt;
}

ui64 CTC::GetUSec64Set()
{
    CHECK( _is_set );
    tcs o_count;
    ::clock_gettime( CLOCK_MONOTONIC, &o_count );
    ui64 dt = (((ui64)o_count.tv_sec * 1000000000ULL + o_count.tv_nsec) - ((ui64)_tc.tv_sec * 1000000000ULL + _tc.tv_nsec)) / 1000.0;
    _tc = o_count;
    return dt;
}

//  TODO: complete
NOINLINE f32 CTC::Compare32( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    return 0;
}

NOINLINE f64 CTC::Compare64( const CTC &second ) const
{
    CHECK( _is_set && second._is_set );
    return 0;
}

ui64 CTC::CompareUSec64( const CTC &second ) const
{
    return 0;
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
}

#endif POSIX
