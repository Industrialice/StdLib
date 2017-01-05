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

static unsigned int WINAPI ThreadFunc( void *argument )
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

CThread::CThread( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, i8 priority )
{
    _thread = 0;
    CThread::Create( stackSize, ExecutionFunc, argument, priority );
}

void CThread::Create( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, i8 priority )
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

	_currentPriority = 0;
	this->PrioritySet( priority );
}
	
i8 CThread::PriorityGet() const
{
	return _currentPriority;
}

bln CThread::PrioritySet( i8 priority )
{
    if( priority != _currentPriority )
    {
        int normalizedPriority;

		if( priority < -85 ) normalizedPriority = THREAD_PRIORITY_IDLE;
		else if( priority < -55 ) normalizedPriority = THREAD_PRIORITY_LOWEST;
		else if( priority < -25 ) normalizedPriority = THREAD_PRIORITY_BELOW_NORMAL;
		else if( priority < 25 ) normalizedPriority = THREAD_PRIORITY_NORMAL;
		else if( priority < 55 ) normalizedPriority = THREAD_PRIORITY_ABOVE_NORMAL;
		else if( priority < 85 ) normalizedPriority = THREAD_PRIORITY_HIGHEST;
		else normalizedPriority = THREAD_PRIORITY_TIME_CRITICAL;

		if( ::SetThreadPriority( _thread, normalizedPriority ) == TRUE )
		{
			_currentPriority = priority;
			return true;
		}
		SOFTBREAK;
        return false;
    }
	return true;
}

CThread::CThread( CThread &&source ) : _thread( source._thread ), _currentPriority( source._currentPriority )
{
	source._thread = 0;
}
	
CThread &CThread::operator = ( CThread &&source )
{
	ASSUME( this != &source );
	this->_thread = source._thread;
	this->_currentPriority = source._currentPriority;
	source._thread = 0;
	return *this;
}

void CThread::SleepCurrent( ui32 msecs )  //  static
{
    ::Sleep( msecs );
}

#endif