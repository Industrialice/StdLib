#ifndef __CVECTOR_HPP__
#define __CVECTOR_HPP__

#include <StdCoreLib.hpp>
#include "_CVectorBase.hpp"
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
template < typename X, typename reservator, typename allocator, uiw static_size > class _CBaseVecConstStatic : public _CBasisVec < X, reservator, allocator, static_size >
{
    typedef _CBasisVec < X, reservator, allocator, static_size > arrType;

public:
    typedef typename arrType::count_type count_type;

protected:
    using arrType::_count;

public:
    typedef Iterator::_IterRandomConst < X, 1 > IterConst;
    typedef Iterator::_IterRandomConst < X, -1 > IterRevConst;

    _CBaseVecConstStatic()
    {
    }

    _CBaseVecConstStatic( const X *arr, count_type size ) : arrType( (X *)arr, size )
    {
    }

    _CBaseVecConstStatic( count_type count ) : arrType( count )
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

    IterConst begin() const
    {
        return IterConst( this->_GetArr() );
    }

    IterConst cbegin() const
    {
        return IterConst( this->_GetArr() );
    }

    IterConst end() const
    {
        return IterConst( this->_GetArr() + _count );
    }

    IterConst cend() const
    {
        return IterConst( this->_GetArr() + _count );
    }

    IterRevConst beginRev() const
    {
        return IterRevConst( this->_GetArr() + _count - 1 );
    }

    IterRevConst cbeginRev() const
    {
        return IterRevConst( this->_GetArr() + _count - 1 );
    }

    IterRevConst endRev() const
    {
        return IterRevConst( this->_GetArr() - 1 );
    }

    IterRevConst cendRev() const
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
    using baseType::Data;
    using baseType::Back;
    using baseType::Front;
    //using baseType::Size;
    //using baseType::IsEmpty;
    using baseType::begin;
    using baseType::cbegin;
    using baseType::end;
    using baseType::cend;
    using baseType::beginRev;
    using baseType::cbeginRev;
    using baseType::endRev;
    using baseType::cendRev;
    using baseType::Get;
    using baseType::operator [];

    _CBaseVecStatic()
    {
    }

    _CBaseVecStatic( X *arr, count_type size ) : baseType( arr, size )
    {
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

    Iter begin()
    {
        return Iter( this->_GetArr() );
    }

    Iter end()
    {
        return Iter( this->_GetArr() + _count );
    }

    IterRev beginRev()
    {
        return IterRev( this->_GetArr() + _count - 1 );
    }

    IterRev endRev()
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
    typedef _CBasisVec < X, reservator, allocator, static_size > arrType;

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

    void _SizeUp( count_type sizeToLeave, count_type newCount )  //  you can't change _count before that call, will not destroy anything
    {
        ASSUME( newCount >= _count && sizeToLeave <= newCount );

        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsIncSize( newCount ) )
            {
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), sizeToLeave );
            }
        }
        else
        {
            _IncSize( newCount );
        }
    }

    void _SizeDown( count_type sizeToLeave, count_type newCount )  //  you can't change _count before that call, will not destroy anything
    {
        ASSUME( newCount <= _count && sizeToLeave <= newCount );

        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsDecSize( newCount ) )
            {
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), sizeToLeave );
            }
        }
        else
        {
            _DecSize( newCount );
        }
    }

    void _SizeToUnknown( count_type sizeToLeave, count_type newCount )  //  you can't change _count before that call, will not destroy anything
    {
        ASSUME( sizeToLeave <= newCount );

        if( _c_typeSemantic == Sem_Strict )
        {
            if( _IsUnkSize( newCount ) )
            {
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), sizeToLeave );
            }
        }
        else
        {
            _UnkSize( newCount );
        }
    }

    template < typename X, bln is_destroySource > struct _CopySelector;
    template < typename X > struct _CopySelector < X, true >
    {
        static void Copy( X *target, X *source )
        {
#           ifdef MOVE_SUPPORTED
                new (target) X( std::move( *source ) );
                source->~X();
#           else
                new (target) X( *source );
                source->~X();
#           endif
        }
    };
    template < typename X > struct _CopySelector < X, false >
    {
        static void Copy( X *target, const X *source )
        {
            new (target) X( *source );
        }
    };

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
                _CopySelector < X, is_destroySource >::Copy( target, source );
                ++target;
                ++source;
            }
        }
    }

    void _Erase( count_type pos, count_type count )
    {
        ASSUME( count && pos < _count && count <= _count && pos + count <= _count );

        if( _c_typeSemantic != Sem_POD )
        {
            _Destroy( this->_GetArr() + pos, count );
        }

        if( _c_typeSemantic == Sem_POD || _c_typeSemantic == Sem_Mov )
        {
            _MemMove( this->_GetArr() + pos, this->_GetArr() + pos + count, (_count - count - pos) * sizeof(X) );
            _SizeDown( _count - count, _count - count );
            _count -= count;
        }
        else
        {
            count_type newCount = _count - count;
            if( _IsDecSize( newCount ) )
            {
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), pos );
                _Copy < true >( this->_GetArr() + pos, oldArr._GetArr() + pos + count, _count - count - pos );
            }
            else
            {
                X *target = this->_GetArr() + pos;
                X *source = this->_GetArr() + pos + count;
                X *end = this->_GetArr() + _count;
                for( ; source != end; ++source, ++target )
                {
                    _CopySelector < X, true >::Copy( target, source );
                }
            }
            _count = newCount;
        }
    }

    X *_InsertRaw( count_type pos, count_type count )
    {
        ASSUME( pos <= _count );
        count_type newCount = _count + count;
        if( _c_typeSemantic == Sem_POD || _c_typeSemantic == Sem_Mov )
        {
            _SizeUp( _count, newCount );
            _MemMove( this->_GetArr() + pos + count, this->_GetArr() + pos, (_count - pos) * sizeof(X) );
        }
        else
        {
            if( _IsIncSize( newCount ) )
            {
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), pos );
                _Copy < true >( this->_GetArr() + pos + count, oldArr._GetArr() + pos, _count - pos );
            }
            else
            {
                X *target = this->_GetArr() + newCount - 1;
                X *source = this->_GetArr() + _count - 1;
                for( ; source != this->_GetArr() + pos - 1; --target, --source )
                {
                    _CopySelector < X, true >::Copy( target, source );
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
    _CBaseVec( ownType &&source ) NOEXCEPT : baseType( std::move( source ) )
    {
        ASSUME( this != &source );
        _count = source._count;
        source._count = 0;
        if( _cis_static )  //  TODO: bad
        {
            _Copy < true >( this->_GetArr(), (X *)source._GetArr(), _count );
        }
    }

    void operator = ( ownType &&source ) NOEXCEPT
    {
        ASSUME( this != &source );
        baseType::operator =( std::move( source ) );
        _count = source._count;
        source._count = 0;
        if( _cis_static )  //  TODO: bad
        {
            _Copy < true >( this->_GetArr(), (X *)source._GetArr(), _count );
        }
    }
#endif

#ifdef INITIALIZER_LISTS_SUPPORTED
    _CBaseVec( std::initializer_list < X > ilist ) : baseType( ilist.size() )
    {
        _count = ilist.size();
        _Copy < false >( this->_GetArr(), (X *)ilist.begin(), ilist.size() );
    }
#endif

    ownType &operator = ( const ownType &source )
    {
        if( this != &source )
        {
            _Destroy( this->_GetArr(), _count );
            _SizeToUnknown( 0, source._count );
            _Copy < false >( this->_GetArr(), (X *)source._GetArr(), source._count );
            _count = source._count;
        }
        return *this;
    }

    void PushBackNum( count_type num = 1 )
    {
        _SizeUp( _count, _count + num );
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
            _SizeUp( _count, _count + 1 );
            new (this->_GetArr() + _count ) X( index < _count ? this->_GetArr()[ index ] : source );
            ++_count;
        }
        else
        {
            _SizeUp( _count, _count + 1 );
            new (this->_GetArr() + _count ) X( source );
            ++_count;
        }
    }

#ifdef VAR_TEMPLATES_SUPPORTED
    template < typename... Args > void EmplaceBack( Args &&... args )
    {
        _SizeUp( _count, _count + 1 );
        new (this->_GetArr() + _count ) X( std::move( args )... );
        ++_count;
    }
#endif

    X *PushBackUninit()
    {
        _SizeUp( _count, _count + 1 );
        X *ret = this->_GetArr() + _count;
        ++_count;
        return ret;
    }

    void PopBack( count_type num = 1 )
    {
        ASSUME( num <= _count );
        _Destroy( this->_GetArr() + _count - 1, num );
        _SizeDown( _count - 1, _count - 1 );
        _count -= num;
    }

    void PopBackSafe( count_type num = 1 )
    {
        PopBack( Funcs::Min( num, _count ) );
    }

    void Reserve( count_type size )
    {
        _SizeUp( _count, size );
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
                arrType oldArr = *this;
                new (this) arrType( newCount );
                _Copy < true >( this->_GetArr(), oldArr._GetArr(), _count );
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

#ifdef VAR_TEMPLATES_SUPPORTED
    template < typename... Args > void Emplace( count_type pos, Args &&... args, count_type count = 1 )
    {
        X *target = _InsertRaw( pos, count );
        for( ; count; --count )
        {
            new (target) X( args... );
            ++target;
        }
    }
#endif

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

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Insert( count_type pos, const X *source, count_type count )
    {
        if( is_checkOverlap )
        {
            uiw index = source - this->_GetArr();
            if( index < _count )  //  overlapped
            {
                DBGBREAK;  //  TODO:
                return;
            }
        }

        X *target = _InsertRaw( pos, count );
        for( ; count; --count )
        {
            new (target) X( *source );
            ++target;
            ++source;
        }
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Insert( count_type pos, const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        ASSUME( pos < _count && source.Size() > start );
        if( count > source.Size() - start )
        {
            count = source.Size() - start;
        }

        Insert < is_checkOverlap >( pos, source.Data() + start, count );  //  TODO: default param
    }

    template < typename IterType VEC_DEF_PARAM( , bln is_checkOverlap = true ) >
    void Insert( IterConst where, IterType begin, IterType end )
    {
        ASSUME( end >= begin );
        uiw dist = Algorithm::Distance( begin, end );
        uiw pos = where.Ptr() - this->_GetArr();
        ASSUME( pos < _count );
        if( IterType::iteratorType == Iterator::Type::Random )
        {
            Insert < is_checkOverlap >( pos, begin.Ptr(), dist );  //  TODO: default param
        }
        else
        {
            X *target = _InsertRaw( pos, dist );
            for( ; first != last; ++first )
            {
                new (target) X( *first );
                ++target;
            }
        }
    }

#ifdef INITIALIZER_LISTS_SUPPORTED
    void Insert( count_type pos, std::initializer_list < X > what )
    {
        Insert < false >( pos, what.begin(), what.size() );
    }

    void Insert( IterConst where, std::initializer_list < X > what )
    {
        Insert < false >( where, what.begin(), what.size() );
    }
#endif

    void Erase( count_type pos, count_type count = uiw_max )
    {
        ASSUME( pos < _count );
        _Erase( pos, Funcs::Min < count_type >( _count - pos, count ) );
    }

    Iter Erase( IterConst where, count_type count = uiw_max )
    {
        count_type index = where.Ptr() - this->_GetArr();
        ASSUME( index < _count );
        Erase( index, count );  //  Erase will check the count
        return Iter( this->_GetArr() + index );
    }

    Iter Erase( IterConst begin, IterConst end )
    {
        ASSUME( end >= begin );
        count_type index = begin.Ptr() - this->_GetArr();
        count_type count = end - begin;
        ASSUME( index < _count && count < _count && index + count <= _count );
        Erase( index, count );
        return Iter( this->_GetArr() + index );
    }

    X *AppendUninit( count_type count )
    {
        _SizeUp( _count, _count + count );
        X *ret = this->_GetArr() + _count;
        _count += count;
        return ret;
    }

    VEC_DEF_PARAM( template < bln is_checkOverlap = true > )
    void Append( const X *source, count_type count )
    {
        if( _cis_static || !is_checkOverlap )
        {
            _SizeUp( _count, _count + count );
            _Copy < false >( this->_GetArr() + _count, (X *)source, count );
        }
        else
        {
            uiw index = source - this->_GetArr();
            _SizeUp( _count, _count + count );
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
    void Append( IterType begin, IterType end )
    {
        DBGBREAK;
        //
    }

#ifdef INITIALIZER_LISTS_SUPPORTED
    void Append( std::initializer_list < X > ilist )
    {
        Append( ilist.begin(), ilist.size() );
    }
#endif

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
            _SizeToUnknown( 0, count );
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
            _SizeToUnknown( 0, count );
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
    void Assign( IterType begin, IterType end )
    {
        DBGBREAK;
        //
    }

#ifdef INITIALIZER_LISTS_SUPPORTED
    void Assign( std::initializer_list < X > ilist )
    {
        Assign < false >( ilist.begin(), ilist.size() );
    }
#endif

    void Clear()
    {
        _Destroy( this->_GetArr(), _count );
        _SizeDown( 0, 0 );
        _count = 0;
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

    CRefVec &operator = ( const CRefVec &source )
    {
        if( this != &source )
        {
            this->~CRefVec();
            new (this) CRefVec( (X *)source.Data(), source.Size() );
        }
        return *this;
    }
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

    CCRefVec &operator = ( const CCRefVec &source )
    {
        if( this != &source )
        {
            this->~CCRefVec();
            new (this) CCRefVec( source.Data(), source.Size() );
        }
        return *this;
    }
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

    CVec( const CRefVec < X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
    {}

    CVec( const CCRefVec < X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
    {}
    
    CVec &operator = ( const CVec &source )
    {
        baseType::operator =( source );
        return *this;
    }

    CVec &operator = ( const CRefVec < X > &source )
    {
        this->Assign( source.Data(), source.Size() );
        return *this;
    }

    CVec &operator = ( const CCRefVec < X > &source )
    {
        this->Assign( source.Data(), source.Size() );
        return *this;
    }

    /*CVec &operator += ( const CRefVec < X > &source )
    {
        this->Append( source.Data(), source.Size() );
        return *this;
    }

    CVec &operator += ( const CCRefVec < X > &source )
    {
        this->Append( source.Data(), source.Size() );
        return *this;
    }

    CVec &operator += ( CVec &source )
    {
        this->Append( source.Data(), source.Size() );
        return *this;
    }*/

#ifdef MOVE_SUPPORTED
    CVec( CVec &&source ) NOEXCEPT = default;
    CVec &operator = ( CVec &&source ) NOEXCEPT = default;
#endif

#ifdef INITIALIZER_LISTS_SUPPORTED
    CVec( std::initializer_list < X > ilist ) : baseType( ilist )
    {}

    CVec &operator = ( std::initializer_list < X > ilist )
    {
        baseType::Assign( ilist );
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
    
    CStaticVec &operator = ( const CStaticVec &source )
    {
        baseType::operator =( source );
        return *this;
    }

#ifdef MOVE_SUPPORTED
    CStaticVec( CStaticVec &&source ) = default;
    CStaticVec &operator = ( CStaticVec &&source ) = default;
#endif

#ifdef INITIALIZER_LISTS_SUPPORTED
    CStaticVec( std::initializer_list < X > ilist ) : baseType( ilist )
    {}

    CStaticVec &operator = ( std::initializer_list < X > ilist )
    {
        baseType::Assign( ilist );
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

//  force all methods compilation to check for correctness
template class CVec < int >;
template class CVec < int, void >;
template class CRefVec < int >;
template class CCRefVec < int >;
template class CStaticVec < int, 100 >;

}  //  namespace StdLib

#endif __CVECTOR_HPP__