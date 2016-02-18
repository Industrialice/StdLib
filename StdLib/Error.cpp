#include "PreHeader.hpp"
#include "Error.hpp"

const char *StdLib::Error::Private::GetErrorDesc( ui32 code )
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
        "INTERRUPTED",
		"CANNOT_OPEN_FILE"
    };
	ASSUME( code < COUNTOF( errors ) );
    return errors[ code ];
}