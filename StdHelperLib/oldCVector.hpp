#ifndef __C_VECTOR_HPP__
#define __C_VECTOR_HPP__

#include "CBaseVector.hpp"

#ifdef ISPOD_SUPPORTED
    #include <type_traits>
#endif

namespace StdLib {

namespace Private {

template < typename X, typename reservator, typename allocator, typename count_type, bln tis_selfManaged, uiw static_size, bln tis_checkForPod > class _CVectorBasis : public Private::CBaseVector < X, reservator, allocator, count_type, tis_selfManaged, static_size >
{
protected:
    typedef CBaseVector < X, reservator, allocator, count_type, tis_selfManaged, static_size > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

public:
    typedef _CVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, static_size, tis_checkForPod > ownType;

    ~_CVectorBasis()
    {
        CHECK( _is_constructed );
        if( tis_selfManaged )
        {
            this->_ArrFree();
        }
    }

    _CVectorBasis() : parentType()
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif
    }

    explicit _CVectorBasis( count_type reserve ) : parentType( reserve )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif
    }

    explicit _CVectorBasis( CNoInit ) : parentType( CNoInit() )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif
    }

    _CVectorBasis( const X &source, count_type reserve )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif

        DBGCODE( _is_constructed = true; );
        this->_ReserveSet( reserve + (reserve == 0) );
        _count = 1;
        this->_ArrAlloc( this->_ReserveGet() );
        _arr[ 0 ] = source;
    }

    _CVectorBasis &Assign( const X &val )
    {
        _ProcReservationUnknown( 1 );
        _arr[ 0 ] = val;
        _count = 1;
    }

    _CVectorBasis &AssignArr( const X *arr, count_type count )
    {
        _ProcReservationUnknown( count );
        for( _count = 0; _count < count; ++_count )
        {
            _arr[ _count ] = arr[ _count ];
        }
    }

    _CVectorBasis &PushBack( const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        _arr[ _count ] = val;
        ++_count;
        return *this;
    }

    _CVectorBasis &PushBackEmpty()
    {
        this->_ProcReservationUp( _count + 1 );
        _arr[ _count ] = X();
        ++_count;
        return *this;
    }

    _CVectorBasis &PushBackArr( const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        _MemCpy( _arr + _count, arr, sizeof(X) * count );
        _count += count;
        return *this;
    }

    _CVectorBasis &PushBackVec( const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        _MemCpy( _arr + _count, &o_vec[ 0 ], sizeof(X) * o_vec.Size() );
        _count += o_vec.Size();
        return *this;
    }

    _CVectorBasis &PushFront( const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        _MomMove( _arr + 1, _arr, sizeof(X) * _count );
        _arr[ 0 ] = val;
        ++_count;
        return *this;
    }

    _CVectorBasis &PushFrontEmpty()
    {
        this->_ProcReservationUp( _count + 1 );
        _MomMove( _arr + 1, _arr, sizeof(X) * _count );
        _arr[ 0 ] = X();
        ++_count;
        return *this;
    }

    _CVectorBasis &PushFrontArr( const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        _MemMove( _arr + count, _arr, sizeof(X) * _count );
        _MemCpy( _arr, arr, sizeof(X) * count );
        _count += count;
        return *this;
    }

    _CVectorBasis &PushFrontVec( const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        _MemMove( _arr + o_vec.Size(), _arr, sizeof(X) * _count );
        _MemCpy( _arr, &o_vec[ 0 ], sizeof(X) * o_vec.Size() );
        _count += o_vec.Size();
        return *this;
    }

    _CVectorBasis &PopBack()
    {
        ASSUME( _count );
        --_count;
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &PopBackSafe()
    {
        if( _count )
        {
            --_count;
            this->_ProcReservationDown( _count );
        }
        return *this;
    }

    _CVectorBasis &PopBackNum( count_type count )
    {
        ASSUME( _count >= count );
        _count -= count;
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &PopBackNumSafe( count_type count )
    {
        _count = count >= _count ? 0 : _count - count;
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &PopFront()
    {
        ASSUME( _count );
        --_count;
        _MemMove( _arr, _arr + 1, sizeof(X) * _count );
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &PopFrontSafe()
    {
        if( _count )
        {
            --_count;
            _MemMove( _arr, _arr + 1, sizeof(X) * _count );
            this->_ProcReservationDown( _count );
        }
        return *this;
    }

    _CVectorBasis &PopFrontNum( count_type count )
    {
        ASSUME( count <= _count );
        if( count == _count )
        {
            _count = 0;
        }
        else
        {
            _count -= count;
            _MemMove( _arr, _arr + count, sizeof(X) * count );
        }
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &PopFrontNumSafe( count_type count )
    {
        if( count >= _count )
        {
            _count = 0;
        }
        else
        {
            _count -= count;
            _MemMove( _arr, _arr + count, sizeof(X) * count );
        }
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CVectorBasis &Insert( count_type index, const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        _MemMove( _arr + index + 1, _arr + index, sizeof(X) * (_count - index) );
        _arr[ index ] = val;
        ++_count;
        return *this;
    }

    _CVectorBasis &InsertDefault( count_type index )
    {
        this->_ProcReservationUp( _count + 1 );
        _MemMove( _arr + index + 1, _arr + index, sizeof(X) * (_count - index) );
        _arr[ index ] = X();
        ++_count;
        return *this;
    }

    _CVectorBasis &InsertNull( count_type index )  //  leave new cells uninitialized
    {
        this->_ProcReservationUp( _count + 1 );
        _MemMove( _arr + index + 1, _arr + index, sizeof(X) * (_count - index) );
        ++_count;
        return *this;
    }

    _CVectorBasis &InsertDefaultArr( count_type index, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        _MemMove( _arr + index + count, _arr + index, sizeof(X) * (_count - index) );
        for( ; count; --count, ++index )
        {
            _arr[ index ] = X();
        }
        _count += count;;
        return *this;
    }

    _CVectorBasis &InsertNullArr( count_type index, count_type count )  //  leave new cells uninitialized
    {
        this->_ProcReservationUp( _count + count );
        _MemMove( _arr + index + count, _arr + index, sizeof(X) * (_count - index) );
        _count += count;
        return *this;
    }

    _CVectorBasis &InsertArr( count_type index, const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        _MemMove( _arr + index + count, _arr + index, sizeof(X) * (_count - index) );
        _MemCpy( _arr + index, arr, sizeof(X) * count );
        _count += count;
        return *this;
    }

    _CVectorBasis &InsertVec( count_type index, const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        _MemMove( _arr + index + o_vec.Size(), _arr + index, sizeof(X) * (_count - index) );
        _MemCpy( _arr + index, &o_vec[ 0 ], sizeof(X) * o_vec.Size() );
        _count += o_vec.Size();
        return *this;
    }

    _CVectorBasis &Clear()
    {
        this->_ProcReservationDown( 0 );
        _count = 0;
        return *this;
    }

    _CVectorBasis &FullClear()
    {
        this->_ReserveSet( 0 );
        _count = 0;
        this->_ArrFree();
        this->_ArrInit();
        return *this;
    }

    _CVectorBasis &Resize( count_type size )
    {
        if( size > _count )
        {
            this->_ProcReservationUp( size );
            for( ; _count < size; ++_count )
            {
                _arr[ _count ] = X();
            }
        }
        else
        {
            this->_ProcReservationDown( size );
            _count = size;
        }
        return *this;
    }

    _CVectorBasis &ResizeNull( count_type size )  //  leave new cells uninitialized
    {
        _ProcReservationUnknown( size );
        _count = size;
        return *this;
    }

    _CVectorBasis &Erase( count_type start, count_type count )
    {
        ASSUME( start >= 0 && start < _count && count <= _count && count <= _count - start );
        _MemMove( _arr + start, _arr + start + count, sizeof(X) * (_count - start - count) );
        _count -= count;
        this->_ProcReservationDown( _count );
        return *this;
    }
};

}  //  namespace Private

template < typename X, uiw static_size, typename count_type = uiw, bln tis_selfManaged = true, bln tis_checkForPod = true > class CVectorStatic : public Private::_CVectorBasis < X, void, void, count_type, tis_selfManaged, static_size, tis_checkForPod >
{
    typedef Private::_CVectorBasis < X, void, void, count_type, tis_selfManaged, static_size, tis_checkForPod > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

public:
    typedef CVectorStatic < X, static_size, count_type, tis_selfManaged, tis_checkForPod > ownType;

    CVectorStatic() : parentType() {  /*  void  */  }

    explicit CVectorStatic( count_type reserve ) : parentType( reserve ) {  /*  void  */  }

    explicit CVectorStatic( CNoInit ) : parentType( CNoInit() ) {  /*  void  */  }

    CVectorStatic( const X &source, count_type reserve ) : parentType( source, reserve ) {  /*  void  */  }

    CVectorStatic( const ownType &source )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif

        ASSUME( this != &source );
        if( tis_selfManaged )
        {
            _count = source._count;
            _MemCpy( _arr, source._arr, sizeof(X) * _count );
        }
        else
        {
            _MemCpy( this, &source, sizeof(source) );
        }
    }

    CVectorStatic &operator =( const CVectorStatic &source )
    {
        if( this != &source )
        {
            if( tis_selfManaged )
            {
                _count = source._count;
                _MemCpy( _arr, source._arr, sizeof(X) * _count );
            }
            else
            {
                _MemCpy( this, &source, sizeof(source) );
            }
        }
        return *this;
    }
};

template < typename X, typename reservator = void, typename allocator = Allocator::Simple < X >, typename count_type = uiw, bln tis_selfManaged = true, bln tis_checkForPod = true > class CVector : public Private::_CVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, 0, tis_checkForPod >
{
    typedef Private::_CVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, 0, tis_checkForPod > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

public:
    typedef CVector < X, reservator, allocator, count_type, tis_selfManaged, tis_checkForPod > ownType;

    CVector() : parentType() {  /*  void  */  }

    explicit CVector( count_type reserve ) : parentType( reserve ) {  /*  void  */  }

    explicit CVector( CNoInit ) : parentType( CNoInit() ) {  /*  void  */  }

    CVector( const X &source, count_type reserve ) : parentType( source, reserve ) {  /*  void  */  }

    CVector( const ownType &source )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == true, "type X is not a pod, disable pod checking for prevent this error" );
        #endif

        ASSUME( this != &source );
        DBGCODE( _is_constructed = true; );
        if( tis_selfManaged )
        {
            _count = source._count;
            this->_ReserveSet( source._ReserveGet() );
            this->_ArrAlloc( this->_ReserveGet() );
            _MemCpy( _arr, source._arr, sizeof(X) * _count );
        }
        else
        {
            _MemCpy( this, &source, sizeof(source) );
        }
    }

    CVector &operator =( const ownType &source )
    {
        if( this != &source )
        {
            if( tis_selfManaged )
            {
                _count = source._count;
                this->_ReserveSet( source._ReserveGet() );
                ASSUME( _count <= this->_ReserveGet() );
                if( this->_ReserveGet() )
                {
                    this->_ArrRealloc( this->_ReserveGet() );
                    _MemCpy( _arr, source._arr, sizeof(X) * _count );
                }
                else
                {
                    this->_ArrFree();
                    this->_ArrInit();
                }
            }
            else
            {
                _MemCpy( this, &source, sizeof(source) );
            }
        }
        return *this;
    }
};

}  //  namespace StdLib

#endif __C_VECTOR_HPP__
