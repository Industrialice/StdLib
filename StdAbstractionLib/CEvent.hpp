#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <StdCoreLib.hpp>

namespace StdLib
{
    class CEvent
    {
        eventHandle _handle;

        CEvent( const CEvent & );
        CEvent & operator = ( const CEvent & );

    public:

        ~CEvent();
        CEvent( bln isInitiallySignaling = false, bln isResetAfterWait = false );

        void WaitFor( ui32 timeout = ui32_max );
        void Reset();
        void Raise();
    };
}

#endif __EVENT_HPP__