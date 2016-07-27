#ifndef __RANDOMIZING_FUNCS_HPP__
#define __RANDOMIZING_FUNCS_HPP__

namespace StdLib {

namespace Funcs
{
    EXTERNALS i32 RandomI32();  //  [ 0x80000000 ; 0x7FffFFff ]
    EXTERNALS ui32 RandomUI32();  //  [ 0 ; 0xFFffFFff ]
    EXTERNALS f32 RandomF32();  //  [ 0.f ; 1.f ]
    EXTERNALS i32 RandomRangeI32( i32 from, i32 to );
    EXTERNALS ui32 RandomRangeUI32( ui32 from, ui32 to );
    EXTERNALS f32 RandomRangeF32( f32 from, f32 to );
    EXTERNALS ui32 RandomUI32Limit( ui32 limit );  //  [ 0; limit )
	EXTERNALS i32 RandomFluctuateI32( i32 target, i32 fluctuation );  //  will cut overflow values, fluctuation must be >= 0
	EXTERNALS ui32 RandomFluctuateUI32( ui32 target, ui32 fluctuation );  //  will cut overflow values
	EXTERNALS f32 RandomFluctuateF32( f32 target, f32 fluctuation );  //  fluctuation must be >= 0
} }

#endif