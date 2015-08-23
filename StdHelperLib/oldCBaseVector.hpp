#ifndef __C_BASE_VECTOR_HPP__
#define __C_BASE_VECTOR_HPP__

#include "Allocators.hpp"
#include "Reservators.hpp"

namespace StdLib {

namespace Private {

template < typename X, typename reservator, typename allocator, typename count_type, uiw static_size > class _CBaseVertorBasis;

template < typename X, typename count_type, uiw static_size > class _CBaseVertorBasis < X, void, void, count_type, static_size >
{
protected:
    X _arr[ static_size ];
    count_type _count;

    void _ProcReservationUp( count_type newCount )
    {
        ASSUME( newCount <= static_size );
    }

    void _ProcReservationDown( count_type newCount )
    {
        ASSUME( newCount <= static_size );
    }

    void _ProcReservationUnknown( count_type newCount )
    {
        ASSUME( newCount <= static_size );
    }

    void _ReserveSet( count_type reserve )
    {
    }

    count_type _ReserveGet() const
    {
        return static_size;
    }

    bln _IsReservable() const
    {
        return true;
    }

    void _ArrInit()
    {
    }

    void _ArrAlloc( count_type count )
    {
        ASSUME( count <= static_size );
    }

    void _ArrRealloc( count_type count )
    {
        ASSUME( count <= static_size );
    }

    void _ArrFree()
    {
    }
};

template < typename X, typename reservator, typename allocator, typename count_type > class _CBaseVertorBasis < X, reservator, allocator, count_type, 0 >
{
protected:
    X *_arr;
    count_type _count;
    count_type _reserved;

    void _ProcReservationUp( count_type newCount )
    {
        ASSUME( _reserved >= _count && newCount >= _count );
        if( reservator::Up( newCount, &_reserved ) )
        {
            ASSUME( _reserved >= _count );
            _arr = (X *)allocator::Realloc( _arr, sizeof(X) * _reserved );
        }
    }

    void _ProcReservationDown( count_type newCount )
    {
        ASSUME( _reserved >= _count && newCount <= _count );
        if( reservator::Down( newCount, &_reserved ) )
        {
            ASSUME( _reserved >= _count );
            _arr = (X *)allocator::Realloc( _arr, sizeof(X) * _reserved );
        }
    }

    void _ProcReservationUnknown( count_type newCount )
    {
        ASSUME( _reserved >= _count );
        if( newCount > _count )
        {
            _ProcReservationUp( newCount );
        }
        else
        {
            _ProcReservationDown( newCount );
        }
    }

    void _ReserveSet( count_type reserve )
    {
        _reserved = reserve;
    }

    count_type _ReserveGet() const
    {
        return _reserved;
    }

    bln _IsReservable() const
    {
        return true;
    }

    void _ArrInit()
    {
        _arr = 0;
    }

    void _ArrAlloc( count_type count )
    {
        _arr = (X *)allocator::Alloc( sizeof(X) * count );
    }

    void _ArrRealloc( count_type count )
    {
        _arr = (X *)allocator::Realloc( _arr, sizeof(X) * count );
    }

    void _ArrFree()
    {
        allocator::Free( _arr );
    }
};

template < typename X, typename allocator, typename count_type > class _CBaseVertorBasis < X, void, allocator, count_type, 0 >
{
protected:
    X *_arr;
    count_type _count;

    void _ProcReservationUp( count_type newCount )
    {
        ASSUME( newCount >= _count );
        _arr = (X *)allocator::Realloc( _arr, sizeof(X) * newCount );
    }

    void _ProcReservationDown( count_type newCount )
    {
        ASSUME( newCount <= _count );
        _arr = (X *)allocator::Realloc( _arr, sizeof(X) * newCount );
    }

    void _ProcReservationUnknown( count_type newCount )
    {
        _arr = (X *)allocator::Realloc( _arr, sizeof(X) * newCount );
    }

    void _ReserveSet( count_type reserve )
    {
    }

    count_type _ReserveGet() const
    {
        return _count;
    }

    bln _IsReservable() const
    {
        return false;
    }

    void _ArrInit()
    {
        _arr = 0;
    }

    void _ArrAlloc( count_type count )
    {
        _arr = (X *)allocator::Alloc( sizeof(X) * count );
    }

    void _ArrRealloc( count_type count )
    {
        _arr = (X *)allocator::Realloc( _arr, sizeof(X) * count );
    }

    void _ArrFree()
    {
        allocator::Free( _arr );
    }
};

template < typename X, typename reservator, typename allocator, typename count_type, bln tis_selfManaged, count_type static_size > class CBaseVector : public _CBaseVertorBasis < X, reservator, allocator, count_type, static_size >
{
protected:
	using _CBaseVertorBasis < X, reservator, allocator, count_type, static_size >::_arr;
	using _CBaseVertorBasis < X, reservator, allocator, count_type, static_size >::_count;

    DBGCODE( bln _is_constructed; );

    typedef CBaseVector < X, reservator, allocator, count_type, tis_selfManaged, static_size > ownType;

    ~CBaseVector()
    {
        if( tis_selfManaged )
        {
            CHECK( _is_constructed );
            DBGCODE( _is_constructed = false );
        }
    }

    CBaseVector()
    {
        Constructor();
    }

    explicit CBaseVector( count_type reserve )
    {
        Constructor( reserve );
    }

    explicit CBaseVector( CNoInit )
    {
        DBGCODE( _is_constructed = false; );
    }

public:
    ownType &Constructor()
    {
        _count = 0;
        this->_ArrInit();
        this->_ReserveSet( 0 );
        DBGCODE( _is_constructed = true; );
        return *this;
    }

    ownType &Constructor( count_type reserve )
    {
        _count = 0;
        this->_ReserveSet( reserve );
        if( this->_ReserveGet() )
        {
            this->_ArrAlloc( this->_ReserveGet() );
        }
        else
        {
            this->_ArrInit();
        }
        DBGCODE( _is_constructed = true; );
        return *this;
    }

    CBaseVector &ReserveHint( count_type reserve )
    {
        if( this->_IsReservable() )
        {
            if( this->_ReserveGet() < reserve )
            {
                this->_ReserveSet( reserve );
                this->_ArrRealloc( _ReserveGet() );
            }
        }
        return *this;
    }

    CBaseVector &ReserveSet( count_type reserve )
    {
        ASSUME( reserve >= _count );
        if( this->_IsReservable() )
        {
            if( this->_ReserveGet() != reserve )
            {
                this->_ReserveSet( reserve );
                this->_ArrRealloc( this->_ReserveGet() );
            }
        }
        return *this;
    }

    CBaseVector &ReserveExtra( count_type extra )
    {
        if( this->_IsReservable() )
        {
            this->_ReserveSet( this->_ReserveGet() + extra );
            this->_ArrRealloc( this->_ReserveGet() );
        }
        return *this;
    }

    count_type ReserveGet() const
    {
        return _ReserveGet();
    }

    uiw Size() const
    {
        return _count;
    }

    X &Front()
    {
        ASSUME( _count );
        return _arr[ 0 ];
    }

    const X &Front() const
    {
        ASSUME( _count );
        return _arr[ 0 ];
    }

    bln FrontSafe( X &val ) const
    {
        if( _count )
        {
            val = _arr[ 0 ];
            return true;
        }
        val = X();
        return false;
    }

    X &Back()
    {
        ASSUME( _count );
        return _arr[ _count - 1 ];
    }

    const X &Back() const
    {
        ASSUME( _count );
        return _arr[ _count - 1 ];
    }

    bln BackSafe( X &val ) const
    {
        if( _count )
        {
            val = _arr[ _count - 1 ];
            return true;
        }
        val = X();
        return false;
    }

    bln Find( const X &source, count_type *p_index, count_type start = 0 )
    {
        for( count_type index = start; index < _count; ++index )
        {
            if( _arr[ index ] == source )
            {
                DSA( p_index, index );
                return true;
            }
        }
        return false;
    }

    X &GetElem( count_type index )
    {
        ASSUME( index < _count );
        return _arr[ index ];
    }

    const X &GetElem( count_type index ) const
    {
        ASSUME( index < _count );
        return _arr[ index ];
    }

    const bln GetElemSafe( count_type index, X &val ) const
    {
        if( index < _count )
        {
            val = _arr[ index ];
            return true;
        }
        val = X();
        return false;
    }

    X &SetElem( count_type index, const X &source )
    {
        ASSUME( index < _count );
        _arr[ index ] = source;
        return _arr[ index ];
    }

    bln SetElemSafe( count_type index, const X &source )
    {
        if( index < _count );
        {
            _arr[ index ] = source;
            return true;
        }
        return false;
    }

    X &operator []( count_type index )
    {
        ASSUME( index < _count );
        return _arr[ index ];
    }

    const X &operator []( count_type index ) const
    {
        ASSUME( index < _count );
        return _arr[ index ];
    }

    CBaseVector &Assign( const X &val );

    CBaseVector &AssignArr( const X *arr, count_type count );

    CBaseVector &PushBack( const X &val );

    CBaseVector &PushBackEmpty();  //  equals to Resize( currentSize + 1 )

    CBaseVector &PushBackArr( const X *arr, count_type count );

    CBaseVector &PushBackVec( const ownType &o_vec );

    CBaseVector &PushFront( const X &val );

    CBaseVector &PushFrontEmpty();

    CBaseVector &PushFrontArr( const X *arr, count_type count );

    CBaseVector &PushFrontVec( const ownType &o_vec );

    CBaseVector &operator =( const ownType &source );

    CBaseVector &PopBack();

    CBaseVector &PopBackSafe();

    CBaseVector &PopBackNum( count_type count );

    CBaseVector &PopBackNumSafe( count_type count );

    CBaseVector &PopFront();

    CBaseVector &PopFrontSafe();

    CBaseVector &PopFrontNum( count_type count );

    CBaseVector &PopFrontNumSafe( count_type count );

    CBaseVector &Insert( count_type index, const X &val );

    CBaseVector &InsertDefault( count_type index );

    CBaseVector &InsertDefaultArr( count_type index, count_type count );

    CBaseVector &InsertArr( count_type index, const X *arr, count_type count );

    CBaseVector &InsertVec( count_type index, const ownType &o_vec );

    CBaseVector &Clear();

    CBaseVector &FullClear();

    CBaseVector &Resize( count_type size );

    CBaseVector &Erase( count_type start, count_type count );
};

}  /*  namespace Private  */  }  /*  namespace StdLib  */

#endif __C_BASE_VECTOR_HPP__
