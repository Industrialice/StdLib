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

template < typename type, iw step, typename retType, bool is_const > class _IterRandomBasis;

template < typename type, iw step, typename retType > class _IterRandomBasis < type, step, retType, false >
{
protected:
    type *_str;
};

template < typename type, iw step, typename retType > class _IterRandomBasis < type, step, retType, true >
{
protected:
    const type *_str;
};

template < typename type, iw step, typename retType, bool is_const > class _IterRandomBase : public _IterRandomBasis < type, step, retType, is_const >
{
    typedef _IterRandomBasis < type, step, retType, is_const > baseType;
    typedef _IterRandomBase < type, step, retType, is_const > ownType;

protected:
    using baseType::_str;

public:
    static const Type::type_t iteratorType = Type::Random;

    _IterRandomBase()
    {
        DBGCODE( _str = 0; )
    }

    _IterRandomBase( const type *source )
    {
        _str = source;
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

    retType &operator ++ ()
    {
        ASSUME( _str );
        _str += step;
        return *this;
    }

    retType operator ++ (int)
    {
        ASSUME( _str );
        retType temp = *this;
        ++(*this);
        return temp;
    }

    retType &operator -- ()
    {
        ASSUME( _str );
        _str -= step;
        return *this;
    }

    retType operator -- (int)
    {
        ASSUME( _str );
        ownType temp = *this;
        --(*this);
        return temp;
    }

    retType operator + ( size_t addition ) const
    {
        return retType( _str + (addition * step) );
    }

    friend retType operator + ( size_t addition, const ownType &it )
    {
        return retType( it._str + (addition * step) );
    }

    retType operator - ( size_t subtraction ) const
    {
        return retType( _str - (subtraction * step) );
    }

    size_t operator - ( const ownType& subtraction ) const
    {
        if( _str > subtraction._str )
            return _str - subtraction._str;
        return subtraction._str - _str;
    }

    retType &operator += ( size_t addition )
    {
        _str += addition * step;
        return *this;
    }

    retType &operator -= ( size_t subtraction )
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

template < typename type, iw step > class _IterRandomConst : public _IterRandomBase < type, step, _IterRandomConst < type, step >, true >
{
    typedef _IterRandomBase < type, step, _IterRandomConst < type, step >, true > baseType;

public:
    using baseType::_str;

    _IterRandomConst()
    {
        DBGCODE( _str = 0; )
    }

    _IterRandomConst( const type *source )
    {
        _str = source;
    }
};

template < typename type, iw step > class _IterRandom : public _IterRandomBase < type, step, _IterRandom < type, step >, false >
{
    typedef _IterRandomBase < type, step, _IterRandom < type, step >, false > baseType;

public:
    using baseType::_str;

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

    operator _IterRandomConst < type, step >() const
    {
        return _IterRandomConst < type, step >( _str );
    }
};

  }  //  namespace Iterator
}  //  namespace StdLib

#endif __ITERATOR_HPP__