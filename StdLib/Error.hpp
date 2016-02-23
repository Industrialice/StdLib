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

class CError
{
	ui32 code;
	const char *errorClass;
	const char *description;

public:
	CError()
	{
		code = 0;
		errorClass = "DEFAULT";
		description = "OK";
	}

	CError( ui32 code, const char *errorClass, const char *description ) : code( code ), errorClass( errorClass ), description( description )
	{
		ASSUME( errorClass && description );
	}

	bln operator == ( const CError &other ) const
	{
		ASSUME( other.errorClass );
		return this->code == other.code && !strcmp( this->errorClass, other.errorClass );
	}

	bln operator != ( const CError &other ) const
	{
		ASSUME( other.errorClass );
		return this->code != other.code || strcmp( this->errorClass, other.errorClass );
	}

#ifdef MOVE_SUPPORTED
	CError( const CError &source ) = default;
	CError &operator = ( const CError &source ) = default;
	CError( CError &&source ) = default;
	CError &operator = ( CError &&source ) = default;
#endif

	const char *Class() const
	{
		return errorClass;
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

	CTError( ui32 code, const char *errorClass, const char *description, const additionT &addition ) : CError( code, errorClass, description ), addition( addition )
	{}

	CTError( const CError &error, const additionT &addition ) : CError( error ), addition( addition )
	{}

	CTError( const CError &source ) : CError( source )
	{}

	CTError &operator = ( const CError &source )
	{
		CError::operator =( source );
		return *this;
	}

	bln operator == ( const CError &other ) const
	{
		return CError::operator ==( other );
	}

	bln operator != ( const CError &other ) const
	{
		return CError::operator !=( other );
	}

#ifdef MOVE_SUPPORTED
	CTError( const CTError &source ) = default;
	CTError &operator = ( const CTError &source ) = default;
	CTError( CTError &&source ) = default;
	CTError &operator = ( CTError &&source ) = default;
#endif

	const additionT &Addition() const
	{
		return addition;
	}
};

namespace Error
{
	inline CError Ok() { return CError( 0, "DEFAULT", "OK" ); }
	inline CError Unknown() { return CError( 1, "DEFAULT", "UNKNOWN" ); }
	inline CError InvalidArgument() { return CError( 2, "DEFAULT", "INVALID_ARGUMENT" ); }
	inline CError OutOfMemory() { return CError( 3, "DEFAULT", "OUT_OF_MEMORY" ); }
	inline CError AlreadyExists() { return CError( 4, "DEFAULT", "ALREADY_EXISTS" ); }
	inline CError DoesNotExist() { return CError( 5, "DEFAULT", "DOES_NOT_EXISTS" ); }
	inline CError NoAccess() { return CError( 6, "DEFAULT", "NO_ACCESS" ); }
	inline CError Unsupported() { return CError( 7, "DEFAULT", "UNSUPPORTED" ); }
	inline CError Unimplemented() { return CError( 8, "DEFAULT", "UNIMPLEMENTED" ); }
	inline CError Obsolete() { return CError( 9, "DEFAULT", "OBSOLETE" ); }
	inline CError Interrupted() { return CError( 10, "DEFAULT", "INTERRUPTED" ); }
	inline CError CannotOpenFile() { return CError( 11, "DEFAULT", "CANNOT_OPEN_FILE" ); }

	const ui32 _MaxDefaultError = 128;
}

}  //  namespace StdLib

#endif