#ifndef __C_FRAMED_STORE_HPP__
#define __C_FRAMED_STORE_HPP__

#include "CVector.hpp"

namespace StdLib {

template < typename X, typename freeTestType, freeTestType freeTestValue, uiw t_freeTestMemberOffset, uiw t_frameSize = 32, uiw t_initialFrames = 1, typename allocator = Allocator::Simple > class CFramedStore : CharMovable
{
    static bln _IsFree( const X *mem )
    {
        return *(freeTestType *)((byte *)mem + t_freeTestMemberOffset) == freeTestValue;
    }

    static void _SetFree( X *mem )
    {
        *(freeTestType *)((byte *)mem + t_freeTestMemberOffset) = freeTestValue;
    }

    struct SFrame
    {
        X *p_mem;
        uiw used;

        ~SFrame()
        {
            allocator::Free( p_mem );
        }

        SFrame()
        {
            p_mem = (X *)allocator::Alloc < X >( sizeof(X) * t_frameSize );
            for( uiw index = 0; index < t_frameSize; ++index )
            {
                _SetFree( &p_mem[ index ] );
            }
            used = 0;
        }
    };

    CVec < SFrame, Reservator::Half <>, 0, Sem_MovableAsPOD > _o_frames;

public:
    CFramedStore() : _o_frames( t_initialFrames )
    {
        _o_frames.Resize( t_initialFrames );
    }

    X *Add()
    {
        return new(AddUninit()) X();
    }

    X *Add( const X &val )
    {
        return new(AddUninit()) X( val );
    }

    X *AddUninit()
    {
        for( uiw frame = 0; frame < _o_frames.Size(); ++frame )
        {
            if( _o_frames[ frame ].used < t_frameSize )
            {
                for( uiw index = 0; ; ++index )
                {
                    ASSUME( index < t_frameSize );
                    if( _IsFree( _o_frames[ frame ].p_mem + index ) )
                    {
                        ++_o_frames[ frame ].used;
                        return &_o_frames[ frame ].p_mem[ index ];
                    }
                }
            }
        }

        _o_frames.Resize( _o_frames.Size() + 1 );
        ++_o_frames.Back().used;

        return &_o_frames.Back().p_mem[ 0 ];
    }

    void Remove( X *p_val )
    {
        for( uiw frame = 0; ; ++frame )
        {
            ASSUME( frame < _o_frames.Size() );
            if( p_val >= _o_frames[ frame ].p_mem && p_val <= _o_frames[ frame ].p_mem + t_frameSize - 1 )
            {
                ASSUME( !_IsFree( p_val ) );
                p_val->~X();
                _SetFree( p_val );
                --_o_frames[ frame ].used;
                break;
            }
        }
    }

    void RemoveByFrameIndex( uiw frame, uiw index )
    {
        ASSUME( frame < _o_frames.Size() && index < t_frameSize );
        ASSUME( !_IsFree( &_o_frames[ frame ].p_mem[ index ] ) );
        _o_frames[ frame ].p_mem[ index ].~X();
        _SetFree( &_o_frames[ frame ].p_mem[ index ] );
        --_o_frames[ frame ].used;
    }

    uiw Size()
    {
        uiw size = 0;
        for( uiw frame = 0; frame < _o_frames.Size(); ++frame )
        {
            size += _o_frames[ frame ].used;
        }
        return size;
    }

    uiw Frames()
    {
        return _o_frames.Size();
    }

    uiw FrameSize()
    {
        return t_frameSize;
    }

    X &Enumerate( uiw *p_frame, uiw *p_index )
    {
        ASSUME( p_frame && p_index );

        for( ; ; )
        {
            uiw frame = *p_frame;
            uiw index = *p_index;
            ++*p_index;
            if( *p_index == t_frameSize )
            {
                *p_index = 0;
                ++*p_frame;
            }
            if( frame == _o_frames.Size() )
            {
                *p_frame = uiw_max;
                return _o_frames[ 0 ].p_mem[ 0 ];
            }
            if( !_IsFree( &_o_frames[ frame ].p_mem[ index ] ) )
            {
                return _o_frames[ frame ].p_mem[ index ];
            }
        }
    }

    void ClearToInitial()
    {
        Clear();
        _o_frames.Resize( t_initialFrames );
    }

    void Clear()
    {
        for( uiw frame = 0; frame < _o_frames.Size(); ++frame )
        {
            for( uiw index = 0; index < t_frameSize; ++index )
            {
                if( !_IsFree( &_o_frames[ frame ].p_mem[ index ] ) )
                {
                    _o_frames[ frame ].p_mem[ index ].~X();
                    _SetFree( &_o_frames[ frame ].p_mem[ index ] );
                }
            }
            _o_frames[ frame ].used = 0;
        }
    }

    void RemoveUnusedFrames()  //  frames count will not go below t_initialFrames
    {
        for( uiw frame = 0; frame < _o_frames.Size() && _o_frames.Size() >= t_initialFrames; ++frame )
        {
            if( _o_frames[ frame ].used == 0 )
            {
                _o_frames.Erase( frame, 1 );
                --frame;
            }
        }
    }

    bln GetFrameIndexByAddr( const void *cp_addr, uiw *p_frame, uiw *p_index )
    {
        ASSUME( p_frame && p_index );
        const byte *cp_byteAddr = (byte *)cp_addr;
        for( uiw frame = 0; frame < _o_frames.Size(); ++frame )
        {
            if( cp_byteAddr >= (byte *)_o_frames[ frame ].p_mem && (X *)cp_byteAddr < _o_frames[ frame ].p_mem + t_frameSize )
            {
                *p_frame = frame;
                cp_byteAddr -= (uiw)_o_frames[ frame ].p_mem;
                *p_index = (uiw)cp_byteAddr / sizeof(X);
                return true;
            }
        }
        return false;
    }

    X &Get( uiw frame, uiw index )
    {
        ASSUME( frame < _o_frames.Size() && index < t_frameSize );
        return _o_frames[ frame ].p_mem[ index ];
    }

    X &operator[] ( uiw index )
    {
        uiw frame = index / t_frameSize;
        index = index % t_frameSize;
        return _o_frames[ frame ].p_mem[ index ];
    }
};

}  //  namespace StdLib

#endif __C_FRAMED_STORE_HPP__
