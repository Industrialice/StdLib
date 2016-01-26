#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#ifdef DEBUG
    #include <typeinfo>
#endif

namespace StdLib {

/*  0b or 0B is binary base  */
/*  0z or 0Z is octal base  */

/*
**  about e-notation: it can start from digit or from dot
**  before e must go at least one digit
**  after e can go sign. if not present, + is assumed
**  after e must go at least one digit
**  most compact form is 4 chars
*/

/*  use template versions instead  */
#define MAX( a, b ) TERSWITCH1( (a) > (b), a, b )
#define MIN( a, b ) TERSWITCH1( (a) > (b), b, a )
#define CLAMP( value, min, max ) TERSWITCH2( (value) > (max), max, \
                                             (value) < (min), min, \
                                             (value) );

#define MEMALIGN( mem, alignment ) (void *)((((uiw)mem) + (alignment - 1)) & ~(alignment - 1))

#define MEMREMAP( mem, oldBase, newBase ) (void *)((uiw)mem + ((uiw)newBase - (uiw)oldBase))

#define DSA( pointer, assigning ) if( pointer ) *(pointer) = (assigning)

#ifdef DEFINE_VARARGS_SUPPORTED
    #ifdef DEBUG
        typedef uiw va_return;
        const va_return va_return_whatever = uiw_max;
    #define VC( func, arg0, ... ) { CAny anys[] = { arg0 __VA_ARGS__ }; va_return ret = func( arg0 __VA_ARGS__ ); if( ret != va_return_whatever && ret != COUNTOF( anys ) ) DBGBREAK; }
    #else
        typedef void va_return;
        #define va_return_whatever
        #define VC( func, arg0, ... ) func( arg0 __VA_ARGS__ )
    #endif
#else
    typedef void va_return;
    #define va_return_whatever
    #define VC( func, arg ) func arg
#endif

namespace Funcs
{
    EXTERNAL i32 RandomI32();  //  [ 0x80000000 ; 0x7FffFFff ]
    EXTERNAL ui32 RandomUI32();  //  [ 0 ; 0xFFffFFff ]
    EXTERNAL f32 RandomF32();  //  [ 0.f ; 1.f ]
    EXTERNAL i32 RandomRangeI32( i32 from, i32 to );
    EXTERNAL ui32 RandomRangeUI32( ui32 from, ui32 to );
    EXTERNAL f32 RandomRangeF32( f32 from, f32 to );
    EXTERNAL ui32 RandomUI32Limit( ui32 limit );  //  [ 0; limit )

    EXTERNAL i32 RoundF32( f32 val );
    EXTERNAL i32 RoundF32WithPrecise( f32 val, ui32 precise );
    EXTERNAL f32 RoundF32DownToStep( f32 val, f32 step );
    EXTERNAL f32 RoundF32UpToStep( f32 val, f32 step );
    EXTERNAL f32 RoundF32ToNearestStep( f32 val, f32 step );
    EXTERNAL i32 RoundF64( f64 val );
    EXTERNAL i32 RoundF64WithPrecise( f64 val, ui32 precise );
    EXTERNAL f64 RoundF64DownToStep( f64 val, f64 step );
    EXTERNAL f64 RoundF64UpToStep( f64 val, f64 step );
    EXTERNAL f64 RoundF64ToNearestStep( f64 val, f64 step );
    EXTERNAL uiw RoundUIDownToStep( uiw val, uiw step );
    EXTERNAL uiw RoundUIUpToStep( uiw val, uiw step );
    EXTERNAL uiw RoundUIToNearestStep( uiw val, uiw step );
    EXTERNAL iw RoundIDownToStep( iw val, iw step );  //  TODO: complete
    EXTERNAL iw RoundIUpToStep( iw val, iw step );  //  TODO: complete
    EXTERNAL iw RoundIToNearestStep( iw val, iw step );  //  TODO: complete

    #define _PowF32( fp, power ) ::powf( fp, (i32)power )
    #define _PowF64( fp, power ) ::pow( fp, (i32)power )
    EXTERNAL bln IsF32Equal( f32 first, f32 second, f32 epsilon );
    #define _IsF32Equal( first, second, epsilon ) (::fabsf( first - second ) < epsilon)
    EXTERNAL bln IsF64Equal( f64 first, f64 second, f64 epsilon );
    #define _IsF64Equal( first, second, epsilon ) (::fabs( first - second ) < epsilon)
    EXTERNAL f32 CeilF32Precise( f32 val, ui32 precise );
    EXTERNAL f64 CeilF64Precise( f64 val, ui32 precise );
    EXTERNAL f32 SaturateF32( f32 val );
    EXTERNAL f64 SaturateF64( f64 val );
    EXTERNAL f32 F32NormalizeRadian( f32 rad );
    EXTERNAL f32 F32FracPart( f32 val );
    EXTERNAL f64 F64FracPart( f64 val );
    EXTERNAL bln IsF32NaN( f32 val );
    EXTERNAL bln IsF64NaN( f64 val );
    EXTERNAL bln IsF32Inf( f32 val );
    EXTERNAL bln IsF32InfPos( f32 val );
    EXTERNAL bln IsF32InfNeg( f32 val );
    EXTERNAL bln IsF64Inf( f64 val );
    EXTERNAL bln IsF64InfPos( f64 val );
    EXTERNAL bln IsF64InfNeg( f64 val );
    EXTERNAL f32 MakeF32ByBits( ui32 sign1, ui32 exp8, ui32 significand23 );
    EXTERNAL f64 MakeF64ByBits( uiw sign1, uiw exp11, ui64 significand52 );
    EXTERNAL f32 MakeF32ByND( i32 numerator, i32 denominator );
    EXTERNAL f64 MakeF64ByND( i32 numerator, i32 denominator );

    EXTERNAL ui32 CheckSum32( const byte *source, uiw len );
    EXTERNAL ui64 CheckSum64( const byte *source, uiw len );
    EXTERNAL uiw CheckSumWord( const byte *source, uiw len );

    EXTERNAL uiw NormalizeMem32( ui32 val, char *p_buf );  //  buffer size after normalize
    EXTERNAL uiw NormalizeMem64( ui64 val, char *p_buf );  //  buffer size after normalize
    EXTERNAL uiw NormalizeMemWord( uiw val, char *p_buf );  //  buffer size after normalize

    EXTERNAL uiw MemCpy( void *RSTR p_target, const void *cp_source, uiw size );
    #define _MemCpy( dest, source, size ) (::memcpy( dest, source, size ), size)
    #define _MemCpyIncr( dest, source, size ) (::memcpy( dest, source, size ), (byte *&)(dest) += size, size)
    #define _MemCpyIncrRev( dest, source, size ) (::memcpy( dest, source, size ), (byte *&)(source) += size, size)
    #define _MemCpyOffset( dest, source, size, offset ) (::memcpy( (byte *)(dest) + offset, source, size ), size)
    #define _MemCpyOffsetIncr( dest, source, size, offset ) (::memcpy( (byte *)(dest) + offset, source, size ), offset += size, size)
    #define _MemCpyOffsetRev( dest, source, size, offset ) (::memcpy( dest, (byte *)(source) + offset, size ), size)
    #define _MemCpyOffsetIncrRev( dest, source, size, offset ) (::memcpy( dest, (byte *)(source) + offset, size ), offset += size, size)
    #define _MemMove( dest, source, size ) (::memmove( dest, source, size ), size)
    EXTERNAL uiw MemZero( void *p_mem, uiw size );
    #define _MemZero( mem, size ) (::memset( mem, 0, size ), size)
    EXTERNAL uiw MemSet( void *p_mem, byte val, uiw size );
    #define _MemSet( mem, value, size ) (::memset( mem, (byte)value, size ), size)
    EXTERNAL bln MemTest( void *p_mem, byte val, uiw size );  //  true if size is 0
    EXTERNAL bln MemEquals( const void *cp_mem0, const void *cp_mem1, uiw size );  //  true if size is 0
    #define _MemEquals( mem0, mem1, size ) (!::memcmp( mem0, mem1, size ))
    EXTERNAL void *MemFindSeq( const void *cp_mem, const void *cp_seq, uiw memSize, uiw seqSize );  //  assume output is const if unput was const
    #define _Clear( mem ) ::memset( mem, 0, sizeof(*mem) )

    typedef bln (*ChrTestFunc)( char );

    EXTERNAL bln IsChrUpperAlpha( char source );
    EXTERNAL bln IsChrLowerAlpha( char source );
    EXTERNAL bln IsChrAlpha( char source );
    EXTERNAL bln IsChrDec( char source );
    EXTERNAL bln IsChrHex( char source );
    EXTERNAL bln IsChrOct( char source );
    EXTERNAL bln IsChrBin( char source );

    //  all with bases but without signes, must contain at least one digit
    EXTERNAL bln IsStrHex( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNAL bln IsStrBin( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNAL bln IsStrFP( const char *cp_str, uiw maxLen = uiw_max );
    EXTERNAL bln IsStrDec( const char *cp_str, uiw maxLen = uiw_max );

    EXTERNAL char ChrToLower( char source );
    EXTERNAL char ChrToUpper( char source );

    EXTERNAL bln IsChrEqual( char one, char two );  //  compare chars
    EXTERNAL bln IsChrEqualWOR( char one, char two );  //  compare chars without register

    //  maxLen without zero symbol

    EXTERNAL uiw StrToLower( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrToLowerAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrToLowerInplace( char *p_str );
    EXTERNAL uiw StrToLowerInplaceAdv( char *p_str, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrToUpper( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrToUpperAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrToUpperInplace( char *p_str );
    EXTERNAL uiw StrToUpperInplaceAdv( char *p_str, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL bln StrEqual( const char *cp_one, const char *cp_two );  //  true if both strings are zero
    EXTERNAL bln StrEqualAdv( const char *cp_one, const char *cp_two, uiw maxLen = uiw_max, char aes = '\0' );  //  true if both strings are zero
    #define _StrEqual( one, two ) (!::strcmp( one, two ))
    EXTERNAL bln StrNEqual( const char *cp_one, const char *cp_two, uiw count );  //  true if both strings are zero or count is zero
    EXTERNAL bln StrNEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes = '\0' );  //  true if both strings are zero or count is zero
    #define _StrNEqual( one, two, count ) (!::strncmp( one, two, count ))
    EXTERNAL bln StrIEqual( const char *cp_one, const char *cp_two );  //  true if both strings are zero
    EXTERNAL bln StrIEqualAdv( const char *cp_one, const char *cp_two, uiw maxLen = uiw_max, char aes = '\0' );  //  true if both strings are zero
    EXTERNAL bln StrINEqual( const char *cp_one, const char *cp_two, uiw count );  //  true if both strings are zero or count is zero
    EXTERNAL bln StrINEqualAdv( const char *cp_one, const char *cp_two, uiw count, char aes = '\0' );  //  true if both strings are zero or count is zero
    EXTERNAL uiw StrLen( const char *cp_str );
    EXTERNAL uiw StrLenAdv( const char *cp_str, uiw maxLen = uiw_max, char aes = '\0' );
    #define _StrLen( str ) ::strlen( str )
    EXTERNAL void StrCat( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrCatAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aesDest = '\0', char aesSrc = '\0' );
    #define _StrCat( dest, source ) (void)::strcat( dest, source )
    EXTERNAL void StrCpy( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrCpyAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    #define _StrCpy( dest, source ) (void)::strcpy( dest, source )
    EXTERNAL uiw StrNCpy( char *RSTR p_dest, const char *cp_source, uiw count );
    #define _StrNCpy( dest, source, count ) (::strncpy( dest, source, count ), count)
    #define _StrNCpyCS( dest, source, minus ) (::strncpy( dest, source, sizeof(source) - minus ), sizeof(source) - minus)
//    #define _StrNCpyCSC( dest, source, minus, counter ) (::strncpy( dest, source, sizeof(source) - minus ), counter += (sizeof(source) - minus), sizeof(source) - minus)
    EXTERNAL void StrConnect( char *RSTR p_dest, const char *cp_first, const char *cp_second );
    EXTERNAL void StrCpyWONull( char *RSTR p_dest, const char *cp_source );
    EXTERNAL void StrCpyWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrCpyAndCount( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrCpyAndCountWONull( char *RSTR p_dest, const char *cp_source );
    EXTERNAL uiw StrCpyAndCountWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrSafeCpyAndCount( char *RSTR p_dest, const char *cp_source, uiw maxLen );
    EXTERNAL uiw StrSafeCpyAndCountWONull( char *RSTR p_dest, const char *cp_source, uiw maxLen );
    EXTERNAL uiw StrSafeCpyAndCountWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen, char aes = '\0' );
    EXTERNAL uiw StrDelim( char **pp_output, char *p_source, char delim );  //  you can pass null as pp_output only to get a number of the final strings
    EXTERNAL uiw StrDelimConst( char **pp_output, const char *cp_source, char delim );  //  TODO: incomplete  //  you can pass null as pp_output only to get a number of the final strings
    EXTERNAL char *StrChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    #define _StrChr( source, symbol ) ((char *)::strchr( source, symbol ))
    EXTERNAL char *StrNotChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrNotChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrChrs( const char *cp_source, const char *cp_chars );
    EXTERNAL char *StrChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL char *StrNotChrs( const char *cp_source, const char *cp_chars );
    EXTERNAL char *StrNotChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL char *StrIChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrINotChrs( const char *cp_source, const char *cp_chars );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrINotChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrINotChr( const char *cp_source, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrINotChrAdv( const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIChrs( const char *cp_source, const char *cp_chars );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIChrsAdv( const char *cp_source, const char *cp_chars, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrChrMask( const char *cp_source, char mask, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIChrMask( const char *cp_source, char mask, char symbol );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIChrMaskAdv( const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrStr( const char *cp_source, const char *cp_under );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrStrAdv( const char *cp_source, const char *cp_under, uiw countSrc = uiw_max, uiw countUnder = uiw_max, char aesSrc = '\0', char aesUnder = '\0' );  //  returning value of the same string - assume it is const if source is const
    #define _StrStr( source, under ) ((char *)::strstr( source, under ))
    EXTERNAL char *StrIStr( const char *cp_source, const char *cp_under );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL char *StrIStrAdv( const char *cp_source, const char *cp_under, uiw countSrc = uiw_max, uiw countUnder = uiw_max, char aesSrc = '\0', char aesUnder = '\0' );  //  returning value of the same string - assume it is const if source is const
    EXTERNAL bln IsStrIsFromThisSymbols( const char *cp_str, const char *cp_symbols );  //  true if both have zero length, false if only one of them have zero length
    EXTERNAL bln IsStrIsFromThisSymbol( const char *cp_str, char symbol, uiw count = uiw_max );  //  false on zero length string
    EXTERNAL bln IsStrMatch( const char *cp_str, ChrTestFunc func );  //  false on zero length string
    EXTERNAL bln IsStrMatchAdv( const char *cp_str, ChrTestFunc func, uiw count = uiw_max, char aes = '\0' );  //  false on zero length string
    EXTERNAL uiw StrExclude( char *RSTR p_target, const char *cp_source, char symbol );  //  you can pass null as p_target to get only proced len
    EXTERNAL uiw StrExcludeAdv( char *RSTR p_target, const char *cp_source, char symbol, uiw count = uiw_max, char aes = '\0' );  //  you can pass null as p_target to get only proced len
    EXTERNAL uiw StrExcludeMask( char *RSTR p_target, const char *cp_source, char mask, char symbol );  //  you can pass null as p_target to get only proced len
    EXTERNAL uiw StrExcludeMaskAdv( char *RSTR p_target, const char *cp_source, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );  //  you can pass null as p_target to get only proced len
    EXTERNAL uiw StrExcludeInplace( char *p_str, char symbol );
    EXTERNAL uiw StrExcludeInplaceAdv( char *p_str, char symbol, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL uiw StrExcludeMaskInplace( char *p_str, char mask, char symbol );
    EXTERNAL uiw StrExcludeMaskInplaceAdv( char *p_str, char mask, char symbol, uiw count = uiw_max, char aes = '\0' );

    //  *p_value is not changed if no suitable conversion is possible TODO: add cp_str capacity info
    EXTERNAL bln StrDecToI32Quest( const char *cp_str, i32 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL bln StrDecToUI32Quest( const char *cp_str, ui32 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL bln StrDecToI64Quest( const char *cp_str, i64 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL bln StrDecToUI64Quest( const char *cp_str, ui64 *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL bln StrDecToIWQuest( const char *cp_str, iw *p_value, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL bln StrDecToUIWQuest( const char *cp_str, uiw *p_value, uiw count = uiw_max, char aes = '\0' );

    //  TODO: add cp_str capacity info
    EXTERNAL i32  StrDecToI32( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL ui32 StrDecToUI32( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL i64  StrDecToI64( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL ui64 StrDecToUI64( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL iw   StrDecToIW( const char *cp_str, uiw count = uiw_max, char aes = '\0' );
    EXTERNAL uiw  StrDecToUIW( const char *cp_str, uiw count = uiw_max, char aes = '\0' );

    EXTERNAL f32 StrToF32( const char *cp_str );
    EXTERNAL f64 StrToF64( const char *cp_str );

    EXTERNAL bln IsHexBase( const char *cp_str );
    EXTERNAL bln IsOctBase( const char *cp_str );
    EXTERNAL bln IsBinBase( const char *cp_str );

    EXTERNAL uiw F32ToStr( f32 val, char *p_buf );
    EXTERNAL uiw F32ToStrWithPrecise( f32 val, ui32 precise, char *p_buf );
    EXTERNAL uiw F64ToStr( f64 val, char *p_buf );
    EXTERNAL uiw F64ToStrWithPrecise( f64 val, ui32 precise, char *p_buf );

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
    EXTERNAL uiw PrintToStrArgList( char *p_str, uiw maxLen, const char *cp_fmt, va_list args );

    //  internal
    EXTERNAL uiw _PrintToContainer( void *cont, char *(*RequestMoreSize)(void *, uiw), const char *cp_fmt, va_list args );
    struct _ArgType
    {
		uiw size;
		bln is_pointer;
		bln is_fp;
		bln is_string;

		_ArgType( uiw size = 0, bln is_pointer = false, bln is_fp = false, bln is_string = false ) :
			size( size ), is_pointer( is_pointer ), is_fp( is_fp ), is_string( is_string )
		{}
    };

#if defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
    EXTERNAL bln _PrintCheckArgs( const _ArgType *argTypes, uiw argsCount, const char *cp_fmt, ... );

    EXTERNAL uiw _PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, ... );

    template < typename X > _ArgType _AnalyzeArg( const X &arg )
    {
        STATIC_CHECK( TypeDesc < X >::is_pod, "you can't pass non-pod args to variadic functions" );
        STATIC_CHECK( TypeDesc < X >::is_array || (sizeof(X) > 0 && sizeof(X) <= 8), "you can't use weird-sized types with variadic functions" );
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
			if( typeid(TypeDesc < X >::type) == typeid(char) )
			{
				argType.is_string = true;
			}
			return argType;
		}
        if( TypeDesc < X >::is_integer || typeid(X) == typeid(bln) || typeid(X) == typeid(char) )
        {
			if( sizeof(X) < sizeof(int) )
			{
				argType.size = sizeof(int);
			}
			return argType;
        }
        DBGBREAK;
        return _ArgType();
    }

    template < typename... Args > bln _AreArgsValid( const char *cp_fmt, const Args &... args )
    {
        static const _ArgType argTypes[] = { _ArgType(), _AnalyzeArg( args )... };
        return _PrintCheckArgs( argTypes + 1, COUNTOF( argTypes ) - 1, cp_fmt, args... );
    }

    template < typename... Args > uiw PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, const Args &... args )
    {
        if( !_AreArgsValid( cp_fmt, args... ) )
        {
            DBGBREAK;
            return 0;
        }
        return _PrintToStr( p_str, maxLen, cp_fmt, args... );
    }
#else
    EXTERNAL uiw PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, ... );
#endif

//  container storage must be linear, allowed to be non-zero size, will be cleared if not empty
#if defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
    template < typename container > uiw _PrintToContainer( container *cont, const char *cp_fmt, ... );

    template < typename container, typename... Args > uiw PrintToContainer( container *cont, const char *cp_fmt, const Args &... args )
    {
        if( !_AreArgsValid( cp_fmt, args... ) )
        {
            DBGBREAK;
            return 0;
        }
        return _PrintToContainer( cont, cp_fmt, args... );
    }
    
    //  won't append string null-terminator
    template < typename container > uiw _PrintToContainer( container *cont, const char *cp_fmt, ... )
#else
    //  won't append string null-terminator
    template < typename container > uiw PrintToContainer( container *cont, const char *cp_fmt, ... )
#endif
    {
        struct NoName
        {
            static char *RequestMoreSize( void *ob, uiw size )  //  can overcommit
            {
                container *cont = (container *)ob;
                cont->Resize( cont->Size() + size );
                return cont->Data();
            }
        };

        va_list variadic;
        va_start( variadic, cp_fmt );
        uiw printed = _PrintToContainer( cont, NoName::RequestMoreSize, cp_fmt, variadic );
        va_end( variadic );

        cont->Resize( printed );  //  removing excessive size( if any )
        return printed;
    }

    //  won't append string null-terminator
    template < typename container > uiw PrintToContainerArgList( container *cont, const char *cp_fmt, va_list args )
    {
        struct NoName
        {
            static char *RequestMoreSize( void *ob, uiw size )  //  can overcommit
            {
                container *cont = (container *)ob;
                cont->Resize( cont->Size() + size );
                return cont->Data();
            }
        };

        uiw printed = _PrintToContainer( cont, NoName::RequestMoreSize, cp_fmt, args );

        cont->Resize( printed );  //  removing excessive size( if any )
        return printed;
    }

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

#if 0
    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X AddWrapAround( X val, Y addition, X wrapValue )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }

    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X AddClamp( X val, Y addition )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }

    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X SubWrapAround( X val, Y subtraction )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }

    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X SubClamp( X val, Y subtraction )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }

    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X MulWrapAround( X val, Y multiplication )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }

    /*  there is no type extension, CHECK YOUR TYPES CAREFULLY TO NOT GET CONFUSED  */
    template < typename X, typename Y > X MulClamp( X val, Y multiplication )
    {
        ASSUME( TypeDesc < X >::is_integer );
        if( TypeDesc < X >::is_signed )
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        else
        {
            NOT_IMPLEMENTED;
            return 0;
        }
        UNREACHABLE;
    }
#endif

    template < typename X > bln IsPowerOf2( const X &val )
    {
        return val && (val & val - 1) == 0;
    }

    template < typename X, typename Y > X BinCopy( const Y &source )
    {
        ASSUME( sizeof(X) == sizeof(Y) );
        return *(X *)&source;
    }

    template < typename X, typename Y > X BinAssign( X *target, const Y &source )
    {
        ASSUME( target && sizeof(X) == sizeof(Y) );
        *target = *(X *)&source;
        return *target;
    }

    template < typename X > X MaxValue( ui32 bits )
    {
        if( bits > TypeDesc < X >::bits )
        {
            return TypeDesc < X >::max;
        }
        return TypeDesc < X >::max >> (TypeDesc < X >::bits - bits);  //  TODO: fix
    }

    //  overflow is undefined behavior here. TODO: safe variant required
    template < typename X > X PowInt( X val, i32 power )
    {
        X powered = 1;
        ui32 absPower = ::abs( power );
        for( ui32 index = 0; index < absPower; ++index )
        {
            powered *= val;
        }
        if( power < 0 )
        {
            return Funcs::RoundF64( 1.0 / powered );
        }
        return powered;
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

    template < typename X > void Swap( X *p_first, X *p_second )  //  TODO: there must be a more adequate variant
    {
        X temp = *p_first;
        *p_first = *p_second;
        *p_second = temp;
    }

    template < typename X > const X &Max( const X &first, const X &second )
    {
        return first > second ? first : second;
    }

    template < typename X > const X &Min( const X &first, const X &second )
    {
        return first < second ? first : second;
    }

    template < typename X > const X &Clamp( const X &val, const X &low, const X &high )
    {
        if( val < low )
        {
            return low;
        }
        if( val > high )
        {
            return high;
        }
        return val;
    }

    template < typename X, typename Y > void BytewiseCopy( X *p_dest, const Y &source )
    {
        _MemCpy( p_dest, &source, sizeof(X) );
    }

    template < typename X > X ChangeEndianness( X val )
    {
        STATIC_CHECK( TypeDesc < X >::is_pod, "val is not a POD type in ChangeEndianness" );
        if( sizeof(val) == 8 )
        {
            return INT64_CHANGE_ENDIANNESS( val );
        }
        if( sizeof(val) == 4 )
        {
            return INT32_CHANGE_ENDIANNESS( val );
        }
        if( sizeof(val) == 2 )
        {
            return INT16_CHANGE_ENDIANNESS( val );
        }
        STATIC_CHECK( sizeof(val) == 1, "incorrect size of val in ChangeEndianness" );
        return val;
    }

    template < typename X > X NativeToLittleEndian( X val )
    {
        #ifdef BIG_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X NativeToBigEndian( X val )
    {
        #ifdef LITTLE_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X LittleEndianToNative( X val )
    {
        #ifdef BIG_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X BigEndianToNative( X val )
    {
        #ifdef LITTLE_ENDIAN
            return ChangeEndianness( val );
        #endif
        return val;
    }

    template < typename X > X CondReset( X val, bln is_reset )  //  set the value to 0 if is_reset is true
    {
        typename IntWithSize < sizeof(X) * 8 >::uint_t test = *(typename IntWithSize < sizeof(X) * 8 >::uint_t *)&val & (*(ui8 *)&is_reset - 1);
        return *(X *)&test;
    }

    template < typename X > X CondLeave( X val, bln is_reset )  //  leave the value unchanged if is_reset is true
    {
        typename IntWithSize < sizeof(X) * 8 >::uint_t test = *(typename IntWithSize < sizeof(X) * 8 >::uint_t *)&val & -*(ui8 *)&is_reset;
        return *(X *)&test;
    }
}

}  //  namespace StdLib

#endif __FUNCS_HPP__
