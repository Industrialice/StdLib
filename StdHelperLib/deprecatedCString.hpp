#ifndef __C_STRING_HPP__
#define __C_STRING_HPP__

#include "Allocators.hpp"
#include "Reservators.hpp"

namespace StdLib {

template < typename count_type = ui32, typename allocator = Allocator::Simple, typename reservator = Reservator::Simple < count_type >, uiw basicSize = 16 > class TCString
{
    typedef TCString < count_type, allocator, reservator, basicSize > ownType;

    struct SDymanicFmt
    {
        char *pointer;
        count_type reserved;
        char is_staticOrDynamic;
    };

    static const uiw dynamic_size = sizeof(SDymanicFmt);
    static const uiw static_size = dynamic_size > basicSize ? dynamic_size : basicSize;
    static const uiw static_last = static_size - 1;

    union
    {
        char _static_str[ static_size ];  //  if last byte is zero - static storage is used, nonzero - dynamic
        struct
        {
            char *_dynamic_str;
            count_type _reserved;  //  without zero symbol
        };
    };
    count_type _count;  //  without zero symbol

    void SetStatic()
    {
        _static_str[ static_last ] = '\0';
    }

    void SetDynamic()
    {
        _static_str[ static_last ] = 0xFF;
    }

    void _ProcReservationUp( count_type newCount )
    {
        ASSUME( IsDynamic() );
        if( reservator::Up( newCount, &_reserved ) )
        {
            _dynamic_str = (char *)allocator::Realloc( _dynamic_str, _reserved + 1 );
        }
        ASSUME( _reserved >= newCount );
    }

    void _ProcReservationDown( count_type newCount )
    {
        ASSUME( IsDynamic() );
        if( reservator::Down( newCount, &_reserved ) )
        {
            _dynamic_str = (char *)allocator::Realloc( _dynamic_str, _reserved + 1 );
        }
        ASSUME( _reserved >= newCount );
    }

    NOINLINE void AddString( const char *str, count_type len )
    {
        if( IsDynamic() )
        {
            _ProcReservationUp( _count + len );
            _MemCpy( _dynamic_str + _count, str, len );
            _count += len;
            _dynamic_str[ _count ] = '\0';
        }
        else
        {
            if( _count + len >= static_size )
            {
                count_type reserved = 0;
                reservator::Up( _count + len, &reserved );
                ASSUME( reserved >= _count + len );
                char *newStr = allocator::Alloc < char >( reserved + 1 );
                _MemCpy( newStr, _static_str, _count );
                _MemCpy( newStr + _count, str, len );
                _count += len;
                newStr[ _count ] = '\0';
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
            }
            else
            {
                _MemCpy( _static_str + _count, str, len );
                _count += len;
                _static_str[ _count ] = '\0';
            }
        }
    }

    NOINLINE void InsertString( count_type index, const char *str, count_type len )
    {
        if( IsDynamic() )
        {
            _ProcReservationUp( _count + len );
            _MemMove( _dynamic_str + index + len, _dynamic_str + index, _count - index );
            _MemCpy( _dynamic_str + index, str, len );
            _count += len;
            _dynamic_str[ _count ] = '\0';
        }
        else
        {
            if( _count + len >= static_size )
            {
                count_type reserved = 0;
                reservator::Up( _count + len, &reserved );
                ASSUME( reserved >= _count + len );
                char *newStr = allocator::Alloc( reserved + 1 );
                _MemCpy( newStr, _static_str, index );
                _MemCpy( newStr + index, str, len );
                _MemCpy( newStr + index + len, _static_str + index, _count - index );
                _count += len;
                newStr[ _count ] = '\0';
                SetDynamic();
                _reserved = reserved;
                _dynamic_str = newStr;
            }
            else
            {
                _MemMove( _static_str + index + len, _static_str + index, _count - index );
                _MemCpy( _static_str + index, str, len );
                _count += len;
                _static_str[ _count ] = '\0';
            }
        }
    }

    NOINLINE void EraseSpace( count_type index, count_type count )
    {
        ASSUME( index + count <= _count );
        char *str = Str();
        _count -= count;
        _MemMove( str + index, str + index + count, _count - index + 1 );
        if( IsDynamic() )
        {
            _ProcReservationDown( _count );
        }
    }

public:
    ~TCString()
    {
        if( IsDynamic() )
        {
            allocator::Free( _dynamic_str );
        }
    }

    TCString()
    {
        _static_str[ 0 ] = '\0';
        SetStatic();
        _count = 0;
    }

    TCString( count_type reserve )
    {
        if( reserve >= static_size )
        {
            SetDynamic();
            _reserved = reserve;
            char *str = allocator::Alloc( _reserved + 1 );
            *str = '\0';
        }
        else
        {
            _static_str[ 0 ] = '\0';
            SetStatic();
        }
        _count = 0;
    }

    TCString( const char *str )
    {
        count_type len = _StrLen( str );
        _count = len;
        if( len >= static_size )
        {
            SetDynamic();
            _reserved = 0;
            reservator::Up( _count, &_reserved );
            ASSUME( _reserved >= len );
            _dynamic_str = allocator::Alloc( _reserved + 1 );
            _MemCpy( _dynamic_str, str, _count + 1 );
        }
        else
        {
            SetStatic();
            _MemCpy( _static_str, str, _count + 1 );
        }
    }

    TCString( const char *str, count_type len )
    {
        _count = len;
        if( len >= static_size )
        {
            SetDynamic();
            _reserved = 0;
            reservator::Up( _count, &_reserved );
            ASSUME( _reserved >= len );
            _dynamic_str = allocator::Alloc( _reserved + 1 );
            _MemCpy( _dynamic_str, str, _count );
            _dynamic_str[ _count ] = '\0';
        }
        else
        {
            SetStatic();
            _MemCpy( _static_str, str, _count );
            _static_str[ _count ] = '\0';
        }
    }

    NOINLINE TCString( const char *str0, count_type len0, const char *str1, count_type len1 )
    {
        _count = len0 + len1;
        if( _count >= static_size )
        {
            SetDynamic();
            _reserved = 0;
            reservator::Up( _count, &_reserved );
            ASSUME( _reserved >= _count );
            _dynamic_str = allocator::Alloc( _reserved + 1 );
            _MemCpy( _dynamic_str, str0, len0 );
            _MemCpy( _dynamic_str + len0, str1, len1 );
            _dynamic_str[ _count ] = '\0';
        }
        else
        {
            SetStatic();
            _MemCpy( _static_str, str0, len0);
            _MemCpy( _static_str + len0, str1, len1 );
            _static_str[ _count ] = '\0';
        }
    }

    TCString( const ownType &source )
    {
        ASSUME( this != &source );
        if( source.IsDynamic() )
        {
            _reserved = source._reserved;
            _count = source._count;
            _dynamic_str = allocator::Alloc( source._reserved + 1 );
            _MemCpy( _dynamic_str, source._dynamic_str, source._count + 1 );
            SetDynamic();
        }
        else
        {
            _MemCpy( _static_str, source._static_str, source._count + 1 );
            _count = source._count;
            SetStatic();
        }
    }

    const char *CStr() const
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

    char *Str()
    {
        return IsStatic() ? _static_str : _dynamic_str;
    }

    count_type Len() const
    {
        return _count;
    }

    count_type Size() const
    {
        return _count + 1;
    }

    bln IsStatic() const
    {
        return _static_str[ static_last ] == '\0';
    }

    bln IsDynamic() const
    {
        return _static_str[ static_last ] != '\0';
    }

    NOINLINE void Compact()
    {
        if( IsDynamic() )
        {
            if( _count < static_size )
            {
                _MemCpy( _static_str, _dynamic_str, _count + 1 );
                SetStatic();
            }
            else if( _reserved > _count )
            {
                _reserved = _count;
                _dynamic_str = allocator::Realloc( _dynamic_str, _reserved + 1 );
            }
        }
    }

    void Insert( count_type index, const ownType &source )
    {
        ASSUME( index < _count );
        InsertString( index, source.CStr(), source.Len() );
    }

    void Insert( count_type index, const char *str )
    {
        ASSUME( index < _count );
        count_type len = _StrLen( str );
        InsertString( index, str, len );
    }

    void Insert( count_type index, const char *str, count_type len )
    {
        ASSUME( index < _count );
        InsertString( index, str, len );
    }

    void Insert( count_type index, char symbol )
    {
        ASSUME( index < _count );
        InsertString( index, &symbol, 1 );
    }

    void Erase( count_type index, count_type count )
    {
        EraseSpace( index, count );
    }

    void Clear()
    {
        if( IsDynamic() )
        {
            _ProcReservationDown( 0 );
            _dynamic_str[ 0 ] = '\0';
        }
        else
        {
            _static_str[ 0 ] = '\0';
        }
        _count = 0;
    }

    void FullClear()
    {
        if( IsDynamic() )
        {
            allocator::Free( _dynamic_str );
            SetStatic();
            _static_str[ 0 ] = '\0';
        }
        _count = 0;
    }

    TCString &operator = ( const ownType &source )
    {
        if( this != &source )
        {
            if( IsDynamic() )
            {
                allocator::Free( _dynamic_str );
            }
            if( source.IsDynamic() )
            {
                SetDynamic();
                _reserved = source._reserved;
                _count = source._count;
                ASSUME( _reserved >= _count );
                _dynamic_str = allocator::Alloc( source._reserved + 1 );
                _MemCpy( _dynamic_str, source._dynamic_str, source._count + 1 );
            }
            else
            {
                //  SetStatic();  //  excess
                _MemCpy( _static_str, source._static_str, source._count + 1 );
                _count = source._count;
            }
        }
        return *this;
    }

    TCString operator + ( const ownType &source )
    {
        return TCString( this->CStr(), this->_count, source.CStr(), source._count );
    }

    TCString operator + ( const char *str )
    {
        return TCString( this->CStr(), this->_count, str, _StrLen( str ) );
    }

    TCString operator + ( char symbol )
    {
        return TCString( this->CStr(), this->_count, &symbol, 1 );
    }

    friend TCString operator + ( const char *str, const ownType &second )
    {
        return TCString( str, _StrLen( str ), second.CStr(), second.Len() );
    }

    friend TCString operator + ( char symbol, const ownType &second )
    {
        return TCString( &symbol, 1, second.CStr(), second.Len() );
    }

    TCString &operator += ( const ownType &source )
    {
        AddString( source.CStr(), source.Len() );
        return *this;
    }

    TCString &operator += ( const char *str )
    {
        AddString( str, _StrLen( str ) );
        return *this;
    }

    TCString &operator += ( char symbol )
    {
        AddString( &symbol, 1 );
        return *this;
    }
};

typedef TCString <> CString;

}  //  namespace StdLib

#endif __C_STRING_HPP__