#include "PreHeader.hpp"
#include "CLogger.hpp"
#include "CVector.hpp"
#include "CMutex.hpp"
#include <Allocators.hpp>
#include <stdarg.h>

namespace
{
    struct SLogger : public CLogger
    {
        SLogger( bool is_on, bool is_multithreaded, unsigned int bufferSize ) : _is_on( is_on ), _is_multithreaded( is_multithreaded ), _bufferLength( bufferSize )
        {}

        unsigned int _bufferLength;
        CVec < CLogger::DirectionFunc, void > _o_dirs;
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

//  TODO: add mutexes

CLogger::CLogger()
{}

template < typename X, uiw static_size, typename allocator = Allocator::Simple > class CSmartBuf
{
    typename AlignmentHelper < X, static_size >::type _arr;
    X *_dynamic_pointer;

public:
    ~CSmartBuf()
    {
        allocator::Free( _dynamic_pointer );
    }

    CSmartBuf( uiw dynamic_size )
    {
        if( dynamic_size > static_size )
        {
            _dynamic_pointer = allocator::Alloc < X >( dynamic_size );
        }
        else
        {
            _dynamic_pointer = 0;
        }
    }

    X *Acquire()
    {
        return _dynamic_pointer ? _dynamic_pointer : (X *)&_arr;
    }

    const X *Acquire() const
    {
        return _dynamic_pointer ? _dynamic_pointer : (X *)&_arr;
    }
};

NOINLINE va_return CLogger::Message( Tag::messageTag_t tag, const char *cp_fmt, ... )
{
    ASSUME( dis );

    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );

    ASSUME( cp_fmt && dis->_bufferLength );

    if( !dis->_is_on || !dis->_o_dirs.Size() )
    {
        return va_return_whatever;
    }

    CSmartBuf < char, 4096 > smartBuf( dis->_bufferLength );
    char *tempBuffer = smartBuf.Acquire();
    size_t printedLen;

    va_list args;
    va_start( args, cp_fmt );

    DBGCODE( va_return argsProced = 2 + ) Funcs::PrintToStrArgList( tempBuffer, dis->_bufferLength, &printedLen, cp_fmt, args );

    va_end( args );

    for( unsigned int index = 0; index < dis->_o_dirs.Size(); ++index )
    {
        dis->_o_dirs[ index ]( tag, tempBuffer, printedLen );
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

void CLogger::BufferLengthSet( unsigned int length )
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    ASSUME( length );
    dis->_bufferLength = length;
}

unsigned int CLogger::BufferLengthGet() const
{
    ASSUME( dis );
    CScopeLock < true > lock( dis->_is_multithreaded ? &dis->_mutex : 0 );
    return dis->_bufferLength;
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

NOINLINE CLogger *CLogger::Create( const char *cp_name, bool is_on, bool is_multithreaded, unsigned int bufferSize /* = 2048 */ )
{
    ASSUME( cp_name && bufferSize );
    size_t len = _StrLen( cp_name ) + 1;
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
