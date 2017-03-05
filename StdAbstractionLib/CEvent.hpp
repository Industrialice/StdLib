#ifndef __EVENT_HPP__
#define __EVENT_HPP__

namespace StdLib
{
    class CEvent
    {
        eventHandle _handle;

        CEvent( const CEvent & );
        CEvent & operator = ( const CEvent & );

    public:

        ~CEvent();
		explicit CEvent( bool isInitiallySignaling = false, bool isResetAfterWait = false );

        void WaitFor( ui32 timeout = ui32_max );
        void Reset();
        void Raise();
    };
}

#endif