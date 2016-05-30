#include "PreHeader.hpp"

#ifdef WINDOWS

#include "CThread.hpp"

#ifndef _WIN32_WCE
    #include <process.h>
#endif

struct ThreadArg
{
    void (*ExecutionFunc)( void *argument );
    void *argument;
};

static unsigned int __stdcall ThreadFunc( void *argument )
{
    ThreadArg threadArg = *(ThreadArg *)argument;
    free( argument );
    threadArg.ExecutionFunc( threadArg.argument );
    return 0;
}

CThread::~CThread()
{
    if( _thread )
    {
        BOOL result = ::CloseHandle( _thread );
        ASSUME( result );
    }
}

CThread::CThread()
{
    _thread = 0;
}

CThread::CThread( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority)
{
    _thread = 0;
    CThread::Create( stackSize, ExecutionFunc, argument, priority );
}

void CThread::Create( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority )
{
    ASSUME( _thread == 0 );

    ThreadArg *threadArg = (ThreadArg *)malloc( sizeof(ThreadArg) );
    threadArg->ExecutionFunc = ExecutionFunc;
    threadArg->argument = argument;

    #ifdef _WIN32_WCE
        _thread = ::CreateThread( 0, stackSize, (LPTHREAD_START_ROUTINE)ThreadFunc, threadArg, 0, 0 );
    #else
        _thread = (HANDLE)_beginthreadex( 0, stackSize, ThreadFunc, threadArg, 0, 0 );
    #endif
    ASSUME( _thread != (HANDLE)-1 );

    if( priority != PriorityNormal )
    {
        int normalizedPriority;
        if( priority == PriorityIdle ) normalizedPriority = THREAD_PRIORITY_IDLE;
        else if( priority == PriorityLowest ) normalizedPriority = THREAD_PRIORITY_LOWEST;
        else if( priority == PriorityBelowNormal ) normalizedPriority = THREAD_PRIORITY_BELOW_NORMAL;
        else if( priority == PriorityAboveNormal ) normalizedPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        else if( priority == PriorityHighest ) normalizedPriority = THREAD_PRIORITY_HIGHEST;
        else if( priority == PriorityTimeCritical ) normalizedPriority = THREAD_PRIORITY_TIME_CRITICAL;
        else
        {
            SOFTBREAK;
            normalizedPriority = THREAD_PRIORITY_NORMAL;
        }

        BOOL result = ::SetThreadPriority( _thread, normalizedPriority );
        ASSUME( result );
    }
}

void CThread::SleepCurrent( ui32 msecs )  //  static
{
    ::Sleep( msecs );
}

#endif