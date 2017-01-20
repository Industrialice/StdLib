#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include "PlatformTypes.hpp"

//  Default errors:
/*
Ok
UnknownError
InvalidArgument
OutOfMemory
AlreadyExists
DoesNotExist
NoAccess
UnsupportedFormat
UnsupportedFeature
Unimplemented
Obsolete
Interrupted
CannotOpenFile
UnknownFormat
Forbidden
*/

namespace StdLib {

template < typename T = void > class CError;

template <> class CError < void >
{
	ui32 code = 0;
	const char *errorClass = "DEFAULT";
	const char *description = "OK";

public:
	CError() = default;

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

	const char *Class() const
	{
		return errorClass;
	}

	const char *Description() const
	{
		return description;
	}

	bln Ok() const
	{
		return code == 0;
	}

	void Assume() const  //  just assume that the result is ok, you'll get an assert if it wasn't true
	{
		ASSUME( code == 0 );
	}
};

template < typename additionT > class CError : public CError < void >
{
	additionT addition = additionT();

public:
	CError() = default;

	CError( ui32 code, const char *errorClass, const char *description, const additionT &addition ) : CError < void >( code, errorClass, description ), addition( addition )
	{}

	CError( ui32 code, const char *errorClass, const char *description, additionT &&addition ) : CError < void >( code, errorClass, description ), addition( std::move( addition ) )
	{}

	CError( const CError < void > &error, const additionT &addition ) : CError < void >( error ), addition( addition )
	{}

	CError( const CError < void > &error, additionT &&addition ) : CError < void >( error ), addition( std::move( addition ) )
	{}

	CError( const CError < void > &source ) : CError < void >( source )
	{}

	CError &operator = ( const CError < void > &source )
	{
		CError < void >::operator =( source );
		return *this;
	}

	CError &operator = ( const CError & ) = default;
	CError( const CError & ) = default;
	CError &operator = ( CError && ) = default;
	CError( CError && ) = default;

	const additionT &Addition() const
	{
		return addition;
	}
};

namespace Error
{
	inline CError<> Ok() { return CError<>( 0, "DEFAULT", "OK" ); }
	inline CError<> UnknownError() { return CError<>( 1, "DEFAULT", "UNKNOWN_ERROR" ); }
	inline CError<> InvalidArgument() { return CError<>( 2, "DEFAULT", "INVALID_ARGUMENT" ); }
	inline CError<> OutOfMemory() { return CError<>( 3, "DEFAULT", "OUT_OF_MEMORY" ); }
	inline CError<> AlreadyExists() { return CError<>( 4, "DEFAULT", "ALREADY_EXISTS" ); }
	inline CError<> DoesNotExist() { return CError<>( 5, "DEFAULT", "DOES_NOT_EXIST" ); }
	inline CError<> NoAccess() { return CError<>( 6, "DEFAULT", "NO_ACCESS" ); }
	inline CError<> UnsupportedFormat() { return CError<>( 7, "DEFAULT", "UNSUPPORTED_FORMAT" ); }
	inline CError<> UnsupportedFeature() { return CError<>( 8, "DEFAULT", "UNSUPPORTED_FEATURE" ); }
	inline CError<> Unimplemented() { return CError<>( 9, "DEFAULT", "UNIMPLEMENTED" ); }
	inline CError<> Obsolete() { return CError<>( 10, "DEFAULT", "OBSOLETE" ); }
	inline CError<> Interrupted() { return CError<>( 11, "DEFAULT", "INTERRUPTED" ); }
	inline CError<> CannotOpenFile() { return CError<>( 12, "DEFAULT", "CANNOT_OPEN_FILE" ); }
	inline CError<> UnknownFormat() { return CError<>( 13, "DEFAULT", "UNKNOWN_FORMAT" ); }
	inline CError<> Forbidden() { return CError<>( 14, "DEFAULT", "FORBIDDEN" ); }

	const ui32 _MaxDefaultError = 128;
}

}  //  namespace StdLib

#endif