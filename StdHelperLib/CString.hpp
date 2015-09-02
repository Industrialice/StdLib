#ifndef __CSTRING_HPP__
#define __CSTRING_HPP__

#include <StdCoreLib.hpp>
#include "Reservators.hpp"
#include "Allocators.hpp"
#include "Iterator.hpp"

#define _MemCpyStr( a, b, c, d ) memcpy( a, b, (c) * d )
#define _MemMoveStr( a, b, c, d ) memmove( a, b, (c) * d )

namespace StdLib
{
const uiw StringDefReserve = 16;

template < typename charType, uiw basicSize = StringDefReserve / sizeof(charType), typename reservator = Reservator::Half <>, typename allocator = Allocator::Simple > class TCStr
{
    typedef TCStr < charType, basicSize, reservator, allocator > ownType;

	static __forceinline i32 StringCompare( const char *first, const char *second )
	{
		return strcmp( first, second );
	}

	static __forceinline i32 StringCompare( const wchar_t *first, const wchar_t *second )
	{
		return wcscmp( first, second );
	}

	template < typename type >
	static __forceinline i32 StringCompare( const type *first, const type *second )
	{
		for( ; *first == *second && *first; ++first, ++second );
		if( *first == *second )
		{
			return 0;
		}
		if( *first < *second )
		{
			return -1;
		}
		return 1;
	}

	template < typename type >
	static __forceinline uiw GetStringLength( const type *str )
	{
		if( sizeof( type ) == 1 )
		{
			return strlen( (char *)str );
		}
		if( sizeof( type ) == 2 )
		{
			return wcslen( (wchar_t *)str );
		}
		uiw len = 0;
		for( ; str[ len ]; ++len );
		return len;
	}

	static __forceinline bool IsStringEquals( const char *first, const char *second )
	{
		return strcmp( first, second ) == 0;
	}

	static __forceinline bool IsStringEquals( const wchar_t *first, const wchar_t *second )
	{
		return wcscmp( first, second ) == 0;
	}

	template < typename type >
	static __forceinline bool IsStringEquals( const type *first, const type *second )
	{
		for( ; *first == *second && *first; ++first, ++second );
		return *first == *second;
	}

	static __forceinline bool IsStrInRange( const charType *str, const charType *lower, const charType *upper )
	{
		return !(str < lower || str >= upper);
	}

    struct SDymanicFmt
    {
        charType *pointer;
        uiw reserved;
        charType is_staticOrDynamic;
    };

    static const uiw dynamic_size = sizeof(SDymanicFmt) / sizeof(charType);
    static const uiw static_size = dynamic_size > basicSize ? dynamic_size : basicSize;
    static const uiw static_last = static_size - 1;

    union
    {
        charType _static_str[ static_size ];  //  if last byte is zero - static storage is used, nonzero - dynamic
        struct
        {
            charType *_dynamic_str;
            uiw _reserved;  //  without zero symbol
        };
    };
    uiw _count;  //  without zero symbol

    void SetStatic()
    {
        _static_str[ static_last ] = (charType)0;
    }

    void SetDynamic()
    {
        _static_str[ static_last ] = (charType)0x1;
    }

    void SetDynamic( uiw reserve )
    {
        _static_str[ static_last ] = (charType)0x1;
		_reserved = reserve;
		_dynamic_str = allocator::Alloc < charType >( _reserved + 1 );
    }

    void _ProcReservationUp( uiw newCount )
    {
        ASSUME( IsDynamic() && _reserved >= _count );
        if( reservator::Up( newCount, &_reserved ) )
        {
            _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
        }
    }

    void _ProcReservationDown( uiw newCount )
    {
        ASSUME( IsDynamic() );
        if( reservator::Down( newCount, &_reserved ) )
        {
            _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
        }
    }

	template < bool isCanBeAliased >
    NOINLINE void AddString( const charType *str, uiw len )
    {
		charType *targetStr;
		charType temp[ static_last ];

        if( IsDynamic() )
        {
			bool isAliased = IsStrInRange( str, _dynamic_str, _dynamic_str + _count );
			const charType *curStr = _dynamic_str;
            _ProcReservationUp( _count + len );
			if( isCanBeAliased && isAliased )
			{
				str += _dynamic_str - curStr;
			}
			targetStr = _dynamic_str;
        }
        else
        {
            if( _count + len >= static_size )
            {
                uiw reserved = _count + len;
                charType *newStr = allocator::Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, _count, sizeof(charType) );
				if( isCanBeAliased && IsStrInRange( str, _static_str, _static_str + _count ) )
				{
					ASSUME( len <= static_last );
					_MemCpyStr( temp, str, len, sizeof(charType) );
					str = temp;
				}
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
				targetStr = newStr;
            }
            else
            {
				targetStr = _static_str;
            }
        }

		_MemCpyStr( targetStr + _count, str, len, sizeof(charType) );
        _count += len;
		targetStr[ _count ] = (charType)0;
    }

	/*  space will be left uninitialized  */
    NOINLINE charType *AddSpace( uiw len )
    {
		charType *thisStr;

        if( IsDynamic() )
        {
            _ProcReservationUp( _count + len );
			thisStr = _dynamic_str;
        }
        else
        {
            if( _count + len >= static_size )
            {
                uiw reserved = _count + len;
                charType *newStr = allocator::Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, _count, sizeof(charType) );
                thisStr = newStr;
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
            }
            else
            {
                thisStr = _static_str;
            }
        }

		thisStr += _count;
        _count += len;
        thisStr[ len ] = (charType)0;
		return thisStr;
    }
	
	template < bool isCanBeAliased >
    NOINLINE void InsertString( uiw index, const charType *str, uiw len )
    {
		charType temp[ static_last ];
		charType *dynPtr;

        if( IsDynamic() )
        {
			if( isCanBeAliased )
			{
				if( IsStrInRange( str, _dynamic_str, _dynamic_str + _count ) )
				{
					dynPtr = allocator::Alloc < charType >( len );
					_MemCpyStr( dynPtr, str, len, sizeof(charType) );
					str = dynPtr;
				}
				else
				{
					dynPtr = 0;
				}
			}
            _ProcReservationUp( _count + len );
            _MemMoveStr( _dynamic_str + index + len, _dynamic_str + index, _count - index + 1, sizeof(charType) );
			_MemCpyStr( _dynamic_str + index, str, len, sizeof(charType) );
			if( isCanBeAliased )
			{
                allocator::Free( dynPtr );
			}
        }
        else
        {
            if( _count + len >= static_size )
            {
                uiw reserved = _count + len;
                charType *newStr = allocator::Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, index, sizeof(charType) );
				_MemCpyStr( newStr + index, str, len, sizeof(charType) );
                _MemCpyStr( newStr + index + len, _static_str + index, _count - index + 1, sizeof(charType) );
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
            }
            else
            {
				if( isCanBeAliased && IsStrInRange( str, _static_str, _static_str + _count ) )
				{
					ASSUME( len < static_size );
					_MemCpyStr( temp, str, len, sizeof(charType) );
					str = temp;
				}
				_MemMoveStr( _static_str + index + len, _static_str + index, _count - index + 1, sizeof(charType) );
				_MemCpyStr( _static_str + index, str, len, sizeof(charType) );
            }
        }

        _count += len;
    }

    NOINLINE charType *InsertSpace( uiw index, uiw len )
    {
		charType *thisStr;

        if( IsDynamic() )
        {
            _ProcReservationUp( _count + len );
            _MemMoveStr( _dynamic_str + index + len, _dynamic_str + index, _count - index + 1, sizeof(charType) );
			thisStr = _dynamic_str;
        }
        else
        {
            if( _count + len >= static_size )
            {
                uiw reserved = _count + len;
                charType *newStr = allocator::Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, index, sizeof(charType) );
                _MemCpyStr( newStr + index + len, _static_str + index, _count - index + 1, sizeof(charType) );
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
				thisStr = newStr;
            }
            else
            {
                _MemMoveStr( _static_str + index + len, _static_str + index, _count - index + 1, sizeof(charType) );
				thisStr = _static_str;
            }
        }

        _count += len;
		return thisStr + index;
    }

    NOINLINE void EraseSpace( uiw index, uiw count )
    {
        ASSUME( (count == 0 || (index < _count)) && count <= _count && index + count <= _count );
        charType *str = Str();
        _count -= count;
        _MemMoveStr( str + index, str + index + count, _count - index + 1, sizeof(charType) );
        if( IsDynamic() )
        {
            _ProcReservationDown( _count );
        }
    }

    charType *Str()
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

public:
    typedef Iterator::_IterRandom < charType, 1 > Iter;
    typedef Iterator::_IterRandomConst < charType, 1 > IterConst;
    typedef Iterator::_IterRandom < charType, -1 > IterRev;
    typedef Iterator::_IterRandomConst < charType, -1 > IterRevConst;

	static const uiw npos = SIZE_MAX;

	Iter Begin()
	{
		return Iter( Str() );
	}

	Iter End()
	{
		return Iter( Str() + _count );
	}

	IterConst Begin() const
	{
		return IterConst( CStr() );
	}

	IterConst CBegin() const
	{
		return IterConst( CStr() );
	}

	IterConst End() const
	{
		return IterConst( CStr() + _count );
	}

	IterConst CEnd() const
	{
		return IterConst( CStr() + _count );
	}

	IterRev RBegin()
	{
		return IterRev( Str() + _count - 1 );
	}

	IterRev REnd()
	{
		return IterRev( Str() - 1 );
	}

	IterRevConst RBegin() const
	{
		return IterRevConst( CStr() + _count - 1 );
	}

	IterRevConst CRBegin() const
	{
		return IterRevConst( CStr() + _count - 1 );
	}

	IterRevConst REnd() const
	{
		return IterRevConst( CStr() - 1 );
	}

	IterRevConst CREnd() const
	{
		return IterRevConst( CStr() - 1 );
	}

    ~TCStr()
    {
        if( IsDynamic() )
        {
            allocator::Free( _dynamic_str );
        }
    }

    TCStr()
    {
        _static_str[ 0 ] = (charType)0;
        SetStatic();
        _count = 0;
    }
	
    NOINLINE TCStr( uiw reserve )
    {
        if( reserve >= static_size )
        {
            SetDynamic( reserve );
            *_dynamic_str = (charType)0;
        }
        else
        {
            _static_str[ 0 ] = (charType)0;
            SetStatic();
        }
        _count = 0;
    }

    NOINLINE TCStr( const charType *str )
    {
        uiw len = GetStringLength( str );
        _count = len;
		charType *thisStr;
        if( len >= static_size )
        {
            SetDynamic( _count );
			thisStr = _dynamic_str;
        }
        else
        {
            SetStatic();
			thisStr = _static_str;
        }
        _MemCpyStr( thisStr, str, _count + 1, sizeof(charType) );
    }

    NOINLINE TCStr( const charType *str, uiw len )
    {
        _count = len;
		charType *thisStr;
        if( len >= static_size )
        {
            SetDynamic( _count );
			thisStr = _dynamic_str;
        }
        else
        {
            SetStatic();
			thisStr = _static_str;
        }
        _MemCpyStr( thisStr, str, _count, sizeof(charType) );
        thisStr[ _count ] = (charType)0;
    }

    NOINLINE TCStr( const charType *str0, uiw len0, const charType *str1, uiw len1 )
    {
        _count = len0 + len1;
		charType *thisStr;
        if( _count >= static_size )
        {
            SetDynamic( _count );
			thisStr = _dynamic_str;
        }
        else
        {
            SetStatic();
			thisStr = _static_str;
        }
        _MemCpyStr( thisStr, str0, len0, sizeof(charType) );
        _MemCpyStr( thisStr + len0, str1, len1, sizeof(charType) );
        thisStr[ _count ] = (charType)0;
    }

	template < typename InputIterator >
	NOINLINE TCStr( InputIterator i1, InputIterator i2 )
	{
		if( i1.iteratorType == Iterator::Type::Random )
		{
			uiw len = Iterator::_IterDist< InputIterator, InputIterator::iteratorType >::Dist( i1, i2 );
			_count = len;
			charType *thisStr;
			if( len >= static_size )
			{
				SetDynamic( _count );
				thisStr = _dynamic_str;
			}
			else
			{
				SetStatic();
				thisStr = _static_str;
			}
			_MemCpyStr( thisStr, &*i1, len, sizeof(charType) );
			thisStr[ _count ] = (charType)0;
		}
		else
		{
			_static_str[ 0 ] = (charType)0;
			SetStatic();
			_count = 0;
			for( ; i1 != i2; ++i1 )
			{
				AddString < false > ( &*i1, 1 );
			}
		}
	}

    NOINLINE TCStr( const ownType &source )
    {
        ASSUME( this != &source );
        _count = source._count;
        if( source.IsDynamic() )
        {
            SetDynamic( source._count );
			_MemCpyStr( _dynamic_str, source._dynamic_str, source._count + 1, sizeof(charType) );
        }
        else
        {
            SetStatic();
			_MemCpyStr( _static_str, source._static_str, source._count + 1, sizeof(charType) );
        }
    }

    NOINLINE TCStr( const ownType &source, uiw pos, uiw len = npos )
    {
        ASSUME( this != &source && pos <= source._count );
		uiw realLen = Funcs::Min< uiw > ( source._count - pos, len );
        _count = realLen;
		charType *thisStr;
        if( realLen >= static_size )
        {
            SetDynamic( realLen );
			thisStr = _dynamic_str;
        }
        else
        {
            SetStatic();
			thisStr = _static_str;
        }
        _MemCpyStr( thisStr, source.CStr() + pos, realLen, sizeof(charType) );
        thisStr[ realLen ] = (charType)0;
	}

    NOINLINE TCStr( uiw n, charType c )
	{
		charType *thisStr;
        if( n >= static_size )
        {
            SetDynamic( n );
			thisStr = _dynamic_str;
        }
        else
        {
            SetStatic();
			thisStr = _static_str;
        }
		for( _count = 0; _count < n; ++_count )
		{
			thisStr[ _count ] = c;
		}
        thisStr[ _count ] = (charType)0;
	}

    const charType *CStr() const
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

    const charType *Data() const
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

    charType *Data()
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

	void PushBack( charType c )
	{
		AddString < false > ( &c, 1 );
	}

	NOINLINE void PopBack()
	{
		ASSUME( _count );
		--_count;
		charType *str;
		if( IsDynamic() )
		{
			str = _dynamic_str;
			_ProcReservationDown( _count );
		}
		else
		{
			str = _static_str;
		}
		str[ _count ] = (charType)0;
	}

    uiw Length() const
    {
        return _count - 1;
    }

    uiw Size() const
    {
        return _count;
    }

	NOINLINE void Reserve( uiw reserve )
	{
		if( IsStatic() )
		{
			if( reserve >= static_size )
			{
				charType *dyn = allocator::Alloc < charType >( reserve + 1 );
				_MemCpyStr( dyn, _static_str, _count + 1, sizeof(charType) );
				SetDynamic();
				_dynamic_str = dyn;
				_reserved = reserve;
			}
		}
		else
		{
			_ProcReservationUp( reserve );
		}
	}

	void Resize( uiw n )
	{
		Resize( n, (charType)0 );
	}

	NOINLINE void Resize( uiw n, charType c )
	{
		if( n < _count )
		{
			_count = n;
			charType *str;
			if( IsDynamic() )
			{
				str = _dynamic_str;
				_ProcReservationDown( _count );
			}
			else
			{
				str = _static_str;
			}
			str[ _count ] = (charType)0;
		}
		else if( n > _count )
		{
			uiw diff = n - _count;
			uiw index = _count;
			AddSpace( diff );
			charType *str = Str();
			for( ; diff; --diff )
			{
				str[ index++ ] = c;
			}
		}
	}

	bool IsEmpty() const
	{
		return _count == 0;
	}

	uiw Reserved() const
	{
		return IsDynamic() ? _reserved : static_last;
	}

    bool IsStatic() const
    {
        return _static_str[ static_last ] == (charType)0;
    }

    bool IsDynamic() const
    {
        return _static_str[ static_last ] != (charType)0;
    }

    charType Back() const
    {
        ASSUME( _count );
        return IsDynamic() ? _dynamic_str[ _count - 1 ] : _static_str[ _count - 1 ];
    }

	ownType SubStr( uiw pos = 0, uiw len = npos ) const
	{
		ASSUME( pos <= _count );
		return TCStr( CStr() + pos, Funcs::Min< uiw > ( len, _count - pos ) );
	}

    NOINLINE void ShrinkToFit()
    {
        if( IsDynamic() )
        {
            if( _count < static_size )
            {
                _MemCpyStr( _static_str, _dynamic_str, _count + 1, sizeof(charType) );
                SetStatic();
            }
            else if( _reserved > _count )
            {
                _reserved = _count;
                _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
            }
        }
    }

	ownType &Insert( uiw pos, const ownType &str )
	{
		InsertString < true > ( pos, str.CStr(), str.length() );
		return *this;
	}

	ownType &Insert( uiw pos, const ownType &str, uiw subpos, uiw sublen )
	{
		ASSUME( subpos <= str._count );
		InsertString < true > ( pos, str.CStr() + subpos, Funcs::Min< uiw > ( str.length() - subpos, sublen ) );
		return *this;
	}

	ownType &Insert( uiw pos, const charType *s )
	{
        InsertString < true > ( pos, s, GetStringLength( s ) );
		return *this;
	}

	ownType &Insert( uiw pos, const charType *s, uiw n )
	{
        InsertString < true > ( pos, s, n );
		return *this;
	}

	ownType &Insert( uiw pos, uiw n, charType c )
	{
		charType *thisStr = InsertSpace( pos, n );
		for( ; n; --n )
		{
			*thisStr++ = c;
		}
		return *this;
	}
    
	void Insert( Iter p, uiw n, charType c )
	{
		uiw pos = &*p - Str();
		ASSUME( pos <= _count );
		charType *thisStr = InsertSpace( pos, n );
		for( ; n; --n )
		{
			*thisStr++ = c;
		}
	}

	Iter Insert( Iter p, charType c )
	{
		uiw pos = &*p - Str();
		ASSUME( pos <= _count );
		InsertString < false > ( pos, &c, 1 );
		return Iter( Str() + pos );
	}

	Iter Insert( Iter p, const charType *str )
	{
		uiw pos = &*p - Str();
		ASSUME( pos <= _count );
		InsertString < true > ( pos, str, GetStringLength( str ) );
		return Iter( Str() + pos );
	}

	Iter Insert( Iter p, const charType *str, uiw len )
	{
		uiw pos = &*p - Str();
		ASSUME( pos <= _count );
		InsertString < true > ( pos, str, len );
		return Iter( Str() + pos );
	}

	template <typename InputIterator>
    NOINLINE void Insert( Iter p, InputIterator first, InputIterator last )
	{
		charType *thisStr = Str();
		uiw pos = &*p - thisStr;
		ASSUME( pos <= _count );
		bool isOverlapped = thisStr <= &*first && thisStr + _count > &*first;
		if( isOverlapped )
		{
			//  TODO: proto
			ownType temp( thisStr, pos );
			temp.Append( first, last );
			temp.Append( thisStr + pos, _count - pos );
			*this = temp;
		}
		else
		{
			if( first.iteratorType == Iterator::Type::Random )
			{
				uiw len = Iterator::_IterDist< InputIterator, InputIterator::iteratorType >::Dist( first, last );
				charType *thisStr = InsertSpace( pos, len );
				_MemCpyStr( thisStr, &*first, len, sizeof(charType) );
			}
			else
			{
				for( ; first != last; ++first )
				{
					InsertString < false > ( pos, &*first, 1 );
				}
			}
		}
	}

	ownType &Erase( uiw pos = 0, uiw len = npos )
	{
        EraseSpace( pos, Funcs::Min< uiw > ( _count - pos, len ) );
		return *this;
	}

	Iter Erase( Iter p )
	{
		charType *thisStr = Str();
		uiw pos = &*p - thisStr;
		ASSUME( pos <= _count );
		EraseSpace( pos, 1 );
		ASSUME( thisStr == Str() );
		return Iter( thisStr + pos );
	}

	Iter Erase( Iter first, Iter last )
	{
		charType *thisStr = Str();
		uiw pos = &*first - thisStr;
		ASSUME( pos <= _count );
		uiw len = last - first;
		ASSUME( len + pos <= _count );
		EraseSpace( pos, len );
		ASSUME( thisStr == Str() );
		return Iter( thisStr + pos );
	}

	ownType &Replace( uiw pos, uiw len, const ownType &str )
	{
		ASSUME( pos <= _count && this != &str );
		EraseSpace( pos, Funcs::Min< uiw > ( len, _count - pos ) );
		InsertString < false > ( pos, str.CStr(), str._count );
		return *this;
	}

	ownType &Replace( Iter i1, Iter i2, const ownType &str )
	{
		ASSUME( this != &str );
		uiw pos = &*i1 - CStr();
		uiw len = i2 - i1;
		EraseSpace( pos, len );
		InsertString < false > ( pos, str.CStr(), str._count );
		return *this;
	}

	ownType &Replace( uiw pos, uiw len, const ownType &str, uiw subpos, uiw sublen )
	{
		ASSUME( pos <= _count && subpos <= str._count && this != &str );
		EraseSpace( pos, Funcs::Min< uiw > ( len, _count - pos ) );
		InsertString < false > ( pos, str.CStr() + subpos, Funcs::Min< uiw > ( str._count - subpos, sublen ) );
		return *this;
	}

	ownType &Replace( uiw pos, uiw len, const charType *s )
	{
		ASSUME( pos <= _count && (s < CStr() || s >= CStr() + _count) );
		EraseSpace( pos, Funcs::Min< uiw > ( len, _count - pos ) );
		InsertString < false > ( pos, s, GetStringLength( s ) );
		return *this;
	}

	ownType &Replace( Iter i1, Iter i2, const charType *s )
	{
		ASSUME( false && (s < CStr() || s >= CStr() + _count) );
		uiw pos = &*i1 - CStr();
		uiw len = i2 - i1;
		EraseSpace( pos, len );
		InsertString < false > ( pos, s, GetStringLength( s ) );
		return *this;
	}

	ownType &Replace( uiw pos, uiw len, const charType *s, uiw n )
	{
		ASSUME( pos <= _count && (s < CStr() || s >= CStr() + _count) );
		EraseSpace( pos, Funcs::Min< uiw > ( len, _count - pos ) );
        InsertString < false > ( pos, s, n );
		return *this;
	}

	ownType &Replace( Iter i1, Iter i2, const charType *s, uiw n )
	{
		uiw pos = &*i1 - CStr();
		uiw len = i2 - i1;
		EraseSpace( pos, len );
		InsertString < false > ( pos, s, n );
		return *this;
	}

	ownType &Replace( uiw pos, uiw len, uiw n, charType c )
	{
		ASSUME( pos <= _count );
		EraseSpace( pos, Funcs::Min< uiw > ( len, _count - pos ) );
		charType *thisStr = InsertSpace( pos, n );
		for( ; n; --n )
		{
			*thisStr++ = c;
		}
		return *this;
	}

	ownType &Replace( Iter i1, Iter i2, uiw n, charType c )
	{
		uiw pos = &*i1 - CStr();
		uiw len = i2 - i1;
		EraseSpace( pos, len );
		charType *thisStr = InsertSpace( pos, n );
		for( ; n; --n )
		{
			*thisStr++ = c;
		}
		return *this;
	}

	template <typename InputIterator>
	ownType &Replace( Iter i1, Iter i2, InputIterator first, InputIterator last )
	{
		ASSUME( &*first < CStr() || &*first >= CStr() + _count );
		uiw pos = &*i1 - CStr();
		uiw len = i2 - i1;
		EraseSpace( pos, len );
		if( first.iteratorType == Iterator::Type::Random )
		{
            uiw dist = Iterator::_IterDist< InputIterator, InputIterator::iteratorType >::Dist( first, last );
			InsertString < false > ( pos, &*first, dist );
		}
		else
		{
			for( ; first != last; ++first )
			{
				InsertString < false > ( pos++, &*first, 1 );
			}
		}
		return *this;
	}

	ownType &Append( const ownType &str )
	{
        AddString < true > ( str.CStr(), str._count );
		return *this;
	}

	ownType &Append( const ownType &str, uiw subpos, uiw sublen )
	{
		ASSUME( subpos <= str._count );
        AddString < true > ( str.CStr() + subpos, Funcs::Min< uiw > ( str._count - subpos, sublen ) );
		return *this;
	}

	ownType &Append( const charType *s )
	{
        AddString < true > ( s, GetStringLength( s ) );
		return *this;
	}

	ownType &Append( const charType *s, uiw n )
	{
        AddString < true > ( s, n );
		return *this;
	}

	ownType &Append( uiw n, charType c )
	{
		charType *str = AddSpace( n );
		for( ; n; --n )
		{
			*str++ = c;
		}
		return *this;
	}

	template <typename InputIterator>
    NOINLINE ownType &Append( InputIterator first, InputIterator last )
	{
		const charType *str = CStr();
		bool isOverlapped = !(&*first < str || &*first >= str + _count);
		if( isOverlapped )
		{
			/*  TODO: proto  */
			Append( ownType( first, last ) );
		}
		else
		{
			if( first.iteratorType == Iterator::Type::Random )
			{
                uiw dist = Iterator::_IterDist< InputIterator, InputIterator::iteratorType >::Dist( first, last );
				AddString < false > ( &*first, dist );
			}
			else
			{
				for( ; first != last; ++first )
				{
					AddString < false > ( &*first, 1 );
				}
			}
		}
		return *this;
	}
	
	NOINLINE ownType &Assign( const ownType &source )
	{
        if( this != &source )
        {
			bool isNeedDynamic = source._count > static_last;
			if( IsStatic() )
			{
				if( isNeedDynamic )
				{
					ASSUME( source.IsDynamic() );
					SetDynamic( source._count );
					_MemCpyStr( _dynamic_str, source._dynamic_str, source._count + 1, sizeof(charType) );
				}
				else
				{
					_MemCpyStr( _static_str, source.CStr(), source._count + 1, sizeof(charType) );
				}
			}
			else
			{
				_ProcReservationUp( source._count );
				_MemCpyStr( _dynamic_str, source.CStr(), source._count + 1, sizeof(charType) );
			}
			_count = source._count;
        }
        return *this;
	}

	NOINLINE ownType &Assign( const ownType &str, uiw subpos, uiw sublen )
	{
		ASSUME( subpos <= str._count );
		uiw realLen = Funcs::Min< uiw > ( str._count - subpos, sublen );
		if( this == &str )
		{
			charType *curStr = Str();
			ASSUME( curStr == str.CStr() );
			_MemMoveStr( curStr, curStr + subpos, realLen, sizeof(charType) );
			curStr[ realLen ] = (charType)0;
			_count = realLen;
			return *this;
		}
		else
		{
			return Assign( str.CStr() + subpos, realLen );
		}
	}

	ownType &Assign( const charType *s )
	{
		return Assign( s, GetStringLength( s ) );
	}

	NOINLINE ownType &Assign( const charType *s, uiw n )
	{
		const charType *curStr = CStr();
		bool isOverlapped = !(s < curStr || s >= curStr + _count);
		if( isOverlapped )  //  TODO: proto
		{
			*this = ownType( s, n );
			return *this;
		}

		bool isNeedDynamic = n > static_last;

		if( isNeedDynamic )
		{
			if( IsStatic() )
			{
				SetDynamic( n );
			}
			else
			{
				_ProcReservationUp( n );
			}
            _MemCpyStr( _dynamic_str, s, n, sizeof(charType) );
			_dynamic_str[ n ] = (charType)0;
		}
		else
		{
			charType *str = Str();
			_MemCpyStr( str, s, n, sizeof(charType) );
			str[ n ] = (charType)0;
		}

		_count = n;

		return *this;
	}

	ownType &Assign( uiw n, charType c )
	{
		Clear();
		charType *str = AddSpace( n );
		for( ; n; --n )
		{
			*str++ = c;
		}
		return *this;
	}

	template <class InputIterator>
	NOINLINE ownType &Assign( InputIterator first, InputIterator last )
	{
		const charType *str = CStr();
		bool isOverlapped = !(&*first < str || &*first >= str + _count);
		if( isOverlapped )
		{
			/*  TODO: proto  */
			Assign( ownType( first, last ) );
		}
		else
		{
			if( first.iteratorType == Iterator::Type::Random )
			{
				uiw len = Iterator::_IterDist< InputIterator, InputIterator::iteratorType >::Dist( first, last );
				if( IsStatic() )
				{
					charType *thisStr;
					if( len >= static_size )
					{
						SetDynamic( len );
						thisStr = _dynamic_str;
					}
					else
					{
						thisStr = _static_str;
					}
					_MemCpyStr( thisStr, &*first, len, sizeof(charType) );
					thisStr[ len ] = (charType)0;
				}
				else
				{
					_ProcReservationUp( len );
					_MemCpyStr( _dynamic_str, &*first, len, sizeof(charType) );
					_dynamic_str[ len ] = (charType)0;
				}
				_count = len;
			}
			else
			{
				Clear();
				for( ; first != last; ++first )
				{
					AddString < false > ( &*first, 1 );
				}
			}
		}
		return *this;
	}

    void Clear()
    {
        if( IsDynamic() )
        {
            _ProcReservationDown( 0 );
            _dynamic_str[ 0 ] = (charType)0;
        }
        else
        {
            _static_str[ 0 ] = (charType)0;
        }
        _count = 0;
    }

    void FullClear()
    {
        if( IsDynamic() )
        {
            allocator::Free( _dynamic_str );
            SetStatic();
        }
        _static_str[ 0 ] = (charType)0;
        _count = 0;
    }

    TCStr &operator = ( const ownType &source )
    {
		return Assign( source );
    }

    TCStr &operator = ( const charType *str )
    {
		return Assign( str );
    }

    NOINLINE TCStr &operator = ( const charType c )
    {
		ASSUME( static_last > 1 );
		if( IsDynamic() )
		{
			_ProcReservationDown( 1 );
		}
        charType *thisStr = Str();
		thisStr[ 0 ] = c;
		thisStr[ 1 ] = (charType)0;
		_count = 1;
		return *this;
    }

    TCStr operator + ( const ownType &source ) const
    {
        return TCStr( this->CStr(), this->_count, source.CStr(), source._count );
    }

    TCStr operator + ( const charType *str ) const
    {
        return TCStr( this->CStr(), this->_count, str, GetStringLength( str ) );
    }

    TCStr operator + ( charType symbol ) const
    {
        return TCStr( this->CStr(), this->_count, &symbol, 1 );
    }

    friend TCStr operator + ( const charType *str, const ownType &second )
    {
        return TCStr( str, GetStringLength( str ), second.CStr(), second._count );
    }

    friend TCStr operator + ( charType symbol, const ownType &second )
    {
        return TCStr( &symbol, 1, second.CStr(), second._count );
    }

    TCStr &operator += ( const ownType &source )
    {
        AddString < true > ( source.CStr(), source._count );
        return *this;
    }

    TCStr &operator += ( const charType *str )
    {
        AddString < true > ( str, GetStringLength( str ) );
        return *this;
    }

    TCStr &operator += ( charType symbol )
    {
        AddString < false > ( &symbol, 1 );
        return *this;
    }

	bool operator == ( const ownType &source ) const
	{
		if( _count == source._count )
		{
			return IsStringEquals( CStr(), source.CStr() );
		}
		return false;
	}

	bool operator == ( const charType *str ) const
	{
		return IsStringEquals( CStr(), str );
	}

	bool operator != ( const ownType &source ) const
	{
		return !this->operator==( source );
	}

	bool operator != ( const charType *str ) const
	{
		return !this->operator==( str );
	}

	bool operator < ( const ownType &source ) const
	{
		return StringCompare( CStr(), source.CStr() ) < 0;
	}

	bool operator < ( const charType *str ) const
	{
		return StringCompare( CStr(), str ) < 0;
	}

	bool operator > ( const ownType &source ) const
	{
		return StringCompare( CStr(), source.CStr() ) > 0;
	}

	bool operator > ( const charType *str ) const
	{
		return StringCompare( CStr(), str ) > 0;
	}

	charType& operator [] ( uiw index )
	{
		return Str()[ index ];
	}

	const charType& operator [] ( uiw index ) const
	{
		return CStr()[ index ];
	}
};

typedef TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple > CStr;
typedef TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple > CWStr;

}  //  StdLib

#undef _MemCpyStr
#undef _MemMoveStr

#endif
