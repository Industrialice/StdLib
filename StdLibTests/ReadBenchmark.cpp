#include <StdAbstractionLib.hpp>
#include <vector>
#include <numeric>

using namespace StdLib;

static void ReadSimple( byte *target, const byte *source, ui64 size )
{
	memcpy( target, source, size );
}

static void ReadComplex( byte *target, const byte *source, ui64 size )
{
	struct TestStruck
	{
		int intVal;
		float x, y, z;
		char name[ 25 ];
	};

	TestStruck *targetInt = (TestStruck *)target;

	for( ; size >= sizeof(TestStruck); )
	{
		targetInt->intVal = *(int *)source;
		source += sizeof(int);
		targetInt->x = *(float *)source;
		source += sizeof(float);
		targetInt->y = *(float *)source;
		source += sizeof(float);
		targetInt->z = *(float *)source;
		source += sizeof(float);
		memcpy( targetInt->name, source, 25 );
		source += 25;

		targetInt++;
		size -= sizeof(TestStruck);
	}
}

void ReadBenchmark( ui32 iterations )
{
	const ui64 size = 1 * 1024 * 1024 * 1024;

	byte *bigRandomBuffer = (byte *)VirtualAlloc( 0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if( bigRandomBuffer == nullptr )
	{
		printf( "failed to allocate big read buffer\n" );
		return;
	}

	byte *bigWrite = (byte *)VirtualAlloc( 0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if( bigWrite == nullptr )
	{
		printf( "failed to allocate big write buffer\n" );
		return;
	}

	std::vector < int > it( iterations );
	std::iota( it.begin(), it.end(), 0 );

	for( auto index : it )
	{
		const char *readType = nullptr;

		TimeMoment moment = TimeMoment::CreateCurrent();

		if( rand() % 2 )
		{
			readType = "complex";
			ReadComplex( bigWrite, bigRandomBuffer, size );
		}
		else
		{
			readType = "simple";
			ReadSimple( bigWrite, bigRandomBuffer, size );
		}

		TimeMoment end = TimeMoment::CreateCurrent();

		float time = end.SinceSec32( moment );
		double throughput = size / time;

		char normalized[ 128 ];
		Funcs::NormalizeMem64( throughput, normalized );

		printf( "it took %.6lf seconds to process using %s method, throughput %s/s\n", time, readType, normalized );
	}
}