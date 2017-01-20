#include "PreHeader.hpp"
#include "CLogger.hpp"
#include "CVector.hpp"
#include "CString.hpp"
#include "CMutex.hpp"
#include <Allocators.hpp>
#include <stdarg.h>
#include <MemoryStreamContainer.hpp>

namespace
{
    struct SLogger : public CLogger
    {
        SLogger( bln is_on, bln is_multithreaded ) : _is_on( is_on ), _is_multithreaded( is_multithreaded )
        {}

        CVec < CLogger::DirectionFunc, void > _o_dirs;
        CStr _buffer;
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

#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
NOINLINE void CLogger::_Message( Tag tag, const char *cp_fmt, ... )
#else
NOINLINE void CLogger::Message( Tag tag, const char *cp_fmt, ... )
#endif
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );

    ASSUME( cp_fmt && dis->_buffer.Size() == 0 );

    if( !dis->_is_on || !dis->_o_dirs.Size() )
    {
        return;
    }

    va_list args;
    va_start( args, cp_fmt );

	MemoryStreamContainer < CStr > stream( &dis->_buffer );

    uiw printedLen = Funcs::PrintToMemoryStreamArgList( stream, cp_fmt, args );

    va_end( args );

    for( uiw index = 0; index < dis->_o_dirs.Size(); ++index )
    {
        dis->_o_dirs[ index ]( tag, dis->_buffer.CStr(), printedLen );
    }

    dis->_buffer.Clear();
    if( dis->_buffer.Reserved() > 512 )
    {
        dis->_buffer.Reserve( 512 );
    }
}

void CLogger::DirectionAdd( DirectionFunc dir )
{
    ASSUME( dir );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_o_dirs.Append( dir );
}

uiw CLogger::DirectionsCount() const
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_o_dirs.Size();
}

CLogger::DirectionFunc CLogger::DirectionGet( uiw index ) const
{
	CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
	return dis->_o_dirs[ index ];
}

void CLogger::DirectionRemove( uiw index )
{
	CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
	dis->_o_dirs.Erase( index, 1 );
}

void CLogger::DirectionToTheTop( uiw index )
{
	CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
	DirectionFunc func = dis->_o_dirs[ index ];
	dis->_o_dirs.Erase( index, 1 );
	dis->_o_dirs.Insert( 0, func );
}

void CLogger::IsOnSet( bln is_on )
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = is_on;
}

bln CLogger::IsOnGet() const
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_on;
}

bln CLogger::IsOnToggle()
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_on = !dis->_is_on;
    return dis->_is_on;
}

bln CLogger::IsMultithreadedGet() const
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_is_multithreaded;
}

void CLogger::IsMitlithreadedSet( bln is_multithreaded )
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    dis->_is_multithreaded = is_multithreaded;
}

const char *CLogger::NameGet() const
{
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( dis->name() );
    return dis->name();
}

NOINLINE CLogger *CLogger::Create( const char *cp_name, bln is_on, bln is_multithreaded )
{
    ASSUME( cp_name );
    uiw len = _StrLen( cp_name ) + 1;
    SLogger *po_logger = new (::malloc( sizeof(SLogger) + len )) SLogger( is_on, is_multithreaded );
    _MemCpy( po_logger->name(), cp_name, len );
    return (CLogger *)po_logger;
}

void CLogger::Delete( CLogger *po_logger )
{
    ASSUME( po_logger );
    SLogger *slogger = ((SLogger *)po_logger);
    slogger->~SLogger();
    ::free( slogger );
}

#undef dis
