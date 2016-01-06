#include "PreHeader.hpp"

using namespace StdLib;

#include <stdio.h>

i32 Funcs::RandomI32()
{
    return rand() * rand() * rand();
}

ui32 Funcs::RandomUI32()
{
    return rand() * rand() * rand();
}

f32 Funcs::RandomF32()
{
    return rand() / (f32)RAND_MAX;
}

i32 Funcs::RandomRangeI32( i32 from, i32 to )
{
    return (rand() * rand() * rand() & 0x7FffFFff) % ((to - from) + 1) + from;
}

ui32 Funcs::RandomRangeUI32( ui32 from, ui32 to )
{
    return rand() * rand() * rand() % ((to - from) + 1) + from;
}

f32 Funcs::RandomRangeF32( f32 from, f32 to )
{
    return from + rand() / (f32)RAND_MAX * (to - from);
}

ui32 Funcs::RandomUI32Limit( ui32 limit )
{
    return (rand() * rand() * rand()) % limit;
}

i32 Funcs::RoundF32( f32 val )  //  TODO: bullshit
{
    return (i32)(val < 0 ? val - 0.55555555555f : val + 0.55555555555f);
}

i32 Funcs::RoundF32WithPrecise( f32 val, ui32 precise )  //  TODO: bullshit
{
    f32 rounder = 0.f;
    f32 adder = 0.50001f;
    for( ; precise; --precise )
    {
        rounder += adder;
        adder *= 0.1f;
    }
    return (i32)(val < 0 ? val - rounder : val + rounder);
}

f32 Funcs::RoundF32DownToStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val );
    return step * val;
}

f32 Funcs::RoundF32UpToStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val );
    return step * (val + 1.f);
}

f32 Funcs::RoundF32ToNearestStep( f32 val, f32 step )
{
    val /= step;
    val = ::floorf( val + 0.5f );
    return step * val;
}

i32 Funcs::RoundF64( f64 val )  //  TODO: bullshit
{
    return (i32)(val < 0 ? val - 0.555555555555555 : val + 0.555555555555555);
}

i32 Funcs::RoundF64WithPrecise( f64 val, ui32 precise )  //  TODO: bullshit
{
    f64 rounder = 0.0;
    f64 adder = 0.5000001;
    for( ; precise; --precise )
    {
        rounder += adder;
        adder *= 0.1;
    }
    return (i32)(val < 0 ? val - rounder : val + rounder);
}

f64 Funcs::RoundF64DownToStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val );
    return step * val;
}

f64 Funcs::RoundF64UpToStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val );
    return step * (val + 1.f);
}

f64 Funcs::RoundF64ToNearestStep( f64 val, f64 step )
{
    val /= step;
    val = ::floor( val + 0.5f );
    return step * val;
}

uiw Funcs::RoundUIDownToStep( uiw val, uiw step )
{
    return val - val % step;
}

uiw Funcs::RoundUIUpToStep( uiw val, uiw step )
{
    return val + (step - val % step);
}

uiw Funcs::RoundUIToNearestStep( uiw val, uiw step )
{
    uiw left = val % step;
    uiw low = val - left;
    uiw up = val + (step - left);
    return left >= step / 2 ? up : low;
}

iw Funcs::RoundIDownToStep( iw val, iw step )
{
    return 0;
}

iw Funcs::RoundIUpToStep( iw val, iw step )
{
    return 0;
}

iw Funcs::RoundIToNearestStep( iw val, iw step )
{
    return 0;
}

bln Funcs::IsF32Equal( f32 first, f32 second, f32 epsilon )
{
    return ::fabsf( first - second ) < epsilon;
}

bln Funcs::IsF64Equal( f64 first, f64 second, f64 epsilon )
{
    return ::fabs( first - second ) < epsilon;
}

f32 Funcs::CeilF32Precise( f32 val, ui32 precise )  //  TODO: rework
{
    f32 press = _PowF32( 10.f, precise );
    f32 multed = val * press;
    multed = ::floorf( multed );
    return multed / press;
}

f64 Funcs::CeilF64Precise( f64 val, ui32 precise )  //  TODO: rework
{
    f64 press = _PowF64( 10.0, precise );
    f64 multed = val * press;
    multed = ::floor( multed );
    return multed / press;
}

f32 Funcs::SaturateF32( f32 val )
{
    if( val < 0.f )
    {
        return 0.f;
    }
    if( val > 1.f )
    {
        return 1.f;
    }
    return val;
}

f64 Funcs::SaturateF64( f64 val )
{
    if( val < 0.0 )
    {
        return 0.0;
    }
    if( val > 1.0 )
    {
        return 1.0;
    }
    return val;
}

f32 Funcs::F32NormalizeRadian( f32 rad )
{
    rad = ::fabsf( rad );
    return Funcs::F32FracPart( rad / (f32_pi * 2) ) * (f32_pi * 2);
}

f32 Funcs::F32FracPart( f32 val )
{
    return val - ::floorf( val );
}

f64 Funcs::F64FracPart( f64 val )
{
    return val - ::floor( val );
}

bln Funcs::IsF32NaN( f32 val )
{
    return val != val;
}

bln Funcs::IsF64NaN( f64 val )
{
    return val != val;
}

bln Funcs::IsF32Inf( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( (ival | f32_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f32_significand_mask) == 0;
}

bln Funcs::IsF32InfPos( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) == 0;
}

bln Funcs::IsF32InfNeg( f32 val )
{
    ui32 ival = *(ui32 *)&val;
    if( !Funcs::IsF32Inf( val ) )
    {
        return false;
    }
    return (ival & f32_sign_mask) != 0;
}

bln Funcs::IsF64Inf( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( (ival | f64_exp_mask) != ival )
    {
        return false;
    }
    return (ival & f64_significand_mask) == 0;
}

bln Funcs::IsF64InfPos( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( !Funcs::IsF64Inf( val ) )
    {
        return false;
    }
    return (ival & f64_sign_mask) == 0;
}

bln Funcs::IsF64InfNeg( f64 val )
{
    ui64 ival = *(ui64 *)&val;
    if( !Funcs::IsF64Inf( val ) )
    {
        return false;
    }
    return (ival & f64_sign_mask) != 0;
}

f32 Funcs::MakeF32ByBits( ui32 sign1, ui32 exp8, ui32 significand23 )
{
    union
    {
        ui32 i;
        f32 f;
    } u;
    u.i = sign1 << 31;
    u.i |= exp8 << 23;
    u.i |= significand23;
    return u.f;
}

f64 Funcs::MakeF64ByBits( uiw sign1, uiw exp11, ui64 significand52 )
{
    union
    {
        ui64 i;
        f64 f;
    } u;
    u.i = (ui64)sign1 << 63;
    u.i |= (ui64)exp11 << 52;
    u.i |= significand52;
    return u.f;
}

f32 Funcs::MakeF32ByND( i32 numerator, i32 denominator )
{
    return (f32)numerator / denominator;
}

f64 Funcs::MakeF64ByND( i32 numerator, i32 denominator )
{
    return (f64)numerator / denominator;
}

ui32 Funcs::CheckSum32( const byte *source, uiw len )
{
    ASSUME( source );
    ui32 sum = 0;
    for( ui32 index = 0, rotate = 0; index < len; ++index, ++rotate )
    {
        if( rotate > 32 )
        {
            rotate = 0;
        }
        sum += ROTATE32L( source[ index ], index );
    }
    return sum;
}

ui64 Funcs::CheckSum64( const byte *source, uiw len )
{
    ASSUME( source );
    ui64 sum = 0;
    for( ui32 index = 0, rotate = 0; index < len; ++index, ++rotate )
    {
        if( rotate > 64 )
        {
            rotate = 0;
        }
        sum += ROTATE64L( source[ index ], index );
    }
    return sum;
}

uiw Funcs::CheckSumWord( const byte *source, uiw len )
{
    ASSUME( source );
    return WSC( Funcs::CheckSum32, Funcs::CheckSum64 )( source, len );
}

NOINLINE uiw Funcs::NormalizeMem32( ui32 val, char *p_buf )
{
    ASSUME( p_buf );
    if( val < 1024u )
    {
        uiw len = Funcs::IntToStrDec( val, p_buf );
        _StrCpy( p_buf + len, " B" );
        return len + 2;
    }
    else if( val < 1024u * 1024u )
    {
        return ::sprintf( p_buf, "%g KB", val / 1024.f );
    }
    else if( val < 1024u * 1024u * 1024u )
    {
        return ::sprintf( p_buf, "%g MB", val / 1048576.f );
    }
    else
    {
        return ::sprintf( p_buf, "%g GB", val / 1073741824.f );
    }
}

NOINLINE uiw Funcs::NormalizeMem64( ui64 val, char *p_buf )
{
    ASSUME( p_buf );
    if( val < 1024ull )
    {
        uiw len = Funcs::IntToStrDec( val, p_buf );
        _StrCpy( p_buf + len, " B" );
        return len + 2;
    }
    else if( val < 1024ull * 1024ull )
    {
        return ::sprintf( p_buf, "%lg KB", val / (f64)1024ull );
    }
    else if( val < 1024ull * 1024ull * 1024ull )
    {
        return ::sprintf( p_buf, "%lg MB", val / (f64)(1024ull * 1024ull) );
    }
    else if( val < 1024ull * 1024ull * 1024ull * 1024ull )
    {
        return ::sprintf( p_buf, "%lg GB", val / (f64)(1024ull * 1024ull * 1024ull) );
    }
    else
    {
        return ::sprintf( p_buf, "%lg TB", val / (f64)(1024ull * 1024ull * 1024ull * 1024ull) );
    }
}

uiw Funcs::NormalizeMemWord( uiw val, char *p_buf )
{
    ASSUME( p_buf );
    return WSC( Funcs::NormalizeMem32, Funcs::NormalizeMem64 )( val, p_buf );
}

uiw Funcs::MemCpy( void *RSTR p_dest, const void *cp_source, uiw size )
{
    ::memcpy( p_dest, cp_source, size );
    return size;
}

uiw Funcs::MemZero( void *p_mem, uiw size )
{
    ASSUME( p_mem );
    ::memset( p_mem, 0, size );
    return size;
}

uiw Funcs::MemSet( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem );
    ::memset( p_mem, val, size );
    return size;
}

bln Funcs::MemTest( void *p_mem, byte val, uiw size )
{
    ASSUME( p_mem );
    byte *p_memByte = (byte *)p_mem;
    while( size )
    {
        if( *p_memByte != val )
        {
            return false;
        }
        ++p_memByte;
        --size;
    }
    return true;
}

bln Funcs::MemEquals( const void *cp_mem0, const void *cp_mem1, uiw size )
{
    ASSUME( cp_mem0 && cp_mem1 );
    return !::memcmp( cp_mem0, cp_mem1, size );
}

void *Funcs::MemFindSeq( const void *cp_mem, const void *cp_seq, uiw memSize, uiw seqSize )
{
    const byte *cp_memByte = (byte *)cp_mem;
    for( ; memSize >= seqSize; --memSize, ++cp_memByte )
    {
        if( _MemEquals( cp_memByte, cp_seq, seqSize ) )
        {
            return (void *)cp_memByte;
        }
    }
    return 0;
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

void Funcs::StrCpy( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
    *p_dest = '\0';
}

uiw Funcs::StrCpyAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++cpy;
    }
    *p_dest = '\0';
    return cpy;
}

uiw Funcs::StrNCpy( char *RSTR p_dest, const char *cp_source, uiw count )
{
    ASSUME( p_dest && cp_source );
    ::strncpy( p_dest, cp_source, count );
    return count;
}

void Funcs::StrConnect( char *RSTR p_dest, const char *cp_first, const char *cp_second )
{
    ASSUME( p_dest && cp_first && cp_second );
    while( *cp_first )
    {
        *p_dest++ = *cp_first++;
    }
    _StrCpy( p_dest, cp_second );
}

void Funcs::StrCpyWONull( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
}

void Funcs::StrCpyWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    while( maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        --maxLen;
    }
}

uiw Funcs::StrCpyAndCount( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    const char *cp_sourceStore = cp_source;

    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }
    *p_dest = '\0';

    return cp_source - cp_sourceStore;
}

uiw Funcs::StrCpyAndCountWONull( char *RSTR p_dest, const char *cp_source )
{
    ASSUME( p_dest && cp_source );
    const char *cp_sourceStore = cp_source;

    while( *cp_source )
    {
        *p_dest++ = *cp_source++;
    }

    return cp_source - cp_sourceStore;
}

uiw Funcs::StrCpyAndCountWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen /* = uiw_max */, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw cpy = 0;
    while( cpy < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++cpy;
    }
    return cpy;
}

uiw Funcs::StrSafeCpyAndCount( char *RSTR p_dest, const char *cp_source, uiw maxLen )
{
    ASSUME( p_dest && cp_source );
    uiw index = 0;
    while( index < maxLen && *cp_source )
    {
        *p_dest++ = *cp_source++;
        ++index;
    }
    *p_dest = '\0';

    return index;
}

uiw Funcs::StrSafeCpyAndCountWONull( char *RSTR p_dest, const char *cp_source, uiw maxLen )
{
    ASSUME( p_dest && cp_source );
    uiw index = 0;
    while( index < maxLen && *cp_source )
    {
        *p_dest++ = *cp_source++;
        ++index;
    }

    return index;
}

uiw Funcs::StrSafeCpyAndCountWONullAdv( char *RSTR p_dest, const char *cp_source, uiw maxLen, char aes /* = '\0' */ )
{
    ASSUME( p_dest && cp_source );
    uiw index = 0;
    while( index < maxLen && *cp_source && *cp_source != aes )
    {
        *p_dest++ = *cp_source++;
        ++index;
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

NOINLINE uiw Funcs::F32ToStr( f32 val, char *p_buf )
{
    ASSUME( p_buf );
    return ::sprintf( p_buf, "%g", val );
}

uiw Funcs::F32ToStrWithPrecise( f32 val, ui32 precise, char *p_buf )
{
    ASSUME( p_buf );
    return ::sprintf( p_buf, "%.*f", precise, val );
}

NOINLINE uiw Funcs::F64ToStr( f64 val, char *p_buf )
{
    ASSUME( p_buf );
    return ::sprintf( p_buf, "%lg", val );
}

uiw Funcs::F64ToStrWithPrecise( f64 val, ui32 precise, char *p_buf )
{
    ASSUME( p_buf );
    return ::sprintf( p_buf, "%.*lf", precise, val );
}

#define VAR_ARGS_CREATE_PARAMS void *__p_pointer__; union { i64 __nothing__; byte __a_data__[ 8 ]; }; DBGCODE( ui32 __paramSize__; )
#define VAR_ARGS_SET_PARAM_POINTER( ch, list ) __p_pointer__ = __a_data__; \
    switch( ch ) { \
    case 'i': case 'u': case 'b': case 'c': case 'h': case 'n': *(ui32 *)__p_pointer__ = va_arg( list, ui32 ); DBGCODE( __paramSize__ = 4 ); break; \
    case 'p': case 'w': case 'o': case 's': case 'a': *(uiw *)__p_pointer__ = va_arg( list, uiw ); DBGCODE( __paramSize__ = sizeof(uiw) ); break; \
    case 'j': case 'm': *(ui64 *)__p_pointer__ = va_arg( list, ui64 ); DBGCODE( __paramSize__ = 8 ); break; \
    case 'f': case 'r': case 'd': case 'q': *(f64 *)__p_pointer__ = va_arg( list, f64 ); DBGCODE( __paramSize__ = 8 ); break; \
    default: __p_pointer__ = 0; DBGCODE( __paramSize__ = 0 ); \
    }
#define VAR_ARGS_GET_PARAM_POINTER __p_pointer__

#define FMT_PARSER_HELPER_NO_PARAM ui32_max
#define FMT_PARSER_HELPER_EXTRA_PARAM ui32_max - 1
#ifdef DEBUG
    #define FMT_PARSER_HELPER_SIZE __paramSize__
#endif

static const char *FmtParserHelper( const char *cp_fmt, char *p_ch, ui32 *p_param )
{
    ASSUME( cp_fmt && p_ch && p_param );

    if( *cp_fmt == '[' )
    {
        ++cp_fmt;
        const char *cp_end = _StrChr( cp_fmt, ']' );
        if( !cp_end )
        {
            return 0;
        }
        if( *cp_fmt == '*' )
        {
            if( cp_fmt + 1 == cp_end )
            {
                *p_param = FMT_PARSER_HELPER_EXTRA_PARAM;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if( cp_end - cp_fmt > 2 && Funcs::IsHexBase( cp_fmt ) )
            {
                cp_fmt += 2;
                if( !Funcs::StrHexToIntQuest( cp_fmt, p_param, cp_end - cp_fmt ) )
                {
                    return 0;
                }
            }
            else
            {
                if( !Funcs::StrDecToUI32Quest( cp_fmt, p_param, cp_end - cp_fmt ) )
                {
                    return 0;
                }
            }
        }
        cp_fmt = cp_end + 1;
    }
    else
    {
        *p_param = FMT_PARSER_HELPER_NO_PARAM;
    }

    *p_ch = *cp_fmt++;
    return cp_fmt;
}

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
l -
m - integer 64 as bin str [param - when non-zero, use upper case]
n - integer 32 as bin str [param - when non-zero, use upper case]
o - signed word
p - pointer, integer word as hex str [param - when non-zero, use upper case]
q - f64 rounded to i32 [param - precision]
r - f32 rounded to i32 [param - precision]
s - string [param - max length]
t -
u - ui32
v -
w - unsigned word
x -
y -
z -
*/

static bln ArgParserHelper( char type, void *p_source, ui32 param, char *p_buf, uiw maxLen, uiw *p_parsed )
{
    ASSUME( p_buf && p_parsed );

    char a_buf[ 128 ];
    uiw len;

    if( !p_source )
    {
        return false;
    }

    switch( type )
    {
    case 's':  //  string [param]
        if( !*(char **)p_source )
        {
            len = Funcs::StrSafeCpyAndCountWONull( p_buf, "[null str]", MIN( maxLen, sizeof("[null str]") - 1 ) );
        }
        else
        {
            len = Funcs::StrSafeCpyAndCountWONull( p_buf, *(char **)p_source, MIN( maxLen, param ) );
        }
        goto retLen;
    case 'f':  //  f32 [param]
    {
        f32 f32val = (f32)*(f64 *)p_source;
        if( param != FMT_PARSER_HELPER_NO_PARAM )
        {
            len = Funcs::F32ToStrWithPrecise( f32val, param, a_buf );
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
    case 'b':  //  bln [param]
        len = *(ui32 *)p_source ? _StrLen( "true" ) : _StrLen( "false" );
        if( maxLen < len )
        {
            return false;
        }
        if( param == FMT_PARSER_HELPER_NO_PARAM )
        {
            param = 0;
        }
        _MemCpy( p_buf, *(ui32 *)p_source ? (param ? "TRUE" : "true") : (param ? "FALSE" : "false"), len );
        goto retLen;
    case 'c':  //  char
        if( !maxLen )
        {
            return false;
        }
        *p_buf = (char)*(i32 *)p_source;
        len = 1;
        goto retLen;
    case 'h':  //  integer 32 as hex str [param]
        if( maxLen < 8 )
        {
            return false;
        }
        len = Funcs::IntToStrHex( param == FMT_PARSER_HELPER_NO_PARAM ? true : param != 0, false, false, p_buf, *(ui32 *)p_source );
        goto retLen;
    case 'j':  //  integer 64 as hex str [param]
        if( maxLen < 16 )
        {
            return false;
        }
        len = Funcs::IntToStrHex( param == FMT_PARSER_HELPER_NO_PARAM ? true : param != 0, false, false, p_buf, *(ui64 *)p_source );
        goto retLen;
    case 'p':  //  pointer, integer word as hex str [param]
        if( maxLen < sizeof(void *) * 2 )
        {
            return false;
        }
        len = Funcs::IntToStrHex( param == FMT_PARSER_HELPER_NO_PARAM ? true : param != 0, false, false, p_buf, *(uiw *)p_source );
        goto retLen;
    case 'n':  //  integer 32 as bin str [param]
        if( maxLen < 32 )
        {
            return false;
        }
        len = Funcs::IntToStrBin( *(ui32 *)p_source, p_buf, false, param == FMT_PARSER_HELPER_NO_PARAM ? false : param != 0 );
        goto retLen;
    case 'm':  //  integer 64 as bin str [param]
        if( maxLen < 64 )
        {
            return false;
        }
        len = Funcs::IntToStrBin( *(ui64 *)p_source, p_buf, false, param == FMT_PARSER_HELPER_NO_PARAM ? false : param != 0 );
        goto retLen;
    case 'a':  //  pointer, integer word as bin str [param]
        if( maxLen < WORD_SIZE )
        {
            return false;
        }
        len = Funcs::IntToStrBin( *(uiw *)p_source, p_buf, false, param == FMT_PARSER_HELPER_NO_PARAM ? false : param != 0 );
        goto retLen;
    case 'w':  //  unsigned word
        len = Funcs::IntToStrDec( *(uiw *)p_source, a_buf );
        goto retBuf;
    case 'o':  //  signed word
        len = Funcs::IntToStrDec( *(iw *)p_source, a_buf );
        goto retBuf;
    case 'd':  //  f64 [param]
        if( param != FMT_PARSER_HELPER_NO_PARAM )
        {
            len = Funcs::F64ToStrWithPrecise( *(f64 *)p_source, param, a_buf );
        }
        else
        {
            len = Funcs::F64ToStr( *(f64 *)p_source, a_buf );
        }
        goto retBuf;
    case 'r':  //  f32 rounded to i32 [param]
    {
        i32 val;
        if( param != FMT_PARSER_HELPER_NO_PARAM )
        {
            val = Funcs::RoundF32WithPrecise( *(f32 *)p_source, param );
        }
        else
        {
            val = Funcs::RoundF32( *(f32 *)p_source );
        }
        len = Funcs::IntToStrDec( val, a_buf );
        goto retBuf;
    }
    case 'q':  //  f64 rounded to i32 [param]
    {
        i32 val;
        if( param != FMT_PARSER_HELPER_NO_PARAM )
        {
            val = Funcs::RoundF64WithPrecise( *(f64 *)p_source, param );
        }
        else
        {
            val = Funcs::RoundF64( *(f64 *)p_source );
        }
        len = Funcs::IntToStrDec( val, a_buf );
        goto retBuf;
    }
    case '%':  //  % symbol
        if( !maxLen )
        {
            return false;
        }
        *p_buf = '%';
        len = 1;
        goto retLen;
    default:
        UNREACHABLE;
    }

retBuf:
    if( len > maxLen )
    {
        return false;
    }
    _MemCpy( p_buf, a_buf, len );
retLen:
    *p_parsed = len;
    return true;
}

EXTERNAL uiw Funcs::PrintToStrArgList( char *p_str, uiw maxLen, const char *cp_fmt, va_list args )
{
    ASSUME( p_str && cp_fmt && maxLen != uiw_max );

    char *p_currStr = p_str;
    uiw parsedLen;
    ui32 param;
    char ch;
    VAR_ARGS_CREATE_PARAMS;

    while( *cp_fmt && (uiw)(p_currStr - p_str) < maxLen )
    {
        if( *cp_fmt == '%' )
        {
            cp_fmt = FmtParserHelper( cp_fmt + 1, &ch, &param );
            if( cp_fmt )
            {
                if( param == FMT_PARSER_HELPER_EXTRA_PARAM )
                {
                    param = va_arg( args, ui32 );
                }
                VAR_ARGS_SET_PARAM_POINTER( ch, args );
                if( ArgParserHelper( ch, VAR_ARGS_GET_PARAM_POINTER, param, p_currStr, maxLen - (p_currStr - p_str), &parsedLen ) )
                {
                    p_currStr += parsedLen;
                    continue;
                }
            }
            DBGBREAK;
            p_currStr = p_str;
            break;
        }
        else
        {
            *p_currStr++ = *cp_fmt++;
        }
    }

    *p_currStr = '\0';

    return p_currStr - p_str;
}

#if defined(DEBUG) && defined(VAR_TEMPLATES_SUPPORTED)
NOINLINE uiw Funcs::_PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, ... )
#else
NOINLINE uiw Funcs::PrintToStr( char *p_str, uiw maxLen, const char *cp_fmt, ... )
#endif
{
    ASSUME( p_str && cp_fmt && maxLen != uiw_max );

    va_list args;
    va_start( args, cp_fmt );

    uiw printedLen = Funcs::PrintToStrArgList( p_str, maxLen, cp_fmt, args );

    va_end( args );

    return printedLen;
}