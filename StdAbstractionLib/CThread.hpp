#ifndef __CTHREAD_HPP__
#define __CTHREAD_HPP__

namespace StdLib {

class CThread
{
    DBGCODE( bln _is_created; )
    threadHandle _thread;

    CThread( const CThread & );
    CThread & operator = ( const CThread & );

public:
    enum Priority_t
    {
        PriorityIdle,
        PriorityLowest,
        PriorityBelowNormal,
        PriorityNormal,
        PriorityAboveNormal,
        PriorityHighest,
        PriorityTimeCritical
    };

    ~CThread();
    CThread();
    CThread( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority = PriorityNormal );
    void Create( uiw stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority = PriorityNormal );
    static void SleepCurrent( ui32 msecs );
};

}  //  namespace StdLib

#endif
