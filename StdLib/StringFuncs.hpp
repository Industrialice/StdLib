#ifndef __STRING_FUNCS_HPP__
#define __STRING_FUNCS_HPP__

#ifdef DEBUG
    #include <typeinfo>
#endif

#include "MemoryStreamInterface.hpp"
#include "FileInterface.hpp"

#define DEBUG_VALIDATE_PRINT_FUNCS

/*  0b or 0B is binary base  */
/*  0z or 0Z is octal base  */

/*
**  about e-notation: it can start from digit or from dot
**  before e must go at least one digit
**  after e can go sign. if not present, + is assumed
**  after e must go at least one digit
**  most compact form is 4 chars
*/

namespace StdLib {
namespace Funcs
{
    EXTERNALS uiw NormalizeMem32( ui32 val, char *p_buf );  //  returns buffer size after normalization
    EXTERNALS uiw NormalizeMem64( ui64 val, char *p_buf );  //  returns buffer size after normalization
    EXTERNALS uiw NormalizeMemWord( uiw val, char *p_buf );  //  returns buffer size after normalization

    typedef bln (*ChrTestFunc)( char );

    EXTERNALS bln IsChrUpperAlpha( char source );
    EXTERNALS bln IsChrLowerAlpha( char source );
    EXTERNALS bln IsChrAlpha( char source );
    EXTERNALS bln IsChrDec( char source );
    EXTERNALS bln IsChrHex( char source );
    EXTERNALS bln IsChrOct( char source );
    EXTERNALS bln IsChrBin( char source );

    //  all with bases but without signes, must contain at least one digit
    EXTERNALD bln IsStrHex( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNALD bln IsStrBin( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNALD bln IsStrFP( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNALD bln IsStrDec( const char *cp_str, uiw maxLen = uiw_max );

    EXTERNALS char ChrToLower( char source );
    EXTERNALS char ChrToUpper( char source );

    EXTERNALS bln IsChrEqual( char one, char two );  //  compare chars
    EXTERNALS bln IsChrEqualWOR( char one, char two );  //  compare chars without register

    //  maxLen without zero symbol

    EXTERNALS uiw StrToLower( char *RSTR p_dest, const char *cp_source );
    EXTERNALS uiw StrToLowerAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNALS uiw StrToLowerInplace( char *p_str );
    EXTERNALS uiw StrToLowerInplaceAdv( char *p_str, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNALS uiw StrToUpper( char *RSTR p_dest, const char *cp_source );
    EXTERNALS uiw StrToUpperAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNALS uiw StrToUpperInplace( char *p_str );
    EXTERNALS uiw StrToUpperInplaceAdv( char *p_str, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNALS bln StrEqual( const char *cp_one, const char *cp_two );  //  true if both strings are zero
    EXTERNALS bln StrEqualAdv( const char *cp_one, const char *cp_two, uiw maxLen = uiw_max, char aes = '\0' );  //  true if both strings are zero
    #define _StrEqual( one, two ) (!::strcmp( one, two ))
    EXTERNALS bln StrNEqual( const char *cp_one, const char *cp_two, uiw count );  //  true if both strings are zero or count is zero
    EXTERNALS bln StrNEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes = '\0' );  //  true if both strings are zero or count is zero
    #define _StrNEqual( one, two, count ) (!::strncmp( one, two, count ))
    EXTERNALS bln StrIEqual( const char *cp_one, const char *cp_two );  //  true if both strings are zero
    EXTERNALS bln StrIEqualAdv( const char *cp_one, const char *cp_two, uiw maxLen = uiw_max, char aes = '\0' );  //  true if both strings are zero
    EXTERNALS bln StrINEqual( const char *cp_one, const char *cp_two, uiw count );  //  true if both strings are zero or count is zero
    EXTERNALS bln StrINEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes = '\0' );  //  true if both strings are zero or count is zero
    EXTERNALS uiw StrLen( const char *cp_str );
    EXTERNALS uiw StrLenAdv( const char *cp_str, uiw maxLen = uiw_max, char aes = '\0' );
    #define _StrLen( str ) ::strlen( str )
    EXTERNALS void StrCat( char *RSTR p_dest, const char *cp_source );
    EXTERNALS uiw StrCatAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aesDest = '\0', char aesSrc = '\0' );
    #define _StrCat( dest, source ) (void)::strcat( dest, source )
    EXTERNALS void StrCpy( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate = true );
    EXTERNALS uiw StrCpyAdv( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate = true, uiw maxLen = uiw_max, char aes = '\0' );
    #define _StrCpy( dest, source ) (void)::strcpy( dest, source )
	EXTERNALS uiw StrCpyAndCount( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate = true );
	EXTERNALS uiw StrCpyAndCountAdv( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate = true, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNALS uiw StrSafeCpyAndCount( char *RSTR p_dest, const char *cp_source, uiw maxLen, bln is_nullTerminate = true );
	EXTERNALS uiw StrSafeCpyAndCountAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen, bln is_nullTerminate = true, char aes = '\0' );
    EXTERNALS uiw StrDelim( char **pp_output, char *p_source, char delim );  //  you can pass null as pp_output only to get a number of the final strings
    EXTERNALS uiw StrDelimConst( char **pp_output, const char *cp_source, char delim );  //  TODO: incomplete  //  you can pass null as pp_output only to get a number of the final strings
    EXTERNALS char *StrChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    #define _StrChr( source, symbol ) ((char *)::strchr( source, symbol ))
    EXTERNALS char *StrNotChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrNotChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrChrs( const char *cp_source, const char *cp_chars );
    EXTERNALS char *StrChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS char *StrNotChrs( const char *cp_source, const char *cp_chars );
    EXTERNALS char *StrNotChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS char *StrIChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrINotChrs( const char *cp_source, const char *cp_chars );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrINotChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrINotChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrINotChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIChrs( const char *cp_source, const char *cp_chars );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrChrMask( const char *cp_source, char mask, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIChrMask( const char *cp_source, char mask, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrStr( const char *cp_source, const char *cp_under );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrStrAdv( const char *cp_source, const char *cp_under, uiw countSrc = uiw_max, uiw countUnder = uiw_max, char aesSrc = '\0', char aesUnder = '\0' );  //  returning value of the same string - assume it is const if source is const
    #define _StrStr( source, under ) ((char *)::strstr( source, under ))
    EXTERNALS char *StrIStr( const char *cp_source, const char *cp_under );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS char *StrIStrAdv( const char *cp_source, const char *cp_under, uiw countSrc = uiw_max, uiw countUnder = uiw_max, char aesSrc = '\0', char aesUnder = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNALS bln IsStrIsFromThisSymbols( const char *cp_str, const char *cp_symbols );  //  true if both have zero length, false if only one of them have zero length
    EXTERNALS bln IsStrIsFromThisSymbol( const char *cp_str, char symbol, uiw count = uiw_max );  //  false on zero length string
    EXTERNALS bln IsStrMatch( const char *cp_str, ChrTestFunc func );  //  false on zero length string
    EXTERNALS bln IsStrMatchAdv( const char *cp_str, ChrTestFunc func, uiw count = uiw_max, char aes = '\0' );  //  false on zero length string
    EXTERNALS uiw StrExclude( char *RSTR p_target, const char *cp_source, char symbol );  //  you can pass null as p_target to get only proced len
    EXTERNALS uiw StrExcludeAdv( char *RSTR p_target, const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  you can pass null as p_target to get only proced len
    EXTERNALS uiw StrExcludeMask( char *RSTR p_target, const char *cp_source, char mask, char symbol );  //  you can pass null as p_target to get only proced len
    EXTERNALS uiw StrExcludeMaskAdv( char *RSTR p_target, const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  you can pass null as p_target to get only proced len
    EXTERNALS uiw StrExcludeInplace( char *p_str, char symbol );
    EXTERNALS uiw StrExcludeInplaceAdv( char *p_str, char symbol, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS uiw StrExcludeMaskInplace( char *p_str, char mask, char symbol );
    EXTERNALS uiw StrExcludeMaskInplaceAdv( char *p_str, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );

    //  *p_value is not changed if no suitable conversion is possible TODO: add cp_str capacity info
    EXTERNALS bln StrDecToI32Quest( const char *cp_str, i32 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS bln StrDecToUI32Quest( const char *cp_str, ui32 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS bln StrDecToI64Quest( const char *cp_str, i64 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS bln StrDecToUI64Quest( const char *cp_str, ui64 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS bln StrDecToIWQuest( const char *cp_str, iw *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS bln StrDecToUIWQuest( const char *cp_str, uiw *p_value, uiw count = uiw_max, char aes = '\0' );

    //  TODO: add cp_str capacity info
    EXTERNALS i32  StrDecToI32( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS ui32 StrDecToUI32( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS i64  StrDecToI64( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS ui64 StrDecToUI64( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS iw   StrDecToIW( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNALS uiw  StrDecToUIW( const char *cp_str, uiw count = uiw_max, char aes = '\0' );

    EXTERNALS f32 StrToF32( const char *cp_str );
    EXTERNALS f64 StrToF64( const char *cp_str );

    EXTERNALS bln IsHexBase( const char *cp_str );
    EXTERNALS bln IsOctBase( const char *cp_str );
    EXTERNALS bln IsBinBase( const char *cp_str );

    EXTERNALS uiw F32ToStr( f32 val, char *p_buf );
    EXTERNALS uiw F32ToStrWithPrecise( f32 val, ui32 precise, char *p_buf );
    EXTERNALS uiw F64ToStr( f64 val, char *p_buf );
    EXTERNALS uiw F64ToStrWithPrecise( f64 val, ui32 precise, char *p_buf );
	
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
    EXTERNALD uiw PrintToMemoryStreamArgList( MemoryStreamInterface *stream, const char *cp_fmt, va_list args );

	EXTERNALD uiw PrintToFileArgList( FileInterface *file, const char *cp_fmt, va_list args );

    struct _ArgType  //  for debug only
    {
		uiw size;
		bln is_pointer;
		bln is_fp;
		bln is_string;

		_ArgType( uiw size = 0, bln is_pointer = false, bln is_fp = false, bln is_string = false ) :
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
	inline uiw PrintToMemoryStream( MemoryStreamInterface *stream, const char *cp_fmt, ... )
	{
        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = PrintToMemoryStreamArgList( stream, cp_fmt, variadic );
        va_end( variadic );

        return stream->Resize( printed );  //  removing excessive size( if any )
	}
    
	inline uiw PrintToFile( FileInterface *file, const char *cp_fmt, ... )
	{
        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = PrintToFileArgList( file, cp_fmt, variadic );
        va_end( variadic );

        return printed;
	}
	
#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
    EXTERNALD bln _PrintCheckArgs( const _ArgType *argTypes, uiw argsCount, const char *cp_fmt, ... );

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
			if( AreTypesTheSame < underlyingTypeX, char >() )
			{
				argType.is_string = true;
			}
			return argType;
		}
        if( TypeDesc < X >::is_integer || AreTypesTheSame < underlyingTypeX, bln >() || AreTypesTheSame < underlyingTypeX, char >() )
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

    template < typename... Args > bln _AreArgsValid( const char *cp_fmt, const Args &... args )
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

    template < typename... Args > uiw PrintToMemoryStreamDebug( MemoryStreamInterface *stream, const char *cp_fmt, const Args &... args )
    {
		if( !_AreArgsValid( cp_fmt, args... ) )
		{
			SOFTBREAK;
			return 0;
		}

		return PrintToMemoryStream( stream, cp_fmt, args... );
    }

    template < typename... Args > uiw PrintToFileDebug( FileInterface *file, const char *cp_fmt, const Args &... args )
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

    template < ChrTestFunc func > bln IsStrMatchT( const char *cp_str, uiw count = uiw_max )
    {
        ASSUME( cp_str && func );
        bln is_match = false;
        while( *cp_str && count )
        {
            is_match = func( *cp_str );
            if( !is_match )
            {
                break;
            }
            --count;
            ++cp_str;
        }
        return is_match;
    }

    //  without base, *p_value is not changed if no suitable conversion possible, count sets a limit to cp_str len
    template < typename X > NOINLINE bln StrHexToIntQuest( const char *cp_str, X *p_value, uiw count = uiw_max )
    {
        ASSUME( cp_str && p_value );
        X value = 0;
        uiw readed = 0;
        while( *cp_str && readed < count && readed < sizeof(X) * 2 )
        {
            value <<= 4;
            if( Funcs::IsChrDec( *cp_str ) )
            {
                value |= *cp_str - '0';
            }
            else if( Funcs::IsChrLowerAlpha( *cp_str ) )
            {
                value |= *cp_str - 'a' + 10;
            }
            else if( Funcs::IsChrUpperAlpha( *cp_str ) )
            {
                value |= *cp_str - 'A' + 10;
            }
            else
            {
                return false;
            }
            ++readed;
            ++cp_str;
        }
        *p_value = value;
        return true;
    }

    //  without base, count sets a limit to cp_str len
    template < typename X > X StrHexToInt( const char *cp_str, uiw count = uiw_max )
    {
        ASSUME( cp_str );
        X value = 0;
        StrHexToIntQuest( cp_str, &value, count );
        return value;
    }

    //  without base, *p_value is not changed if no suitable conversion possible, count sets a limit to cp_str len
    template < typename X > NOINLINE bln StrBinToIntQuest( const char *cp_str, X *p_value, uiw count = uiw_max )
    {
        ASSUME( cp_str && p_value );
        X value = 0;
        uiw readed = 0;
        while( *cp_str && readed < count && readed < sizeof(X) * 8 )
        {
            if( *cp_str == '_' )
            {
                --count;
                ++cp_str;
                continue;
            }
            value <<= 1;
            if( *cp_str == '1' )
            {
                value |= 1;
            }
            else if( *cp_str != '0' )
            {
                return false;
            }
            ++readed;
            ++cp_str;
        }
        *p_value = value;
        return true;
    }

    //  without base, count sets a limit to cp_str len
    template < typename X > X StrBinToInt( const char *cp_str, uiw count = uiw_max )
    {
        ASSUME( cp_str );
        X value = 0;
        StrBinToIntQuest( cp_str, &value, count );
        return value;
    }

    //  p_buf must be able to contain at least sizeof(X) * 2 + 1 bytes
    template < typename X > NOINLINE uiw IntToStrHex( bln is_upper, bln is_setBase, bln is_dropFrontZeroes, char *p_buf, X val )
    {
        ASSUME( p_buf );
        const char *cp_set = "0123456789abcdef0123456789ABCDEF" + (is_upper << 4);
        char *p_sourceBuf = p_buf;
        if( is_setBase )
        {
            *p_buf++ = '0';
            *p_buf++ = 'x';
        }
        for( typename TypeDesc < X >::uint_variant shift = sizeof(X) * 8 - 4; shift <= sizeof(X) * 8 - 4; shift -= 4 )
        {
            typename TypeDesc < X >::uint_variant half_byte = val >> shift & 0xF;
            if( is_dropFrontZeroes && !half_byte )
            {
                continue;
            }
            is_dropFrontZeroes = false;
            *p_buf++ = cp_set[ half_byte ];
        }
        *p_buf = '\0';
        return p_buf - p_sourceBuf;
    }

    //  p_buf must be able to contain at least sizeof(X) * 8 + 1 bytes
    template < typename X > NOINLINE uiw IntToStrBin( X val, char *p_buf, bln is_setBase = false, bln is_dropFrontZeroes = false  )
    {
        ASSUME( p_buf );
        char *p_sourceBuf = p_buf;
        if( is_setBase )
        {
            *p_buf++ = '0';
            *p_buf++ = 'b';
        }
        for( typename TypeDesc < X >::uint_variant test = (typename TypeDesc < X >::uint_variant)1 << TypeDesc < X >::bits - 1; test; test >>= 1 )
        {
            X tested = val & test;
            bln is_nzero = tested != 0;
            if( is_dropFrontZeroes && !is_nzero )
            {
                continue;
            }
            is_dropFrontZeroes = false;
            *p_buf++ = '0' + is_nzero;
        }
        *p_buf = '\0';
        return p_buf - p_sourceBuf;
    }

    //  TODO: p_buf must be able to contain at least TypeDesc < X >::decDigits + 2 bytes
    template < typename X > NOINLINE uiw IntToStrDec( X val, char *const p_buf )
    {
        ASSUME( p_buf );
        char a_buf[ TypeDesc < X >::decDigits + 2 ];
        uiw index = 0;
        char *p_bufNew = p_buf;

        if( TypeDesc < X >::is_signed )
        {
            if( val < 0 )
            {
                *p_bufNew++ = '-';
            }
            for( ; ; )
            {
                a_buf[ index++ ] = '0' + abs( (iw)(val % 10) );
                val /= 10;
                if( !val )
                {
                    break;
                }
            }
        }
        else
        {
            for( ; ; )
            {
                a_buf[ index++ ] = '0' + val % 10;
                if( val < 10 )
                {
                    break;
                }
                val /= 10;
            }
        }

        do
        {
            *p_bufNew++ = a_buf[ --index ];
        } while( index );
        *p_bufNew = '\0';

        return p_bufNew - p_buf;
    }

    //  TODO: p_buf must be able to contain at least TypeDesc < X >::decDigits + 2 bytes
    //  use this function when you know exactly how many chars val is going to take
    template < typename X > NOINLINE char *IntToStrDecAlt( X val, char *p_buf, uiw bufSize )
    {
        ASSUME( p_buf && bufSize >= TypeDesc < X >::decDigits + 2 );
        char *p_end = p_buf + bufSize - 1;
        *p_end-- = '\0';

        if( TypeDesc < X >::is_signed )
        {
            bln is_lessZero = val < 0;
            for( ; ; )
            {
                *p_end = '0' + abs( (iw)(val % 10) );
                val /= 10;
                if( !val )
                {
                    break;
                }
                --p_end;
            }

            if( is_lessZero )
            {
                --p_end;
                *p_end = '-';
            }
        }
        else
        {
            for( ; ; )
            {
                *p_end = '0' + val % 10;
                if( val < 10 )
                {
                    break;
                }
                val /= 10;
                --p_end;
            }
        }

        return p_end;
    }
} }

#endif