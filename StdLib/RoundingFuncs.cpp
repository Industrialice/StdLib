#include "PreHeader.hpp"
#include "RoundingFuncs.hpp"

i32 Funcs::RoundF32( f32 val )  //  TODO: bullshit
{
	return std::round( val );  //  C++11
    //return (i32)(val < 0 ? val - 0.55555555555f : val + 0.55555555555f);
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

f32 Funcs::RoundF32DownToStep( f32 val, f32 step )  //  TOD: wtf is this?
{
    val /= step;
    val = ::floorf( val );
    return step * val;
}

f32 Funcs::RoundF32UpToStep( f32 val, f32 step )  //  TOD: wtf is this?
{
    val /= step;
    val = ::floorf( val );
    return step * (val + 1.f);
}

f32 Funcs::RoundF32ToNearestStep( f32 val, f32 step )  //  TOD: wtf is this?
{
    val /= step;
    val = ::floorf( val + 0.5f );
    return step * val;
}

i32 Funcs::RoundF64( f64 val )  //  TODO: bullshit
{
	return std::round( val );  //  C++11
    //return (i32)(val < 0 ? val - 0.555555555555555 : val + 0.555555555555555);
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

f64 Funcs::RoundF64DownToStep( f64 val, f64 step )  //  TOD: wtf is this?
{
    val /= step;
    val = ::floor( val );
    return step * val;
}

f64 Funcs::RoundF64UpToStep( f64 val, f64 step )  //  TOD: wtf is this?
{
    val /= step;
    val = ::floor( val );
    return step * (val + 1.f);
}

f64 Funcs::RoundF64ToNearestStep( f64 val, f64 step )  //  TOD: wtf is this?
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
	NOT_IMPLEMENTED;
    return 0;
}

iw Funcs::RoundIUpToStep( iw val, iw step )
{
	NOT_IMPLEMENTED;
    return 0;
}

iw Funcs::RoundIToNearestStep( iw val, iw step )
{
	NOT_IMPLEMENTED;
    return 0;
}