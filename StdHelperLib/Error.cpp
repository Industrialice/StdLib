#include "PreHeader.hpp"
#include "Error.hpp"

const char *const *Error::Private::GetErrorsDesc()
{
    static const char *const errors[] = 
    { 
        "OK",
        "UNKNOWN",
        "INVALID_ARGUMENT",
        "OUT_OF_MEMORY",
        "ALREADY_EXISTS",
        "DOES_NOT_EXISTS",
        "NO_ACCESS",
        "UNSUPPORTED",
        "UNIMPLEMENTED",
        "OBSOLETE",
        "INTERRUPTED"
    };
    return errors;
}

SError Error::Get( ui32 code, ui32 addition /* = 0 */ )
{
    SError o_error = { code, addition, Private::GetErrorsDesc()[ code ] };
    return o_error;
}

SError Error::GetOther( ui32 code, const char *const descs[], ui32 addition /* = 0 */ )
{
    ASSUME( code >= Error::Private::MaxDefaultError && descs );
    ui32 otherCode = code - Error::Private::MaxDefaultError;
    SError o_error = { otherCode, addition, descs[ otherCode ] };
    return o_error;
}
