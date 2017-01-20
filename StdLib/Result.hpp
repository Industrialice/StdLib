#ifndef __RESULT_HPP__
#define __RESULT_HPP__

#include "Error.hpp"

namespace StdLib
{
	template < typename T, typename errorAddition = void > class CResult
	{
		T _value;  //  TODO: should be possible to make this nullable( optional )
		CError < errorAddition > _error;
		DBGCODE( bln _is_unwrappedValue = false );
		DBGCODE( bln _is_unwrappedError = false );

	public:
		CResult( const T &value, const CError < errorAddition > &error = CError < errorAddition >() ) : _value( value ), _error( error )
		{}

		CResult( const T &value, CError < errorAddition > &&error ) : _value( value ), _error( std::move( error ) )
		{}

		CResult( T &&value, CError < errorAddition > &&error ) : _value( std::move( value ) ), _error( std::move( error ) )
		{}

		CResult( T &&value, const CError < errorAddition > &error = CError < errorAddition >() ) : _value( std::move( value ) ), _error( error )
		{}

		T &&Unwrap()
		{
			ASSUME( _error.Ok() );
			DBGCODE( ASSUME( _is_unwrappedValue == false ) );
			DBGCODE( _is_unwrappedValue = true );
			return std::move( _value );
		}

		T UnwrapOrGet( const T &value )
		{
			DBGCODE( ASSUME( _is_unwrappedValue == false ) );
			DBGCODE( _is_unwrappedValue = true );
			if( _error.Ok() )
			{
				return _value;
			}
			return value;
		}

		bln Ok() const
		{
			return _error.Ok();
		}

		const CError < errorAddition > &ErrorDesc() const
		{
			return _error;
		}

		CError < errorAddition > &&UnwrapError()
		{
			DBGCODE( ASSUME( _is_unwrappedError == false ) );
			DBGCODE( _is_unwrappedError = true );
			return std::move( _error );
		}
	};
}

#endif