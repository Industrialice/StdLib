#ifndef __CVECTOR_BASE_HPP__
#define __CVECTOR_BASE_HPP__

namespace StdLib
{
namespace Private
{
template < typename X, typename reservator, typename allocator, uiw static_size > class _CBasisVec;

template < typename X, uiw static_size > class _CBasisVec < X, void, void, static_size > : CharMovable  /*  static base  */
{
    _CBasisVec( const _CBasisVec &source );
    _CBasisVec &operator = ( const _CBasisVec &source );
#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source );
    void operator = ( _CBasisVec &&source );
#endif

public:
    typedef uiw count_type;

    ~_CBasisVec()
    {}

    _CBasisVec() : _count( 0 )
    {}

    _CBasisVec( count_type count, count_type reserve ) : _count( count )
    {
        ASSUME( count <= static_size && reserve != TypeDesc < count_type >::max );
    }

    void _Transfer( _CBasisVec *source )
    {
        _count = source->_count;
        source->_count = 0;
    }

    X *_GetArr()
    {
        return (X *)&_arr;
    }

    const X *_GetArr() const
    {
        return (const X *)&_arr;
    }

    void _IncSize( count_type newCount )
    {
        ASSUME( newCount <= static_size && newCount >= _count );
        _count = newCount;
    }

    void _DecSize( count_type newCount )
    {
        ASSUME( newCount <= static_size && newCount <= _count );
        _count = newCount;
    }

    void _UnkSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
        _count = newCount;
    }

    count_type _TryIncSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount <= static_size );
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryDecSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount <= static_size );
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryUnkSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount <= static_size );
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    void _FlushReserved()
    {}

    count_type _TryFlushReservedLocally()  //  will return TypeDesc < count_type >::max on success
    {
        return TypeDesc < count_type >::max;
    }

    count_type _Reserved() const
    {
        return static_size;
    }

    count_type _Size() const
    {
        return _count;
    }
    
    bln _IsStatic() const
    {
        return true;
    }

private:
    typename AlignmentHelper < X, static_size >::type _arr;
    count_type _count;
};

template < typename X, typename reservator, typename allocator > class _CBasisVec < X, reservator, allocator, 0 > : CharMovable  /*  dynamic base reservable  */
{
    _CBasisVec( const _CBasisVec &source );
    _CBasisVec &operator = ( const _CBasisVec &source );
#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source );
    _CBasisVec &operator = ( _CBasisVec &&source );
#endif

public:
    typedef typename reservator::count_type count_type;

    ~_CBasisVec()
    {
        allocator::Free( _arr );
    }

    _CBasisVec() : _arr( 0 ), _reserved( 0 ), _count( 0 )
    {}

    _CBasisVec( count_type count, count_type reserve ) : _reserved( reserve ), _count( count )
    {
        ASSUME( reserve != TypeDesc < count_type >::max );
        if( _reserved < _count )
        {
            _reserved = _count;
        }
        _arr = allocator::Alloc < X >( _reserved );
    }

    void _Transfer( _CBasisVec *source )
    {
        allocator::Free( _arr );
        _arr = source->_arr;
        source->_arr = 0;
        _reserved = source->_reserved;
        source->_reserved = 0;
        _count = source->_count;
        source->_count = 0;
    }

    X *_GetArr()
    {
        return _arr;
    }

    const X *_GetArr() const
    {
        return _arr;
    }

    void _IncSize( count_type newCount )
    {
        ASSUME( newCount >= _count );
        if( reservator::Up( newCount, &_reserved ) )
        {
            _arr = allocator::Realloc( _arr, _reserved );
        }
        _count = newCount;
    }

    void _DecSize( count_type newCount )
    {
        ASSUME( newCount <= _count );
        if( reservator::Down( newCount, &_reserved ) )
        {
            _arr = allocator::Realloc( _arr, _reserved );
        }
        _count = newCount;
    }

    void _UnkSize( count_type newCount )
    {
        if( newCount < _reserved )
        {
            _DecSize( newCount );
        }
        else
        {
            _IncSize( newCount );
        }
        _count = newCount;
    }

    count_type _TryIncSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount >= _count );
        count_type newReserve = _reserved;
        if( reservator::Up( newCount, &newReserve ) )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newReserve ) )
            {
                return newReserve;
            }
        }
        _reserved = newReserve;
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryDecSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount <= _count );
        count_type newReserve = _reserved;
        if( reservator::Down( newCount, &newReserve ) )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newReserve ) )
            {
                return newReserve;
            }
        }
        _reserved = newReserve;
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryUnkSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        if( newCount > _count )
        {
            return _TryIncSizeLocally( newCount );
        }
        return _TryDecSizeLocally( newCount );
    }

    void _FlushReserved()
    {
        if( _count < _reserved )
        {
            _reserved = _count;
            _arr = allocator::Realloc( _arr, _reserved );
        }
    }

    count_type _TryFlushReservedLocally()  //  will return TypeDesc < count_type >::max on success
    {
        if( _count < _reserved )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, _count ) )
            {
                return _count;
            }
            _reserved = _count;
        }
        return TypeDesc < count_type >::max;
    }

    count_type _Reserved() const
    {
        return _reserved;
    }

    count_type _Size() const
    {
        return _count;
    }

    bln _IsStatic() const
    {
        return false;
    }

private:
    X *_arr;
    count_type _reserved;
    count_type _count;
};

template < typename X, typename allocator > class _CBasisVec < X, void, allocator, 0 > : CharMovable  /*  dynamic base non-reservable  */
{
    _CBasisVec( const _CBasisVec &source );
    _CBasisVec &operator = ( const _CBasisVec &source );
#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source );
    _CBasisVec &operator = ( _CBasisVec &&source );
#endif

public:
    typedef uiw count_type;

    ~_CBasisVec()
    {
        allocator::Free < X >( _arr );
    }

    _CBasisVec() : _arr( 0 ), _count( 0 )
    {}

    _CBasisVec( count_type count, count_type reserve ) : _count( count )
    {
        ASSUME( reserve != TypeDesc < count_type >::max );
        _arr = allocator::Alloc < X >( count );
    }

    void _Transfer( _CBasisVec *source )
    {
        allocator::Free < X >( _arr );
        _arr = source->_arr;
        source->_arr = 0;
        _count = source->_count;
        source->_count = 0;
    }
    
    X *_GetArr()
    {
        return _arr;
    }

    const X *_GetArr() const
    {
        return _arr;
    }

    void _IncSize( count_type newCount )
    {
        ASSUME( newCount >= _count );
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
        _count = newCount;
    }

    void _DecSize( count_type newCount )
    {
        ASSUME( newCount <= _count );
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
        _count = newCount;
    }

    void _UnkSize( count_type newCount )
    {
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
        _count = newCount;
    }

    count_type _TryIncSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount >= _count );
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return newCount;
            }
        }
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryDecSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        ASSUME( newCount <= _count );
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return newCount;
            }
        }
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    count_type _TryUnkSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return newCount;
            }
        }
        _count = newCount;
        return TypeDesc < count_type >::max;
    }

    void _FlushReserved()
    {}

    count_type _TryFlushReservedLocally()  //  will return TypeDesc < count_type >::max on success
    {
        return TypeDesc < count_type >::max;
    }

    count_type _Reserved() const
    {
        return _count;
    }

    count_type _Size() const
    {
        return _count;
    }

    bln _IsStatic() const
    {
        return false;
    }

private:
    X *_arr;
    count_type _count;
};

template < typename X, typename allocator, uiw static_size > class _CBasisVec < X, void, allocator, static_size > : CharMovable  /*  dynamic base non-reservable with fixed preallocation  */
{
    _CBasisVec( const _CBasisVec &source );
    _CBasisVec &operator = ( const _CBasisVec &source );
#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source );
    _CBasisVec &operator = ( _CBasisVec &&source );
#endif

public:
    typedef uiw count_type;

    ~_CBasisVec()
    {
        if( _count > static_size )
        {
            allocator::Free < X >( _dynamicArr );
        }
    }

    _CBasisVec() : _count( 0 )
    {}

    _CBasisVec( count_type count, count_type reserve ) : _count( count )
    {
        ASSUME( reserve != TypeDesc < count_type >::max );
        if( _count > static_size )
        {
            _dynamicArr = allocator::Alloc < X >( count );
        }
    }

    void _Transfer( _CBasisVec *source )
    {
        if( _count > static_size )
        {
            allocator::Free( _dynamicArr );
        }
        if( source->_count > static_size )  //  static move can't be handled here, do nothing if static
        {
            _dynamicArr = source->_dynamicArr;  //  no need to set source's dynamic array to null, it will become static
        }
        else
        {
            DBGBREAK;
        }
        _count = source->_count;
        source->_count = 0;
    }
    
    X *_GetArr()
    {
        return _count > static_size ? _dynamicArr : (X *)&_preallocatedArr;
    }

    const X *_GetArr() const
    {
        return _count > static_size ? _dynamicArr : (X *)&_preallocatedArr;
    }

    void _IncSize( count_type newCount )
    {
        if( newCount != _count )
        {
            if( newCount > static_size )  //  reallocation required
            {
                if( _count <= static_size )  //  we're currently using a static array
                {
                    X *tempArr = allocator::Alloc < X >( newCount );
                    _MemCpy( tempArr, (X *)&_preallocatedArr, sizeof(X) * _count );
                    _dynamicArr = tempArr;
                }
                else
                {
                    _dynamicArr = allocator::Realloc( _dynamicArr, newCount );
                }
            }
        }
    }

    void _DecSize( count_type newCount )
    {
        if( newCount != _count )
        {
            if( newCount > static_size )  //  reallocation required
            {
                _dynamicArr = allocator::Realloc( _dynamicArr, newCount );
            }
            else
            {
                if( _count > static_size )  //  we're currently using a dynamic array, but we need to switch to static
                {
                    X *tempArr = _dynamicArr;
                    _MemCpy( (X *)&_preallocatedArr, tempArr, sizeof(X) * newCount );
                    allocator::Free( tempArr );
                }
            }
        }
    }

    void _UnkSize( count_type newCount )
    {
        if( newCount > _count )
        {
            _IncSize( newCount );
        }
        else
        {
            _DecSize( newCount );
        }
    }

    count_type _TryIncSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        if( newCount != _count )
        {
            ASSUME( newCount > _count );

            if( newCount > static_size )  //  dynamic arr
            {
                if( _count <= static_size )  //  we're currently using a static array
                {
                    return newCount;  //  can't switch between static and dynamic without reallocation
                }
                else
                {
                    if( !allocator::ReallocInplaceIfPossible( _dynamicArr, newCount ) )
                    {
                        return newCount;
                    }
                }
            }
        }
        return TypeDesc < count_type >::max;
    }

    count_type _TryDecSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        if( newCount != _count )
        {
            ASSUME( newCount < _count );

            if( newCount <= static_size )
            {
                if( _count > static_size )
                {
                    return newCount;  //  can't switch between static and dynamic without reallocation
                }
            }
            else
            {
                if( !allocator::ReallocInplaceIfPossible( _dynamicArr, newCount ) )
                {
                    return newCount;
                }
            }
        }
        return TypeDesc < count_type >::max;
    }

    count_type _TryUnkSizeLocally( count_type newCount )  //  will return TypeDesc < count_type >::max on success
    {
        if( newCount > _count )
        {
            return _TryIncSizeLocally( newCount );
        }
        return _TryDecSizeLocally( newCount );
    }

    void _FlushReserved()
    {}

    count_type _TryFlushReservedLocally()  //  will return TypeDesc < count_type >::max on success
    {
        return TypeDesc < count_type >::max;
    }

    count_type _Reserved() const
    {
        return _count;
    }

    count_type _Size() const
    {
        return _count;
    }

    bln _IsStatic() const
    {
        return _count <= static_size;
    }

private:
    union
    {
        typename AlignmentHelper < X, static_size >::type _preallocatedArr;
        X *_dynamicArr;
    };
    count_type _count;
};

template < typename X > class _CBasisVec < X, void, void, 0 > : CharMovable  /*  stores an external pointer  */
{
public:
    typedef uiw count_type;

    _CBasisVec()
    {
        _arr = 0;
        _count = 0;
    }

    _CBasisVec( X *arr, count_type count )
    {
        _arr = arr;
        _count = count;
    }
    
    X *_GetArr()
    {
        return _arr;
    }

    const X *_GetArr() const
    {
        return _arr;
    }

    void _SetArr( X *arr, count_type count )
    {
        _arr = arr;
        _count = count;
    }

    count_type _Size() const
    {
        return _count;
    }

private:
    X *_arr;
    count_type _count;
};
}  //  namespace Private
}  //  namespace StdLib

#endif  //  __CVECTOR_BASE_HPP__
