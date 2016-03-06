#ifndef __CSTRING_HPP__
#define __CSTRING_HPP__

#include <wchar.h>

#include <StdCoreLib.hpp>
#include "Reservators.hpp"
#include "Allocators.hpp"
#include "Iterator.hpp"
#include "Algorithm.hpp"

#define _MemCpyStr( a, b, c ) memcpy( a, b, (c) * sizeof(charType) )
#define _MemMoveStr( a, b, c ) memmove( a, b, (c) * sizeof(charType) )

namespace StdLib
{
const uiw StringDefReserve = 16;

template < typename forCharType > struct _EmptyStr
{
    static FORCEINLINE const forCharType *Get()
    {
        static const forCharType ret[ 1 ] = {};
        return ret;
    }
};

template <> struct _EmptyStr < char >
{
    static FORCEINLINE const char *Get()
    {
        return "";
    }
};

template <> struct _EmptyStr < wchar_t >
{
    static FORCEINLINE const wchar_t *Get()
    {
        return L"";
    }
};

template < typename charType, uiw basicSize = StringDefReserve / sizeof(charType), typename reservator = Reservator::Half <>, typename allocator = Allocator::Simple > class TCStr : CharMovable
{
    typedef TCStr < charType, basicSize, reservator, allocator > ownType;

    static FORCEINLINE i32 StringCompare( const char *first, const char *second )
    {
        ASSUME( first && second );
        return strcmp( first, second );
    }

    static FORCEINLINE i32 StringCompare( const wchar_t *first, const wchar_t *second )
    {
        ASSUME( first && second );
        return wcscmp( first, second );
    }

    template < typename type > static FORCEINLINE i32 StringCompare( const type *first, const type *second )
    {
        ASSUME( first && second );
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

    template < typename type > static FORCEINLINE uiw GetStringLength( const type *str )
    {
        ASSUME( str );
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

    static FORCEINLINE bln IsStringEquals( const char *first, const char *second )
    {
        ASSUME( first && second );
        return strcmp( first, second ) == 0;
    }

    static FORCEINLINE bln IsStringEquals( const wchar_t *first, const wchar_t *second )
    {
        ASSUME( first && second );
        return wcscmp( first, second ) == 0;
    }

    template < typename type > static FORCEINLINE bln IsStringEquals( const type *first, const type *second )
    {
        ASSUME( first && second );
        for( ; *first == *second && *first; ++first, ++second );
        return *first == *second;
    }

    static FORCEINLINE bln IsStrInRange( const charType *str, const charType *lower, const charType *upper )
    {
        ASSUME( str );
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
        _dynamic_str = allocator::template Alloc < charType >( _reserved + 1 );
    }

    void _ProcReservationUp( uiw newCount )
    {
        ASSUME( IsDynamic() && _reserved >= _count );
        if( reservator::Up( newCount, &_reserved ) )
        {
            _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
        }
    }

    bln _ProcReservationDown( uiw newCount )
    {
        ASSUME( IsDynamic() );
        if( reservator::Down( newCount, &_reserved ) )
        {
            _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
            return true;
        }
        return false;
    }

    template < bln isCanBeAliased > NOINLINE void AddString( const charType *str, uiw len )
    {
        ASSUME( str != 0 || len == 0 );

        charType *targetStr;
        charType temp[ static_last ];

        if( IsDynamic() )
        {
            bln isAliased = IsStrInRange( str, _dynamic_str, _dynamic_str + _count );
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
                charType *newStr = allocator::template Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, _count );
                if( isCanBeAliased && IsStrInRange( str, _static_str, _static_str + _count ) )
                {
                    ASSUME( len <= static_last );
                    _MemCpyStr( temp, str, len );
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

        _MemCpyStr( targetStr + _count, str, len );
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
                charType *newStr = allocator::template Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, _count );
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

        _count += len;
        thisStr[ _count ] = (charType)0;
        return thisStr;
    }

    template < bln isCanBeAliased > NOINLINE void InsertString( uiw index, const charType *str, uiw len )
    {
        ASSUME( str != 0 || len == 0 );

        charType temp[ static_last ];
        charType *dynPtr;

        if( IsDynamic() )
        {
            if( isCanBeAliased )
            {
                if( IsStrInRange( str, _dynamic_str, _dynamic_str + _count ) )
                {
                    dynPtr = allocator::template Alloc < charType >( len );
                    _MemCpyStr( dynPtr, str, len );
                    str = dynPtr;
                }
                else
                {
                    dynPtr = 0;
                }
            }
            _ProcReservationUp( _count + len );
            _MemMoveStr( _dynamic_str + index + len, _dynamic_str + index, _count - index + 1 );
            _MemCpyStr( _dynamic_str + index, str, len );
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
                charType *newStr = allocator::template Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, index );
                _MemCpyStr( newStr + index, str, len );
                _MemCpyStr( newStr + index + len, _static_str + index, _count - index + 1 );
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
            }
            else
            {
                if( isCanBeAliased && IsStrInRange( str, _static_str, _static_str + _count ) )
                {
                    ASSUME( len < static_size );
                    _MemCpyStr( temp, str, len );
                    str = temp;
                }
                _MemMoveStr( _static_str + index + len, _static_str + index, _count - index + 1 );
                _MemCpyStr( _static_str + index, str, len );
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
            _MemMoveStr( _dynamic_str + index + len, _dynamic_str + index, _count - index + 1 );
            thisStr = _dynamic_str;
        }
        else
        {
            if( _count + len >= static_size )
            {
                uiw reserved = _count + len;
                charType *newStr = allocator::template Alloc < charType >( reserved + 1 );
                _MemCpyStr( newStr, _static_str, index );
                _MemCpyStr( newStr + index + len, _static_str + index, _count - index + 1 );
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
                thisStr = newStr;
            }
            else
            {
                _MemMoveStr( _static_str + index + len, _static_str + index, _count - index + 1 );
                thisStr = _static_str;
            }
        }

        _count += len;
        return thisStr;
    }

    NOINLINE charType *EraseSpace( uiw index, uiw count )
    {
        ASSUME( (count == 0 || (index < _count)) && count <= _count && index + count <= _count );
        charType *str = Str();
        _count -= count;
        _MemMoveStr( str + index, str + index + count, _count - index + 1 );
        if( IsDynamic() )
        {
            if( _ProcReservationDown( _count ) )
            {
                return Str();
            }
        }
        return str;
    }

    NOINLINE charType *EraseAndInsertSpace( const uiw index, uiw replaceCount, const uiw len )
    {
        ASSUME( (replaceCount == 0 || (index < _count)) && replaceCount <= _count && index + replaceCount <= _count );
        charType *thisStr = Str();
        if( replaceCount != len )
        {
            if( replaceCount > len )  //  deleting more than copying, shifting string's part to the left
            {
                thisStr = EraseSpace( index + len, replaceCount - len );
            }
            else  //  if( replaceCount < len ) copying more than deleting, shifting string's part to the right if needed
            {
                if( index + replaceCount > _count )  //  shifting is not needed, string space is exeeded
                {
                    thisStr = AddSpace( len - replaceCount );
                }
                else
                {
                    uiw diff = len - replaceCount;
                    thisStr = InsertSpace( index + replaceCount, diff );
                }
            }
        }
        return thisStr;
    }

    void EraseAndInsert( const uiw index, uiw replaceCount, const charType *const str, const uiw len )
    {
        ASSUME( str != 0 || len == 0 );
        ASSUME( (replaceCount == 0 || (index < _count)) && replaceCount <= _count && index + replaceCount <= _count );
        charType *thisStr = EraseAndInsertSpace( index, replaceCount, len );
        _MemCpyStr( thisStr + index, str, len );
        thisStr[ _count ] = (charType)0;
    }

public:
    typedef Iterator::_IterRandom < charType, 1 > Iter;
    typedef Iterator::_IterRandomConst < charType, 1 > IterConst;
    typedef Iterator::_IterRandom < charType, -1 > IterRev;
    typedef Iterator::_IterRandomConst < charType, -1 > IterRevConst;

    Iter begin()
    {
        return Iter( Str() );
    }

    IterConst begin() const
    {
        return IterConst( CStr() );
    }

    IterRev beginRev()
    {
        return IterRev( Str() + _count - 1 );
    }

    IterRevConst beginRev() const
    {
        return IterRevConst( Str() + _count - 1 );
    }

    IterConst cbegin() const
    {
        return IterConst( CStr() );
    }

    IterRevConst cbeginRev() const
    {
        return IterRevConst( CStr() + _count - 1 );
    }

    Iter end()
    {
        return Iter( Str() + _count );
    }

    IterConst end() const
    {
        return IterConst( CStr() + _count );
    }

    IterRev endRev()
    {
        return IterRev( Str() - 1 );
    }

    IterRevConst endRev() const
    {
        return IterRevConst( Str() - 1 );
    }

    IterConst cend() const
    {
        return IterConst( CStr() + _count );
    }

    IterRevConst cendRev() const
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
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }

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
        _MemCpyStr( thisStr, str, _count + 1 );
    }

    NOINLINE TCStr( const charType *str, uiw len )
    {
        ASSUME( str != 0 || len == 0 );

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
        _MemCpyStr( thisStr, str, _count );
        thisStr[ _count ] = (charType)0;
    }

    NOINLINE TCStr( const charType *str0, uiw len0, const charType *str1, uiw len1 )
    {
        ASSUME( str0 != 0 || len0 == 0 );
        ASSUME( str1 != 0 || len1 == 0 );

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
        _MemCpyStr( thisStr, str0, len0 );
        _MemCpyStr( thisStr + len0, str1, len1 );
        thisStr[ _count ] = (charType)0;
    }

    template < typename InputIterator > NOINLINE TCStr( InputIterator begin, InputIterator end )
    {
        if( begin.iteratorType == Iterator::Type::Random )
        {
            uiw len = Iterator::_IterDist < InputIterator, InputIterator::iteratorType >::Dist( begin, end );
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
            _MemCpyStr( thisStr, begin.Ptr(), len );
            thisStr[ _count ] = (charType)0;
        }
        else
        {
            _static_str[ 0 ] = (charType)0;
            SetStatic();
            _count = 0;
            for( ; begin != end; ++begin )
            {
                AddString < false >( begin.Ptr(), 1 );
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
            _MemCpyStr( _dynamic_str, source._dynamic_str, source._count + 1 );
        }
        else
        {
            SetStatic();
            _MemCpyStr( _static_str, source._static_str, source._count + 1 );
        }
    }

    NOINLINE TCStr( const ownType &source, uiw pos, uiw len = uiw_max )
    {
        ASSUME( this != &source && pos <= source._count );
        uiw realLen = Funcs::Min < uiw >( source._count - pos, len );
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
        _MemCpyStr( thisStr, source.CStr() + pos, realLen );
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

	charType *Str()
	{
		return IsStatic() ? _static_str : _dynamic_str;
	}

	const charType *Str() const
	{
		return IsStatic() ? _static_str : _dynamic_str;
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

    charType Back() const
    {
        ASSUME( _count );
        return CStr()[ _count - 1 ];
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

    //  without null-terminator
    uiw Size() const
    {
        return _count;
    }

    NOINLINE void Reserve( uiw reserve )
    {
        if( reserve > _reserved )
        {
            if( IsStatic() )
            {
                if( reserve >= static_size )
                {
                    charType *dyn = allocator::template Alloc < charType >( reserve + 1 );
                    _MemCpyStr( dyn, _static_str, _count + 1 );
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
        else if( reserve < _reserved )
        {
            if( IsDynamic() )
            {
                _dynamic_str = allocator::Realloc( _dynamic_str, reserve + 1 );
                _reserved = reserve;
            }
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

    bln IsEmpty() const
    {
        return _count == 0;
    }

    uiw Reserved() const
    {
        return IsDynamic() ? _reserved : static_last;
    }

    bln IsStatic() const
    {
        return _static_str[ static_last ] == (charType)0;
    }

    bln IsDynamic() const
    {
        return _static_str[ static_last ] != (charType)0;
    }

    ownType SubStr( uiw pos = 0, uiw len = uiw_max ) const
    {
        ASSUME( pos <= _count );
        return TCStr( CStr() + pos, Funcs::Min < uiw >( len, _count - pos ) );
    }

    NOINLINE void ShrinkToFit()
    {
        if( IsDynamic() )
        {
            if( _count < static_size )
            {
                _MemCpyStr( _static_str, _dynamic_str, _count + 1 );
                SetStatic();
            }
            else if( _reserved > _count )
            {
                _reserved = _count;
                _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
            }
        }
    }

    ownType &Insert( const ownType &str, uiw pos )
    {
        InsertString < true >( pos, str.CStr(), str.Size() );
        return *this;
    }

    ownType &Insert( const ownType &str, uiw pos, uiw subpos, uiw sublen )
    {
        ASSUME( subpos <= str._count );
        InsertString < true >( pos, str.CStr() + subpos, Funcs::Min < uiw >( str.Size() - subpos, sublen ) );
        return *this;
    }

    ownType &Insert( const charType *s, uiw pos )
    {
        if( s == 0 )
        {
            return *this;
        }
        InsertString < true >( pos, s, GetStringLength( s ) );
        return *this;
    }

    ownType &Insert( const charType *s, uiw pos, uiw n = uiw_max )
    {
        if( s == 0 )
        {
            ASSUME( n == 0 || n == uiw_max );
        }
        else
        {
            if( n == uiw_max )
            {
                n = GetStringLength( s );
            }
            InsertString < true >( pos, s, n );
        }
        return *this;
    }

    ownType &Insert( charType c, uiw pos, uiw n = 1 )
    {
        charType *thisStr = InsertSpace( pos, n ) + pos;
        for( ; n; --n )
        {
            *thisStr++ = c;
        }
        return *this;
    }

    void Insert( charType c, IterConst where, uiw n = 1 )
    {
        uiw pos = where.Ptr() - Str();
        ASSUME( pos <= _count );
        charType *thisStr = InsertSpace( pos, n ) + pos;
        for( ; n; --n )
        {
            *thisStr++ = c;
        }
    }

    Iter Insert( charType c, IterConst where )
    {
        uiw pos = where.Ptr() - Str();
        ASSUME( pos <= _count );
        InsertString < false >( pos, &c, 1 );
        return Iter( Str() + pos );
    }

    Iter Insert( const charType *str, IterConst where, uiw n = uiw_max )
    {
		uiw pos = where.Ptr() - Str();
        if( str == 0 )
        {
            ASSUME( n == 0 || n == uiw_max );
        }
        else
        {
            ASSUME( pos <= _count );
            if( n == uiw_max )
            {
                n = GetStringLength( str );
            }
            InsertString < true >( pos, str, n );
        }
        return Iter( Str() + pos );
    }

    template < typename InputIterator > NOINLINE void Insert( IterConst where, InputIterator first, InputIterator last )
    {
        charType *thisStr = Str();
        uiw pos = where.Ptr() - thisStr;
        ASSUME( pos <= _count );
        bln isOverlapped = thisStr <= first.Ptr() && thisStr + _count > first.Ptr();
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
                uiw len = Iterator::_IterDist < InputIterator, InputIterator::iteratorType >::Dist( first, last );
                thisStr = InsertSpace( pos, len ) + pos;
                _MemCpyStr( thisStr, first.Ptr(), len );
            }
            else
            {
                for( ; first != last; ++first )
                {
                    InsertString < false >( pos, first.Ptr(), 1 );
                }
            }
        }
    }

    ownType &Erase( uiw pos, uiw len = uiw_max )
    {
        EraseSpace( pos, Funcs::Min < uiw >( _count - pos, len ) );
        return *this;
    }

    Iter Erase( IterConst where )
    {
        charType *thisStr = Str();
        uiw pos = where.Ptr() - thisStr;
        ASSUME( pos <= _count );
        EraseSpace( pos, 1 );
        ASSUME( thisStr == Str() );
        return Iter( thisStr + pos );
    }

    Iter Erase( IterConst begin, IterConst end )
    {
        ASSUME( end >= begin );
        charType *thisStr = Str();
        uiw pos = begin.Ptr() - thisStr;
        ASSUME( pos <= _count );
        uiw len = end - begin;
        ASSUME( len + pos <= _count );
        EraseSpace( pos, len );
        ASSUME( thisStr == Str() );
        return Iter( thisStr + pos );
    }

    ownType &Replace( const ownType &str, uiw replacePos, uiw replaceLen = 0 )
    {
        ASSUME( replacePos <= _count && this != &str );
        if( this == &str )
        {
            ownType temp( str );
            EraseAndInsert( replacePos, Funcs::Min < uiw >( replaceLen, _count - replacePos ), temp.CStr(), temp._count );
        }
        else
        {
            EraseAndInsert( replacePos, Funcs::Min < uiw >( replaceLen, _count - replacePos ), str.CStr(), str._count );
        }
        return *this;
    }

    ownType &Replace( const ownType &str, IterConst replaceBegin, IterConst replaceEnd )
    {
        ASSUME( replaceEnd >= replaceBegin );
        const ownType &temp = this == &str ? ownType( str ) : str;
        uiw pos = replaceBegin.Ptr() - CStr();
        uiw len = replaceEnd - replaceBegin;
        EraseAndInsert( pos, len, temp.CStr(), temp._count );
        return *this;
    }

    ownType &Replace( const ownType &str, uiw replacePos, uiw replaceLen, uiw subpos = 0, uiw sublen = uiw_max )
    {
        ASSUME( replacePos <= _count && subpos <= str._count );
        if( this == &str )
        {
            ownType temp( str, subpos, sublen );
            EraseAndInsert( replacePos, Funcs::Min < uiw >( replaceLen, _count - replacePos ), temp.CStr(), temp._count );
        }
        else
        {
            EraseAndInsert( replacePos, Funcs::Min < uiw >( replaceLen, _count - replacePos ), str.CStr() + subpos, Funcs::Min < uiw >( str._count - subpos, sublen ) );
        }
        return *this;
    }

    ownType &Replace( const charType *s, IterConst begin, IterConst end, uiw len = uiw_max )  //  TODO:
    {
        if( s == 0 )
        {
            ASSUME( len == 0 || len == uiw_max );
        }
        else
        {
            ASSUME( end >= begin );
            if( len == uiw_max )
            {
                len = GetStringLength( s );
            }
            uiw pos = begin.Ptr() - CStr();
            ASSUME( pos < _count );
            uiw diff = begin - end;
            EraseAndInsert( pos, diff, s, len );
        }
        return *this;
    }

    ownType &Replace( const charType *s, uiw replacePos, uiw replaceLen = 0, uiw strLen = uiw_max )  //  TODO: overlapping
    {
        if( s == 0 )
        {
            ASSUME( strLen == 0 || strLen == uiw_max );
        }
        else
        {
            ASSUME( replacePos <= _count );
            if( strLen == uiw_max )
            {
                strLen = GetStringLength( s );
            }
            replaceLen = Funcs::Min < uiw >( replaceLen, _count - replacePos );
            EraseAndInsert( replacePos, replaceLen, s, strLen );
        }
        return *this;
    }

    ownType &Replace( charType c, uiw pos, uiw len, uiw n = 1 )
    {
        ASSUME( pos <= _count );
        EraseSpace( pos, Funcs::Min < uiw >( len, _count - pos ) );
        charType *thisStr = InsertSpace( pos, n ) + pos;
        for( ; n; --n )
        {
            *thisStr++ = c;
        }
        return *this;
    }

    ownType &Replace( charType c, IterConst begin, IterConst end, uiw n = 1 )
    {
        ASSUME( end >= begin );
        uiw pos = begin.Ptr() - CStr();
        uiw len = end - begin;
        EraseSpace( pos, len );
        charType *thisStr = InsertSpace( pos, n ) + pos;
        for( ; n; --n )
        {
            *thisStr++ = c;
        }
        return *this;
    }

    template < typename InputIterator > ownType &Replace( IterConst begin, IterConst end, InputIterator first, InputIterator last )  //  TODO: overlapping
    {
        ASSUME( end >= begin );
        uiw pos = begin.Ptr() - CStr();
        uiw len = end - begin;
        uiw dist = Algorithm::Distance( first, last );
        if( first.iteratorType == Iterator::Type::Random )
        {
            EraseAndInsert( pos, len, first.Ptr(), dist );
        }
        else
        {
            charType *thisStr = EraseAndInsertSpace( pos, len, dist );
            charType *str = thisStr + pos;
            for( ; first != last; ++first )
            {
                *str = *first;
                ++str;
            }
            thisStr[ _count ] = (charType)0;
        }
        return *this;
    }

    ownType &Append( const ownType &str )
    {
        AddString < true >( str.CStr(), str._count );
        return *this;
    }

    ownType &Append( const ownType &str, uiw subpos, uiw sublen )
    {
        ASSUME( subpos <= str._count );
        AddString < true >( str.CStr() + subpos, Funcs::Min < uiw >( str._count - subpos, sublen ) );
        return *this;
    }

    ownType &Append( const charType *s )
    {
        if( s != 0 )
        {
            AddString < true >( s, GetStringLength( s ) );
        }
        return *this;
    }

    ownType &Append( const charType *s, uiw n )
    {
        ASSUME( s != 0 || n == 0 );
        AddString < true >( s, n );
        return *this;
    }

    ownType &Append( charType c, uiw n = 1 )
    {
        charType *str = AddSpace( n ) + _count;
        for( ; n; --n )
        {
            *str++ = c;
        }
        return *this;
    }

    template < typename InputIterator > NOINLINE ownType &Append( InputIterator first, InputIterator last )
    {
        const charType *str = CStr();
        bln isOverlapped = !(first.Ptr() < str || first.Ptr() >= str + _count);
        if( isOverlapped )
        {
            /*  TODO: proto  */
            Append( ownType( first, last ) );
        }
        else
        {
            uiw dist = Algorithm::Distance( first, last );
            if( first.iteratorType == Iterator::Type::Random )
            {
                AddString < false >( first.Ptr(), dist );
            }
            else
            {
                uiw pos = _count;
                str = AddSpace( dist ) + pos;
                for( ; first != last; ++first )
                {
                    *str = *first;
                    ++str;
                }
            }
        }
        return *this;
    }

    NOINLINE ownType &Assign( const ownType &source )
    {
        if( this != &source )
        {
            bln isNeedDynamic = source._count > static_last;
            if( IsStatic() )
            {
                if( isNeedDynamic )
                {
                    ASSUME( source.IsDynamic() );
                    SetDynamic( source._count );
                    _MemCpyStr( _dynamic_str, source._dynamic_str, source._count + 1 );
                }
                else
                {
                    _MemCpyStr( _static_str, source.CStr(), source._count + 1 );
                }
            }
            else
            {
                _ProcReservationUp( source._count );
                _MemCpyStr( _dynamic_str, source.CStr(), source._count + 1 );
            }
            _count = source._count;
        }
        return *this;
    }

    NOINLINE ownType &Assign( const ownType &str, uiw subpos, uiw sublen )
    {
        ASSUME( subpos <= str._count );
        uiw realLen = Funcs::Min < uiw >( str._count - subpos, sublen );
        if( this == &str )
        {
            charType *curStr = Str();
            ASSUME( curStr == str.CStr() );
            _MemMoveStr( curStr, curStr + subpos, realLen );
            curStr[ realLen ] = (charType)0;
            _count = realLen;
            return *this;
        }
        else
        {
            return Assign( str.CStr() + subpos, realLen );
        }
    }

    NOINLINE ownType &Assign( const charType *s, uiw n = uiw_max )
    {
        if( s == 0 )
        {
            ASSUME( n == 0 || n == uiw_max );
            s = _EmptyStr < charType >::Get();
        }

        if( n == uiw_max )
        {
            n = GetStringLength( s );
        }
        const charType *curStr = CStr();
        bln isOverlapped = !(s < curStr || s >= curStr + _count);
        if( isOverlapped )  //  TODO: proto
        {
            *this = ownType( s, n );
            return *this;
        }

        bln isNeedDynamic = n > static_last;

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
            _MemCpyStr( _dynamic_str, s, n );
            _dynamic_str[ n ] = (charType)0;
        }
        else
        {
            charType *str = Str();
            _MemCpyStr( str, s, n );
            str[ n ] = (charType)0;
        }

        _count = n;

        return *this;
    }

    ownType &Assign( charType c, uiw n = 1 )
    {
        Clear();
        charType *str = AddSpace( n ) + _count;
        for( ; n; --n )
        {
            *str++ = c;
        }
        return *this;
    }

    template < typename InputIterator > NOINLINE ownType &Assign( InputIterator first, InputIterator last )
    {
        const charType *str = CStr();
        bln isOverlapped = !(first.Ptr() < str || first.Ptr() >= str + _count);
        if( isOverlapped )
        {
            /*  TODO: proto  */
            Assign( ownType( first, last ) );
        }
        else
        {
            if( first.iteratorType == Iterator::Type::Random )
            {
                uiw len = Iterator::_IterDist < InputIterator, InputIterator::iteratorType >::Dist( first, last );
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
                    _MemCpyStr( thisStr, first.Ptr(), len );
                    thisStr[ len ] = (charType)0;
                }
                else
                {
                    _ProcReservationUp( len );
                    _MemCpyStr( _dynamic_str, first.Ptr(), len );
                    _dynamic_str[ len ] = (charType)0;
                }
                _count = len;
            }
            else
            {
                Clear();
                for( ; first != last; ++first )
                {
                    AddString < false >( first.Ptr(), 1 );
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
        return Assign( str );  //  Assign will check for null
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
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
        return TCStr( this->CStr(), this->_count, str, GetStringLength( str ) );
    }

    TCStr operator + ( charType symbol ) const
    {
        return TCStr( this->CStr(), this->_count, &symbol, 1 );
    }

    friend TCStr operator + ( const charType *str, const ownType &second )
    {
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
        return TCStr( str, GetStringLength( str ), second.CStr(), second._count );
    }

    friend TCStr operator + ( charType symbol, const ownType &second )
    {
        return TCStr( &symbol, 1, second.CStr(), second._count );
    }

    TCStr &operator += ( const ownType &source )
    {
        AddString < true >( source.CStr(), source._count );
        return *this;
    }

    TCStr &operator += ( const charType *str )
    {
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
        AddString < true >( str, GetStringLength( str ) );
        return *this;
    }

    TCStr &operator += ( charType symbol )
    {
        AddString < false >( &symbol, 1 );
        return *this;
    }

    bln operator == ( const ownType &source ) const
    {
        if( _count == source._count )
        {
            return IsStringEquals( CStr(), source.CStr() );
        }
        return false;
    }

    bln operator == ( const charType *str ) const
    {
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
        return IsStringEquals( CStr(), str );
    }

    bln operator != ( const ownType &source ) const
    {
        return !this->operator==( source );
    }

    bln operator != ( const charType *str ) const
    {
        return !this->operator==( str );
    }

    bln operator < ( const ownType &source ) const
    {
        return StringCompare( CStr(), source.CStr() ) < 0;
    }

    bln operator < ( const charType *str ) const
    {
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
        return StringCompare( CStr(), str ) < 0;
    }

    bln operator >( const ownType &source ) const
    {
        return StringCompare( CStr(), source.CStr() ) > 0;
    }

    bln operator >( const charType *str ) const
    {
        if( str == 0 )
        {
            str = _EmptyStr < charType >::Get();
        }
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

//  defined in StdHelperLib.cpp
#if defined(EXTERN_TEMPLATES_SUPPORTED) && defined(EXTERN_TEMPLATES_ALLOWED)
    extern template class TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
    extern template class TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple >;
#endif

typedef TCStr< char, StringDefReserve, Reservator::Half <>, Allocator::Simple > CStr;
typedef TCStr< wchar_t, StringDefReserve, Reservator::Half <>, Allocator::Simple > CWStr;

}  //  StdLib

#undef _MemCpyStr
#undef _MemMoveStr

#endif
