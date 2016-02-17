#ifndef __ERROR_HPP__
#define __ERROR_HPP__

namespace StdLib {

struct SError
{
    ui32 code;
    ui32 addition;
    const char *description;
};

namespace Error
{
    EXTERNAL SError Get( ui32 code, ui32 addition = 0 );
    EXTERNAL SError GetOther( ui32 code, const char *const descs[], ui32 addition = 0 );

    const ui32 Ok = 0;
    const ui32 Unknown = 1;
    const ui32 InvalidArgument = 2;
    const ui32 OutOfMemory = 3;
    const ui32 AlreadyExists = 4;
    const ui32 DoesNotExists = 5;
    const ui32 NoAccess = 6;
    const ui32 Unsupported = 7;
    const ui32 Unimplemented = 8;
    const ui32 Obsolete = 9;
    const ui32 Interrupted = 10;

    namespace Private
    {
        const ui32 MaxDefaultError = 128;
    
        EXTERNAL const char *const *GetErrorsDesc();
    }
}

}  //  namespace StdLib

#endif __ERROR_HPP__