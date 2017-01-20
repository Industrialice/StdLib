#include "PreHeader.hpp"

#ifdef POSIX

#include <time.h>

CTC::CTC( bln is_set )
{
    if( is_set )
    {
        Set();
    }
    else
    {
        _MemSet( &_tc, 0, sizeof(_tc) );
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

void TimeMoment::Initialize()
{
}

#endif