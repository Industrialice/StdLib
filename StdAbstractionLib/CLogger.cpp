#include "PreHeader.hpp"
#include "CLogger.hpp"
#include "CVector.hpp"
#include "CString.hpp"
#include "CMutex.hpp"
#include <Allocators.hpp>
#include <stdarg.h>

namespace
{
    struct SLogger : public CLogger
    {
        SLogger( bool is_on, bool is_multithreaded ) : _is_on( is_on ), _is_multithreaded( is_multithreaded )
        {}

        CVec < CLogger::DirectionFunc, void > _o_dirs;
        CStr _buffer;
        bool _is_on;
        bool _is_multithreaded;
        mutable CMutex _mutex;
        char *name()
        {
            return (char *)this + sizeof(*this);
        }
    };
}

#define dis ((SLogger *)this)

CLogger::CLogger()
{}

#if defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
NOINLINE void CLogger::_Message( Tag::messageTag_t tag, const char *cp_fmt, ... )
#else
NOINLINE void CLogger::Message( Tag::messageTag_t tag, const char *cp_fmt, ... )
#endif
{
    ASSUME( dis );

    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );

    ASSUME( cp_fmt && dis->_buffer.Size() == 0 );

    if( !dis->_is_on || !dis->_o_dirs.Size() )
    {
        return;
    }

    va_list args;
    va_start( args, cp_fmt );

    uiw printedLen = Funcs::PrintToContainerArgList( &dis->_buffer, cp_fmt, args );

    va_end( args );

    for( unsigned int index = 0; index < dis->_o_dirs.Size(); ++index )
    {
        dis->_o_dirs[ index ]( tag, dis->_buffer.CStr(), printedLen );
    }

    dis->_buffer.Clear();
    if( dis->_buffer.Reserved() > 512 )
    {
        dis->_buffer.Reserve( 512 );
    }
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

size_t CLogger::DirectionsCount() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_o_dirs.Size();
}

CLogger::DirectionFunc CLogger::DirectionByIndexGet( size_t index ) const
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

void CLogger::DirectionByIndexSet( size_t index, DirectionFunc dir )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( index < dis->_o_dirs.Size() && dir );
    if( index < dis->_o_dirs.Size() )
    {
        dis->_o_dirs[ index ] = dir;
    }
}

void CLogger::IsOnSet( bool is_on )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = is_on;
}

bool CLogger::IsOnGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_on;
}

bool CLogger::IsOnToggle()
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = !dis->_is_on;
    return dis->_is_on;
}

bool CLogger::IsMultithreadedGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_multithreaded;
}

void CLogger::IsMitlithreadedSet( bool is_multithreaded )
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

NOINLINE CLogger *CLogger::Create( const char *cp_name, bool is_on, bool is_multithreaded )
{
    ASSUME( cp_name );
    size_t len = _StrLen( cp_name ) + 1;
    SLogger *po_logger = new (::malloc( sizeof(SLogger) + len )) SLogger( is_on, is_multithreaded );
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
