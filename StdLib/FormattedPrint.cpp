#include "PreHeader.hpp"
#include "FormattedPrint.hpp"
#include "FileMemoryStream.hpp"

class ParserParam
{
	ui32 _param;
	bool _is_present;

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

	bool IsPresent() const
	{
		return _is_present;
	}
};

static const char *FmtParserHelper( const char *cp_fmt, char *p_ch, ParserParam *param, bool *is_extra )
{
    ASSUME( cp_fmt && p_ch && param );

	param->SetAbsent();
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
				auto conversionResult = Funcs::StrHexToIntQuest < ui32 >( cp_fmt, cp_end - cp_fmt );
                if( false == conversionResult.IsNull() )
				{
					param->SetParam( conversionResult.Get() );
				}
				else
                {
                    SOFTBREAK;
                    return 0;
                }
            }
            else
            {
				auto conversionResult = Funcs::StrDecToUI32Quest( cp_fmt, cp_end - cp_fmt );
                if( false == conversionResult.IsNull() )
				{
					param->SetParam( conversionResult.Get() );
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

bool ArgParserHelper( char type, void *p_source, ParserParam param, uiw *written, FileInterface *file )
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
    case 'b':  //  bool [param]
	{
        len = *(ui32 *)p_source ? _StrLen( "true" ) : _StrLen( "false" );
		bool is_capitalize = param.IsPresent() == false || param.GetParamU() == 0 ? false : true;
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

template < bool is_validateStep > inline Nullable < uiw > PrintToStrArgListImpl( const Funcs::_ArgType *argTypes, uiw argsCount, const char *cp_fmt, va_list args, FileInterface *file )
{
    using namespace Funcs;

	const char *const dbgStr = "#error parsing fmt string#";
	bool is_extraParam;
	uiw appendedLen = 0, written;
	const char *cp_fmtFlushed = cp_fmt;
    uiw argIndex = 0;
    ui32 intParam;
	ParserParam param;
    char ch;
    ALIGNED_PRE( 8 ) ui64 argumentData ALIGNED_POST( 8 );

    #define LOAD_ARG( expectedType, target ) \
        if( is_validateStep && argIndex >= argsCount ) { HARDBREAK; return nullptr; } \
        if( expectedType.is_fp ) \
        { \
			ASSUME( sizeof(target) >= 8 ); \
            if( is_validateStep && (!argTypes[ argIndex ].is_fp || argTypes[ argIndex ].is_pointer) ) { HARDBREAK; return nullptr; } \
            *(f64 *)&target = va_arg( args, f64 ); \
        } \
        else if( expectedType.size == 4 ) \
        { \
			ASSUME( sizeof(target) >= 4 ); \
            if( is_validateStep && ((expectedType.is_string && !argTypes[ argIndex ].is_string) || (expectedType.size > argTypes[ argIndex ].size) || argTypes[ argIndex ].size > 4) ) { HARDBREAK; return nullptr; } \
            *(ui32 *)&target = va_arg( args, ui32 ); \
        } \
        else if( expectedType.size == 8 ) \
        { \
			ASSUME( sizeof(target) >= 8 ); \
            if( is_validateStep && ((expectedType.is_string && !argTypes[ argIndex ].is_string) || (expectedType.size > argTypes[ argIndex ].size) || argTypes[ argIndex ].size > 8) ) { HARDBREAK; return nullptr; } \
            *(ui64 *)&target = va_arg( args, ui64 ); \
        } \
        else \
        { \
            HARDBREAK; \
            return nullptr; \
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
						return nullptr;
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
							return nullptr;
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
        return nullptr;
    }

	if( is_validateStep == false && cp_fmt > cp_fmtFlushed )
	{
		if( !file->Write( cp_fmtFlushed, cp_fmt - cp_fmtFlushed ) )
		{
			SOFTBREAK;
			return nullptr;
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

	FileMemoryStream file( stream, FileProcMode::Write );
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

uiw Funcs::PrintToMemoryStreamArgList( MemoryStreamInterface &stream, const char *cp_fmt, va_list args )
{
    ASSUME( stream.IsWritable() && cp_fmt );

	FileMemoryStream file( stream, FileProcMode::WriteAppend );
	ASSUME( file.IsOpened() );

    Nullable < uiw > written = PrintToStrArgListImpl < false >( 0, 0, cp_fmt, args, &file );

    return stream.Resize( written.IsNull() ? 0 : written.Get() );
}

uiw Funcs::PrintToFileArgList( FileInterface &file, const char *cp_fmt, va_list args )
{
	ASSUME( file.IsOpened() && !!(file.ProcModeGet() & FileProcMode::Write) && cp_fmt );

	return PrintToStrArgListImpl < false >( 0, 0, cp_fmt, args, &file ).ValueOrDefault( 0 );
}

#if defined(DEBUG_VALIDATE_PRINT_FUNCS) && defined(DEBUG)
bool Funcs::_PrintCheckArgs( const _ArgType *argTypes, uiw argsCount, const char *cp_fmt, ... )
{
    ASSUME( argTypes );

    va_list args;
    va_start( args, cp_fmt );

    Nullable < uiw > written = PrintToStrArgListImpl < true >( argTypes, argsCount, cp_fmt, args, 0 );

    va_end( args );

    return written.IsNull() == false;
}
#endif