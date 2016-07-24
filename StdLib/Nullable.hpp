#ifndef __NULLABLE_HPP__
#define __NULLABLE_HPP__

#include "PlatformTypes.hpp"

namespace StdLib {

/*  References aren't supported!  */
template < typename X > class Nullable
{
    typename AlignmentHelper < X >::type _object;
    bln _is_null;

    X &ToRef()
    {
        return *(X *)&_object;
    }

    const X &ToRef() const
    {
        return *(const X *)&_object;
    }

public:
    ~Nullable()
    {
        if( !_is_null )
        {
            ToRef().~X();
        }
    }

    Nullable()
    {
        _is_null = true;
    }

    Nullable( const X &source )
    {
        new (&_object) X( source );
        _is_null = false;
    }

    Nullable( Nullv )
    {
        _is_null = true;
    }

    Nullable &operator = ( const X &source )
    {
        if( _is_null )
        {
            new (&_object) X( source );
            _is_null = false;
        }
        else
        {
            ToRef() = source;
        }
        return *this;
    }

    Nullable &operator = ( Nullv )
    {
        if( !_is_null )
        {
            ToRef().~X();
            _is_null = true;
        }
        return *this;
    }

    bln operator == ( const Nullable &source ) const
    {
        return !_is_null && !source._is_null && ToRef() == source.ToRef();
    }

    bln operator == ( Nullv ) const
    {
        return _is_null;
    }

#ifdef NULLPTR_SUPPORTED
    Nullable &operator = ( std::nullptr_t )
    {
        return (*this = nullv);
    }

    bln operator == ( std::nullptr_t ) const
    {
        return _is_null;
    }
#endif

    bln IsNull() const
    {
        return _is_null;
    }

    operator const X & () const
    {
        ASSUME( _is_null == false );
        return ToRef();
    }

	X &Get()
    {
	    return ToRef();
    }

	const X &Get() const
    {
	    return ToRef();
    }

	X *operator -> ()
	{
		return &ToRef();
	}

	const X *operator -> () const
	{
		return &ToRef();
	}
};

//  To prevent using Nullable as a holder for reference types
template < typename X > class Nullable < X & >
{
    Nullable();
};

}

#endif