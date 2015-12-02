#ifndef __CTHREAD_HPP__
#define __CTHREAD_HPP__

#include <StdCoreLib.hpp>

namespace StdLib {

class CThread
{
    DBGCODE( bool _is_created; )
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
    CThread( size_t stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority = PriorityNormal );
    void Create( size_t stackSize, void (*ExecutionFunc)( void *argument ), void *argument, Priority_t priority = PriorityNormal );
    static void SleepCurrent( unsigned int msecs );
};

}  //  namespace StdLib

#endif __CTHREAD_HPP__
