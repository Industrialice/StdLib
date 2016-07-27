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

	bln operator == ( const X &source ) const
	{
		if( this->_is_null )
		{
			return false;
		}
		return this->ToRef() == source;
	}

	bln operator != ( const X &source ) const
	{
		return !this->operator != ( source );
	}

    bln operator == ( const Nullable &source ) const
    {
		if( _is_null == source._is_null )
		{
			if( !_is_null )
			{
				return ToRef() == source.ToRef();
			}
			return true;
		}
        return false;
    }

    bln operator != ( const Nullable &source ) const
	{
		return !this->operator == ( source );
	}

    bln operator == ( Nullv ) const
    {
        return _is_null;
    }

    bln operator != ( Nullv ) const
	{
		return !_is_null;
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

    bln operator != ( std::nullptr_t ) const
	{
		return !_is_null;
	}
#endif

	Nullable( const Nullable &source )
	{
		if( source._is_null == false )
		{
			new (&_object) X( source.ToRef() );
		}
		this->_is_null = source._is_null;
	}

	Nullable &operator = ( const Nullable &source )
	{
		if( this != &source )
		{
			if( this->_is_null == source._is_null )
			{
				if( this->_is_null == false )
				{
					this->ToRef() = source->ToRef();
				}
			}
			else
			{
				if( this->_is_null == false )
				{
					this->ToRef().~X();
				}
				else
				{
					new (&this->_object) X( source.ToRef() );
				}

				this->_is_null = source._is_null;
			}
		}
		return *this;
	}

#ifdef MOVE_SUPPORTED
	Nullable( Nullable &&source )
	{
		if( source._is_null == false )
		{
			ToRef() = std::move( *(X *)&source._object );
		}
		this->_is_null = source._is_null;
	}

	Nullable &operator = ( Nullable &&source )
	{
		ASSUME( this != &source );

		if( this->_is_null == source._is_null )
		{
			if( this->_is_null == false )
			{
				*(X *)&this->_object = std::move( *(X *)&source._object );
			}
		}
		else
		{
			if( this->_is_null == false )
			{
				this->ToRef().~X();
			}
			else
			{
				new (&this->_object) X( std::move( *(X *)&source._object ) );
			}

			this->_is_null = source._is_null;
		}

		source->_is_null = true;

		return *this;
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