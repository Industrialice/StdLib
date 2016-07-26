#include "PreHeader.hpp"
#include "RandomizingFuncs.hpp"

static ui32 WideRandom()
{
	return rand() | (rand() << 15) | (rand() << 30);
}

i32 Funcs::RandomI32()
{
    return (i32)WideRandom();
}

ui32 Funcs::RandomUI32()
{
    return WideRandom();
}

f32 Funcs::RandomF32()
{
    return rand() / (f32)RAND_MAX;
}

i32 Funcs::RandomRangeI32( i32 from, i32 to )
{
    return (WideRandom() & 0x7FffFFff) % ((to - from) + 1) + from;
}

ui32 Funcs::RandomRangeUI32( ui32 from, ui32 to )
{
    return WideRandom() % ((to - from) + 1) + from;
}

f32 Funcs::RandomRangeF32( f32 from, f32 to )
{
    return from + (rand() / (f32)RAND_MAX) * (to - from);
}

ui32 Funcs::RandomUI32Limit( ui32 limit )
{
    return WideRandom() % limit;
}
	
i32 Funcs::RandomFluctuateI32( i32 target, i32 fluctuation )
{
	ASSUME( fluctuation >= 0 );
	i32 distToMin = target - i32_min;
	i32 distToMax = i32_max - target;
	i32 from = target - (distToMin > fluctuation ? fluctuation : distToMin);
	i32 to = target + (distToMax > fluctuation ? fluctuation : distToMax);
	return RandomRangeI32( from, to );
}

ui32 Funcs::RandomFluctuateUI32( ui32 target, ui32 fluctuation )
{
	ui32 distToMin = target;
	ui32 distToMax = ui32_max - target;
	ui32 from = target - (distToMin > fluctuation ? fluctuation : distToMin);
	ui32 to = target + (distToMax > fluctuation ? fluctuation : distToMax);
	return RandomRangeUI32( from, to );
}

f32 Funcs::RandomFluctuateF32( f32 target, f32 fluctuation )
{
	ASSUME( fluctuation >= 0.0f );
	return RandomRangeF32( target - fluctuation, target + fluctuation );
}