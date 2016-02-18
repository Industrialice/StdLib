#ifndef __ERROR_HPP__
#define __ERROR_HPP__

//  Default errors:
/*
Ok
Unknown
InvalidArgument
OutOfMemory
AlreadyExists
DoesNotExist
NoAccess
Unsupported
Unimplemented
Obsolete
Interrupted
CannotOpenFile
*/

namespace StdLib {

namespace Error
{
	namespace Private
	{
		EXTERNAL const char *GetErrorDesc( ui32 code );
		const ui32 MaxDefaultError = 128;
	}
}

class CError
{
protected:
	ui32 code;
	const char *description;

public:
	CError()
	{
		code = 0;
		description = Error::Private::GetErrorDesc( 0 );
	}

	CError( ui32 code, const char *description ) : code( code ), description( description )
	{}

	bln operator == ( const CError &other )
	{
		return this->code == other.code;
	}

	bln operator != ( const CError &other )
	{
		return this->code != other.code;
	}

	const char *Description() const
	{
		return description;
	}
};

template < typename additionT > class CTError : public CError
{
	additionT addition;

public:
	CTError() : CError(), addition( additionT() )
	{}

	CTError( ui32 code, const char *description, const additionT &addition ) : CError( code, description ), addition( addition )
	{}

	const additionT &Addition() const
	{
		return addition;
	}
};

namespace Error
{
	inline CError Ok() { return CError( 0, Private::GetErrorDesc( 0 ) ); }
	inline CError Unknown() { return CError( 1, Private::GetErrorDesc( 1 ) ); }
	inline CError InvalidArgument() { return CError( 2, Private::GetErrorDesc( 2 ) ); }
	inline CError OutOfMemory() { return CError( 3, Private::GetErrorDesc( 3 ) ); }
	inline CError AlreadyExists() { return CError( 4, Private::GetErrorDesc( 4 ) ); }
	inline CError DoesNotExist() { return CError( 5, Private::GetErrorDesc( 5 ) ); }
	inline CError NoAccess() { return CError( 6, Private::GetErrorDesc( 6 ) ); }
	inline CError Unsupported() { return CError( 7, Private::GetErrorDesc( 7 ) ); }
	inline CError Unimplemented() { return CError( 8, Private::GetErrorDesc( 8 ) ); }
	inline CError Obsolete() { return CError( 9, Private::GetErrorDesc( 9 ) ); }
	inline CError Interrupted() { return CError( 10, Private::GetErrorDesc( 10 ) ); }
	inline CError CannotOpenFile() { return CError( 11, Private::GetErrorDesc( 11 ) ); }
}

}  //  namespace StdLib

#endif __ERROR_HPP__