#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <StdCoreLib.hpp>
#include <iterator>

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

template < typename X, Type::type_t type > struct _IterDist
{
    static uiw Dist( X first, const X &second )
    {
        uiw dist = 0;
        for( ; first != second; ++first )
        {
            ++dist;
        }
        return dist;
    }
};

template < typename X > struct _IterDist < X, Type::Random >
{
    static uiw Dist( const X &first, const X &second )
    {
        ASSUME( first <= second );
        return second - first;
    }
};

struct _TypeIterator {};

template < typename type, iw step, typename retType, bln is_const > class _IterRandomBasis;

template < typename type, iw step, typename retType > class _IterRandomBasis < type, step, retType, false > : _TypeIterator, public std::iterator < std::random_access_iterator_tag, type >
{
protected:
    type *_str;
};

template < typename type, iw step, typename retType > class _IterRandomBasis < type, step, retType, true > : _TypeIterator, public std::iterator < std::random_access_iterator_tag, type >
{
protected:
    const type *_str;
};

template < typename type, iw step, typename retType, bln is_const > class _IterRandomBase : public _IterRandomBasis < type, step, retType, is_const >
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

    const type *Ptr() const
    {
        return _str;
    }

    const type &operator *() const
    {
        return *_str;
    }

    const type *operator ->() const
    {
        return _str;
    }

    const type &operator [] ( uiw index ) const
    {
        return _str[ index ];
    }

    retType &operator ++ ()
    {
        ASSUME( _str );
        _str += step;
        return *(retType *)this;
    }

    retType operator ++ (int)
    {
        ASSUME( _str );
        retType temp( this->_str );
        ++(*this);
        return temp;
    }

    retType &operator -- ()
    {
        ASSUME( _str );
        _str -= step;
        return *(retType *)this;
    }

    retType operator -- (int)
    {
        ASSUME( _str );
        retType temp( this->_str );
        --(*this);
        return temp;
    }

    retType operator + ( uiw addition ) const
    {
        return retType( _str + (addition * step) );
    }

    friend retType operator + ( uiw addition, const ownType &it )
    {
        return retType( it._str + (addition * step) );
    }

    retType operator - ( uiw subtraction ) const
    {
        return retType( _str - (subtraction * step) );
    }

    uiw operator - ( const ownType& subtraction ) const
    {
        if( _str > subtraction._str )
            return _str - subtraction._str;
        return subtraction._str - _str;
    }

    retType &operator += ( uiw addition )
    {
        _str += addition * step;
        return *(retType *)this;
    }

    retType &operator -= ( uiw subtraction )
    {
        _str -= subtraction * step;
        return *(retType *)this;
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

	explicit _IterRandomConst( const type *source )
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

    explicit _IterRandom( type *source )
    {
        _str = source;
    }

    type *Ptr()
    {
        return _str;
    }

    type &operator *()
    {
        return *_str;
    }

    type *operator ->()
    {
        return _str;
    }

    type &operator [] ( uiw index )
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

#endif