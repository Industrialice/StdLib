#ifndef __UNIQUE_PTR_HPP__
#define __UNIQUE_PTR_HPP__

#include "PlatformTypes.hpp"

namespace StdLib {

template < typename X, typename Deleter = NewDeleter < X > > class UniquePtr
{
    UniquePtr( const UniquePtr &source );
    UniquePtr &operator = ( const UniquePtr &source );

protected:
    X *_ptr;

public:
    ~UniquePtr()
    {
        Deleter( (X *)_ptr );  //  this dummy C-cast will eliminate a syntax confusion between creating a local variable _ptr and calling a constructor with the argument _ptr
    }

    UniquePtr() : _ptr( 0 )
    {}

    UniquePtr( X *ptr ) : _ptr( ptr )
    {}

#ifdef MOVE_SUPPORTED
    UniquePtr( UniquePtr &&source )
    {
        _ptr = source._ptr;
        source._ptr = 0;
    }

    UniquePtr &operator = ( UniquePtr &&source )
    {
        ASSUME( this != &source );
        Deleter( (X *)_ptr );
        _ptr = source._ptr;
        source._ptr = 0;
        return *this;
    }
#endif

    void Own( UniquePtr *source )
    {
        ASSUME( (_ptr != source->_ptr) || (_ptr == 0) );
        Deleter( (X *)_ptr );
        _ptr = source->_ptr;
        source->_ptr = 0;
    }

    void Release()
    {
        Deleter( (X *)_ptr );
        _ptr = 0;
    }

    X *TakeAway()
    {
        X *ret = _ptr;
        _ptr = 0;
        return ret;
    }

    X *Get()
    {
        return _ptr;
    }

    const X *Get() const
    {
        return _ptr;
    }

    X *const *Addr()
    {
        return &_ptr;
    }

    X **AddrModifiable()
    {
        ASSUME( _ptr == 0 );
        return &_ptr;
    }

    X **AddrModifiableRelease()
    {
        Release();
        return &_ptr;
    }

    X **AddrModifiableUnsafe()
    {
        return &_ptr;
    }

    const X *const *Addr() const
    {
        return &_ptr;
    }

    const X &operator * () const
    {
        ASSUME( _ptr );
        return *_ptr;
    }

    X &operator * ()
    {
        ASSUME( _ptr );
        return *_ptr;
    }
      
    const X *operator -> () const
    {
        ASSUME( _ptr );
        return _ptr;
    }

    X *operator -> ()
    {
        ASSUME( _ptr );
        return _ptr;
    }

    operator X * ()
    {
        return _ptr;
    }

    operator const X * () const
    {
        return _ptr;
    }
};

template < typename X, typename Deleter > class UniquePtr < X[], Deleter > : public UniquePtr < X, Deleter >
{
public:
    UniquePtr()
    {
        this->_ptr = 0;
    }

    UniquePtr( X *ptr )
    {
        this->_ptr = ptr;
    }
};

}

#endif