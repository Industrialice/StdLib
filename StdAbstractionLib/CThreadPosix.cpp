#include "PreHeader.hpp"

#ifdef POSIX

#include "CThread.hpp"

#include <time.h>

struct ThreadArg
{
    void (*ExecutionFunc)( void *argument );
    void *argument;
};

static void *ThreadFunc( void *argument )
{
    ThreadArg threadArg = *(ThreadArg *)argument;
    free( argument );
    threadArg.ExecutionFunc( threadArg.argument );
    return 0;
}

CThread::~CThread()
{
}

CThread::CThread()
{
    DBGCODE( _is_created = false; )
}

CThread::CThread( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority )
{
    DBGCODE( _is_created = false; )
    CThread::Create( stackSize, ExecutionFunc, argument, priority );
}

void CThread::Create( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority )
{
    DBGCODE( ASSUME( _is_created == false ); )

    pthread_attr_t *attrPtr = 0;
    pthread_attr_t attributes;
    int result;

    if( stackSize )
    {
        attrPtr = &attributes;
        result = pthread_attr_init( attrPtr );
        ASSUME( result == 0 );
        result = pthread_attr_setstacksize( attrPtr, stackSize );
        ASSUME( result == 0 );
    }

    ThreadArg *threadArg = (ThreadArg *)malloc( sizeof(ThreadArg) );
    threadArg->ExecutionFunc = ExecutionFunc;
    threadArg->argument = argument;

    result = pthread_create( &_thread, attrPtr, ThreadFunc, threadArg );
    ASSUME( result == 0 );

    if( attrPtr )
    {
        result = pthread_attr_destroy( attrPtr );
        ASSUME( result == 0 );
    }
}

void CThread::SleepCurrent( ui32 msecs )  //  static
{
    timespec o_req;
    o_req.tv_sec = msecs / 1000;
    o_req.tv_nsec = (msecs % 1000) * (1000 * 1000);
    int result = ::nanosleep( &o_req, 0 );
    ASSUME( result == 0 );
}

#endif