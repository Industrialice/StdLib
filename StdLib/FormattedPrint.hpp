#ifndef __FORMATTED_PRINT_HPP__
#define __FORMATTED_PRINT_HPP__

#include "StringFuncs.hpp"

namespace StdLib {
namespace Funcs
{
	//  TODO: add flags that allow to check for success
	//  TODO: wchar_t handling
	/*
	a - pointer, integer word as bin str [param - when non-zero, use upper case]
	b - boolean [param - when non-zero, use upper case]
	c - char
	d - f64 [param - precision]
	e -
	f - f32 [param - precision]
	g -
	h - integer 32 as hex str [param - when non-zero, use upper case]
	i - i32
	j - integer 64 as hex str [param - when non-zero, use upper case]
	k -
	l - i64
	m - integer 64 as bin str [param - when non-zero, use upper case]
	n - integer 32 as bin str [param - when non-zero, use upper case]
	o - signed word
	p - pointer, integer word as hex str [param - when non-zero, use upper case]
	q -
	r -
	s - string [param - max length]
	t -
	u - ui32
	v - ui64
	w - unsigned word
	x -
	y -
	z -
	*/

	//  maxLen means size of the p_str, including null-terminator. must be at least 1( to hold the null-terminator ). 0 will be returned if maxLen was less than one, debug will halt the program
    EXTERNALD uiw PrintToStrArgList( char *p_str, uiw maxLen, const char *cp_fmt, va_list args );
	
    //  won't append string null-terminator
    EXTERNALD uiw PrintToMemoryStreamArgList( MemoryStreamInterface &stream, const char *cp_fmt, va_list args );

	EXTERNALD uiw PrintToFileArgList( FileInterface &file, const char *cp_fmt, va_list args );

    struct _ArgType  //  for debug only
    {
		uiw size;
		bool is_pointer;
		bool is_fp;
		bool is_string;

		_ArgType( uiw size = 0, bool is_pointer = false, bool is_fp = false, bool is_string = false ) :
			size( size ), is_pointer( is_pointer ), is_fp( is_fp ), is_string( is_string )
		{}
    };

	#undef PrintToStr
	#undef PrintToMemoryStream
	#undef PrintToFile

	inline uiw PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, ... )
	{
        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = PrintToStrArgList( p_str, maxLen, cp_fmt, variadic );
		va_end( variadic );

		return printed;
	}
    
    //  won't append string null-terminator
	inline uiw PrintToMemoryStream( MemoryStreamInterface &stream, const char *cp_fmt, ... )
	{
        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = PrintToMemoryStreamArgList( stream, cp_fmt, variadic );
        va_end( variadic );

        return stream.Resize( printed );  //  removing excessive size( if any )
	}
    
	inline uiw PrintToFile( FileInterface &file, const char *cp_fmt, ... )
	{
        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = PrintToFileArgList( file, cp_fmt, variadic );
        va_end( variadic );

        return printed;
	}
	
#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
    EXTERNALD bool _PrintCheckArgs( const _ArgType *argTypes, uiw argsCount, const char *cp_fmt, ... );

    template < typename X > _ArgType _AnalyzeArg( const X &arg )
    {
        STATIC_CHECK( TypeDesc < X >::is_pod, "you can't pass non-pod args to variadic functions" );
        STATIC_CHECK( TypeDesc < X >::is_array || (sizeof(X) > 0 && sizeof(X) <= 8), "you can't use weird-sized types with variadic functions" );
		typedef typename TypeDesc < X >::type underlyingTypeX;
		_ArgType argType = { sizeof(X), false, false, false };
        if( TypeDesc < X >::is_fp )
        {
			argType.is_fp = true;
            return argType;
        }
		if( TypeDesc < X >::is_pointer || TypeDesc < X >::is_array )
		{
			argType.size = sizeof(uiw); //  when array, size can be off
			argType.is_pointer = true;
			if( std::is_same < underlyingTypeX, char >() )
			{
				argType.is_string = true;
			}
			return argType;
		}
        if( TypeDesc < X >::is_integer || std::is_same < underlyingTypeX, bool >() || std::is_same < underlyingTypeX, char >() )
        {
			if( sizeof(X) < sizeof(int) )
			{
				argType.size = sizeof(int);
			}
			return argType;
        }
        SOFTBREAK;
        return _ArgType();
    }

    template < typename... Args > bool _AreArgsValid( const char *cp_fmt, const Args &... args )
    {
        static const _ArgType argTypes[] = { _ArgType(), _AnalyzeArg( args )... };
        return _PrintCheckArgs( argTypes + 1, COUNTOF( argTypes ) - 1, cp_fmt, args... );
    }

    template < typename... Args > uiw PrintToStrDebug( char *p_str, uiw maxLen, const char *cp_fmt, const Args &... args )
    {
		ASSUME( maxLen );

		if( !_AreArgsValid( cp_fmt, args... ) )
		{
			SOFTBREAK;
			return 0;
		}

		return PrintToStr( p_str, maxLen, cp_fmt, args... );
    }

    template < typename... Args > uiw PrintToMemoryStreamDebug( MemoryStreamInterface &stream, const char *cp_fmt, const Args &... args )
    {
		if( !_AreArgsValid( cp_fmt, args... ) )
		{
			SOFTBREAK;
			return 0;
		}

		return PrintToMemoryStream( stream, cp_fmt, args... );
    }

    template < typename... Args > uiw PrintToFileDebug( FileInterface &file, const char *cp_fmt, const Args &... args )
    {
		if( !_AreArgsValid( cp_fmt, args... ) )
		{
			SOFTBREAK;
			return 0;
		}

		return PrintToFile( file, cp_fmt, args... );
    }

	#define PrintToStr PrintToStrDebug
	#define PrintToMemoryStream PrintToMemoryStreamDebug
	#define PrintToFile PrintToFileDebug
#endif
}}

#endif