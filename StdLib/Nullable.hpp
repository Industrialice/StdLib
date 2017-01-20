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
		return !this->operator == ( source );
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

    bln operator == ( std::nullptr_t ) const
    {
        return _is_null;
    }

    bln operator != ( std::nullptr_t ) const
	{
		return !_is_null;
	}

	Nullable( std::nullptr_t )
	{
		_is_null = true;
	}

    Nullable &operator = ( std::nullptr_t )
    {
        if( !_is_null )
        {
            ToRef().~X();
            _is_null = true;
        }
        return *this;
    }

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
					this->ToRef() = source.ToRef();
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

	Nullable( Nullable &&source )
	{
		this->_is_null = source._is_null;
		if( source._is_null == false )
		{
			ToRef() = std::move( *(X *)&source._object );
			source._is_null = true;
		}
	}

	Nullable &operator = ( Nullable &&source )
	{
		ASSUME( this != &source );

		if( this->_is_null == source._is_null )
		{
			if( this->_is_null == false )
			{
				ToRef() = std::move( *(X *)&source._object );
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

    bln IsNull() const
    {
        return _is_null;
    }

    explicit operator const X & () const
    {
        ASSUME( _is_null == false );
        return ToRef();
    }

	X &InitializeAndGet()
	{
		if( _is_null )
		{
			new (&_object) X();
			_is_null = false;
		}
		return ToRef();
	}

	const X &InitializeAndCGet()
	{
		if( _is_null )
		{
			new (&_object) X();
			_is_null = false;
		}
		return ToRef();
	}

	X &Get()
    {
        ASSUME( _is_null == false );
	    return ToRef();
    }

	const X &Get() const
    {
        ASSUME( _is_null == false );
	    return ToRef();
    }

	const X &CGet() const
    {
        ASSUME( _is_null == false );
	    return ToRef();
    }

	X *InitializeAndAddr()
	{
		if( _is_null )
		{
			new (&_object) X();
			_is_null = false;
		}
		return &ToRef();
	}

	const X *InitializeAndCAddr() const
	{
		if( _is_null )
		{
			new (&_object) X();
			_is_null = false;
		}
		return &ToRef();
	}

	X *Addr()
	{
        ASSUME( _is_null == false );
		return &ToRef();
	}

	const X *Addr() const
	{
        ASSUME( _is_null == false );
		return &ToRef();
	}

	const X *CAddr() const
	{
        ASSUME( _is_null == false );
		return &ToRef();
	}

	X *operator -> ()
	{
        ASSUME( _is_null == false );
		return &ToRef();
	}

	const X *operator -> () const
	{
        ASSUME( _is_null == false );
		return &ToRef();
	}

	X ValueOrDefault() const
	{
		if( _is_null )
		{
			return X();
		}
		return ToRef();
	}

	X ValueOrDefault( const X &def ) const
	{
		if( _is_null )
		{
			return def;
		}
		return ToRef();
	}
};

//  To prevent using Nullable as a holder for reference types
template < typename X > class Nullable < X & >
{
    Nullable();
};

template < typename X > class Nullable < const X & >
{
    Nullable();
};

}

#endif