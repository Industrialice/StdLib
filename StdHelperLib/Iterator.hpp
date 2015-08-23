#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <StdCoreLib.hpp>

namespace StdLib
{ namespace Iterator
  {

namespace Type
{
    enum type_t
    {
        Forward,
        Bidir,
        Random
    };
}

template < typename type, iw step > class _IterRandomConst
{
    typedef _IterRandomConst < type, step > ownType;

protected:
    type *_str;

public:
    static const Type::type_t iteratorType = Type::Random;

    _IterRandomConst()
    {
        DBGCODE( _str = 0; )
    }

    _IterRandomConst( const type *source )
    {
        _str = (type *)source;
    }

    const type &operator *() const
    {
        return *_str;
    }

    const type *operator ->() const
    {
        return _str;
    }

    const type &operator [] ( size_t index ) const
    {
        return _str[ index ];
    }

    ownType &operator ++ ()
    {
        ASSUME( _str );
        _str += step;
        return *this;
    }

    ownType operator ++ (int)
    {
        ASSUME( _str );
        ownType temp = *this;
        ++(*this);
        return temp;
    }

    ownType &operator -- ()
    {
        ASSUME( _str );
        _str -= step;
        return *this;
    }

    ownType operator -- (int)
    {
        ASSUME( _str );
        ownType temp = *this;
        --(*this);
        return temp;
    }

    ownType operator + ( size_t addition ) const
    {
        return ownType( _str + (addition * step) );
    }

    friend ownType operator + ( size_t addition, const ownType &it )
    {
        return ownType( it._str + (addition * step) );
    }

    ownType operator - ( size_t subtraction ) const
    {
        return ownType( _str - (subtraction * step) );
    }

    size_t operator - ( const ownType& subtraction ) const
    {
        if( _str > subtraction._str )
            return _str - subtraction._str;
        return subtraction._str - _str;
    }

    ownType &operator += ( size_t addition )
    {
        _str += addition * step;
        return *this;
    }

    ownType &operator -= ( size_t subtraction )
    {
        _str -= subtraction * step;
        return *this;
    }

    bln operator != ( const ownType &other ) const
    {
        return _str != other._str;
    }

    bln operator == ( const ownType &other ) const
    {
        return _str == other._str;
    }

    bln operator < ( const ownType &other ) const
    {
        if( step == 1 )
            return _str < other._str;
        return _str > other._str;
    }

    bln operator > ( const ownType &other ) const
    {
        if( step == 1 )
            return _str > other._str;
        return _str < other._str;
    }

    bln operator <= ( const ownType &other ) const
    {
        if( step == 1 )
            return _str <= other._str;
        return _str >= other._str;
    }

    bln operator >= ( const ownType &other ) const
    {
        if( step == 1 )
            return _str >= other._str;
        return _str <= other._str;
    }
};

template < typename type, iw step > class _IterRandom : public _IterRandomConst < type, step >
{
public:
    using _IterRandomConst < type, step >::_str;

    _IterRandom()
    {
        DBGCODE( _str = 0; )
    }

    _IterRandom( type *source )
    {
        _str = source;
    }

    type &operator *()
    {
        return *_str;
    }

    type *operator ->()
    {
        return _str;
    }

    type &operator [] ( size_t index )
    {
        return _str[ index ];
    }
};

  }  //  namespace Iterator
}  //  namespace StdLib

#endif __ITERATOR_HPP__