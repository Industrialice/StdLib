#ifndef __C_OBJECT_VECTOR_HPP__
#define __C_OBJECT_VECTOR_HPP__

#include "CBaseVector.hpp"

#ifdef ISPOD_SUPPORTED
    #include <type_traits>
#endif

namespace StdLib {

namespace Private {

template < typename X, typename reservator, typename allocator, typename count_type, bln tis_selfManaged, uiw static_size, bln tis_checkForPod > class _CObjectVectorBasis : public Private::CBaseVector < X, reservator, allocator, count_type, tis_selfManaged, static_size >
{
protected:
    typedef CBaseVector < X, reservator, allocator, count_type, tis_selfManaged, static_size > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

    void MoveElements( count_type targetIndex, count_type sourceIndex, count_type count )
    {
        for( ; count; --count )
        {
            new (&_arr[ targetIndex++ ]) X( _arr[ sourceIndex ] );
            _arr[ sourceIndex++ ].~X();
        }
    }

public:
    typedef _CObjectVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, static_size, tis_checkForPod > ownType;

    ~_CObjectVectorBasis()
    {
        CHECK( _is_constructed );
        if( tis_selfManaged )
        {
            for( count_type index = 0; index < _count; ++index )
            {
                _arr[ index ].~X();
            }
            this->_ArrFree();
        }
    }

    _CObjectVectorBasis() : parentType()
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif
    }

    explicit _CObjectVectorBasis( count_type reserve ) : parentType( reserve )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif
    }

    explicit _CObjectVectorBasis( CNoInit ) : parentType( CNoInit() )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif
    }

    _CObjectVectorBasis( const X &source, count_type reserve )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod == false || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif

        DBGCODE( _is_constructed = true; );
        this->_ReserveSet( reserve + (reserve == 0) );
        _count = 1;
        this->_ArrAlloc( this->_ReserveGet() );
        new (_arr) X( source );
    }

    _CObjectVectorBasis &Assign( const X &val )
    {
        if( _count > 1 )
        {
            for( count_type index = 1; index < _count; ++index )
            {
                _arr[ index ].~X();
            }
            this->_ProcReservationDown( 1 );
        }
        else
        {
            this->_ProcReservationUp( 1 );
            if( _count == 1 )
            {
                _arr[ index ] = val;
            }
            else
            {
                _count = 1;
                new (_arr) X( val );
            }
        }
    }

    _CObjectVectorBasis &AssignArr( const X *arr, count_type count )
    {
        if( count > _count )
        {
            this->_ProcReservationUp( count );
            for( count_type index = 0; index < _count; ++index )
            {
                _arr[ index ] = arr[ index ];
            }
            for( ; _count < count; ++_count )
            {
                new (_arr + _count) X( arr[ _count ] );
            }
        }
        else
        {
            for( count_type index = count; index < _count; ++index )
            {
                _arr[ index ].~X();
            }
            this->_ProcReservationDown( count );
            for( _count = 0; _count < count; ++_count )
            {
                _arr[ _count ] = arr[ _count ];
            }
        }
    }

    _CObjectVectorBasis &PushBack( const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        new (_arr + _count) X( val );
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &PushBackEmpty()
    {
        this->_ProcReservationUp( _count + 1 );
        new (_arr + _count) X();
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &PushBackArr( const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        for( uiw cpyIndex = 0; cpyIndex < count; ++cpyIndex )
        {
            new (_arr + _count) X( arr[ cpyIndex ] );
            ++_count;
        }
        return *this;
    }

    _CObjectVectorBasis &PushBackVec( const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        for( uiw cpyIndex = 0; cpyIndex < o_vec.Size(); ++cpyIndex )
        {
            new (_arr + _count) X( o_vec[ cpyIndex ] );
            ++_count;
        }
        return *this;
    }

    _CObjectVectorBasis &PushFront( const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        MoveElements( 1, 0, _count );
        new (_arr) X( val );
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &PushFrontEmpty()
    {
        this->_ProcReservationUp( _count + 1 );
        MoveElements( 1, 0, _count );
        new (_arr) X();
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &PushFrontArr( const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        MoveElements( count, 0, _count );
        for( uiw cpyIndex = 0; cpyIndex < count; ++cpyIndex )
        {
            new (_arr + cpyIndex) X( arr[ cpyIndex ] );
        }
        _count += count;
        return *this;
    }

    _CObjectVectorBasis &PushFrontVec( const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        MoveElements( o_vec.Size(), 0, _count );
        for( uiw cpyIndex = 0; cpyIndex < o_vec.Size(); ++cpyIndex )
        {
            new (_arr + cpyIndex) X( o_vec[ cpyIndex ] );
        }
        _count += o_vec.Size();
        return *this;
    }

    _CObjectVectorBasis &PopBack()
    {
        ASSUME( _count );
        --_count;
        _arr[ _count ].~X();
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &PopBackSafe()
    {
        if( _count )
        {
            --_count;
            _arr[ _count ].~X();
            this->_ProcReservationDown( _count );
        }
        return *this;
    }

    _CObjectVectorBasis &PopBackNum( count_type count )
    {
        ASSUME( _count >= count );
        for( ; count; --count )
        {
            --_count;
            _arr[ _count ].~X();
        }
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &PopBackNumSafe( count_type count )
    {
        for( ; count; --count )
        {
            if( !_count )
            {
                break;
            }
            --_count;
            _arr[ _count ].~X();
        }
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &PopFront()
    {
        ASSUME( _count );
        --_count;
        _arr[ 0 ].~X();
        MoveElements( 0, 1, _count );
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &PopFrontSafe()
    {
        if( _count )
        {
            --_count;
            _arr[ 0 ].~X();
            MoveElements( 0, 1, _count );
            this->_ProcReservationDown( _count );
        }
        return *this;
    }

    _CObjectVectorBasis &PopFrontNum( count_type count )
    {
        ASSUME( _count >= count );
        for( count_type index = 0; index < count; ++index )
        {
            _arr[ index ].~X();
        }
        _count -= count;
        MoveElements( 0, count, _count );
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &PopFrontNumSafe( count_type count )
    {
        if( count >= _count )
        {
            for( count_type index = 0; index < _count; ++index )
            {
                _arr[ index ].~X();
            }
            _count = 0;
        }
        else
        {
            for( count_type index = 0; index < count; ++index )
            {
                _arr[ index ].~X();
            }
            _count -= count;
            MoveElements( 0, count, _count );
        }
        this->_ProcReservationDown( _count );
        return *this;
    }

    _CObjectVectorBasis &Insert( count_type index, const X &val )
    {
        this->_ProcReservationUp( _count + 1 );
        MoveElements( index + 1, index, _count - index );
        new (_arr + index) X( val );
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &InsertDefault( count_type index )
    {
        this->_ProcReservationUp( _count + 1 );
        MoveElements( index + 1, index, _count - index );
        new (_arr + index) X();
        ++_count;
        return *this;
    }

    _CObjectVectorBasis &InsertDefaultArr( count_type index, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        MoveElements( index + count, index, _count - index );
        for( uiw cpyIndex = 0; cpyIndex < count; ++cpyIndex )
        {
            new (_arr + cpyIndex) X();
        }
        _count += count;
        return *this;
    }

    _CObjectVectorBasis &InsertArr( count_type index, const X *arr, count_type count )
    {
        this->_ProcReservationUp( _count + count );
        MoveElements( index + count, index, _count - index );
        for( uiw cpyIndex = 0; cpyIndex < count; ++cpyIndex )
        {
            new (_arr + index + cpyIndex) X( arr[ cpyIndex ] );
        }
        _count += count;
        return *this;
    }

    _CObjectVectorBasis &InsertVec( count_type index, const ownType &o_vec )
    {
        this->_ProcReservationUp( _count + o_vec.Size() );
        MoveElements( index + o_vec.Size(), index, _count - index );
        for( uiw cpyIndex = 0; cpyIndex < o_vec.Size(); ++cpyIndex )
        {
            new (_arr + index + cpyIndex) X( o_vec[ cpyIndex ] );
        }
        _count += o_vec.Size();
        return *this;
    }

    _CObjectVectorBasis &Clear()
    {
        for( count_type index = 0; index < _count; ++index )
        {
            _arr[ index ].~X();
        }
        _count = 0;
        this->_ProcReservationDown( 0 );
        return *this;
    }

    _CObjectVectorBasis &FullClear()
    {
        this->_ReserveSet( 0 );
        for( count_type index = 0; index < _count; ++index )
        {
            _arr[ index ].~X();
        }
        _count = 0;
        this->_ArrFree();
        this->_ArrInit();
        return *this;
    }

    _CObjectVectorBasis &Resize( count_type size )
    {
        if( size > _count )
        {
            this->_ProcReservationUp( size );
            for( ; _count < size; ++_count )
            {
                new (_arr + _count) X();
            }
        }
        else
        {
            for( count_type index = 1; index <= _count - size; ++index )
            {
                _arr[ _count - index ].~X();
            }
            _count = size;
            this->_ProcReservationDown( size );
        }
        return *this;
    }

    _CObjectVectorBasis &Erase( count_type start, count_type count )
    {
        ASSUME( start >= 0 && start < _count && count <= _count && count <= _count - start );
        for( count_type startIndex = start, erased = 0; erased < count; ++startIndex, ++erased )
        {
            _arr[ startIndex ].~X();
        }
        MoveElements( start, start + count, _count - start - count );
        _count -= count;
        this->_ProcReservationDown( _count );
        return *this;
    }
};

}  //  namespace Private

template < typename X, uiw static_size, typename count_type = uiw, bln tis_selfManaged = true, bln tis_checkForPod = false > class CObjectVectorStatic : public Private::_CObjectVectorBasis < X, void, void, count_type, tis_selfManaged, static_size, tis_checkForPod >
{
    typedef Private::_CObjectVectorBasis < X, void, void, count_type, tis_selfManaged, static_size, tis_checkForPod > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

public:
    typedef CObjectVectorStatic < X, static_size, count_type, tis_selfManaged, tis_checkForPod > ownType;

    CObjectVectorStatic() : parentType() {  /*  void  */  }

    explicit CObjectVectorStatic( count_type reserve ) : parentType( reserve ) {  /*  void  */  }

    explicit CObjectVectorStatic( CNoInit ) : parentType( CNoInit() ) {  /*  void  */  }

    CObjectVectorStatic( const X &source, count_type reserve ) : parentType( source, reserve ) {  /*  void  */  }

    CObjectVectorStatic( const ownType &source )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif

        ASSUME( this != &source );
        DBGCODE( _is_constructed = true; );
        if( tis_selfManaged )
        {
            for( _count = 0; _count < source._count; ++_count )
            {
                new (_arr + _count) X( source._arr[ _count ] );
            }
        }
        else
        {
            _MemCpy( this, &source, sizeof(source) );
        }
    }

    CObjectVectorStatic &operator =( const ownType &source )
    {
        if( this != &source )
        {
            if( tis_selfManaged )
            {
                count_type newSize = 0;
                for( ; newSize < _count && newSize < source._count; ++newSize )
                {
                    _arr[ newSize ] = source._arr[ newSize ];
                }
                for( count_type index = newSize; index < _count; ++index )
                {
                    _arr[ index ].~X();
                }
                _count = source._count;
                for( _count = newSize; _count < source._count; ++_count )
                {
                    new (_arr + _count) X( source._arr[ _count ] );
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

template < typename X, typename reservator = void, typename allocator = Allocator::Simple < X >, typename count_type = uiw, bln tis_selfManaged = true, bln tis_checkForPod = false > class CObjectVector : public Private::_CObjectVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, 0, tis_checkForPod >
{
    typedef Private::_CObjectVectorBasis < X, reservator, allocator, count_type, tis_selfManaged, 0, tis_checkForPod > parentType;

    using parentType::_arr;
    using parentType::_count;
    DBGCODE( using parentType::_is_constructed; );

public:
    typedef CObjectVector < X, reservator, allocator, count_type, tis_selfManaged, tis_checkForPod > ownType;

    CObjectVector() : parentType() {  /*  void  */  }

    explicit CObjectVector( count_type reserve ) : parentType( reserve ) {  /*  void  */  }

    explicit CObjectVector( CNoInit ) : parentType( CNoInit() ) {  /*  void  */  }

    CObjectVector( const X &source, count_type reserve ) : parentType( source, reserve ) {  /*  void  */  }

    CObjectVector( const ownType &source )
    {
        #ifdef ISPOD_SUPPORTED
            STATIC_CHECK( tis_checkForPod || std::is_pod < X >::value == false, "you should not use object vectors for pod types" );
        #endif

        ASSUME( this != &source );
        DBGCODE( _is_constructed = true; );
        if( tis_selfManaged )
        {
            this->_ReserveSet( source._ReserveGet() );
            _count = source._count;
            this->_ArrAlloc( this->_ReserveGet() );
            for( _count = 0; _count < source._count; ++_count )
            {
                new (_arr + _count) X( source._arr[ _count ] );
            }
        }
        else
        {
            _MemCpy( this, &source, sizeof(source) );
        }
    }

    CObjectVector &operator =( const ownType &source )
    {
        if( this != &source )
        {
            if( tis_selfManaged )
            {
                count_type newSize = 0;
                for( ; newSize < _count && newSize < source._count; ++newSize )
                {
                    _arr[ newSize ] = source._arr[ newSize ];
                }
                for( count_type index = newSize; index < _count; ++index )
                {
                    _arr[ index ].~X();
                }
                _count = source._count;
                this->_ReserveSet( source._ReserveGet() );
                ASSUME( _count <= this->_ReserveGet() );
                if( this->_ReserveGet() )
                {
                    this->_ArrRealloc( this->_ReserveGet() );
                    for( _count = newSize; _count < source._count; ++_count )
                    {
                        new (_arr + _count) X( source._arr[ _count ] );
                    }
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

#endif __C_OBJECT_VECTOR_HPP__
