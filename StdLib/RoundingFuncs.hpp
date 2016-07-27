#ifndef __ROUNDING_FUNCS_HPP__
#define __ROUNDING_FUNCS_HPP__

namespace StdLib {

//  TODO: this is embarrassment

namespace Funcs
{
    EXTERNALS i32 RoundF32( f32 val );
    EXTERNALS i32 RoundF32WithPrecise( f32 val, ui32 precise );
    EXTERNALS f32 RoundF32DownToStep( f32 val, f32 step );
    EXTERNALS f32 RoundF32UpToStep( f32 val, f32 step );
    EXTERNALS f32 RoundF32ToNearestStep( f32 val, f32 step );
    EXTERNALS i32 RoundF64( f64 val );
    EXTERNALS i32 RoundF64WithPrecise( f64 val, ui32 precise );
    EXTERNALS f64 RoundF64DownToStep( f64 val, f64 step );
    EXTERNALS f64 RoundF64UpToStep( f64 val, f64 step );
    EXTERNALS f64 RoundF64ToNearestStep( f64 val, f64 step );
    EXTERNALS uiw RoundUIDownToStep( uiw val, uiw step );
    EXTERNALS uiw RoundUIUpToStep( uiw val, uiw step );
    EXTERNALS uiw RoundUIToNearestStep( uiw val, uiw step );
    EXTERNALS iw RoundIDownToStep( iw val, iw step );  //  TODO: complete
    EXTERNALS iw RoundIUpToStep( iw val, iw step );  //  TODO: complete
    EXTERNALS iw RoundIToNearestStep( iw val, iw step );  //  TODO: complete
} }

#endif