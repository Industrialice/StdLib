#include "PreHeader.hpp"
#include "CLogger.hpp"
#include "CVector.hpp"
#include "CMutex.hpp"
#include <stdarg.h>

namespace
{
    struct SLogger : public CLogger
    {
        SLogger( bln is_on, bln is_multithreaded, ui32 bufferSize ) : _is_on( is_on ), _is_multithreaded( is_multithreaded ), _bufferLength( bufferSize )
        {}

        ui32 _bufferLength;
        CVec < CLogger::DirectionFunc, void > _o_dirs;
        bln _is_on;
        bln _is_multithreaded;
        mutable CMutex _mutex;
        char *name()
        {
            return (char *)this + sizeof(*this);
        }
    };
}

#define dis ((SLogger *)this)

//  TODO: add mutexes

CLogger::CLogger()
{}

NOINLINE va_return CLogger::Message( Tag::messageTag_t tag, const char *cp_fmt, ... )
{
    ASSUME( dis );

    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );

    ASSUME( cp_fmt && dis->_bufferLength );

    if( !dis->_is_on || !dis->_o_dirs.Size() )
    {
        return va_return_whatever;
    }

    char a_buf[ 4096 ];
    char *p_buffer = dis->_bufferLength > COUNTOF( a_buf ) ? (char *)::malloc( dis->_bufferLength ) : a_buf;
    uiw printedLen;

    va_list args;
    va_start( args, cp_fmt );

    DBGCODE( va_return argsProced = 2 + ) Funcs::PrintToStrArgList( p_buffer, dis->_bufferLength, &printedLen, cp_fmt, args );

    va_end( args );

    for( ui32 index = 0; index < dis->_o_dirs.Size(); ++index )
    {
        dis->_o_dirs[ index ]( tag, p_buffer, printedLen );
    }

    if( dis->_bufferLength > COUNTOF( a_buf ) )
    {
        ::free( p_buffer );
    }

    DBGCODE( return argsProced );
}

void CLogger::AddDirection( DirectionFunc dir )
{
    ASSUME( dis && dir );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_o_dirs.PushBack( dir );
}

void CLogger::PopDirection()
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_o_dirs.PopBackSafe();
}

uiw CLogger::DirectionsCount() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_o_dirs.Size();
}

CLogger::DirectionFunc CLogger::DirectionByIndexGet( uiw index ) const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( index < dis->_o_dirs.Size() );
    if( index < dis->_o_dirs.Size() )
    {
        return dis->_o_dirs[ index ];
    }
    return 0;
}

void CLogger::DirectionByIndexSet( uiw index, DirectionFunc dir )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( index < dis->_o_dirs.Size() && dir );
    if( index < dis->_o_dirs.Size() )
    {
        dis->_o_dirs[ index ] = dir;
    }
}

void CLogger::BufferLengthSet( ui32 length )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( length );
    dis->_bufferLength = length;
}

ui32 CLogger::BufferLengthGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_bufferLength;
}

void CLogger::IsOnSet( bln is_on )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = is_on;
}

bln CLogger::IsOnGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_on;
}

bln CLogger::IsOnToggle()
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = !dis->_is_on;
    return dis->_is_on;
}

bln CLogger::IsMultithreadedGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_multithreaded;
}

void CLogger::IsMitlithreadedSet( bln is_multithreaded )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_multithreaded = is_multithreaded;
}

const char *CLogger::NameGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( dis->name() );
    return dis->name();
}

NOINLINE CLogger *CLogger::Create( const char *cp_name, bln is_on, bln is_multithreaded, ui32 bufferSize /* = 2048 */ )
{
    ASSUME( cp_name && bufferSize );
    uiw len = _StrLen( cp_name ) + 1;
    SLogger *po_logger = new (::malloc( sizeof(SLogger) + len )) SLogger( is_on, is_multithreaded, bufferSize );
    _MemCpy( po_logger->name(), cp_name, len );
    return (CLogger *)po_logger;
}

void CLogger::Delete( CLogger *po_logger )
{
    ASSUME( po_logger );
    SLogger *slogger = ((SLogger *)po_logger);
    CScopeLock < true > lock( slogger->_is_multithreaded ? &slogger->_mutex : 0 );
    slogger->~SLogger();
    ::free( slogger );
}

#undef dis
