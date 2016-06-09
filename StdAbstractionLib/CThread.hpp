#ifndef __CTHREAD_HPP__
#define __CTHREAD_HPP__

namespace StdLib {

class CThread
{
private:
    DBGCODE( bln _is_created; )  //  used in POSIX
    threadHandle _thread;
	i8 _currentPriority;  //  -128 is the lowest, 0 normal, 127 time-critical

    CThread( const CThread & );
    CThread & operator = ( const CThread & );

public:
    ~CThread();
    CThread();
    CThread( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, i8 priority = 0 );
    void Create( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, i8 priority = 0 );
	i8 PriorityGet() const;
	bln PrioritySet( i8 priority );
    static void SleepCurrent( ui32 msecs );

#ifdef MOVE_SUPPORTED
	CThread( CThread &&source );
	CThread &operator = ( CThread &&source );
#endif
};

}  //  namespace StdLib

#endif
