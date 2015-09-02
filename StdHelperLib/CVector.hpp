#ifndef __CVECTOR_HPP__
#define __CVECTOR_HPP__

#include <StdCoreLib.hpp>
#include "Reservators.hpp"
#include "Allocators.hpp"
#include "Iterator.hpp"

#ifdef DEFAULT_FUNC_PARAMS_SUPPORTED
    #define VEC_DEF_PARAM( ... ) __VA_ARGS__
#else
    #define VEC_DEF_PARAM( ... )
#endif

namespace StdLib
{
  enum TypeSemantic_t
  {
      Sem_POD,
      Sem_Mov,
      Sem_Strict
  };

namespace Private
{

template < typename X, typename reservator, typename allocator, uiw static_size > class _CBasisVec;

template < typename X, uiw static_size > class _CBasisVec < X, void, void, static_size > : CharMovable
{
    typename AlignmentHelper < X, static_size >::type _arr;

protected:
    typedef uiw count_type;
    count_type _count;

    static const bln _cis_static = true;

    ~_CBasisVec()
    {}

    _CBasisVec()
    {}

    _CBasisVec(count_type count )
    {
        ASSUME( count <= static_size );
    }

    X *_GetArr()
    {
        return (X *)&_arr;
    }

    const X *_GetArr() const
    {
        return (const X *)&_arr;
    }

    void _SetArr( X *arr, count_type newCount )
    {
        DBGBREAK;
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

    void _Free( X *arr )
    {}

    count_type _Reserved() const
    {
        return static_size;
    }
};

template < typename X, typename reservator, typename allocator > class _CBasisVec < X, reservator, allocator, 0 > : CharMovable
{
    X *_arr;

public:
    typedef typename reservator::count_type count_type;

protected:
    count_type _count, _reserved;

    static const bln _cis_static = false;

    ~_CBasisVec()
    {
        allocator::Free( _arr );
    }

    _CBasisVec() : _arr( 0 ), _reserved( 0 )
    {}

    _CBasisVec( count_type count ) : _reserved( count )
    {
        _arr = allocator::Alloc < X >( count );
    }

#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source ) NOEXEPT
    {
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

    void _SetArr( X *arr, count_type newCount )
    {
        ASSUME( arr == 0 && newCount <= _reserved );
        _arr = allocator::Alloc < X >( _reserved );
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
            _reserved = _count;
            if( !allocator::ReallocInplaceIfPossible( _arr, _reserved ) )
            {
                return true;
            }
        }
        return false;
    }

    void _Free( X *arr )
    {
        allocator::Free( arr );
    }

    count_type _Reserved() const
    {
        return _reserved;
    }
};

template < typename X, typename allocator > class _CBasisVec < X, void, allocator, 0 > : CharMovable
{
    X *_arr;

public:
    typedef uiw count_type;

protected:
    count_type _count;

    static const bln _cis_static = false;

    ~_CBasisVec()
    {
        allocator::Free < X >( _arr );
    }

    _CBasisVec() : _arr( 0 )
    {}

    _CBasisVec( count_type count )
    {
        _arr = allocator::Alloc < X >( count );
    }

#ifdef MOVE_SUPPORTED
    _CBasisVec( _CBasisVec &&source ) NOEXEPT
    {
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

    void _SetArr( X *arr, count_type newCount )
    {
        ASSUME( arr == 0 );
        _arr = allocator::Alloc < X >( newCount );
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

    void _Free( X *arr )
    {
        allocator::Free( arr );
    }

    count_type _Reserved() const
    {
        return _count;
    }
};

template < typename X > class _CBasisVec < X, void, void, 0 > : CharMovable
{
    X *_arr;

public:
    typedef uiw count_type;

protected:
    count_type _count;

    _CBasisVec()
    {
        _arr = 0;
        _count = 0;
    }

    _CBasisVec( count_type count )
    {
        DBGBREAK;
    }

    X *_GetArr()
    {
        return _arr;
    }

    const X *_GetArr() const
    {
        return _arr;
    }

    void _SetArr( X *arr, count_type newCount )
    {
        ASSUME( arr != 0 || newCount == 0 );
        _arr = arr;
        _count = newCount;
    }

public:
    void SetSource( X *arr, count_type count )
    {
        ASSUME( arr != 0 || count == 0 );
        _arr = arr;
        _count = count;
    }
};

template < typename X, typename reservator, typename allocator, uiw static_size > class _CBaseVecConstStatic : public _CBasisVec < X, reservator, allocator, static_size >
{
    typedef _CBasisVec < X, reservator, allocator, static_size > baseType;

public:
    typedef typename baseType::count_type count_type;

protected:
    using baseType::_count;

public:
    typedef Iterator::_IterRandomConst < X, 1 > IterConst;
    typedef Iterator::_IterRandomConst < X, -1 > IterRevConst;

    _CBaseVecConstStatic()
    {
    }

    _CBaseVecConstStatic( const X *arr, count_type size )
    {
        this->_SetArr( (X *)arr, size );
    }

    _CBaseVecConstStatic( count_type count ) : baseType( count )
    {
    }

    const X *Data() const
    {
        return this->_GetArr();
    }

    const X &Back() const
    {
        ASSUME( _count );
        return this->_GetArr()[ _count - 1 ];
    }

    const X &Front() const
    {
        ASSUME( _count );
        return this->_GetArr()[ 0 ];
    }

    count_type Size() const
    {
        return _count;
    }

    bln IsEmpty() const
    {
        return _count == 0;
    }

    IterConst Begin() const
    {
        return IterConst( this->_GetArr() );
    }

    IterConst CBegin() const
    {
        return IterConst( this->_GetArr() );
    }

    IterConst End() const
    {
        return IterConst( this->_GetArr() + _count );
    }

    IterConst CEnd() const
    {
        return IterConst( this->_GetArr() + _count );
    }

    IterRevConst BeginRev() const
    {
        return IterRevConst( this->_GetArr() + _count - 1 );
    }

    IterRevConst CBeginRev() const
    {
        return IterRevConst( this->_GetArr() + _count - 1 );
    }

    IterRevConst EndRev() const
    {
        return IterRevConst( this->_GetArr() - 1 );
    }

    IterRevConst CEndRev() const
    {
        return IterRevConst( this->_GetArr() - 1 );
    }

    const X &Get( count_type index ) const
    {
        ASSUME( index < _count );
        return this->_GetArr()[ index ];
    }

    const X &operator [] ( count_type index ) const
    {
        ASSUME( index < _count );
        return this->_GetArr()[ index ];
    }
};

template < typename X, typename reservator, typename allocator, uiw static_size > class _CBaseVecStatic : public _CBaseVecConstStatic < X, reservator, allocator, static_size >
{
    typedef _CBaseVecConstStatic < X, reservator, allocator, static_size > baseType;

public:
    typedef typename baseType::count_type count_type;

protected:
    using baseType::_count;

public:
    /*using baseType::IterConst;
    using baseType::IterRevConst;*/
    typedef Iterator::_IterRandom < X, 1 > Iter;
    typedef Iterator::_IterRandom < X, -1 > IterRev;

    _CBaseVecStatic()
    {
    }

    _CBaseVecStatic( X *arr, count_type size )
    {
        this->_SetArr( arr, size );
    }

    _CBaseVecStatic( count_type count ) : baseType( count )
    {
    }

    X *Data()
    {
        return this->_GetArr();
    }

    X &Back()
    {
        ASSUME( _count );
        return this->_GetArr()[ _count - 1 ];
    }

    X &Front()
    {
        ASSUME( _count );
        return this->_GetArr()[ 0 ];
    }

    Iter Begin()
    {
        return Iter( this->_GetArr() );
    }

    Iter End()
    {
        return Iter( this->_GetArr() + _count );
    }

    IterRev BeginRev()
    {
        return IterRev( this->_GetArr() + _count - 1 );
    }

    IterRev EndRev()
    {
        return IterRev( this->_GetArr() - 1 );
    }

    X &Get( count_type index )
    {
        ASSUME( index < _count );
        return this->_GetArr()[ index ];
    }

    void Set( count_type index, const X &val )
    {
        ASSUME( index < _count );
        this->_GetArr()[ index ] = val;
    }

    X &operator [] ( count_type index )
    {
        ASSUME( index < _count );
        return this->_GetArr()[ index ];
    }
};

template < typename X, typename reservator, typename allocator, TypeSemantic_t typeSemantic, uiw static_size > class _CBaseVec : public _CBaseVecStatic < X, reservator, allocator, static_size >
{
    typedef _CBaseVecStatic < X, reservator, allocator, static_size > baseType;

    typedef _CBaseVec < X, reservator, allocator, typeSemantic, static_size > ownType;

public:
    typedef typename baseType::count_type count_type;

    static const count_type count_type_max = TypeDesc < count_type >::max;

private:
    using baseType::_count;

    static const TypeSemantic_t _c_typeSemantic = TERSWITCH2(
        typeSemantic == Sem_POD || TypeDesc < X >::is_pod, Sem_POD,
        typeSemantic == Sem_Mov || TypeDesc < X >::is_movable, Sem_Mov,
        typeSemantic );

    #ifndef DEFAULT_FUNC_PARAMS_SUPPORTED
        static const bln is_checkOverlap = true;
    #endif

    void _SizeUp( count_type newCount )
    {
        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsIncSize( newCount ) )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, newCount );
                _Copy < true >( this->_GetArr(), arr, _count );
                this->_Free( arr );
            }
        }
        else
        {
            _IncSize( newCount );
        }
    }

    void _SizeDown( count_type newCount )
    {
        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsDecSize( newCount ) )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, newCount );
                _Copy < true >( this->_GetArr(), arr, _count );
                this->_Free( arr );
            }
        }
        else
        {
            _DecSize( newCount );
        }
    }

    void _SizeToUnknown( count_type newCount )
    {
        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsUnkSize( newCount ) )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, newCount );
                _Copy < true >( this->_GetArr(), arr, _count );
                this->_Free( arr );
            }
        }
        else
        {
            _UnkSize( newCount );
        }
    }

    template < bln is_destroySource > void _Copy( X *RSTR target, X *source, count_type count )
    {
        if( _c_typeSemantic == Sem_POD )
        {
            _MemCpy( target, source, count * sizeof(X) );
        }
        else
        {
            for( ; count; --count )
            {
#               ifdef MOVE_SUPPORTED
                    if( is_destroySource )
                    {
                        new (target) X( std::move( *source ) );
                        source->~X();
                    }
                    else
                    {
                        new (target) X( *source );
                    }
#               else
                    new (target) X( *source );
                    if( is_destroySource )
                    {
                        source->~X();
                    }
#               endif
                ++target;
                ++source;
            }
        }
    }

    void _Erase( count_type pos, count_type count )
    {
        ASSUME( pos < _count && count <= _count && pos + count <= _count );

        if( _c_typeSemantic != Sem_POD )
        {
            _Destroy( this->_GetArr() + pos, count );
        }

        if( _c_typeSemantic == Sem_POD || _c_typeSemantic == Sem_Mov )
        {
            _MemMove( this->_GetArr() + pos, this->_GetArr() + pos + count, (_count - count - pos) * sizeof(X) );
            _count -= count;
            _SizeDown( _count );
        }
        else
        {
            count_type newCount = _count - count;
            if( _IsDecSize( newCount ) )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, newCount );
                _Copy < true >( this->_GetArr(), arr, pos );
                _Copy < true >( this->_GetArr() + pos, arr + pos + count, _count - count - pos );
                this->_Free( arr );
            }
            else
            {
                X *target = this->_GetArr() + pos;
                X *source = this->_GetArr() + pos + count;
                X *end = this->_GetArr() + _count;
                for( ; source != end; ++source, ++target )
                {
#                   ifdef MOVE_SUPPORTED
                        new (target) X( std::move( *source ) );
#                   else
                        new (target) X( *source );
#                   endif
                    source->~X();
                }
            }
            _count = newCount;
        }
    }

    X *_InsertRaw( count_type pos, count_type count )
    {
        ASSUME( pos < _count && count <= _count && pos + count <= _count );
        count_type newCount = _count + count;
        if( _c_typeSemantic == Sem_POD || _c_typeSemantic == Sem_Mov )
        {
            _SizeUp( newCount );
            _MemMove( this->_GetArr() + pos + count, this->_GetArr() + pos, (_count - pos) * sizeof(X) );
        }
        else
        {
            if( _IsIncSize( newCount ) )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, newCount );
                _Copy < true >( this->_GetArr(), arr, pos );
                _Copy < true >( this->_GetArr() + pos + count, arr + pos, _count - pos );
                this->_Free( arr );
            }
            else
            {
                X *target = this->_GetArr() + newCount - 1;
                X *source = this->_GetArr() + _count - 1;
                for( ; source != this->_GetArr() + pos; --target, --source )
                {
                    new (target) X( *source );
                    source->~X();
                }
            }
        }
        _count = newCount;
        return this->_GetArr() + pos;
    }

    void _Destroy( X *target, count_type count )
    {
        if( _c_typeSemantic != Sem_POD )
        {
            for( ; count; --count )
            {
                target->~X();
                ++target;
            }
        }
    }

public:

    ~_CBaseVec()
    {
        _Destroy( this->_GetArr(), _count );
    }

    _CBaseVec() : baseType()
    {
        _count = 0;
    }

    _CBaseVec( count_type size, count_type reserve ) : baseType( Funcs::Max( size, reserve ) )
    {
        _count = size;
        if( _c_typeSemantic != Sem_POD )
        {
            X *target = this->_GetArr();
            for( ; size; --size )
            {
                new (target) X();
                ++target;
            }
        }
    }

    _CBaseVec( const X *source, count_type size, count_type reserve ) : baseType( Funcs::Max( size, reserve ) )
    {
        _count = size;
        _Copy < false >( this->_GetArr(), (X *)source, size );
    }

    _CBaseVec( const ownType &source ) : baseType( source._count )
    {
        ASSUME( this != &source );
        _count = source._count;
        _Copy < false >( this->_GetArr(), (X *)source._GetArr(), source._count );
    }

#ifdef MOVE_SUPPORTED
    _CBaseVec( ownType &&source ) NOEXEPT : baseType( std::move( source ) )
    {
        ASSUME( this != &source );
        _count = source._count;
        source._count = 0;
        if( _cis_static )  //  TODO: bad
        {
            _Copy < false >( this->_GetArr(), (X *)source._GetArr(), _count );
        }
    }
#endif

    ownType &operator = ( const ownType &source )
    {
        if( this != &source )
        {
            _Destroy( this->_GetArr(), _count );
            _SizeToUnknown( source._count );
            _Copy < false >( this->_GetArr(), source._GetArr(), source._count );
            _count = source._count;
        }
        return *this;
    }

    void PushBackNum( count_type num = 1 )
    {
        _SizeUp( _count + num );
        if( _c_typeSemantic != Sem_POD )
        {
            X *target = this->_GetArr() + _count;
            for( count_type counter = num; counter; --counter )
            {
                new (target) X();
                ++target;
            }
        }
        _count += num;
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void PushBack( const X &source )
    {
        if( !_cis_static && is_checkOverlap )
        {
            uiw index = &source - this->_GetArr();
            _SizeUp( _count + 1 );
            new (this->_GetArr() + _count ) X( index < _count ? this->_GetArr()[ index ] : source );
            ++_count;
        }
        else
        {
            _SizeUp( _count + 1 );
            new (this->_GetArr() + _count ) X( source );
            ++_count;
        }
    }

#ifdef VAR_TEMPLATES_SUPPORTED
    template < typename... Args > void EmplaceBack( Args &&... args )
    {
        _SizeUp( _count + 1 );
        new (this->_GetArr() + _count ) X( args... );
        ++_count;
    }
#endif

    X *PushBackUninit()
    {
        _SizeUp( _count + 1 );
        X *ret = this->_GetArr() + _count;
        ++_count;
        return ret;
    }

    void PopBack( count_type num = 1 )
    {
        ASSUME( num <= _count );
        _count -= num;
        _Destroy( this->_GetArr() + _count, num );
        _SizeDown( _count );
    }

    void PopBackSafe( count_type num = 1 )
    {
        PopBack( Funcs::Min( num, _count ) );
    }

    void Reserve( count_type size )
    {
        _SizeUp( size );
    }

    count_type Reserved() const
    {
        return this->_Reserved();
    }

    void ShrinkToFit()
    {
        if( _c_typeSemantic == Sim_Strict )
        {
            if( this->_IsFlushReserved() )
            {
                X *arr = this->_GetArr();
                this->_SetArr( 0, _count );
                _Copy < true >( this->_GetArr(), arr, _count );
                this->_Free( arr );
            }
        }
        else
        {
            this->_FlushReserved();
        }
    }

    void Resize( count_type size )  //  Proto
    {
        if( size > _count )
        {
            PushBackNum( size - _count );
        }
        else
        {
            PopBack( _count - size );
        }
    }

    X *InsertUninit( count_type pos, count_type count )
    {
        return _InsertRaw( pos, count );
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Insert( count_type pos, const X &source, count_type count )
    {
        uiw offset = &source - this->_GetArr();
        bln is_belongs = offset < _count;
        const X *from = &source;
        X *target = _InsertRaw( pos, count );
        if( is_checkOverlap && is_belongs )
        {
            if( offset >= pos )
            {
                from += count;
            }
        }
        for( ; count; --count )
        {
            new (target) X( *from );
            ++target;
        }
    }

#ifdef VAR_TEMPLATES_SUPPORTED
    template < typename... Args > void Emplace( count_type pos, Args &&... args, count_type count )
    {
        X *target = _InsertRaw( pos, count );
        for( ; count; --count )
        {
            new (target) X( args... );
            ++target;
        }
    }
#endif

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Insert( count_type pos, const X *source, count_type count )
    {
        //  TODO: overlapping
        X *target = _InsertRaw( pos, count );
        for( ; count; --count )
        {
            new (target) X( source );
            ++target;
            ++source;
        }
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Insert( count_type pos, const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        DBGBREAK;
        //
    }

#ifdef VAR_TEMPLATES_SUPPORTED
    template < typename... Args > void Emplace( count_type pos, Args &&... args, count_type start = 0, count_type count = count_type_max )
    {
        DBGBREAK;
        //
    }
#endif

    template < typename IterType VEC_DEF_PARAM( , bln is_checkOverlap = true ) >
    void InsertIter( IterType begin, IterType end )
    {
        DBGBREAK;
        //
    }

    void Erase( count_type pos, count_type count )
    {
        _Erase( pos, count );
    }

    Iter Erase( IterConst where )
    {
        count_type index = &*where - this->_GetArr();
        ASSUME( index < _count );
        Erase( index, 1 );
        return Iter( this->_GetArr() + index );
    }

    Iter Erase( IterConst begin, IterConst end )
    {
        count_type index = &*where - this->_GetArr();
        count_type count = end - begin;
        ASSUME( index < _count && count < _count && index + count <= _count );
        Erase( index, count );
        return Iter( this->_GetArr() + index );
    }

    X *AppendUninit( count_type count )
    {
        _SizeUp( _count + count );
        X *ret = this->_GetArr() + _count;
        _count += count;
        return ret;
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Append( const X *source, count_type count )
    {
        if( _cis_static || !is_checkOverlap )
        {
            _SizeUp( _count + count );
            _Copy < false >( this->_GetArr() + _count, (X *)source, count );
        }
        else
        {
            uiw index = source - this->_GetArr();
            _SizeUp( _count + count );
            if( index < _count )
            {
                source = &this->_GetArr()[ index ];
            }
            _Copy < false >( this->_GetArr() + _count, (X *)source, count );
        }
        _count += count;
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Append( const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        ASSUME( start < source._count || count == 0 );
        DBGBREAK;
        //
    }

    template < typename IterType VEC_DEF_PARAM( , bln is_checkOverlap = true ) >
    void AppendIter( IterType begin, IterType end )
    {
        DBGBREAK;
        //
    }

private:
    NOINLINE void AssignOverlapped( const X &source, count_type count, count_type index )
    {
        DBGBREAK;
        //
    }

    NOINLINE void AssignOverlapped( const X *source, count_type count, count_type index )
    {
        DBGBREAK;
        //
    }

public:
    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Assign( const X &source, count_type count = 1 )
    {
        uiw index = &source - this->_GetArr();
        if( is_checkOverlap && index < _count )
        {
            AssignOverlapped( source, count, index );
        }
        else
        {
            _Destroy( this->_GetArr(), _count );
            _SizeToUnknown( count );
            for( _count = 0; _count < count; ++_count )
            {
                new (&this->_GetArr()[ _count ]) X( source );
            }
        }
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Assign( const X *source, count_type count )
    {
        uiw index = source - this->_GetArr();
        if( is_checkOverlap && index < _count )
        {
            AssignOverlapped( source, count, index );
        }
        else
        {
            _Destroy( this->_GetArr(), _count );
            _SizeToUnknown( count );
            _Copy < false >( this->_GetArr(), (X *)source, count );
            _count = count;
        }
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Assign( const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        DBGBREAK;
        //
    }

    template < typename IterType VEC_DEF_PARAM( , bln is_checkOverlap = true ) >
    void AssignIter( IterType begin, IterType end )
    {
        DBGBREAK;
        //
    }

    void Clear()
    {
        _Destroy( this->_GetArr(), _count );
        _count = 0;
        _SizeDown( 0 );
    }
};

}  //  namespace Private

template < typename X > class CRefVec;
template < typename X > class CCRefVec;
template < typename X, typename reservator = Reservator::Half <>, TypeSemantic_t typeSemantic = Sem_Strict, typename allocator = Allocator::Simple > class CVec;
template < typename X, uiw static_size, TypeSemantic_t typeSemantic = Sem_Strict > class CStaticVec;

template < typename X > class CRefVec : public Private::_CBaseVecStatic < X, void, void, 0 >
{
    typedef Private::_CBaseVecStatic < X, void, void, 0 > baseType;

public:
    typedef typename baseType::count_type count_type;

    CRefVec() : baseType()
    {}

    CRefVec( X *source, count_type size ) : baseType( source, size )
    {}
};

template < typename X > class CCRefVec : public Private::_CBaseVecConstStatic < X, void, void, 0 >
{
    typedef Private::_CBaseVecConstStatic < X, void, void, 0 > baseType;

public:
    typedef typename baseType::count_type count_type;

    CCRefVec() : baseType()
    {}

    CCRefVec( const X *source, count_type size ) : baseType( source, size )
    {}
};

template < typename X, typename reservator, TypeSemantic_t typeSemantic, typename allocator > class CVec : public Private::_CBaseVec < X, reservator, allocator, typeSemantic, 0 >
{
    typedef Private::_CBaseVec < X, reservator, allocator, typeSemantic, 0 > baseType;

public:
    typedef typename baseType::count_type count_type;

    CVec() : baseType()
    {}

    explicit CVec( count_type size, count_type reserve = 0 ) : baseType( size, reserve )
    {}

    CVec( const X *source, count_type size, count_type reserve ) : baseType( source, size, reserve )
    {}

    CVec( const CVec &source ) : baseType( source )
    {}

    CVec( const CRefVec< X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
    {}

    CVec( const CCRefVec< X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
    {}

    CVec &operator = ( const CRefVec< X > &source )
    {
        this->Assign( source.Data(), source.Size() );
        return *this;
    }

    CVec &operator = ( const CCRefVec< X > &source )
    {
        this->Assign( source.Data(), source.Size() );
        return *this;
    }

#ifdef MOVE_SUPPORTED
    CVec( CVec &&source ) : baseType( std::move( source ) )
    {}

    CVec &operator = ( const CVec &source )
    {
        baseType::operator = ( source );
        return *this;
    }
#endif

    CRefVec < X > ToRef()
    {
        return CRefVec < X >( this->Data(), this->Size() );
    }

    CCRefVec < X > ToRef() const
    {
        typedef _CBaseVecConstStatic < X, reservator, allocator, 0 > constBaseType;
        return CCRefVec < X >( constBaseType::Data(), constBaseType::Size() );
    }

    CCRefVec < X > ToCRef() const
    {
        typedef _CBaseVecConstStatic < X, reservator, allocator, 0 > constBaseType;
        return CCRefVec < X >( constBaseType::Data(), constBaseType::Size() );
    }
};

template < typename X, uiw static_size, TypeSemantic_t typeSemantic > class CStaticVec : public Private::_CBaseVec < X, void, void, typeSemantic, static_size >
{
    typedef Private::_CBaseVec < X, void, void, typeSemantic, static_size > baseType;

public:
    typedef typename baseType::count_type count_type;

    CStaticVec() : baseType()
    {}

    explicit CStaticVec( count_type size ) : baseType( size, 0 )
    {}

    CStaticVec( const X *source, count_type size ) : baseType( source, size, 0 )
    {}

    CStaticVec( const CStaticVec &source ) : baseType( source )
    {}

#ifdef MOVE_SUPPORTED
    CStaticVec( CStaticVec &&source ) : baseType( std::move( source ) )
    {}

    CStaticVec &operator = ( const CStaticVec &source )
    {
        baseType::operator = ( source );
        return *this;
    }
#endif

    CRefVec < X > ToRef()
    {
        return CRefVec < X >( this->Data(), this->Size() );
    }

    CCRefVec < X > ToRef() const
    {
        typedef _CBaseVecConstStatic < X, void, void, static_size > constBaseType;
        return CCRefVec < X >( constBaseType::Data(), constBaseType::Size() );
    }

    CCRefVec < X > ToCRef() const
    {
        typedef _CBaseVecConstStatic < X, void, void, static_size > constBaseType;
        return CCRefVec < X >( constBaseType::Data(), constBaseType::Size() );
    }
};

}  //  namespace StdLib

#endif __CVECTOR_HPP__