#ifndef __CVECTOR_HPP__
#define __CVECTOR_HPP__

#include <StdCoreLib.hpp>
#include "_CVectorBase.hpp"
#include "Reservators.hpp"
#include "Allocators.hpp"
#include "Iterator.hpp"
#include "Algorithm.hpp"

namespace StdLib
{
namespace Private
{
template < typename X, typename reservator, typename allocator, uiw static_size > class _CBaseVecConstStatic : public _CBasisVec < X, reservator, allocator, static_size >
{
    typedef _CBasisVec < X, reservator, allocator, static_size > arrType;

public:
    typedef typename arrType::count_type count_type;

public:
    typedef Iterator::_IterRandomConst < X, 1 > IterConst;
    typedef Iterator::_IterRandomConst < X, -1 > IterRevConst;

    _CBaseVecConstStatic()
    {}

    _CBaseVecConstStatic( count_type size, count_type reserve ) : arrType( size, reserve )
    {
    }

    const X *Data() const
    {
        return this->_GetArr();
    }

    const X &Back() const
    {
        ASSUME( this->_Size() );
        return this->_GetArr()[ this->_Size() - 1 ];
    }

    const X &Front() const
    {
        ASSUME( this->_Size() );
        return this->_GetArr()[ 0 ];
    }

    count_type Size() const
    {
        return this->_Size();
    }

    count_type LastIndex() const  //  can't be called on an emtpy vector
    {
        ASSUME( this->_Size() );
        return this->_Size() - 1;
    }

    bool IsEmpty() const
    {
        return this->_Size() == 0;
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
        return IterConst( this->_GetArr() + this->_Size() );
    }

    IterConst cend() const
    {
        return IterConst( this->_GetArr() + this->_Size() );
    }

    IterRevConst beginRev() const
    {
        return IterRevConst( this->_GetArr() + this->_Size() - 1 );
    }

    IterRevConst cbeginRev() const
    {
        return IterRevConst( this->_GetArr() + this->_Size() - 1 );
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
        ASSUME( index < this->_Size() );
        return this->_GetArr()[ index ];
    }

    const X &operator [] ( count_type index ) const
    {
        ASSUME( index < this->_Size() );
        return this->_GetArr()[ index ];
    }
};

template < typename X, typename reservator, typename allocator, uiw static_size > class _CBaseVecStatic : public _CBaseVecConstStatic < X, reservator, allocator, static_size >
{
    typedef _CBaseVecConstStatic < X, reservator, allocator, static_size > baseType;

public:
    typedef typename baseType::count_type count_type;

public:
    using typename baseType::IterConst;
    using typename baseType::IterRevConst;
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
    {}

    _CBaseVecStatic( count_type size, count_type reserve ) : baseType( size, reserve )
    {
    }

    X *Data()
    {
        return this->_GetArr();
    }

    X &Back()
    {
        ASSUME( this->_Size() );
        return this->_GetArr()[ this->_Size() - 1 ];
    }

    X &Front()
    {
        ASSUME( this->_Size() );
        return this->_GetArr()[ 0 ];
    }

    Iter begin()
    {
        return Iter( this->_GetArr() );
    }

    Iter end()
    {
        return Iter( this->_GetArr() + this->_Size() );
    }

    IterRev beginRev()
    {
        return IterRev( this->_GetArr() + this->_Size() - 1 );
    }

    IterRev endRev()
    {
        return IterRev( this->_GetArr() - 1 );
    }

    X &Get( count_type index )
    {
        ASSUME( index < this->_Size() );
        return this->_GetArr()[ index ];
    }

    void Set( count_type index, const X &val )
    {
        ASSUME( index < this->_Size() );
        this->_GetArr()[ index ] = val;
    }

    X &operator [] ( count_type index )
    {
        ASSUME( index < this->_Size() );
        return this->_GetArr()[ index ];
    }
};

template < typename X, typename reservator, typename allocator, TypeSemantic_t typeSemantic, uiw static_size > class _CBaseVec : public _CBaseVecStatic < X, reservator, allocator, static_size >
{
    typedef _CBasisVec < X, reservator, allocator, static_size > arrType;

    typedef _CBaseVecStatic < X, reservator, allocator, static_size > baseType;

    typedef _CBaseVec < X, reservator, allocator, typeSemantic, static_size > ownType;

public:
	using typename baseType::IterConst;
	using typename baseType::IterRevConst;
	using typename baseType::Iter;
	using typename baseType::IterRev;

    typedef typename baseType::count_type count_type;

    static const count_type count_type_max = TypeDesc < count_type >::max;

    static const bool _cis_POD = typeSemantic == Sem_POD || TypeDesc < X >::is_pod;
    static const bool _cis_MoveAsPOD = typeSemantic == Sem_MovableAsPOD || TypeDesc < X >::is_movableAsPOD;

	NOINLINE void _SizeUpWithReserveIncreasion( count_type sizeToLeave, count_type newCount )  //  will not destroy anything
	{
        ASSUME( newCount >= this->_Size() && sizeToLeave <= this->_Size() );

        if( _cis_MoveAsPOD == false )
        {
            count_type newReserve = this->_TryIncSizeLocally( newCount );
            if( newReserve != TypeDesc < count_type >::max )
            {
                arrType newArr( newCount, newReserve );
                ASSUME( newArr._IsStatic() == false );
                _Copy < true >( newArr._GetArr(), this->_GetArr(), sizeToLeave );
                this->_Transfer( &newArr );
            }
        }
        else
        {
            this->_IncSize( newCount );
        }
	}

    void _SizeUp( count_type sizeToLeave, count_type newCount )  //  will not destroy anything
    {
        ASSUME( newCount >= this->_Size() && sizeToLeave <= this->_Size() );
		if( !this->_IncSizeIfHasEnoughReserve( newCount ) )
		{
			this->_SizeUpWithReserveIncreasion( sizeToLeave, newCount );
		}
    }

    void _SizeDown( count_type sizeToLeave, count_type newCount )  //  will not destroy anything
    {
        ASSUME( newCount <= this->_Size() && sizeToLeave <= this->_Size() );

        if( _cis_MoveAsPOD == false )
        {
            count_type newReserve = this->_TryDecSizeLocally( newCount );
            if( newReserve != TypeDesc < count_type >::max )
            {
                ASSUME( this->_IsStatic() == false );
                arrType newArr;
                newArr._Transfer( this );
                new (this) arrType( newCount, newReserve );
                _Copy < true >( this->_GetArr(), newArr._GetArr(), sizeToLeave );
            }
        }
        else
        {
            this->_DecSize( newCount );
        }
    }

    void _SizeToUnknown( count_type sizeToLeave, count_type newCount )  //  will not destroy anything
    {
        ASSUME( sizeToLeave <= this->_Size() );

        if( _cis_MoveAsPOD == false )
        {
            count_type newReserve = this->_TryUnkSizeLocally( newCount );
            if( newReserve != TypeDesc < count_type >::max )
            {
                arrType newArr( newCount, newReserve );
                if( newArr._IsStatic() )  //  shrinking from dynamic to static
                {
                    ASSUME( this->_IsStatic() == false );
                    newArr._Transfer( this );
                    new (this) arrType( newCount, newReserve );
                    _Copy < true >( this->_GetArr(), newArr._GetArr(), sizeToLeave );
                }
                else  //  either shrinking from dynamic to dynamic, or growing from static to dynamic or dynamic to dynamic
                {
                    _Copy < true >( newArr._GetArr(), this->_GetArr(), sizeToLeave );
                    this->_Transfer( &newArr );
                }
            }
        }
        else
        {
            this->_UnkSize( newCount );
        }
    }

    template < typename Type, bool is_useMoveContructor > struct _MoveTo;

    template < typename Type > struct _MoveTo < Type, true >
    {
        static void Move( Type *target, Type *source )
        {
            new (target) Type( std::move( *source ) );
            source->~Type();
        }
    };

    template < typename Type > struct _MoveTo < Type, false >
    {
        static void Move( Type *target, Type *source )
        {
            new (target) Type( *source );
            source->~Type();
        }
    };

    template < typename Type, bool is_destroySource > struct _CopySelector;
    template < typename Type > struct _CopySelector < Type, true >
    {
        static void Copy( Type *target, Type *source )
        {
            _MoveTo < Type, std::is_move_constructible < Type >::value >::Move( target, source );
        }
    };
    template < typename Type > struct _CopySelector < Type, false >
    {
        static void Copy( Type *target, const Type *source )
        {
            new (target) Type( *source );
        }
    };

    template < bool is_destroySource, bool is_canBeOverlapped = false > void _Copy( X *target, X *source, count_type count )
    {
        ASSUME( target != source || count == 0 );

        if( _cis_POD )
        {
            if( is_canBeOverlapped )
            {
                _MemMove( target, source, count * sizeof(X) );
            }
            else
            {
                _MemCpy( target, source, count * sizeof(X) );
            }
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
        ASSUME( count && pos < this->_Size() && count <= this->_Size() && pos + count <= this->_Size() );

        if( _cis_POD == false )
        {
            _Destroy( this->_GetArr() + pos, count );
        }

        if( _cis_MoveAsPOD )
        {
            _MemMove( this->_GetArr() + pos, this->_GetArr() + pos + count, (this->_Size() - count - pos) * sizeof(X) );
            _SizeDown( this->_Size() - count, this->_Size() - count );
        }
        else
        {
            count_type curCount = this->_Size();
            count_type newCount = curCount - count;
            count_type newReserve = this->_TryDecSizeLocally( newCount );
            if( newReserve != TypeDesc < count_type >::max )
            {
                ASSUME( this->_IsStatic() == false );
                arrType newArr;
                newArr._Transfer( this );
                new (this) arrType( newCount, newReserve );
                _Copy < true >( this->_GetArr(), newArr._GetArr(), pos );
                _Copy < true >( this->_GetArr() + pos, newArr._GetArr() + pos + count, curCount - count - pos );
            }
            else
            {
                _Copy < true >( this->_GetArr() + pos, this->_GetArr() + pos + count, curCount - count - pos );
            }
        }
    }

    X *_InsertRaw( count_type pos, count_type count )
    {
        ASSUME( pos <= this->_Size() );
        count_type curCount = this->_Size();
        count_type newCount = curCount + count;
        if( _cis_MoveAsPOD )
        {
            _SizeUp( curCount, newCount );
            _MemMove( this->_GetArr() + pos + count, this->_GetArr() + pos, (curCount - pos) * sizeof(X) );
        }
        else
        {
            count_type newReserve = this->_TryIncSizeLocally( newCount );
            if( newReserve != TypeDesc < count_type >::max )
            {
                arrType newArr( newCount, newReserve );
                ASSUME( newArr._IsStatic() == false );
                _Copy < true >( newArr._GetArr(), this->_GetArr(), pos );
                _Copy < true >( newArr._GetArr() + pos + count, this->_GetArr() + pos, this->_Size() - pos );
                this->_Transfer( &newArr );
            }
            else
            {
                _Copy < true >( this->_GetArr() + pos + count, this->_GetArr() + pos, curCount - pos );
            }
        }
        return this->_GetArr() + pos;
    }

    void _Destroy( X *target, count_type count )
    {
        if( _cis_POD == false )
        {
            for( ; count; --count )
            {
                target->~X();
                ++target;
            }
        }
    }

	NOINLINE void _AppendWithReserveIncreasion( const X &source )
	{
		count_type curCount = this->_Size();
		uiw index = &source - this->_GetArr();
		_SizeUp( curCount, curCount + 1 );
		new (this->_GetArr() + curCount) X( index < curCount ? this->_GetArr()[ index ] : source );
	}

public:

    ~_CBaseVec()
    {
        _Destroy( this->_GetArr(), this->_Size() );
    }

	_CBaseVec() : baseType()
	{}

    _CBaseVec( count_type reserve ) : baseType( 0, reserve )
    {}

    _CBaseVec( count_type size, count_type reserve ) : baseType( size, reserve )
    {
        if( _cis_POD == false )
        {
            X *target = this->_GetArr();
            for( ; size; --size )
            {
                new (target) X();
                ++target;
            }
        }
    }

    _CBaseVec( const X *source, count_type size, count_type reserve ) : baseType( size, reserve )
    {
        _Copy < false >( this->_GetArr(), (X *)source, size );
    }

    template < typename IterType, typename = typename EnableIf< IsDerivedFrom < IterType, Iterator::_TypeIterator >::value >::type > 
    _CBaseVec( IterType begin, IterType end, count_type reserve ) : baseType( Algorithm::Distance( begin, end ), reserve )
    {
        if( IterType::iteratorType == Iterator::Type::Random )
        {
            Assign < false >( begin.Ptr(), this->_Size() );
        }
        else  //  can't be overlapped because CVec iterator is random
        {
            X *target = this->_GetArr();
            for( ; begin != end; ++begin, ++target )
            {
                new (target) X( *begin );
            }
        }
    }

    _CBaseVec( const ownType &source ) : baseType( source._Size(), source._Size() )
    {
        ASSUME( this != &source );
        _Copy < false >( this->_GetArr(), (X *)source._GetArr(), source._Size() );
    }

    _CBaseVec( ownType &&source ) NOEXCEPT
    {
        ASSUME( this != &source );
        if( source._IsStatic() )  //  when static, the basic move constructor won't do anything
        {
            _SizeUp( 0, source._Size() );
            _Copy < true >( this->_GetArr(), source._GetArr(), source._Size() );
            source._SizeDown( 0, 0 );
        }
        else
        {
            this->_Transfer( &source );
        }
    }

    void operator = ( ownType &&source ) NOEXCEPT
    {
        ASSUME( this != &source );
        if( source._IsStatic() )  //  when static, the basic move constructor won't do anything
        {
            _SizeUp( 0, source._Size() );
            _Copy < true >( this->_GetArr(), source._GetArr(), source._Size() );
            source._SizeDown( 0, 0 );
        }
        else
        {
            this->_Transfer( &source );
        }
    }

    _CBaseVec( std::initializer_list < X > ilist ) : baseType( ilist.size(), ilist.size() )
    {
        _Copy < false >( this->_GetArr(), (X *)ilist.begin(), ilist.size() );
    }

    ownType &operator = ( const ownType &source )
    {
        if( this != &source )
        {
            _Destroy( this->_GetArr(), this->_Size() );
            _SizeToUnknown( 0, source._Size() );
            _Copy < false >( this->_GetArr(), (X *)source._GetArr(), source._Size() );
        }
        return *this;
    }

    template < typename... Args > void EmplaceBack( Args &&... args )
    {
        count_type curCount = this->_Size();
        _SizeUp( curCount, curCount + 1 );
        new (this->_GetArr() + curCount ) X( std::forward < Args >( args )... );
    }

    X *PushBackUninit()
    {
        count_type curCount = this->_Size();
        _SizeUp( curCount, curCount + 1 );
        return this->_GetArr() + curCount;
    }

    void PopBack( count_type num = 1 )
    {
        ASSUME( num <= this->_Size() );
        _Destroy( this->_GetArr() + this->_Size() - num, num );
        _SizeDown( this->_Size() - 1, this->_Size() - 1 );
    }

    void PopBackSafe( count_type num = 1 )
    {
        PopBack( Funcs::Min( num, this->_Size() ) );
    }

    void Reserve( count_type size )
    {
        this->_Reserve( size );
    }

    count_type Reserved() const
    {
        return this->_Reserved();
    }

    void ShrinkToFit()
    {
        if( _cis_MoveAsPOD == false )
        {
            count_type newReserve = this->_TryFlushReservedLocally();
            if( newReserve != TypeDesc < count_type >::max )
            {
                ASSUME( this->_IsStatic() == false );
                arrType newArr;
                newArr._Transfer( this );
                new (this) arrType( newArr._Size(), newReserve );
                _Copy < true >( this->_GetArr(), newArr._GetArr(), newArr._Size() );
            }
        }
        else
        {
            this->_FlushReserved();
        }
    }

    void Resize( count_type size, bool is_initialize = true )  //  Proto
    {
		if( size != this->_Size() )
		{
			if( size > this->_Size() )
			{
				AppendNum( size - this->_Size(), is_initialize );
			}
			else
			{
				PopBack( this->_Size() - size );
			}
		}
    }

    template < typename... Args > void Emplace( count_type pos, Args &&... args, count_type count = 1 )
    {
        X *target = _InsertRaw( pos, count );
        for( ; count; --count )
        {
            new (target) X( args... );
            ++target;
        }
    }

    X *InsertNum( count_type pos, count_type num = 1, bool is_initialize = true )
    {
        X *target = _InsertRaw( pos, num );
		if( _cis_POD == false && is_initialize )
		{
			X *cpy = target;
			for( count_type counter = num; counter; --counter )
			{
				new (cpy) X();
				++cpy;
			}
		}
		return target;
    }

    template < bool is_checkOverlap = true > void Insert( count_type pos, const X &source, count_type count = 1 )
    {
        uiw offset = &source - this->_GetArr();
        bool is_belongs = offset < this->_Size();
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

    void Insert( count_type pos, X &&source )
    {
        uiw offset = &source - this->_GetArr();
        ASSUME( offset >= this->_Size() );  //  overlapping isn't allowed
        X *target = _InsertRaw( pos, 1 );
        new (target) X( std::move( source ) );
    }

    template < bool is_checkOverlap = true > void Insert( count_type pos, const X *source, count_type count )
    {
        if( is_checkOverlap )
        {
            uiw index = source - this->_GetArr();
            if( index < this->_Size() )  //  overlapped
            {
                InsertOverlapped( pos, count, index );
                return;
            }
        }

        X *target = _InsertRaw( pos, count );
        _Copy < false, false >( target, (X *)source, count );
    }

    template < uiw count > void Insert( count_type pos, const X (&source)[ count ] )
    {
        Insert < false >( pos, source, count );
    }

    template < bool is_checkOverlap = true > void Insert( count_type pos, const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        ASSUME( pos < this->_Size() && source.Size() > start );
        if( count > source.Size() - start )
        {
            count = source.Size() - start;
        }

        Insert < is_checkOverlap >( pos, source.Data() + start, count );
    }

    template < typename IterType, bool is_checkOverlap = true, typename = typename EnableIf< IsDerivedFrom < IterType, Iterator::_TypeIterator >::value >::type >
    void Insert( IterConst where, IterType begin, IterType end )
    {
        uiw dist = Algorithm::Distance( begin, end );
        uiw pos = where.Ptr() - this->_GetArr();
        ASSUME( pos < this->_Size() );
        if( IterType::iteratorType == Iterator::Type::Random )
        {
            Insert < is_checkOverlap >( pos, begin.Ptr(), dist );
        }
        else  //  can't be overlapped because CVec iterator is random
        {
            X *target = _InsertRaw( pos, dist );
            for( ; begin != end; ++begin, ++target )
            {
                new (target) X( *begin );
            }
        }
    }

    void Insert( count_type pos, std::initializer_list < X > what )
    {
        Insert < false >( pos, what.begin(), what.size() );
    }

    void Insert( IterConst where, std::initializer_list < X > what )
    {
        Insert < false >( where, what.begin(), what.size() );
    }

    void Erase( count_type pos, count_type count = uiw_max )
    {
        ASSUME( pos < this->_Size() );
        _Erase( pos, Funcs::Min < count_type >( this->_Size() - pos, count ) );
    }

    Iter Erase( IterConst where, count_type count = uiw_max )
    {
        count_type index = where.Ptr() - this->_GetArr();
        ASSUME( index < this->_Size() );
        Erase( index, count );  //  Erase will check the count
        return Iter( this->_GetArr() + index );
    }

    Iter Erase( IterConst begin, IterConst end )
    {
        ASSUME( end >= begin );
        count_type index = begin.Ptr() - this->_GetArr();
        count_type count = end - begin;
        ASSUME( index < this->_Size() && count < this->_Size() && index + count <= this->_Size() );
        Erase( index, count );
        return Iter( this->_GetArr() + index );
    }

	void Append( const X &source )
	{
		count_type curCount = this->_Size();
		if( this->_IncSizeIfHasEnoughReserve( curCount + 1 ) )
		{
			new (this->_GetArr() + curCount) X( source );
		}
		else
		{
			this->_AppendWithReserveIncreasion( source );
		}
	}

	void Append( X &&source )
	{
		count_type curCount = this->_Size();
		_SizeUp( curCount, curCount + 1 );
		new (this->_GetArr() + curCount) X( std::move( source ) );
	}

	X *AppendNum( count_type num = 1, bool is_initialize = true )
	{
		count_type curCount = this->_Size();
		_SizeUp( curCount, curCount + num );
		X *target = this->_GetArr() + curCount;
		if( _cis_POD == false && is_initialize )
		{
			X *cpy = target;
			for( count_type counter = num; counter; --counter )
			{
				new (cpy) X();
				++cpy;
			}
		}
		return target;
	}

    template < bool is_checkOverlap = true > void Append( const X *source, count_type count )
    {
        count_type curCount = this->_Size();
        if( this->_IsStatic() || !is_checkOverlap )
        {
            _SizeUp( curCount, curCount + count );
            _Copy < false >( this->_GetArr() + curCount, (X *)source, count );
        }
        else
        {
            uiw index = source - this->_GetArr();
            _SizeUp( curCount, curCount + count );
            if( index < curCount )  //  true if overlapped
            {
                source = &this->_GetArr()[ index ];
            }
            _Copy < false, true >( this->_GetArr() + curCount, (X *)source, count );
        }
    }

    template < uiw count > void Append( const X (&source)[ count ] )
    {
        this->Append < false >( source, count );
    }

    template < bool is_checkOverlap = true > void Append( const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        ASSUME( start < source._Size() || count == 0 );
        count = Funcs::Min( count, source.Size() - start );
        uiw curCount = this->_Size();
        _SizeUp( curCount, curCount + count );
        _Copy < false, true >( this->_GetArr() + curCount, (X *)source.Data() + start, count );
    }

    template < typename IterType, bool is_checkOverlap = true, typename = typename EnableIf< IsDerivedFrom < IterType, Iterator::_TypeIterator >::value >::type >
    void Append( IterType begin, IterType end )
    {
        uiw dist = Algorithm::Distance( begin, end );
        if( IterType::iteratorType == Iterator::Type::Random )
        {
            Append < is_checkOverlap >( begin.Ptr(), dist );
        }
        else  //  can't be overlapped because CVec iterator is random
        {
            count_type curCount = this->_Size();
            _SizeUp( curCount, curCount + dist );
            X *target = this->_GetArr() + curCount;
            for( ; begin != end; ++begin, ++target )
            {
                new (target) X( *begin );
            }
        }
    }

    void Append( std::initializer_list < X > ilist )
    {
        Append( ilist.begin(), ilist.size() );
    }

private:
    NOINLINE void InsertOverlapped( count_type start, count_type count, count_type index )
    {
		X *tempArray = allocator::template Alloc < X >( count );  //  TODO: bad
		_Copy < false, false >( tempArray, this->_GetArr() + index, count );
        X *target = _InsertRaw( start, count );
        _Copy < false, false >( target, tempArray, count );
		allocator::Free( tempArray );
    }

    NOINLINE void AssignOverlappedSingle( count_type count, count_type index )
    {
        ASSUME( this->Size() && count );
        _Destroy( this->_GetArr(), index );
        _Destroy( this->_GetArr() + index + 1, this->Size() - index - 1 );
        if( index )
        {
            _Copy < true, true >( this->_GetArr(), this->_GetArr() + index, 1 );
        }
        _SizeToUnknown( 1, count );
        for( count_type copyIndex = 1; copyIndex < count; ++copyIndex )
        {
            new (this->_GetArr() + copyIndex) X( *this->_GetArr() );
        }
    }

    NOINLINE void AssignOverlappedArray( count_type count, count_type index )
    {
        ASSUME( this->_Size() - index >= count );
        _Destroy( this->_GetArr(), index );
        _Destroy( this->_GetArr() + index + 1, this->Size() - index - 1 );
        if( index )
        {
            _Copy < true, true >( this->_GetArr(), this->_GetArr() + index, count );
        }
        _SizeToUnknown( count, count );
    }

public:
    template < bool is_checkOverlap = true > void Assign( const X &source, count_type count = 1 )
    {
        count_type curCount = this->_Size();
        uiw index = &source - this->_GetArr();
        if( is_checkOverlap && index < curCount )
        {
            AssignOverlappedSingle( count, index );
        }
        else
        {
            _Destroy( this->_GetArr(), curCount );
            _SizeToUnknown( 0, count );
            for( curCount = 0; curCount < count; ++curCount )
            {
                new (&this->_GetArr()[ curCount ]) X( source );
            }
        }
    }

    void Assign( X &&source )
    {
        count_type curCount = this->_Size();
        uiw index = &source - this->_GetArr();
        ASSUME( index >= curCount );  //  overlapping isn't allowed
        _Destroy( this->_GetArr(), curCount );
        _SizeToUnknown( 0, source.Size() );
        for( curCount = 0; curCount < source.Size(); ++curCount )
        {
            new (&this->_GetArr()[ curCount ]) X( std::move( source ) );
        }
    }

    template < bool is_checkOverlap = true > void Assign( const X *source, count_type count )
    {
        uiw index = source - this->_GetArr();
        if( is_checkOverlap && index < this->_Size() )
        {
            AssignOverlappedArray( count, index );
        }
        else
        {
            _Destroy( this->_GetArr(), this->_Size() );
            _SizeToUnknown( 0, count );
            _Copy < false >( this->_GetArr(), (X *)source, count );
        }
    }

    template < uiw count > void Assign( const X (&source)[ count ] )
    {
        this->Assign < false >( source, count );
    }

    template < bool is_checkOverlap = true > void Assign( const ownType &source, count_type start = 0, count_type count = count_type_max )
    {
        ASSUME( start <= source.Size() || count == 0 );
        count = Funcs::Min( source.Size() - start, count );
        if( is_checkOverlap && this == &source )  //  overlapped
        {
            AssignOverlappedArray( count, start );
        }
        else
        {
            _Destroy( this->_GetArr(), this->_Size() );
            _SizeToUnknown( 0, count );
            _Copy < false, false >( this->_GetArr(), (X *)source.Data() + start, count );
        }
    }

    template < typename IterType, bool is_checkOverlap = true, typename = typename EnableIf< IsDerivedFrom < IterType, Iterator::_TypeIterator >::value >::type >
    void Assign( IterType begin, IterType end )
    {
        uiw dist = Algorithm::Distance( begin, end );
        if( IterType::iteratorType == Iterator::Type::Random )
        {
            Assign < is_checkOverlap >( begin.Ptr(), dist );
        }
        else  //  can't be overlapped because CVec iterator is random
        {
            _Destroy( this->_GetArr(), this->_Size() );
            _SizeToUnknown( 0, dist );
            X *target = this->_GetArr();
            for( ; begin != end; ++begin, ++target )
            {
                new (target) X( *begin );
            }
        }
    }

    void Assign( std::initializer_list < X > ilist )
    {
        Assign < false >( ilist.begin(), ilist.size() );
    }

    void Clear()
    {
        _Destroy( this->_GetArr(), this->_Size() );
        _SizeDown( 0, 0 );
    }
};

}  //  namespace Private

template < typename X, typename reservator = Reservator::Half <>, uiw static_size = 0, TypeSemantic_t typeSemantic = Sem_Strict, typename allocator = Allocator::Simple > class CVec;

template < typename X > class CRefVec : public Private::_CBaseVecStatic < X, void, void, 0 >
{
    typedef Private::_CBaseVecStatic < X, void, void, 0 > baseType;
    typedef Private::_CBasisVec < X, void, void, 0 > arrType;

public:
    typedef typename baseType::count_type count_type;

    CRefVec()
    {}

    CRefVec( X *source, count_type size )
    {
        this->_SetArr( source, size );
    }

    template < uiw count > CRefVec( X (&source)[ count ] )
    {
        this->_SetArr( source, count );
    }

    CRefVec &operator = ( const CRefVec < X > &source )  //  TODO: wot
    {
        if( this != &source )
        {
            this->~CRefVec();
            new (this) CRefVec( (X *)source.Data(), source.Size() );
        }
        return *this;
    }
};

template < typename X > class CRefVec < const X > : public Private::_CBaseVecConstStatic < const X, void, void, 0 >
{
    typedef Private::_CBaseVecConstStatic < X, void, void, 0 > baseType;
    typedef Private::_CBasisVec < X, void, void, 0 > arrType;

public:
    typedef typename baseType::count_type count_type;

    CRefVec()
    {}

    CRefVec( const X *source, count_type size )
    {
        this->_SetArr( (X *)source, size );
    }

    CRefVec( const CRefVec < const X > &source )
    {
        this->_SetArr( (X *)source.Data(), source.Size() );
    }

    CRefVec( const CRefVec < X > &source )
    {
        this->_SetArr( (X *)source.Data(), source.Size() );
    }

    template < uiw count > CRefVec( const X (&source)[ count ] )
    {
        this->_SetArr( (X *)source, count );
    }

    CRefVec &operator = ( const CRefVec < const X > &source )
    {
        if( this != &source )
        {
            this->_SetArr( (X *)source.Data(), source.Size() );
        }
        return *this;
    }

    CRefVec &operator = ( const CRefVec < X > &source )
    {
        this->_SetArr( (X *)source.Data(), source.Size() );
        return *this;
    }
};

template < typename X, typename reservator, uiw static_size, TypeSemantic_t typeSemantic, typename allocator > class CVec : public Private::_CBaseVec < X, reservator, allocator, typeSemantic, static_size >
{
    typedef Private::_CBaseVec < X, reservator, allocator, typeSemantic, static_size > baseType;

public:
    typedef typename baseType::count_type count_type;

    explicit CVec( count_type reserve = 0 ) : baseType( reserve )
    {}
    
    CVec( count_type reserve, count_type size ) : baseType( size, reserve )
    {}

    CVec( count_type reserve, const X *source, count_type size ) : baseType( source, size, reserve )
    {}

    template < typename IterType, typename = typename EnableIf< IsDerivedFrom < IterType, Iterator::_TypeIterator >::value >::type > 
    CVec( count_type reserve, IterType begin, IterType end ) : baseType( begin, end, reserve )
    {}

    CVec( const CVec &source ) : baseType( source )
    {}

    template < uiw count > CVec( const X (&source)[ count ] ) : baseType( source, count, count )
    {}

    CVec( const CRefVec < X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
    {}

    CVec( const CRefVec < const X > &source ) : baseType( source.Data(), source.Size(), source.Size() )
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
    
    CVec &operator = ( const CRefVec < const X > &source )
    {
        this->Assign( source.Data(), source.Size() );
        return *this;
    }

    template < uiw count > CVec &operator = ( const X (&source)[ count ] )
    {
        this->Assign( source, count );
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

    CVec( CVec &&source ) NOEXCEPT = default;
    CVec &operator = ( CVec &&source ) NOEXCEPT = default;

    CVec( std::initializer_list < X > ilist ) : baseType( ilist )
    {}

    CVec &operator = ( std::initializer_list < X > ilist )
    {
        baseType::Assign( ilist );
        return *this;
    }

    CRefVec < X > ToRef()
    {
        return CRefVec < X >( this->Data(), this->Size() );
    }

    CRefVec < const X > ToRef() const
    {
        typedef Private::_CBaseVecConstStatic < X, reservator, allocator, static_size > constBaseType;
        return CRefVec < const X >( constBaseType::Data(), constBaseType::Size() );
    }

    CRefVec < const X > ToCRef() const
    {
        typedef Private::_CBaseVecConstStatic < X, reservator, allocator, static_size > constBaseType;
        return CRefVec < const X >( constBaseType::Data(), constBaseType::Size() );
    }

    operator CRefVec < X >()
    {
        return ToRef();
    }

    operator CRefVec < const X >() const
    {
        return ToCRef();
    }
};

//  there's no const version because there's no reason to have it. if you want const, use CCVecRef. I might as well create an aliased type, but I might not
//  unlike CVecRef, you can actually modify array's content, but use it on your own risk! there's no protection from destructing
//  uninitialized data, or from rewriting initialized data without destructing it first
template < typename X, TypeSemantic_t typeSemantic = Sem_Strict > class CVecArr : public Private::_CBaseVec < X, void, void, typeSemantic, 0 >
{
	typedef Private::_CBaseVec < X, void, void, typeSemantic, 0 > baseType;

public:
	typedef typename baseType::count_type count_type;

private:
	void Reserve( count_type size );  //  overriding base method so it can't be used

public:
	~CVecArr()
	{
		this->_SetArr( 0, 0, 0 );
	}

	CVecArr()
	{}

	CVecArr( X *arr, count_type initialSize = 0, count_type maxSize = TypeDesc < count_type >::max )
	{
		this->_SetArr( arr, initialSize, maxSize );
	}

	void Set( X *arr, count_type initialSize = 0, count_type maxSize = TypeDesc < count_type >::max )
	{
		this->_SetArr( arr, initialSize, maxSize );
	}
};

template < typename X > CRefVec < const X > MakeRefVec( const X *source, uiw count )
{
    return CRefVec < const X >( source, count );
}

template < typename X > CRefVec < X > MakeRefVec( X *source, uiw count )
{
    return CRefVec < X >( source, count );
}

template < typename X, uiw count > CRefVec < const X > MakeRefVec( const X (&source)[ count ] )
{
    return CRefVec < const X >( source, count );
}

template < typename X, uiw count > CRefVec < X > MakeRefVec( X (&source)[ count ] )
{
    return CRefVec < X >( source, count );
}

}  //  namespace StdLib

#endif