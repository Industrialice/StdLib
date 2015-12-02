#ifndef __CVECTOR_BASE_HPP__
#define __CVECTOR_BASE_HPP__

namespace StdLib
{
namespace Private
{
template < typename X, typename reservator, typename allocator, uiw static_size > class _CBasisVec;

template < typename X, uiw static_size > class _CBasisVec < X, void, void, static_size > : CharMovable  /*  static base  */
{
    typename AlignmentHelper < X, static_size >::type _arr;

public:
    ~_CBasisVec()
    {}

    _CBasisVec( const _CBasisVec &source )
    {
        DBGBREAK;  //  must not be called in any case
    }

    typedef uiw count_type;

    _CBasisVec()
    {}

    _CBasisVec( count_type count )
    {
        ASSUME( count <= static_size );
    }

    _CBasisVec &operator = ( const _CBasisVec &source )
    {
        DBGBREAK;  //  must not be called in any case
    }

#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source ) NOEXCEPT
    {
        //  do nothing here
    }

    void operator = ( _CBasisVec &&source ) NOEXCEPT
    {
        //  do nothing here
    }
#endif

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
        ASSUME( newCount <= static_size );
    }

    void _DecSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
    }

    void _UnkSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
    }

    bln _IsIncSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
        return false;
    }

    bln _IsDecSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
        return false;
    }

    bln _IsUnkSize( count_type newCount )
    {
        ASSUME( newCount <= static_size );
        return false;
    }

    void _FlushReserved()
    {}

    bln _IsFlushReserved()
    {
        return false;
    }

    count_type _Reserved() const
    {
        return static_size;
    }
    
    static const bln _cis_static = true;

    count_type _count;
};

template < typename X, typename reservator, typename allocator > class _CBasisVec < X, reservator, allocator, 0 > : CharMovable  /*  dynamic base reservable  */
{
public:
    typedef typename reservator::count_type count_type;

    ~_CBasisVec()
    {
        allocator::Free( _arr );
    }

    _CBasisVec( const _CBasisVec &source )
    {
        _arr = source._arr;
        _count = source._count;
        _reserved = source._reserved;
    }

    _CBasisVec() : _arr( 0 ), _reserved( 0 )
    {}

    _CBasisVec( count_type count ) : _reserved( count )
    {
        _arr = allocator::Alloc < X >( count );
    }

    _CBasisVec &operator = ( const _CBasisVec &source )
    {
        DBGBREAK;  //  must not be called in any case
    }

#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source ) NOEXCEPT
    {
        _arr = source._arr;
        source._arr = 0;
        _reserved = source._reserved;
        source._reserved = 0;
    }

    void operator = ( _CBasisVec &&source ) NOEXCEPT
    {
        ASSUME( this != &source );
        _arr = source._arr;
        source._arr = 0;
        _reserved = source._reserved;
        source._reserved = 0;
    }
#endif

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
        if( reservator::Up( newCount, &_reserved ) )
        {
            _arr = allocator::Realloc( _arr, _reserved );
        }
    }

    void _DecSize( count_type newCount )
    {
        if( reservator::Down( newCount, &_reserved ) )
        {
            _arr = allocator::Realloc( _arr, _reserved );
        }
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
    }

    bln _IsIncSize( count_type newCount )
    {
        if( reservator::Up( newCount, &_reserved ) )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, _reserved ) )
            {
                return true;
            }
        }
        return false;
    }

    bln _IsDecSize( count_type newCount )
    {
        if( reservator::Down( newCount, &_reserved ) )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, _reserved ) )
            {
                return true;
            }
        }
        return false;
    }

    bln _IsUnkSize( count_type newCount )
    {
        if( newCount > _reserved )
        {
            return _IsIncSize( newCount );
        }
        return _IsDecSize( newCount );
    }

    void _FlushReserved()
    {
        if( _count > _reserved )
        {
            _reserved = _count;
            _arr = allocator::Realloc( _arr, _reserved );
        }
    }

    bln _IsFlushReserved()
    {
        if( _count > _reserved )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, _reserved ) )
            {
                return true;
            }
            _reserved = _count;
        }
        return false;
    }

    count_type _Reserved() const
    {
        return _reserved;
    }

private:
    X *_arr;
    count_type _reserved;
    
protected:
    count_type _count;

    static const bln _cis_static = false;
};

template < typename X, typename allocator > class _CBasisVec < X, void, allocator, 0 > : CharMovable  /*  dynamic base non-reservable  */
{
public:
    typedef uiw count_type;

    ~_CBasisVec()
    {
        allocator::Free < X >( _arr );
    }

    _CBasisVec( const _CBasisVec &source )
    {
        _arr = source._arr;
        _count = source._count;
    }

    _CBasisVec() : _arr( 0 )
    {}

    _CBasisVec( count_type count )
    {
        _arr = allocator::Alloc < X >( count );
    }

    _CBasisVec &operator = ( const _CBasisVec &source )
    {
        DBGBREAK;  //  must not be called in any case
    }

#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source ) NOEXCEPT
    {
        _arr = source._arr;
        source._arr = 0;
    }

    void operator = ( _CBasisVec &&source ) NOEXCEPT
    {
        ASSUME( this != &source );
        _arr = source._arr;
        source._arr = 0;
    }
#endif
    
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
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
    }

    void _DecSize( count_type newCount )
    {
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
    }

    void _UnkSize( count_type newCount )
    {
        if( newCount != _count )
        {
            _arr = allocator::Realloc( _arr, newCount );
        }
    }

    bln _IsIncSize( count_type newCount )
    {
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return true;
            }
        }
        return false;
    }

    bln _IsDecSize( count_type newCount )
    {
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return true;
            }
        }
        return false;
    }

    bln _IsUnkSize( count_type newCount )
    {
        if( newCount != _count )
        {
            if( !allocator::ReallocInplaceIfPossible( _arr, newCount ) )
            {
                return true;
            }
        }
        return false;
    }

    void _FlushReserved()
    {}

    bln _IsFlushReserved()
    {
        return false;
    }

    count_type _Reserved() const
    {
        return _count;
    }

private:
    X *_arr;

protected:
    count_type _count;

    static const bln _cis_static = false;
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

private:
    X *_arr;

protected:
    count_type _count;
};
}  //  namespace Private
}  //  namespace StdLib

#endif  //  __CVECTOR_BASE_HPP__
