#include "PreHeader.hpp"
#include "StringFuncs.hpp"
#include "FileMemoryStream.hpp"

#include <cstdio>

template < typename X > NOINLINE uiw NormalizeMem( X val, char *buf )
{
	ASSUME( buf );

	f64 normalizedValue;
	const char *execStr;

	if( val < 1024 )
	{
		normalizedValue = val;
		execStr = "%lg B";
	}
	else if( val < 1024 * 1024 )
	{
		normalizedValue = val / (f64)1024;
		execStr = "%lg KB";
	}
	else if( val < (1024ull * 1024ull * 1024ull) )
	{
		normalizedValue = val / (f64)(1024ull * 1024ull);
		execStr = "%lg MB";
	}
	else if( val < (1024ull * 1024ull * 1024ull * 1024ull) )
	{
		normalizedValue = val / (f64)(1024ull * 1024ull * 1024ull);
		execStr = "%lg GB";
	}
	else
	{
		normalizedValue = val / (f64)(1024ull * 1024ull * 1024ull * 1024ull);
		execStr = "%lg TB";
	}

	return ::sprintf( buf, execStr, normalizedValue );
}

uiw Funcs::NormalizeMem32( ui32 val, char *p_buf )
{
	return NormalizeMem( val, p_buf );
}

uiw Funcs::NormalizeMem64( ui64 val, char *p_buf )
{
	return NormalizeMem( val, p_buf );
}

uiw Funcs::NormalizeMemWord( uiw val, char *p_buf )
{
	return NormalizeMem( val, p_buf );
}

bln Funcs::IsChrUpperAlpha( char source )
{
    return source >= 'A' && source <= 'Z';
}

bln Funcs::IsChrLowerAlpha( char source )
{
    return source >= 'a' && source <= 'z';
}

bln Funcs::IsChrAlpha( char source )
{
    return source >= 'a' && source <= 'z' || source >= 'A' && source <= 'Z';
}

bln Funcs::IsChrDec( char source )
{
    return source >= '0' && source <= '9';
}

bln Funcs::IsChrHex( char source )
{
    return source >= '0' && source <= '9' || source >= 'A' && source <= 'F' || source >= 'a' && source <= 'f';
}

bln Funcs::IsChrOct( char source )
{
    return source >= '0' && source <= '7';
}

bln Funcs::IsChrBin( char source )
{
    return source == '0' || source == '1';
}

char Funcs::ChrToLower( char source )
{
    return source >= 'A' && source <= 'Z' ? source | 32 : source;
}

char Funcs::ChrToUpper( char source )
{
    return source >= 'a' && source <= 'z' ? source & ~32 : source;
}

bln Funcs::IsChrEqual( char one, char two )
{
    return one == two;
}

bln Funcs::IsChrEqualWOR( char one, char two )
{
    return ChrToLower( one ) == ChrToLower( two );
}

bln Funcs::IsStrHex( const char *cp_str, uiw maxLen /* = uiw_max */ )
{
    ASSUME( cp_str );
    if( maxLen < 3 || !Funcs::IsHexBase( cp_str ) || !cp_str[ 2 ] )
    {
        return false;
    }
    cp_str += 2;
    for( ; ; )
    {
        if( !Funcs::IsChrHex( *cp_str ) )
        {
            return false;
        }
        ++cp_str;
        --maxLen;
        if( !maxLen || !*cp_str )
        {
            break;
        }
    }
    return true;
}

bln Funcs::IsStrBin( const char *cp_str, uiw maxLen /* = uiw_max */ )
{
    ASSUME( cp_str );
    if( maxLen < 3 || !Funcs::IsBinBase( cp_str ) || !cp_str[ 2 ] )
    {
        return false;
    }
    cp_str += 2;
    for( ; ; )
    {
        if( !Funcs::IsChrBin( *cp_str ) )
        {
            return false;
        }
        ++cp_str;
        --maxLen;
        if( !maxLen || !*cp_str )
        {
            break;
        }
    }
    return true;
}

bln Funcs::IsStrFP( const char *cp_str, uiw maxLen /* = uiw_max */ )
{
    ASSUME( cp_str );
    if( !maxLen )
    {
        return false;
    }
    bln is_digitBeforeE = false;
    bln is_dotSpotted = false;
    bln is_digitAfterE = false;
    for( ; ; )
    {
        char ch = *cp_str++;
        --maxLen;
        if( Funcs::IsChrDec( ch ) )
        {
            is_digitBeforeE = true;
        }
        else if( ch == '.' )
        {
            if( is_dotSpotted )
            {
                return false;
            }
            is_dotSpotted = true;
        }
        else if( ch == 'e' || ch == 'E' )
        {
            break;
        }
        else
        {
            return false;
        }
        if( !maxLen || !*cp_str )
        {
            return is_digitBeforeE;
        }
    }
    if( !maxLen || !is_digitBeforeE )
    {
        return false;
    }
    if( *cp_str == '+' || *cp_str == '-' )
    {
        --maxLen;
        ++cp_str;
    }
    for( ; maxLen && *cp_str; --maxLen, ++cp_str )
    {
        if( !Funcs::IsChrDec( *cp_str ) )
        {
            return false;
        }
        is_digitAfterE = true;
    }
    return is_digitAfterE;
}

bln Funcs::IsStrDec( const char *cp_str, uiw maxLen /* = uiw_max */ )
{
    ASSUME( cp_str );
    if( !maxLen || !*cp_str )
    {
        return false;
    }
    for( ; ; )
    {
        if( !Funcs::IsChrDec( *cp_str ) )
        {
            return false;
        }
        ++cp_str;
        --maxLen;
        if( !maxLen || !*cp_str )
        {
            break;
        }
    }
    return true;
}

uiw Funcs::StrToLower( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( *cp_source )
    {
        *p_dest++ = ChrToLower( *cp_source++ );
        ++cpy;
    }
    *p_dest = '\0';
    return cpy;
}

uiw Funcs::StrToLowerAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = ChrToLower( *cp_source++ );
        ++cpy;
    }
    *p_dest = '\0';
    return cpy;
}

uiw Funcs::StrToLowerInplace( char *p_str )
{
    ASSUME( p_str );
    uiw cpy = 0;
    while( *p_str )
    {
        *p_str = ChrToLower( *p_str );
        ++p_str;
        ++cpy;
    }
    return cpy;
}

uiw Funcs::StrToLowerInplaceAdv( char *p_str, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_str );
    uiw cpy = 0;
    while( cpy < maxLen && *p_str && *p_str != aes )
    {
        *p_str = ChrToLower( *p_str );
        ++p_str;
        ++cpy;
    }
    return cpy;
}

uiw Funcs::StrToUpper( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( *cp_source )
    {
        *p_dest++ = ChrToUpper( *cp_source++ );
        ++cpy;
    }
    *p_dest = '\0';
    return cpy;
}

uiw Funcs::StrToUpperAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = ChrToUpper( *cp_source++ );
        ++cpy;
    }
    *p_dest = '\0';
    return cpy;
}

uiw Funcs::StrToUpperInplace( char *p_str )
{
    ASSUME( p_str );
    uiw cpy = 0;
    while( *p_str )
    {
        *p_str = ChrToUpper( *p_str );
        ++p_str;
        ++cpy;
    }
    return cpy;
}

uiw Funcs::StrToUpperInplaceAdv( char *p_str, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_str );
    uiw cpy = 0;
    while( cpy < maxLen && *p_str && *p_str != aes )
    {
        *p_str = ChrToUpper( *p_str );
        ++p_str;
        ++cpy;
    }
    return cpy;
}

bln Funcs::StrEqual( const char *cp_one, const char *cp_two )
{
    ASSUME( cp_one && cp_two );
    uiw cpy = 0;
    while( *cp_one && *cp_two )
    {
        if( *cp_one != *cp_two )
        {
            return false;
        }
        ++cpy;
    }
    return *cp_one == *cp_two;
}

bln Funcs::StrEqualAdv( const char *cp_one, const char *cp_two, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_one && cp_two );
    uiw cpy = 0;
    while( cpy < maxLen && cp_one && *cp_two && *cp_one != aes && *cp_two != aes )
    {
        if( *cp_one != *cp_two )
        {
            return false;
        }
        ++cpy;
    }
    if( cpy == maxLen )
    {
        return true;
    }
    return *cp_one == *cp_two;
}

bln Funcs::StrNEqual( const char *cp_one, const char *cp_two, uiw count )
{
    ASSUME( cp_one && cp_two );
    for( ; count && *cp_two; --count )
    {
        if( *cp_one != *cp_two )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return true;
}

bln Funcs::StrNEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes /* = '\0' */ )
{
    ASSUME( cp_one && cp_two );
    for( ; count && *cp_two && *cp_two != aes; --count )
    {
        if( *cp_one != *cp_two )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return true;
}

bln Funcs::StrIEqual( const char *cp_one, const char *cp_two )
{
    ASSUME( cp_one && cp_two );
    while( *cp_one && *cp_two )
    {
        if( !Funcs::IsChrEqualWOR( *cp_one, *cp_two ) )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return *cp_one == *cp_two;
}

bln Funcs::StrIEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes /* = '\0' */ )
{
    ASSUME( cp_one && cp_two );
    while( *cp_one && *cp_two && *cp_one != aes && *cp_two != aes )
    {
        if( !Funcs::IsChrEqualWOR( *cp_one, *cp_two ) )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return *cp_one == *cp_two;
}

bln Funcs::StrINEqual( const char *cp_one, const char *cp_two, uiw count )
{
    ASSUME( cp_one && cp_two );
    for( ; count && *cp_two; --count )
    {
        if( !Funcs::IsChrEqualWOR( *cp_one, *cp_two ) )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return true;
}

bln Funcs::StrINEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes /* = '\0' */ )
{
    ASSUME( cp_one && cp_two );
    for( ; count && *cp_two && *cp_two != aes; --count )
    {
        if( !Funcs::IsChrEqualWOR( *cp_one, *cp_two ) )
        {
            return false;
        }
        ++cp_one;
        ++cp_two;
    }

    return true;
}

uiw Funcs::StrLen( const char *cp_str )
{
    ASSUME( cp_str );
    uiw len = 0;
    while( *cp_str )
    {
        ++cp_str;
        ++len;
    }
    return len;
}

uiw Funcs::StrLenAdv( const char *cp_str, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_str );
    uiw len = 0;
    while( len < maxLen && *cp_str && *cp_str != aes )
    {
        ++cp_str;
        ++len;
    }
    return len;
}

void Funcs::StrCat( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    while( *p_dest )
    {
        ++p_dest;
    }
    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
    *p_dest = '\0';
}

uiw Funcs::StrCatAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aesDest /* = '\0' */, char aesSrc /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    while( *p_dest && *p_dest != aesDest )
    {
        ++p_dest;
    }
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aesSrc )
    {
        *p_dest++ = *cp_source++;
    }
    *p_dest = '\0';
    return cpy;
}

void Funcs::StrCpy( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate )
{
    ASSUME( p_dest && cp_source );
    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
}

uiw Funcs::StrCpyAdv( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++cpy;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
    return cpy;
}

uiw Funcs::StrCpyAndCount( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate )
{
    ASSUME( p_dest && cp_source );
    const char *cp_sourceStore = cp_source;

    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
    return cp_source - cp_sourceStore;
}

uiw Funcs::StrCpyAndCountAdv( char *RSTR p_dest, const char *cp_source, bln is_nullTerminate, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++cpy;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
    return cpy;
}

uiw Funcs::StrSafeCpyAndCount( char *RSTR p_dest, const char *cp_source, uiw maxLen, bln is_nullTerminate )
{
    ASSUME( p_dest && cp_source );
    uiw index = 0;
    while( index < maxLen && *cp_source )
    {
        *p_dest++ = *cp_source++;
        ++index;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
    return index;
}

uiw Funcs::StrSafeCpyAndCountAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen, bln is_nullTerminate, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw index = 0;
    while( index < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++index;
    }
	if( is_nullTerminate )
	{
		*p_dest = '\0';
	}
    return index;
}

NOINLINE uiw Funcs::StrDelim( char **pp_output, char *p_source, char delim )
{
    ASSUME( p_source );
    uiw count = 0;
    char *p_search;
    for( ; ; )
    {
        p_search = StrChr( p_source, delim );
        if( pp_output )
        {
            ASSUME( pp_output[ count ] );
            if( p_search )
            {
                *p_search = '\0';
            }
            pp_output[ count ] = p_source;
        }
        ++count;
        if( !p_search )
        {
            break;
        }
        p_source = p_search + 1;
    }
    return count;
}

uiw Funcs::StrDelimConst( char **pp_output, const char *cp_source, char delim )
{
    ASSUME( cp_source );
	NOT_IMPLEMENTED;
    return 0;
}

char *Funcs::StrChr( const char *cp_source, char symbol )
{
    ASSUME( cp_source );
    for( ; *cp_source; ++cp_source )
    {
        if( *cp_source == symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrChrAdv( const char *cp_source, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    for( ; count && *cp_source && *cp_source != aes; ++cp_source, --count )
    {
        if( *cp_source == symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrNotChr( const char *cp_source, char symbol )
{
    ASSUME( cp_source );
    for( ; *cp_source; ++cp_source )
    {
        if( *cp_source != symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrNotChrAdv( const char *cp_source, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    for( ; count && *cp_source && *cp_source != aes; ++cp_source, --count )
    {
        if( *cp_source != symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrChrs( const char *cp_source, const char *cp_chars )
{
    ASSUME( cp_source && cp_chars );
    for( ; *cp_source; ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            if( *cp_source == *cp_charsIt )
            {
                return (char *)cp_source;
            }
        }
    }
    return 0;
}

char *Funcs::StrChrsAdv( const char *cp_source, const char *cp_chars, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source && cp_chars );
    for( ; count && *cp_source && *cp_source != aes; ++cp_source, --count )
    {
        const char *cp_charsIt = cp_chars;
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            if( *cp_source == *cp_charsIt )
            {
                return (char *)cp_source;
            }
        }
    }
    return 0;
}

char *Funcs::StrNotChrs( const char *cp_source, const char *cp_chars )
{
    ASSUME( cp_source && cp_chars );
    for( ; *cp_source; ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            if( *cp_source == *cp_charsIt )
            {
                goto noLuck;
            }
        }
        return (char *)cp_source;
        noLuck:;
    }
    return 0;
}

char *Funcs::StrNotChrsAdv( const char *cp_source, const char *cp_chars, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source && cp_chars );
    for( ; count && *cp_source && *cp_source != aes; ++cp_source, --count )
    {
        const char *cp_charsIt = cp_chars;
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            if( *cp_source == *cp_charsIt )
            {
                goto noLuck;
            }
        }
        return (char *)cp_source;
        noLuck:;
    }
    return 0;
}

char *Funcs::StrIChr( const char *cp_source, char symbol )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    for( ; *cp_source; ++cp_source )
    {
        if( ChrToLower( *cp_source ) == symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrIChrAdv( const char *cp_source, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    for( ; count && *cp_source && *cp_source != aes; --count, ++cp_source )
    {
        if( ChrToLower( *cp_source ) == symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrINotChr( const char *cp_source, char symbol )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    for( ; *cp_source; ++cp_source )
    {
        if( ChrToLower( *cp_source ) != symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrINotChrAdv( const char *cp_source, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    for( ; count && *cp_source && *cp_source != aes; --count, ++cp_source )
    {
        if( ChrToLower( *cp_source ) != symbol )
        {
            return (char *)cp_source;
        }
    }
    return 0;
}

char *Funcs::StrIChrs( const char *cp_source, const char *cp_chars )
{
    ASSUME( cp_source && cp_chars );
    for( ; *cp_source; ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        char sourceLower = ChrToLower( *cp_source );
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            char symbol = ChrToLower( *cp_charsIt );
            if( sourceLower == symbol )
            {
                return (char *)cp_source;
            }
        }
    }
    return 0;
}

char *Funcs::StrIChrsAdv( const char *cp_source, const char *cp_chars, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source && cp_chars );
    for( ; count && *cp_source && *cp_source != aes; --count, ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        char sourceLower = ChrToLower( *cp_source );
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            char symbol = ChrToLower( *cp_charsIt );
            if( sourceLower == symbol )
            {
                return (char *)cp_source;
            }
        }
    }
    return 0;
}

char *Funcs::StrINotChrs( const char *cp_source, const char *cp_chars )
{
    ASSUME( cp_source && cp_chars );
    for( ; *cp_source; ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        char sourceLower = ChrToLower( *cp_source );
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            char symbol = ChrToLower( *cp_charsIt );
            if( sourceLower == symbol )
            {
                goto noLuck;
            }
            return (char *)cp_source;
        }
        noLuck:;
    }
    return 0;
}

char *Funcs::StrINotChrsAdv( const char *cp_source, const char *cp_chars, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source && cp_chars );
    for( ; count && *cp_source && *cp_source != aes; --count, ++cp_source )
    {
        const char *cp_charsIt = cp_chars;
        char sourceLower = ChrToLower( *cp_source );
        for( ; *cp_charsIt; ++cp_charsIt )
        {
            char symbol = ChrToLower( *cp_charsIt );
            if( sourceLower == symbol )
            {
                goto noLuck;
            }
            return (char *)cp_source;
        }
        noLuck:;
    }
    return 0;
}

char *Funcs::StrChrMask( const char *cp_source, char mask, char symbol )
{
    ASSUME( cp_source );
    while( *cp_source )
    {
        if( *cp_source == mask )
        {
            if( cp_source[ 1 ] == '\0' )
            {
                return 0;
            }
            cp_source += 2;
        }
        else
        {
            if( *cp_source == symbol )
            {
                return (char *)cp_source;
            }
            ++cp_source;
        }
    }
    return 0;
}

char *Funcs::StrChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    while( count && *cp_source && *cp_source != aes )
    {
        if( *cp_source == mask )
        {
            if( count == 1 || cp_source[ 1 ] == '\0' || cp_source[ 1 ] == aes )
            {
                return 0;
            }
            count -= 2;
            cp_source += 2;
        }
        else
        {
            if( *cp_source == symbol )
            {
                return (char *)cp_source;
            }
            --count;
            ++cp_source;
        }
    }
    return 0;
}

char *Funcs::StrIChrMask( const char *cp_source, char mask, char symbol )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    while( *cp_source )
    {
        if( *cp_source == mask )
        {
            if( cp_source[ 1 ] == '\0' )
            {
                return 0;
            }
            cp_source += 2;
        }
        else
        {
            if( ChrToLower( *cp_source ) == symbol )
            {
                return (char *)cp_source;
            }
            ++cp_source;
        }
    }
    return 0;
}

char *Funcs::StrIChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    symbol = ChrToLower( symbol );
    while( count && *cp_source && *cp_source != aes )
    {
        if( *cp_source == mask )
        {
            if( count == 1 || cp_source[ 1 ] == '\0' || cp_source[ 1 ] == aes )
            {
                return 0;
            }
            count -= 2;
            cp_source += 2;
        }
        else
        {
            if( ChrToLower( *cp_source ) == symbol )
            {
                return (char *)cp_source;
            }
            ++count;
            ++cp_source;
        }
    }
    return 0;
}

char *Funcs::StrStr( const char *cp_source, const char *cp_under )
{
    ASSUME( cp_source && cp_under );
    return (char *)::strstr( cp_source, cp_under );
}

char *Funcs::StrStrAdv( const char *cp_source, const char *cp_under, uiw countSrc /* = uiw_max */, uiw countUnder /* = uiw_max */, char aesSrc /* = '\0' */, char aesUnder /* = '\0' */ )
{
    ASSUME( cp_source && cp_under );
    for( ; countSrc; --countSrc, ++cp_source )
    {
        uiw countUnderLeft = countUnder;
        uiw countSrcLeft = countSrc;
        for( const char *cp_testSrc = cp_source, *cp_testUnder = cp_under; countUnderLeft && *cp_testUnder && *cp_testUnder != aesUnder; --countUnderLeft, --countSrcLeft, ++cp_testSrc, ++cp_testUnder )
        {
            if( !countSrcLeft || !*cp_testSrc || *cp_testSrc == aesSrc )
            {
                return 0;
            }
            if( *cp_testSrc != *cp_testUnder )
            {
                goto noLuck;
            }
        }
        return (char *)cp_source;
    noLuck:;
    }
    return 0;
}

char *Funcs::StrIStr( const char *cp_source, const char *cp_under )
{
    ASSUME( cp_source && cp_under );
    for( ; ; ++cp_source )
    {
        for( const char *cp_sourceTest = cp_source, *cp_underTest = cp_under; ; ++cp_sourceTest, ++cp_underTest )
        {
            if( !*cp_underTest )
            {
                return (char *)cp_source;
            }
            if( !*cp_sourceTest )
            {
                return 0;
            }
            if( !Funcs::IsChrEqualWOR( *cp_sourceTest, *cp_underTest ) )
            {
                break;
            }
        }
    }
    UNREACHABLE;
}

char *Funcs::StrIStrAdv( const char *cp_source, const char *cp_under, uiw countSrc /* = uiw_max */, uiw countUnder /* = uiw_max */, char aesSrc /* = '\0' */, char aesUnder /* = '\0' */ )
{
    ASSUME( cp_source && cp_under );
    for( ; countSrc; --countSrc, ++cp_source )
    {
        uiw countUnderLeft = countUnder;
        uiw countSrcLeft = countSrc;
        for( const char *cp_testSrc = cp_source, *cp_testUnder = cp_under; countUnderLeft && *cp_testUnder && *cp_testUnder != aesUnder; --countUnderLeft, --countSrcLeft, ++cp_testSrc, ++cp_testUnder )
        {
            if( !countSrcLeft || !*cp_testSrc || *cp_testSrc == aesSrc )
            {
                return 0;
            }
            if( !Funcs::IsChrEqualWOR( *cp_testSrc, *cp_testUnder ) )
            {
                goto noLuck;
            }
        }
        return (char *)cp_source;
    noLuck:;
    }
    return 0;
}

NOINLINE bln Funcs::IsStrIsFromThisSymbols( const char *cp_str, const char *cp_symbols )
{
    ASSUME( cp_str && cp_symbols );
    if( !*cp_symbols )
    {
        return !*cp_str;
    }
    do
    {
        for( const char *cp_symbolsTest = cp_symbols; ; ++cp_symbolsTest )
        {
            if( !*cp_symbolsTest )
            {
                return false;
            }
            if( *cp_str == *cp_symbolsTest )
            {
                break;
            }
        }
        ++cp_str;
    } while( *cp_str );
    return true;
}

NOINLINE bln Funcs::IsStrIsFromThisSymbol( const char *cp_str, char symbol, uiw count /* = uiw_max */ )
{
    ASSUME( cp_str );
    bln is_from = false;
    while( count && *cp_str )
    {
        is_from = *cp_str++ == symbol;
        if( !is_from )
        {
            break;
        }
        --count;
    }
    return is_from;
}

NOINLINE bln Funcs::IsStrMatch( const char *cp_str, ChrTestFunc func )
{
    ASSUME( cp_str && func );
    bln is_match = false;
    while( *cp_str )
    {
        is_match = func( *cp_str );
        if( !is_match )
        {
            break;
        }
        ++cp_str;
    }
    return is_match;
}

NOINLINE bln Funcs::IsStrMatchAdv( const char *cp_str, ChrTestFunc func, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_str && func );
    bln is_match = false;
    while( count && *cp_str && *cp_str != aes )
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

uiw Funcs::StrExclude( char *RSTR p_target, const char *cp_source, char symbol )
{
    ASSUME( cp_source );
    uiw len = 0;
    while( *cp_source )
    {
        if( *cp_source != symbol )
        {
            ++len;
            if( p_target )
            {
                *p_target++ = *cp_source;
            }
        }
        ++cp_source;
    }
    if( p_target )
    {
        *p_target = '\0';
    }
    return len;
}

uiw Funcs::StrExcludeAdv( char *RSTR p_target, const char *cp_source, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    uiw len = 0;
    while( count && *cp_source && *cp_source != aes )
    {
        if( *cp_source != symbol )
        {
            ++len;
            if( p_target )
            {
                *p_target++ = *cp_source;
            }
        }
        ++cp_source;
        --count;
    }
    if( p_target )
    {
        *p_target = '\0';
    }
    return len;
}

uiw Funcs::StrExcludeMask( char *RSTR p_target, const char *cp_source, char mask, char symbol )
{
    ASSUME( cp_source );
    uiw len = 0;
    bln is_masked = false;
    for( ; *cp_source; ++cp_source )
    {
        if( !is_masked && *cp_source == mask )
        {
            is_masked = true;
            continue;
        }
        if( is_masked || *cp_source != symbol )
        {
            ++len;
            if( p_target )
            {
                *p_target++ = *cp_source;
            }
        }
        is_masked = false;
    }
    if( p_target )
    {
        *p_target = '\0';
    }
    return len;
}

uiw Funcs::StrExcludeMaskAdv( char *RSTR p_target, const char *cp_source, char mask, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( cp_source );
    uiw len = 0;
    bln is_masked = false;
    for( ; count && *cp_source && *cp_source != aes; ++cp_source, --count )
    {
        if( !is_masked && *cp_source == mask )
        {
            is_masked = true;
            continue;
        }
        if( is_masked || *cp_source != symbol )
        {
            ++len;
            if( p_target )
            {
                *p_target++ = *cp_source;
            }
        }
        is_masked = false;
    }
    if( p_target )
    {
        *p_target = '\0';
    }
    return len;
}

uiw Funcs::StrExcludeInplace( char *const p_str, char symbol )
{
    ASSUME( p_str );
    char *p_target = p_str, *p_source = p_str;
    for( ; *p_source; ++p_source )
    {
        if( *p_source != symbol )
        {
            *p_target++ = *p_source;
        }
    }
    *p_target = '\0';
    return p_target - p_str;
}

uiw Funcs::StrExcludeInplaceAdv( char *const p_str, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_str );
    char *p_target = p_str, *p_source = p_str;
    for( ; count && *p_source && *p_source != aes; ++p_source, --count )
    {
        if( *p_source != symbol )
        {
            *p_target++ = *p_source;
        }
    }
    *p_target = '\0';
    return p_target - p_str;
}

uiw Funcs::StrExcludeMaskInplace( char *p_str, char mask, char symbol )
{
    ASSUME( p_str );
    char *p_target = p_str, *p_source = p_str;
    bln is_masked = false;
    for( ; *p_source; ++p_source )
    {
        if( !is_masked && *p_source == mask )
        {
            is_masked = true;
            continue;
        }
        if( is_masked || *p_source != symbol )
        {
            *p_target++ = *p_source;
        }
        is_masked = false;
    }
    *p_target = '\0';
    return p_target - p_str;
}

uiw Funcs::StrExcludeMaskInplaceAdv( char *p_str, char mask, char symbol, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_str );
    char *p_target = p_str, *p_source = p_str;
    bln is_masked = false;
    for( ; count && *p_source && *p_source != aes; ++p_source, --count )
    {
        if( !is_masked && *p_source == mask )
        {
            is_masked = true;
            continue;
        }
        if( is_masked || *p_source != symbol )
        {
            *p_target++ = *p_source;
        }
        is_masked = false;
    }
    *p_target = '\0';
    return p_target - p_str;
}

//  *p_value is not changed if no suitable conversion possible TODO: add cp_str capacity info
template < typename X > NOINLINE bln StrDecToIntQuest( const char *cp_str, X *p_value, uiw count = uiw_max, char aes = '\0' )
{
    ASSUME( cp_str && p_value );
    bln is_minus = *cp_str == '-';
    cp_str += is_minus || *cp_str == '+';
    uiw len = Funcs::StrLenAdv( cp_str, count, aes );
    typedef typename TypeDesc < X >::uint_variant ux_t;
    ux_t mult = 1;
    ux_t uval = 0;
    ux_t uvalLast = 0;
    for( count = len - 1; count != uiw_max; --count )
    {
        if( !Funcs::IsChrDec( cp_str[ count ] ) )
        {
            /*if( cp_str[ count ] == '.' )
            {
                break;
            }*/
            return false;
        }
        uval += (cp_str[ count ] - '0') * mult;
        if( uval < uvalLast )
        {
            return false;
        }
        uvalLast = uval;
        mult *= 10;
    }
    ux_t allowedMax = (ux_t)TypeDesc < X >::max;
    if( is_minus )
    {
        if( uval > allowedMax )
        {
            if( uval > allowedMax + 1 )
            {
                return false;
            }
            else
            {
                --uval;
                *p_value = (-(X)uval) - 1;
            }
        }
        else
        {
            *p_value = -(X)uval;
        }
    }
    else
    {
        if( uval > allowedMax )
        {
            return false;
        }
        *p_value = (X)uval;
    }
    return true;
}

template < typename X > X StrDecToInt( const char *cp_str, uiw count = uiw_max, char aes = '\0' )
{
    ASSUME( cp_str );
    X value = 0;
    StrDecToIntQuest( cp_str, &value, count, aes );
    return value;
}

//  *p_value is not changed if no suitable conversion possible TODO: add cp_str capacity info
template < typename X > NOINLINE bln StrDecToUIntQuest( const char *cp_str, X *p_value, uiw count = uiw_max, char aes = '\0' )
{
    ASSUME( cp_str && p_value );
    uiw len = Funcs::StrLenAdv( cp_str, count, aes );
    X val = 0;
    X mult = 1;
    cp_str += *cp_str == '+';
    for( count = len - 1; count != uiw_max; --count )
    {
        if( !Funcs::IsChrDec( cp_str[ count ] ) )
        {
            /*if( cp_str[ count ] == '.' )
            {
                break;
            }*/
            return false;
        }
        val += (cp_str[ count ] - '0') * mult;
        mult *= 10;
    }
    *p_value = val;
    return true;
}

template < typename X > X StrDecToUInt( const char *cp_str, uiw count = uiw_max, char aes = '\0' )
{
    ASSUME( cp_str );
    X value = 0;
    StrDecToUIntQuest( cp_str, &value, count, aes );
    return value;
}

bln Funcs::StrDecToI32Quest( const char *cp_str, i32 *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToIntQuest( cp_str, p_value, count, aes );
}

bln Funcs::StrDecToUI32Quest( const char *cp_str, ui32 *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUIntQuest( cp_str, p_value, count, aes );
}

bln Funcs::StrDecToI64Quest( const char *cp_str, i64 *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToIntQuest( cp_str, p_value, count, aes );
}

bln Funcs::StrDecToUI64Quest( const char *cp_str, ui64 *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUIntQuest( cp_str, p_value, count, aes );
}

bln Funcs::StrDecToIWQuest( const char *cp_str, iw *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToIntQuest( cp_str, p_value, count, aes );
}

bln Funcs::StrDecToUIWQuest( const char *cp_str, uiw *p_value, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUIntQuest( cp_str, p_value, count, aes );
}

NOINLINE i32 Funcs::StrDecToI32( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToInt < i32 >( cp_str, count, aes );
}

NOINLINE ui32 Funcs::StrDecToUI32( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUInt < ui32 >( cp_str, count, aes );
}

NOINLINE i64 Funcs::StrDecToI64( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToInt < i64 >( cp_str, count, aes );
}

NOINLINE ui64 Funcs::StrDecToUI64( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUInt < ui64 >( cp_str, count, aes );
}

iw Funcs::StrDecToIW( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToInt < iw >( cp_str, count, aes );
}

uiw Funcs::StrDecToUIW( const char *cp_str, uiw count /* = uiw_max */, char aes /* = '\0' */ )
{
    return StrDecToUInt < uiw >( cp_str, count, aes );
}

f32 Funcs::StrToF32( const char *cp_str )
{
    ASSUME( cp_str );
    return (f32)::atof( cp_str );
}

f64 Funcs::StrToF64( const char *cp_str )
{
    ASSUME( cp_str );
    return ::atof( cp_str );
}

bln Funcs::IsHexBase( const char *cp_str )
{
    ASSUME( cp_str );
    return cp_str[ 0 ] == '0' && (cp_str[ 1 ] == 'x' || cp_str[ 1 ] == 'X');
}

bln Funcs::IsOctBase( const char *cp_str )
{
    ASSUME( cp_str );
    return cp_str[ 0 ] == '0' && (cp_str[ 1 ] == 'z' || cp_str[ 1 ] == 'Z');
}

bln Funcs::IsBinBase( const char *cp_str )
{
    ASSUME( cp_str );
    return cp_str[ 0 ] == '0' && (cp_str[ 1 ] == 'b' || cp_str[ 1 ] == 'B');
}

template < typename ft > uiw FloatToStr( ft val, char *p_buf, uiw precise, bln is_cutToShortest )  //  TODO: this code is fail
{
	ASSUME( p_buf );
/*#ifdef _MSC_VER
	char *result = ::_gcvt( val, 31, p_buf );
	ASSUME( result == p_buf );
	uiw len = 0;
	char *dotLocation = 0;
	bln is_scientific = false;
	for( ; p_buf[ len ]; ++len )
	{
		if( p_buf[ len ] == '.' )
		{
			ASSUME( dotLocation == 0 );
			dotLocation = p_buf + len;
		}
		else if( p_buf[ len ] == 'e' || p_buf[ len ] == 'E' )
		{
			ASSUME( is_scientific == false );
			is_scientific = true;
		}
	}
	ASSUME( len );
	if( is_cutToShortest == false || is_scientific || dotLocation == 0 )  //  no shortening for the scientific notation
	{
		return len;
	}
	++dotLocation;
	for( char *startTest = dotLocation; *startTest; ++startTest )
	{
		if( startTest[ 0 ] == startTest[ 1 ] && startTest[ 1 ] == startTest[ 2 ] )  //  we encountered 3 identical numbers, cut the number off
		{
			len = &startTest[ 2 ] - result;
			startTest[ 2 ] = '\0';
			goto toRet;
		}
	}
	uiw nonPreciseLen = dotLocation - result;
	uiw preciseLen = len - nonPreciseLen;
	if( preciseLen > precise )
	{
		uiw diff = preciseLen - precise;
		len -= diff;
	}
toRet:
	ASSUME( len );
	while( p_buf[ len - 1 ] == '0' )
	{
		--len;
	}
	p_buf[ len ] = '\0';
	return len;
#else*/
	const char *modifier0 = sizeof(ft) == sizeof(f32) ? "%g" : "%lg";
	const char *modifier1 = sizeof(ft) == sizeof(f32) ? "%.*f" : "%.*lf";
	if( is_cutToShortest )
	{
		return ::sprintf( p_buf, modifier0, val );
	}
	return ::sprintf( p_buf, modifier1, precise, val );
//#endif
}

NOINLINE uiw Funcs::F32ToStr( f32 val, char *p_buf )
{
	return FloatToStr( val, p_buf, 6, true );
}

NOINLINE uiw Funcs::F32ToStrWithPrecise( f32 val, ui32 precise, char *p_buf )
{
	return FloatToStr( val, p_buf, precise, false );
}

NOINLINE uiw Funcs::F64ToStr( f64 val, char *p_buf )
{
	return FloatToStr( val, p_buf, 6, true );
}

NOINLINE uiw Funcs::F64ToStrWithPrecise( f64 val, ui32 precise, char *p_buf )
{
	return FloatToStr( val, p_buf, precise, false );
}

class ParserParam
{
	ui32 _param;
	bln _is_present;

public:
	i32 GetParamI() const
	{
		ASSUME( _is_present );
		return *(i32 *)&_param;
	}

	ui32 GetParamU() const
	{
		ASSUME( _is_present );
		return _param;
	}

	void SetParam( ui32 param )
	{
		_is_present = true;
		_param = param;
	}

	void SetAbsent()
	{
		_is_present = false;
	}

	bln IsPresent() const
	{
		return _is_present;
	}
};

static const char *FmtParserHelper( const char *cp_fmt, char *p_ch, ParserParam *param, bln *is_extra )
{
    ASSUME( cp_fmt && p_ch && param );

	param->SetAbsent();
	ui32 intParam;
	*is_extra = false;

    if( *cp_fmt == '[' )
    {
        ++cp_fmt;
        const char *cp_end = _StrChr( cp_fmt, ']' );
        if( !cp_end )
        {
            SOFTBREAK;
            return 0;
        }
        if( *cp_fmt == '*' )
        {
            if( cp_fmt + 1 == cp_end )
            {
				*is_extra = true;
            }
            else
            {
                SOFTBREAK;
                return 0;
            }
        }
        else
        {
            if( cp_end - cp_fmt > 2 && Funcs::IsHexBase( cp_fmt ) )
            {
                cp_fmt += 2;
                if( Funcs::StrHexToIntQuest( cp_fmt, &intParam, cp_end - cp_fmt ) )
				{
					param->SetParam( intParam );
				}
				else
                {
                    SOFTBREAK;
                    return 0;
                }
            }
            else
            {
                if( Funcs::StrDecToUI32Quest( cp_fmt, &intParam, cp_end - cp_fmt ) )
				{
					param->SetParam( intParam );
				}
				else
                {
                    SOFTBREAK;
                    return 0;
                }
            }
        }
        cp_fmt = cp_end + 1;
    }

    *p_ch = *cp_fmt++;
    return cp_fmt;
}

bln ArgParserHelper( char type, void *p_source, ParserParam param, uiw *written, FileInterface *file )
{
	char a_buf[ 128 ];
    uiw len;

    switch( type )
    {
    case 's':  //  string [param]
    {
        const char *sourceStr = *(char **)p_source;
        if( !sourceStr )
        {
            sourceStr = "[null]";
            len = _StrLen( "[null]" );
        }        
        else
        {
			len = _StrLen( sourceStr );
			if( param.IsPresent() )
			{
				len = Funcs::Min < uiw >( len, param.GetParamU() );
			}
        }
		if( !file->Write( sourceStr, len ) )
		{
			return false;
		}
        goto retLen;
    }
    case 'f':  //  f32 [param]
    {
        f32 f32val = (f32)*(f64 *)p_source;
        if( param.IsPresent() )
        {
            len = Funcs::F32ToStrWithPrecise( f32val, param.GetParamU(), a_buf );
        }
        else
        {
            len = Funcs::F32ToStr( f32val, a_buf );
        }
        goto retBuf;
    }
    case 'i':  //  i32
        len = Funcs::IntToStrDec( *(i32 *)p_source, a_buf );
        goto retBuf;
    case 'u':  //  ui32
        len = Funcs::IntToStrDec( *(ui32 *)p_source, a_buf );
        goto retBuf;
	case 'l':  //  i64
        len = Funcs::IntToStrDec( *(i64 *)p_source, a_buf );
		goto retBuf;
	case 'v':  //  ui64
        len = Funcs::IntToStrDec( *(ui64 *)p_source, a_buf );
		goto retBuf;
    case 'b':  //  bln [param]
	{
        len = *(ui32 *)p_source ? _StrLen( "true" ) : _StrLen( "false" );
		bln is_capitalize = param.IsPresent() == false || param.GetParamU() == 0 ? false : true;
		if( !file->Write( *(ui32 *)p_source ? (is_capitalize ? "TRUE" : "true") : (is_capitalize ? "FALSE" : "false"), len ) )
		{
			return false;
		}
        goto retLen;
	}
    case 'c':  //  char
		if( !file->Write( p_source, 1 ) )
		{
			return false;
		}
        len = 1;
        goto retLen;
    case 'h':  //  integer 32 as hex str [param]
        len = Funcs::IntToStrHex( param.IsPresent() && param.GetParamU(), false, false, a_buf, *(ui32 *)p_source );
        goto retBuf;
    case 'j':  //  integer 64 as hex str [param]
        len = Funcs::IntToStrHex( param.IsPresent() && param.GetParamU(), false, false, a_buf, *(ui64 *)p_source );
        goto retBuf;
    case 'p':  //  pointer, integer word as hex str [param]
        len = Funcs::IntToStrHex( param.IsPresent() && param.GetParamU(), false, false, a_buf, *(uiw *)p_source );
        goto retBuf;
    case 'n':  //  integer 32 as bin str [param]
        len = Funcs::IntToStrBin( *(ui32 *)p_source, a_buf, false, param.IsPresent() && param.GetParamU() );
        goto retBuf;
    case 'm':  //  integer 64 as bin str [param]
        len = Funcs::IntToStrBin( *(ui64 *)p_source, a_buf, false, param.IsPresent() && param.GetParamU() );
        goto retBuf;
    case 'a':  //  pointer, integer word as bin str [param]
        len = Funcs::IntToStrBin( *(uiw *)p_source, a_buf, false, param.IsPresent() && param.GetParamU() );
        goto retBuf;
    case 'w':  //  unsigned word
        len = Funcs::IntToStrDec( *(uiw *)p_source, a_buf );
        goto retBuf;
    case 'o':  //  signed word
        len = Funcs::IntToStrDec( *(iw *)p_source, a_buf );
        goto retBuf;
    case 'd':  //  f64 [param]
        if( param.IsPresent() )
        {
            len = Funcs::F64ToStrWithPrecise( *(f64 *)p_source, param.GetParamU(), a_buf );
        }
        else
        {
            len = Funcs::F64ToStr( *(f64 *)p_source, a_buf );
        }
        goto retBuf;
    case '%':  //  % symbol
		if( !file->Write( "%", 1 ) )
        {
            return false;
        }
        len = 1;
        goto retLen;
    default:
        UNREACHABLE;
    }

retBuf:
	if( !file->Write( a_buf, len ) )
    {
        return false;
    }
retLen:
    *written = len;
    return true;
}

template < bln is_validateStep > inline Nullable < uiw > PrintToStrArgListImpl( const Funcs::_ArgType *argTypes, uiw argsCount, const char *cp_fmt, va_list args, FileInterface *file )
{
    using namespace Funcs;

	const char *const dbgStr = "#error parsing fmt string#";
	bln is_extraParam;
	uiw appendedLen = 0, written;
	const char *cp_fmtFlushed = cp_fmt;
    uiw argIndex = 0;
    ui32 intParam;
	ParserParam param;
    char ch;
    ALIGNED_PRE( 8 ) ui64 argumentData ALIGNED_POST( 8 );

    #define LOAD_ARG( expectedType, target ) \
        if( is_validateStep && argIndex >= argsCount ) { HARDBREAK; return nullv; } \
        if( expectedType.is_fp ) \
        { \
			ASSUME( sizeof(target) >= 8 ); \
            if( is_validateStep && (!argTypes[ argIndex ].is_fp || argTypes[ argIndex ].is_pointer) ) { HARDBREAK; return nullv; } \
            *(f64 *)&target = va_arg( args, f64 ); \
        } \
        else if( expectedType.size == 4 ) \
        { \
			ASSUME( sizeof(target) >= 4 ); \
            if( is_validateStep && ((expectedType.is_string && !argTypes[ argIndex ].is_string) || (expectedType.size > argTypes[ argIndex ].size) || argTypes[ argIndex ].size > 4) ) { HARDBREAK; return nullv; } \
            *(ui32 *)&target = va_arg( args, ui32 ); \
        } \
        else if( expectedType.size == 8 ) \
        { \
			ASSUME( sizeof(target) >= 8 ); \
            if( is_validateStep && ((expectedType.is_string && !argTypes[ argIndex ].is_string) || (expectedType.size > argTypes[ argIndex ].size) || argTypes[ argIndex ].size > 8) ) { HARDBREAK; return nullv; } \
            *(ui64 *)&target = va_arg( args, ui64 ); \
        } \
        else \
        { \
            HARDBREAK; \
            return nullv; \
        } \
        if( is_validateStep ) ++argIndex;

    while( *cp_fmt )
    {
        if( *cp_fmt == '%' )
        {
			const char *cp_lastFmt = cp_fmt;
            cp_fmt = FmtParserHelper( cp_fmt + 1, &ch, &param, &is_extraParam );
            if( cp_fmt )
            {
				if( is_validateStep == false && cp_lastFmt > cp_fmtFlushed )
				{
					if( !file->Write( cp_fmtFlushed, cp_lastFmt - cp_fmtFlushed ) )
					{
						SOFTBREAK;
						return nullv;
					}
					appendedLen += cp_lastFmt - cp_fmtFlushed;
				}

				cp_fmtFlushed = cp_fmt;

                if( is_extraParam )
                {
                    LOAD_ARG( _ArgType( sizeof(int), false, false, false ), intParam );
					param.SetParam( intParam );
                }

                switch( ch )
                {
                    case 'i': case 'u': case 'b': case 'c': case 'h': case 'n':
                        LOAD_ARG( _ArgType( 4, false, false, false ), argumentData );
                        break;
                    case 'p': case 'w': case 'o': case 'a':
						LOAD_ARG( _ArgType( sizeof(void *), false, false, false ), argumentData );
                        break;
                    case 's':
                        LOAD_ARG( _ArgType( sizeof(void *), false, false, true ), argumentData );
                        break;
					case 'j': case 'm': case 'l': case 'v':
                        LOAD_ARG( _ArgType( 8, false, false, false ), argumentData );
                        break;
                    case 'f': case 'd':
                        LOAD_ARG( _ArgType( 8, false, true, false ), argumentData );
                        break;
                    default:
                        SOFTBREAK;
						if( is_validateStep )
						{
							return nullv;
						}
						file->Write( dbgStr, _StrLen( dbgStr ) );
                        return appendedLen + _StrLen( dbgStr );
                }

				if( is_validateStep )
				{
					continue;
				}
                if( ArgParserHelper( ch, &argumentData, param, &written, file ) )
                {
                    appendedLen += written;
                    continue;
                }
            }
            SOFTBREAK;
			file->Write( dbgStr, _StrLen( dbgStr ) );
            return appendedLen + _StrLen( dbgStr );
        }
        else
        {
            ++cp_fmt;
        }
    }

    if( is_validateStep && argIndex != argsCount )
    {
        HARDBREAK;
        return nullv;
    }

	if( is_validateStep == false && cp_fmt > cp_fmtFlushed )
	{
		if( !file->Write( cp_fmtFlushed, cp_fmt - cp_fmtFlushed ) )
		{
			SOFTBREAK;
			return nullv;
		}
		appendedLen += cp_fmt - cp_fmtFlushed;
	}

    return appendedLen;
}

uiw Funcs::PrintToStrArgList( char *p_str, uiw maxLen, const char *cp_fmt, va_list args )
{
    ASSUME( p_str && cp_fmt && maxLen != uiw_max && maxLen );

	if( maxLen == 0 )
	{
		return 0;
	}
	--maxLen;

	MemoryStreamFixedExt stream( p_str, maxLen, 0 );

	FileMemoryStream file( &stream, FileProcMode::Write );
	ASSUME( file.IsOpened() );

    Nullable < uiw > written = PrintToStrArgListImpl < false >( 0, 0, cp_fmt, args, &file );

	uiw actuallWritten = written.ValueOrDefault( 0 );
	if( actuallWritten > maxLen )
	{
		SOFTBREAK;  //  target string is too small
		actuallWritten = maxLen;
	}

    p_str[ actuallWritten ] = '\0';

    return actuallWritten;
}

uiw Funcs::PrintToMemoryStreamArgList( MemoryStreamInterface *stream, const char *cp_fmt, va_list args )
{
    ASSUME( stream && stream->IsWritable() && cp_fmt );

	FileMemoryStream file( stream, FileProcMode::WriteAppend );
	ASSUME( file.IsOpened() );

    Nullable < uiw > written = PrintToStrArgListImpl < false >( 0, 0, cp_fmt, args, &file );

    return stream->Resize( written.IsNull() ? 0 : written.Get() );
}

uiw Funcs::PrintToFileArgList( FileInterface *file, const char *cp_fmt, va_list args )
{
	ASSUME( file && file->IsOpened() && (file->ProcModeGet() & FileProcMode::Write) && cp_fmt );

	return PrintToStrArgListImpl < false >( 0, 0, cp_fmt, args, file ).ValueOrDefault( 0 );
}

#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
bln Funcs::_PrintCheckArgs( const _ArgType *argTypes, uiw argsCount, const char *cp_fmt, ... )
{
    ASSUME( argTypes );

    va_list args;
    va_start( args, cp_fmt );

    Nullable < uiw > written = PrintToStrArgListImpl < true >( argTypes, argsCount, cp_fmt, args, 0 );

    va_end( args );

    return written.IsNull() == false;
}
#endif