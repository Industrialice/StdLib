#ifndef __PREALLOCATED_CONTAINER_HPP__
#define __PREALLOCATED_CONTAINER_HPP__

#include "Allocators.hpp"

namespace StdLib {

template < typename X, const uiw prealloc, typename Y, const Y cmpValue, const uiw memberOffset, typename count_type = uiw, typename Allocator = Allocator::Simple > class CPreallocatedContainer
{
    X _array[ prealloc ];
    uiw _knownFreeIndex;
    uiw _usedCells;

public:
    CPreallocatedContainer()
    {
        ASSUME( prealloc );
        _knownFreeIndex = 0;
        _usedCells = 0;
        const Y setValue = cmpValue;
        for( uiw index = 0; index < prealloc; ++index )
        {
            _MemCpy( (byte *)(_array + index) + memberOffset, &setValue, sizeof(Y) );
        }
    }

    X *Add()
    {
        if( _knownFreeIndex != ui32_max )
        {
            X *p_addr = _array + _knownFreeIndex;
            _knownFreeIndex = ui32_max;
            ++_usedCells;
            return p_addr;
        }
        if( _usedCells >= prealloc )
        {
            return (X *)Allocator::Alloc < X >( sizeof(X) );
        }
        const Y compare = cmpValue;
        for( uiw index = 0; ; ++index )
        {
            ASSUME( index < prealloc );
            if( _MemEquals( (byte *)(_array + index) + memberOffset, &compare, sizeof(Y) ) )
            {
                ++_usedCells;
                return _array + index;
            }
        }
    }

    void Remove( X *p_addr )
    {
        if( p_addr >= _array && p_addr < _array + prealloc )
        {
            uiw addr = (uiw)p_addr;
            addr -= (uiw)_array;
            uiw index = addr / sizeof(X);
            _knownFreeIndex = index;
            --_usedCells;
            const Y setValue = cmpValue;
            _MemCpy( (byte *)(_array + index) + memberOffset, &setValue, sizeof(Y) );
        }
        else
        {
            Allocator::Free( p_addr );
        }
    }
};

}  //  namespace StdLib

#endif __PREALLOCATED_CONTAINER_HPP__
