﻿#include <StdAbstractionLib.hpp>

#include <assert.h>
#include <array>

#pragma comment( lib, "StdCoreLib_Static.lib" )
#pragma comment( lib, "StdHelperLib_Static.lib" )
#pragma comment( lib, "StdAbstractionLib_Static.lib" )

#include <stdio.h>

#include "TextFixer.hpp"
//#include <FileIO.hpp>
//#include <Files.hpp>
#include <FileMapping.hpp>

#include <PackedIntArray.hpp>
#include "DeleteShit.hpp"
#include <FileCFILEStream.hpp>

#include "RingBuffer.h"

using namespace StdLib;

struct SDummy
{
    ~SDummy()
    {
        ::printf( "destr\n" );
    }

    SDummy()
    {
        ::printf( "constr\n" );
    }

    SDummy( const SDummy &source )
    {

    }

    /*SDummy &operator =( const SDummy &source )
    {
        return *this;
    }*/
};

void OutVec3( const char *cp_str, const vec3 *cpo_vec )
{
    char a_bb[ 1024 ];
    Funcs::PrintToStr( a_bb, 1024, "%s\n%[3]f %[3]f %[3]f\n", cp_str, cpo_vec->x, cpo_vec->y, cpo_vec->z );
    ::printf( "%s\n\n", a_bb );
}

void OutVec4( const char *cp_str, const vec4 *cpo_vec )
{
    char a_bb[ 1024 ];
    Funcs::PrintToStr( a_bb, 1024, "%s\n%[3]f %[3]f %[3]f %[3]f\n", cp_str, cpo_vec->x, cpo_vec->y, cpo_vec->z, cpo_vec->w );
    ::printf( "%s\n\n", a_bb );
}

void Out4x3( const char *cp_str, const m4x3 *cpo_m )
{
    char a_bb[ 1024 ];
    Funcs::PrintToStr( a_bb, 1024, "%s\n%[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f\n", cp_str, cpo_m->e00, cpo_m->e01, cpo_m->e02,
                                                                                                                  cpo_m->e10, cpo_m->e11, cpo_m->e12,
                                                                                                                  cpo_m->e20, cpo_m->e21, cpo_m->e22,
                                                                                                                  cpo_m->e30, cpo_m->e31, cpo_m->e32 );
    ::printf( "%s\n\n", a_bb );
}

void Out3x4( const char *cp_str, const m3x4 *cpo_m )
{
    char a_bb[ 1024 ];
    Funcs::PrintToStr( a_bb, 1024, "%s\n%[3]f %[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f %[3]f\n", cp_str, cpo_m->e00, cpo_m->e01, cpo_m->e02,
                                                                                                                  cpo_m->e10, cpo_m->e11, cpo_m->e12,
                                                                                                                  cpo_m->e20, cpo_m->e21, cpo_m->e22 );
    ::printf( "%s\n\n", a_bb );
}

void Out4x4( const char *cp_str, const m4x4 *cpo_m )
{
    char a_bb[ 1024 ];
    Funcs::PrintToStr( a_bb, 1024, "%s\n%[3]f %[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f %[3]f\n%[3]f %[3]f %[3]f %[3]f\n", cp_str, cpo_m->e00, cpo_m->e01, cpo_m->e02, cpo_m->e03,
                                                                                                                  cpo_m->e10, cpo_m->e11, cpo_m->e12, cpo_m->e13,
                                                                                                                  cpo_m->e20, cpo_m->e21, cpo_m->e22, cpo_m->e23,
                                                                                                                  cpo_m->e30, cpo_m->e31, cpo_m->e32, cpo_m->e33 );
    ::printf( "%s\n\n", a_bb );
}

#if 0

static void Dir( CLogger::Tag tag, const char *cp_text, ui32 len )
{
    const char *cp_tag;
    switch( tag )
    {
    case CLogger::Tag::info:
        cp_tag = "info";
        break;
    case CLogger::Tag::warning:
        cp_tag = "warning";
        break;
    case CLogger::Tag::error:
        cp_tag = "error";
        break;
    case CLogger::Tag::debug:
        cp_tag = "debug";
        break;
    case CLogger::Tag::user:
        cp_tag = "user";
        break;
    case CLogger::Tag::important:
        cp_tag = "important";
        break;
    default:
        UNREACHABLE;
    }
    ::printf( "[%s] %s", cp_tag, cp_text );
}

static void Dir2( CLogger::Tag tag, const char *cp_text, ui32 len )
{
    const char *cp_tag;
    switch( tag )
    {
    case CLogger::Tag::info:
        cp_tag = "info";
        break;
    case CLogger::Tag::warning:
        cp_tag = "warning";
        break;
    case CLogger::Tag::error:
        cp_tag = "error";
        break;
    case CLogger::Tag::debug:
        cp_tag = "debug";
        break;
    case CLogger::Tag::user:
        cp_tag = "user";
        break;
    case CLogger::Tag::important:
        cp_tag = "important";
        break;
    default:
        UNREACHABLE;
    }
    char a_cat[ 2048 ];
    _StrCpy( a_cat, "[" );
    _StrCat( a_cat, cp_tag );
    _StrCat( a_cat, "] " );
    _StrCat( a_cat, cp_text );
    OutputDebugStringA( a_cat );
}

namespace StdLib {
namespace LiceMath
{
    static void M4x4InverseTranspose( m4x4 *RSTR pout, const m4x4 *pm )
    {
        f32 SubFactor00 = pm->m[2][2] * pm->m[3][3] - pm->m[3][2] * pm->m[2][3];
        f32 SubFactor01 = pm->m[2][1] * pm->m[3][3] - pm->m[3][1] * pm->m[2][3];
        f32 SubFactor02 = pm->m[2][1] * pm->m[3][2] - pm->m[3][1] * pm->m[2][2];
        f32 SubFactor03 = pm->m[2][0] * pm->m[3][3] - pm->m[3][0] * pm->m[2][3];
        f32 SubFactor04 = pm->m[2][0] * pm->m[3][2] - pm->m[3][0] * pm->m[2][2];
        f32 SubFactor05 = pm->m[2][0] * pm->m[3][1] - pm->m[3][0] * pm->m[2][1];
        f32 SubFactor06 = pm->m[1][2] * pm->m[3][3] - pm->m[3][2] * pm->m[1][3];
        f32 SubFactor07 = pm->m[1][1] * pm->m[3][3] - pm->m[3][1] * pm->m[1][3];
        f32 SubFactor08 = pm->m[1][1] * pm->m[3][2] - pm->m[3][1] * pm->m[1][2];
        f32 SubFactor09 = pm->m[1][0] * pm->m[3][3] - pm->m[3][0] * pm->m[1][3];
        f32 SubFactor10 = pm->m[1][0] * pm->m[3][2] - pm->m[3][0] * pm->m[1][2];
        f32 SubFactor11 = pm->m[1][1] * pm->m[3][3] - pm->m[3][1] * pm->m[1][3];
        f32 SubFactor12 = pm->m[1][0] * pm->m[3][1] - pm->m[3][0] * pm->m[1][1];
        f32 SubFactor13 = pm->m[1][2] * pm->m[2][3] - pm->m[2][2] * pm->m[1][3];
        f32 SubFactor14 = pm->m[1][1] * pm->m[2][3] - pm->m[2][1] * pm->m[1][3];
        f32 SubFactor15 = pm->m[1][1] * pm->m[2][2] - pm->m[2][1] * pm->m[1][2];
        f32 SubFactor16 = pm->m[1][0] * pm->m[2][3] - pm->m[2][0] * pm->m[1][3];
        f32 SubFactor17 = pm->m[1][0] * pm->m[2][2] - pm->m[2][0] * pm->m[1][2];
        f32 SubFactor18 = pm->m[1][0] * pm->m[2][1] - pm->m[2][0] * pm->m[1][1];

        pout->m[0][0] = + (pm->m[1][1] * SubFactor00 - pm->m[1][2] * SubFactor01 + pm->m[1][3] * SubFactor02);
        pout->m[0][1] = - (pm->m[1][0] * SubFactor00 - pm->m[1][2] * SubFactor03 + pm->m[1][3] * SubFactor04);
        pout->m[0][2] = + (pm->m[1][0] * SubFactor01 - pm->m[1][1] * SubFactor03 + pm->m[1][3] * SubFactor05);
        pout->m[0][3] = - (pm->m[1][0] * SubFactor02 - pm->m[1][1] * SubFactor04 + pm->m[1][2] * SubFactor05);

        pout->m[1][0] = - (pm->m[0][1] * SubFactor00 - pm->m[0][2] * SubFactor01 + pm->m[0][3] * SubFactor02);
        pout->m[1][1] = + (pm->m[0][0] * SubFactor00 - pm->m[0][2] * SubFactor03 + pm->m[0][3] * SubFactor04);
        pout->m[1][2] = - (pm->m[0][0] * SubFactor01 - pm->m[0][1] * SubFactor03 + pm->m[0][3] * SubFactor05);
        pout->m[1][3] = + (pm->m[0][0] * SubFactor02 - pm->m[0][1] * SubFactor04 + pm->m[0][2] * SubFactor05);

        pout->m[2][0] = + (pm->m[0][1] * SubFactor06 - pm->m[0][2] * SubFactor07 + pm->m[0][3] * SubFactor08);
        pout->m[2][1] = - (pm->m[0][0] * SubFactor06 - pm->m[0][2] * SubFactor09 + pm->m[0][3] * SubFactor10);
        pout->m[2][2] = + (pm->m[0][0] * SubFactor11 - pm->m[0][1] * SubFactor09 + pm->m[0][3] * SubFactor12);
        pout->m[2][3] = - (pm->m[0][0] * SubFactor08 - pm->m[0][1] * SubFactor10 + pm->m[0][2] * SubFactor12);

        pout->m[3][0] = - (pm->m[0][1] * SubFactor13 - pm->m[0][2] * SubFactor14 + pm->m[0][3] * SubFactor15);
        pout->m[3][1] = + (pm->m[0][0] * SubFactor13 - pm->m[0][2] * SubFactor16 + pm->m[0][3] * SubFactor17);
        pout->m[3][2] = - (pm->m[0][0] * SubFactor14 - pm->m[0][1] * SubFactor16 + pm->m[0][3] * SubFactor18);
        pout->m[3][3] = + (pm->m[0][0] * SubFactor15 - pm->m[0][1] * SubFactor17 + pm->m[0][2] * SubFactor18);

        f32 det = + pm->m[0][0] * pout->m[0][0] + pm->m[0][1] * pout->m[0][1] + pm->m[0][2] * pout->m[0][2] + pm->m[0][3] * pout->m[0][3];
        f32 revDet = 1.f / det;

        LiceMath::M4x4MultScalarInplace( pout, revDet );
    }

    static void M4x4InverseTranspose4x3( m4x4 *RSTR pout, const m4x3 *pm )
    {
        f32 SubFactor00 = pm->m[2][2];
        f32 SubFactor01 = pm->m[2][1];
        f32 SubFactor02 = pm->m[2][1] * pm->m[3][2] - pm->m[3][1] * pm->m[2][2];
        f32 SubFactor03 = pm->m[2][0];
        f32 SubFactor04 = pm->m[2][0] * pm->m[3][2] - pm->m[3][0] * pm->m[2][2];
        f32 SubFactor05 = pm->m[2][0] * pm->m[3][1] - pm->m[3][0] * pm->m[2][1];
        f32 SubFactor06 = pm->m[1][2];
        f32 SubFactor07 = pm->m[1][1];
        f32 SubFactor08 = pm->m[1][1] * pm->m[3][2] - pm->m[3][1] * pm->m[1][2];
        f32 SubFactor09 = pm->m[1][0];
        f32 SubFactor10 = pm->m[1][0] * pm->m[3][2] - pm->m[3][0] * pm->m[1][2];
        f32 SubFactor11 = pm->m[1][1];
        f32 SubFactor12 = pm->m[1][0] * pm->m[3][1] - pm->m[3][0] * pm->m[1][1];
        f32 SubFactor13 = 0.f;
        f32 SubFactor14 = 0.f;
        f32 SubFactor15 = pm->m[1][1] * pm->m[2][2] - pm->m[2][1] * pm->m[1][2];
        f32 SubFactor16 = 0.f;
        f32 SubFactor17 = pm->m[1][0] * pm->m[2][2] - pm->m[2][0] * pm->m[1][2];
        f32 SubFactor18 = pm->m[1][0] * pm->m[2][1] - pm->m[2][0] * pm->m[1][1];

        pout->m[0][0] = + (pm->m[1][1] * SubFactor00 - pm->m[1][2] * SubFactor01);
        pout->m[0][1] = - (pm->m[1][0] * SubFactor00 - pm->m[1][2] * SubFactor03);
        pout->m[0][2] = + (pm->m[1][0] * SubFactor01 - pm->m[1][1] * SubFactor03);
        pout->m[0][3] = - (pm->m[1][0] * SubFactor02 - pm->m[1][1] * SubFactor04 + pm->m[1][2] * SubFactor05);

        pout->m[1][0] = - (pm->m[0][1] * SubFactor00 - pm->m[0][2] * SubFactor01);
        pout->m[1][1] = + (pm->m[0][0] * SubFactor00 - pm->m[0][2] * SubFactor03);
        pout->m[1][2] = - (pm->m[0][0] * SubFactor01 - pm->m[0][1] * SubFactor03);
        pout->m[1][3] = + (pm->m[0][0] * SubFactor02 - pm->m[0][1] * SubFactor04 + pm->m[0][2] * SubFactor05);

        pout->m[2][0] = + (pm->m[0][1] * SubFactor06 - pm->m[0][2] * SubFactor07);
        pout->m[2][1] = - (pm->m[0][0] * SubFactor06 - pm->m[0][2] * SubFactor09);
        pout->m[2][2] = + (pm->m[0][0] * SubFactor11 - pm->m[0][1] * SubFactor09);
        pout->m[2][3] = - (pm->m[0][0] * SubFactor08 - pm->m[0][1] * SubFactor10 + pm->m[0][2] * SubFactor12);

        pout->m[3][0] = - (pm->m[0][1] * SubFactor13 - pm->m[0][2] * SubFactor14);
        pout->m[3][1] = + (pm->m[0][0] * SubFactor13 - pm->m[0][2] * SubFactor16);
        pout->m[3][2] = - (pm->m[0][0] * SubFactor14 - pm->m[0][1] * SubFactor16);
        pout->m[3][3] = + (pm->m[0][0] * SubFactor15 - pm->m[0][1] * SubFactor17 + pm->m[0][2] * SubFactor18);

        f32 det = + pm->m[0][0] * pout->m[0][0] + pm->m[0][1] * pout->m[0][1] + pm->m[0][2] * pout->m[0][2];
        f32 revDet = 1.f / det;

        LiceMath::M4x4MultScalarInplace( pout, revDet );
    }
}
}

/*void PrintStats( const FileIO::SStats *stats )
{
    ::printf( "writesToBufferCount %u\n", stats->writesToBufferCount );
    ::printf( "writesToFileCount %u\n", stats->writesToFileCount );
    ::printf( "readsFromBufferCount %u\n", stats->readsFromBufferCount );
    ::printf( "readsFromFileCount %u\n", stats->readsFromFileCount );
    ::printf( "bytesFromBufferReaded %u\n", stats->bytesFromBufferReaded );
    ::printf( "bytesFromFileReaded %u\n", stats->bytesFromFileReaded );
    ::printf( "bytesToBufferWritten %u\n", stats->bytesToBufferWritten );
    ::printf( "bytesToFileWritten %u\n", stats->bytesToFileWritten );
    ::printf( "bufferedWrites %u\n", stats->bufferedWrites );
    ::printf( "unbufferedWrites %u\n", stats->unbufferedWrites );
    ::printf( "bufferedReads %u\n", stats->bufferedReads );
    ::printf( "unbufferedReads %u\n", stats->unbufferedReads );
}*/

using namespace StdLib;

const char *modifier = "";
uiw CurLine = 0;

typedef void (*ActionFunc)();

void NewLineFunc()
{
    ::printf( "\n" );
}

void NextLineFunc()
{
    COORD pos = { 30, CurLine++ };
    ::SetConsoleCursorPosition( ::GetStdHandle( STD_OUTPUT_HANDLE ), pos );
}

ActionFunc af = NewLineFunc;

struct SS : CharMovable
{
    i32 v;

    ~SS()
    {
        ::printf( "%sdestr of %i", modifier, v );
        af();
    }

    SS() : v( 0 )
    {
        ::printf( "%sdef constr", modifier );
        af();
    }

    SS( i32 source ) : v( source )
    {
        ::printf( "%sconstr with %i", modifier, source );
        af();
    }

    SS( const SS &source ) : v( source.v )
    {
        ::printf( "%scopy of %i", modifier, source.v );
        af();
    }
};
#endif

#include <vector>
#include <LiceMathTypes.hpp>
#include <CThread.hpp>
#include <FileIO.hpp>
#include <Files.hpp>

void Words();

void GLFuncsCreate();

void GetSizes();

int StartFrom = 2000;
int Counted = 0;

static void EnumFilesCallback( Files::CFileEnumInfo *info, void * )
{
    /*++Counted;
    if( Counted < StartFrom )
    {
        return;
    }

    if( _StrStr( info->PNN(), "\\vo\\" ) )
    {
        return;
    }*/

    /*char buf[ 1024 ];
    char name[ 256 ];
    Files::ExtractNameFromString( info->PNN(), name );
    ::printf( "%s\n", info->PNN() );*/

    /*STARTUPINFO si = {};
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi = {};

    ::printf( "%s\n", buf );
    BOOL result = ::CreateProcessA( 0, buf, 0, 0, FALSE, 0, 0, 0, &si, &pi );
    if( result == FALSE )
    {
        ::printf( "failed to create a process\n" );
    }

    ::WaitForSingleObject( pi.hProcess, 100 );*/
}

struct TestStruct
{
    int a;

    TestStruct() : a( rand() )
    {}

    TestStruct( TestStruct &&source )
    {
        ::printf( "move\n" );
        a = source.a;
        source.a = 0;
    };

    TestStruct( const TestStruct &source )
    {
        ::printf( "copy\n" );
        a = source.a;
    };
};

#include <memory>
#include <CString.hpp>

class MoveTest
{
    int *p;

public:
    ~MoveTest()
    {
        delete[] p;
    }

    MoveTest()
    {
        p = new int[ 10 ];
    }

    MoveTest( const MoveTest &source )
    {
        p = new int[ 10 ];
    }

    MoveTest &operator = ( const MoveTest &source )
    {
        p = new int[ 10 ];
    }

    MoveTest( MoveTest &&source )
    {
        p = new int[ 10 ];
        delete[] source.p;
        source.p = 0;
    }

    MoveTest &operator = ( MoveTest &&source )
    {
        p = new int[ 10 ];
        delete[] source.p;
        source.p = 0;
    }
};

//  force all methods compilation to check for correctness
namespace StdLib
{
	template class CVec < int >;
	template class CVec < CVec < int > >;
	template class CVec < int, void >;
	template class CVec < CVec < int, void >, void >;
	template class CVec < int, void, 128 >;
	template class CVec < CVec < int, void, 128 >, void, 128 >;
	template class CRefVec < int >;
	template class CRefVec < const int >;
	template class CVecArr < int >;
	template class TCStr < char, 128 >;
	template class TCStr < wchar_t, 128 >;
}

struct TestMe
{
public:
    int a;

    TestMe( int a ) : a( a )
    {}

    TestMe()
    {
        a = 0;
    }

    TestMe( const TestMe &source ) = delete;
    /*{
        a = source.a;
    }*/

    TestMe &operator = ( const TestMe &source ) = delete;
    TestMe &operator = ( TestMe &&source ) = delete;
    TestMe( TestMe &&source )// = delete;
    {
        a = source.a;
    }
};

template < typename X > struct TypeTest
{
    template < typename X, uiw count >
    TypeTest( const X (&source)[ count ] )
    {
    }
};

#include <algorithm>

NOINLINE CStr Dissect( int value )
{
    char temp[ 64 ];
    Funcs::IntToStrDec( value, temp );
    return temp;
}

NOINLINE CStr Dissect( const char *value )
{
    return value;
}

template < typename... Args > __forceinline CStr LogEntry( const Args &... args )
{
    CStr retStr;
    int wot[] = { 0, (retStr += Dissect(args), 0)... };
    return retStr;
}

template < typename... Args > CStr Concat( const Args &... strings )
{
    static_assert( AreTypesTheSame< typename TypeDesc < Args >::type, CStr >() )...;
}

void FindBrokenNames();
void FileEnumBenchmark();
void CopyFilesToRotate();

#include <locale>

#include <LiceMathFuncs.hpp>

#include <MemoryStreamContainer.hpp>

#include <FileMemoryStream.hpp>

#include <CAtomicFlag.hpp>

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

/*#include "MTMessageQueue.hpp"

using namespace GameTest;

void ThreadFunc( std::shared_ptr < MTMessageQueue > queue )
{
	for( ; ; )
	{
		queue->ExecWait();
	}
}*/

void FuncTest0( int a, float b )
{
	::printf( "FuncTest0 %i %f\n", a, b );
}

void FuncTest1( const CVec < int > &vec )
{
	for( auto i : vec )
	{
		::printf( "%i\n", i );
	}
}

void PrintSameFiles( const wchar_t *pnn );

static f32 RadiansWrapTest( f32 rad )
{
	/*while( rad < 0.0f )
	{
		rad += f32_pi * 2;
	}

	while( rad >= f32_pi * 2 )
	{
		rad -= f32_pi * 2;
	}

	return rad;*/

	 /*return rad - f32_pi2 * floorf(
                  rad * (1.f / f32_pi2));*/

	 rad = std::remainder(rad, f32_pi2);
 
	 if (std::signbit( rad))
	  rad += f32_pi2;
	 return rad;
}

void ReadBenchmark( ui32 iterations );

int __cdecl main()
{
    StdAbstractionLib_Initialize();

	RingBuffer<int, 255> rb;

	for (int i = 0; i < rb.MaxElements * 4; ++i)
	{
		if (i % 5 == 0 && rb.size()) rb.pop_back();
		rb.push_back(i);

		uiw size = rb.size();
		//assert(size == std::min<uiw>(rb.MaxElements, i + 1));
	}

	if (rb.size()) rb.pop_back();

	for (const auto &v : rb)
	{
		printf("%i\n", v);
	}

	printf("size %i\n", rb.size());

	//ReadBenchmark( 25 );

	//Fix( TextFixerMode::fanfic );
	
	/*m4x4 m1 { 1, 2, 3, 4,
			  5, 6, 7, 8,
			  9, 8, 7, 6,
			  5, 4, 3, 2 };

	vec4 v1 { 1, 2, 3, 4 };

	vec4 v2;
	LiceMath::Vec4MultM4x4( &v2, &v1, &m1 );

	OutVec4( "first ", &v2 );

	LiceMath::M4x4TransposeInplace( &m1 );

	v2.x = m1.m[0][0] * v1.x + m1.m[0][1] * v1.y + m1.m[0][2] * v1.z + m1.m[0][3] * v1.w;
	v2.y = m1.m[1][0] * v1.x + m1.m[1][1] * v1.y + m1.m[1][2] * v1.z + m1.m[1][3] * v1.w;
	v2.z = m1.m[2][0] * v1.x + m1.m[2][1] * v1.y + m1.m[2][2] * v1.z + m1.m[2][3] * v1.w;
	v2.w = m1.m[3][0] * v1.x + m1.m[3][1] * v1.y + m1.m[3][2] * v1.z + m1.m[3][3] * v1.w;

	OutVec4( "second ", &v2 );*/

	//PrintSameFiles( L"D:\\Pictures" );

	/*FileCFILEStream::fileError err;
	FileCFILEStream file( L"test.txt", FileOpenMode::CreateAlways, FileProcMode::Write, FileCacheMode::Default, &err );
	if( !file.IsOpened() )
	{
		printf( "failed to open file test.txt, error %s %s\n", err.Description(), err.Addition() );
	}
	else
	{
		if( !file.Write( "hey hey hey", strlen( "hey hey hey" ) ) )
		{
			printf( "writing failed\n" );
		}
	}*/

	/*for( ui32 index = 0; index < 100; ++index )
	{
		f32 randValue = Funcs::RandomRangeF32( -1000, 1000 );
		f32 ref = RadiansWrapTest( randValue );
		f32 fast = Funcs::F32NormalizeRadian( randValue );
		::printf( "value = %f, reference = %f, fast = %f, diff %f\n", randValue, ref, fast, std::fabs( ref - fast ) );
	}*/

#if 0
	FileIO::CFile testFile( L"print_to_file_test.txt", FileIO::OpenMode::CreateAlways, FileProcMode::Write | FileProcMode::Read );
	if( !testFile.IsOpened() )
	{
		::printf( "failed to open file print_to_file_test.txt\n" );
	}
	else
	{
		const char *str = "let's get started\n";
		testFile.Write( str, _StrLen( str ) );

		i64 offset = testFile.OffsetGet();
		FileMapping::mappingError error;

		if( !testFile.SizeSet( 999 ) )
		{
			::printf( "set file size failed\n" );
		}

		i64 offsetToEnd = testFile.OffsetGet( FileOffsetMode::FromEnd );
		::printf( "offset to end %i\n", (int)offsetToEnd );

		FileMapping::Mapping mapping( &testFile, 0, uiw_max, false, &error );
		if( !mapping.IsOpened() )
		{
			::printf( "failed to create mapping, error %s _ %s\n", error.Description(), error.Addition() );
		}
		else
		{
			uiw printed = Funcs::PrintToMemoryStream( &mapping.CreateMemoryStream( offset ), "hey %u half-life %h no way %[0x000]b\n", 9999123, 43252345, 5 );
			mapping = FileMapping::Mapping();
			if( !testFile.SizeSet( printed, &error ) )
			{
				::printf( "set file size 2 failed %s _ %s\n", error.Description(), error.Addition() );
			}
		}
	}

	char smallBuf[ 160 ];
	uiw printed = Funcs::PrintToStr( smallBuf, sizeof(smallBuf), "hey %u half-life %h no way %[0x000]b\n", 9999123, 43252345, 5 );
	::printf( "printed %u\n", printed );
	::printf( "%s", smallBuf );

	char buf[ 256 ];
	Funcs::PrintToStr( buf, 256, "%s%s\n", "testing", " my shit" );
	::printf( buf );

	CStr str;
	MemoryStreamContainer < CStr > stream( &str );

	Funcs::PrintToMemoryStream( &stream, "%s", "shit!_" );

	FileMemoryStream file( &stream, FileProcMode::WriteAppend );
	if( !file.IsOpened() )
	{
		::printf( "failed to open file\n" );
	}
	else
	{
		i64 offset = file.OffsetGet();

		::printf( "current offset %i\n", (int)offset );

		const char *cstr = "holy fucking shit, ok";

		file.Write( cstr, _StrLen( cstr ) );

		offset = file.OffsetGet();

		::printf( "current offset %i\n", (int)offset );

		file.OffsetSet( FileOffsetMode::FromBegin, 10 );

		file.Write( cstr, _StrLen( cstr ) );

		offset = file.OffsetGet();

		::printf( "current offset %i\n", (int)offset );

		::printf( "%s\n", str.CStr() );
	}
#endif

	/*int test[ 15 ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	CVecArr < int > arr;
	arr.Set( test, 10, _countof(test) );
	arr.Insert( 1, 99 );
	arr.Append( 55 );
	arr.Append( 44 );
	arr.Append( 11 );
	arr.Append( 22 );

	for( int v : arr )
	{
		::printf( "%i\n", v );
	}*/

	//FindBrokenNames();
	//FileEnumBenchmark();

	/*FileMapping::mappingError error;
	FileIO::CFile testFile( "test.txt", FileIO::OpenMode::OpenExisting, FileIO::ProcMode::Read );
	FileMapping::Mapping mapping( &testFile, 0, uiw_max, false, &error );
	if( !mapping.IsOpened() )
	{
		::printf( "failed to create mapping, error %s:%s", error.Description(), error.Addition() );
		getchar();
		return 1;
	}

	ui64 fileSize = testFile.SizeGet();
	testFile.Close();
	const char *str = (char *)mapping.Memory();

	char buf[ 128 ];
	Funcs::PrintToStr( buf, sizeof(buf), "mapping content %[*]s\n", (ui32)fileSize, str );
	::printf( "%s\n", buf );*/

#if 0
	uiw copied;
	char buf[ MAX_PATH_LENGTH * 2 ];
	if( Files::CurrentWorkingPathGet( buf, sizeof(buf), &copied ) == false )
	{
		::printf( "failed to get working path, copied %u\n", copied );
	}
	else
	{
		::printf( "%s\n", buf );
	}

	if( Files::CurrentWorkingPathSet( "C:/" ) == false )
	{
		::printf( "failed to set working dir\n" );
	}

	if( Files::CurrentWorkingPathGet( buf, sizeof(buf), &copied ) == false )
	{
		::printf( "failed to get working path 2, copied %u\n", copied );
	}
	else
	{
		::printf( "%s\n", buf );
	}
#endif

#if 0
	::printf( "sizeof %u\n", sizeof(FileIO::CFile) );
	::printf( "sizeof %u\n", sizeof(FileIO::SStats) );

	char path[ 64 ];
	uiw copied;
	if( Files::CurrentWorkingPath( path, sizeof(path), &copied ) == false )
	{
		::printf( "CurrentWorkingPath failed, reason %s\n", copied == 0 ? "some error" : "buffer wasn't big enough" );
	}
	else
	{
		::printf( "%s\n", path );
	}

	FileIO::fileError error;
	FileIO::CFile testFile( "test.txt", FileIO::OpenMode::CreateAlways, FileIO::ProcMode::Write, FileIO::CacheMode::Default, &error );
	if( !testFile.IsOpened() )
	{
		::printf( "failed to 0 open file error %s addition %s\n", error.Description(), error.Addition() );
		getchar();
		return 1;
	}

	::printf( "size 0 %u\n", testFile.SizeGet() );

	testFile.Write( "hey", 3 );

	testFile.Close();

	testFile.Open( "test.txt", FileIO::OpenMode::CreateIfDoesNotExist, FileIO::ProcMode::Write | FileIO::ProcMode::Append, FileIO::CacheMode::Default, &error );
	if( !testFile.IsOpened() )
	{
		::printf( "failed to 1 open file error %s addition %s\n", error.Description(), error.Addition() );
		getchar();
		return 1;
	}

	::printf( "size 0 %u\n", testFile.SizeGet() );

	testFile.Write( "yo", 2 );
	
	testFile.Close();
#endif

    //CStr entry = LogEntry( "you have ", 52, " apples" );
    //CStr entry = Concat( CStr{ "one" }, CStr{ "two" } );

    /*CStr str;
    bool boolean = true;
	const char testStr[ 64 ] = "apples";
	char target[ 512 ];
    Funcs::PrintToStr( target, 512, "int %u string %s char %c bool %b ui8 %u pointer %p fp32 %f fp64 %f int64_hex %j int64 %l", 52, testStr, 'c', boolean, ui8( 5 ), &boolean, 15.4f, 152.7, i64( 66 ), i64( 555 ) );
    ::printf( "%s\n", target );*/

	/*const char testStr[ 64 ] = "what";
	CStr str;
	Funcs::PrintToContainer( &str, "%i\n", testStr );
	::printf( "%s\n", str.CStr() );*/

    /*char buf[ 512 ];
    Funcs::PrintToStr( buf, 511, "int %u string %s char %c bool %b ui8 %u pointer %p fp32 %f fp64 %f int64 %j", 52, "apples", 'c', boolean, ui8( 5 ), &boolean, 15.4f, 152.7, i64( 66 ) );
    ::printf( "%s\n", buf );*/

    /*for( int index = 0; index < 10000; ++index )
    {
        FuncWithAlloca();
    }*/

    /*{
        CVec < std::pair< int, CStr > > v = { { 1, "init" }, { 2, "init" }, { 3, "init" } };

        v = { { 2, "init2" }, { 3, "init2" }, { 4, "init2" } };

        for( uiw index = 0; index < 100; ++index )
        {
            if( rand() % 2 )
            {
                v.Append( { { 4, "high append" }, { 5, "high append" }, { 6, "high append" } } );
            }

            if( rand() % 2 && v.Size() )
            {
                v.PopBack();
            }

            if( rand() % 2 && v.Size() )
            {
                v.PopBack();
            }

            if( rand() % 2 && v.Size() )
            {
                v.PopBack();
            }

            if( rand() % 2 )
            {
                v.Insert( 1, { { 999, "high insert" } } );
            }

            if( rand() % 2 && v.Size() )
            {
                v.Erase( 2, 1 );
            }

            if( rand() % 2 )
            {
                v.Insert( v.Size(), { { 555, "low insert" }, { 666, "low insert" }, { 777, "low insert" } } );
            }

            if( rand() % 2 && v.Size() )
            {
                v.Erase( v.Size() / 2, 3 );
            }
        }

        CRefVec < std::pair< int, CStr > > ref;
        ref = v.ToRef();

        CRefVec < std::pair< int, CStr > > ref2 = ref;

        ::printf( "size %i\n", ref2.Size() );

        for( auto i : ref2 )
        {
            ::printf( "%i ... %s\n", i.first, i.second.CStr() );
        }
    }*/

    /*CVec < TestStruct > vec;
    vec.PushBack( TestStruct() );
    vec.PushBack( TestStruct() );
    vec.PushBack( TestStruct() );
    vec.PushBack( TestStruct() );
    vec.PushBack( TestStruct() );
    vec.PushBack( TestStruct() );
    vec.Reserve( 100 );
    ::system( "Cls" );
    CVec < TestStruct >::Iter wh = vec.Begin() + 1;
    wh--;
    ++wh;
    --wh;
    wh++;
    CVec < TestStruct >::IterConst ci = vec.Erase( wh );

    ::printf( "%i\n%i\n", wh->a, ci->a );*/

    /*CVec < char > vec;
    vec.Append( "123456789", 10 );

    CStr str;
    str = "I do love when the director of engineering slings emails about peoples";
    str.Replace( str, str.Begin(), str.End() );

    for( CStr::IterConst it = str.Begin(); it != str.End(); ++it )
    {
        ::printf( "%c", *it );
    }
    ::printf( "\n" );
    for( CStr::IterRevConst it = str.RBegin(); it != str.REnd(); ++it )
    {
        ::printf( "%c", *it );
    }

    ::printf( "\n" );

    CStr sstr = "123";
    ::printf( "len %u\nsize %u\n", sstr.Length(), sstr.Size() );*/

    //foo(false);

    /*CStaticVec < TestStruct, 16 > vec;

    ::printf( "%i\n%i\n", ALIGNOF( vec ), sizeof( vec ) );*/

    /*CVec < i32 > test;
    for( i32 index = 0; index < 10; ++index )
    {
        test.PushBack( index );
    }

    for( auto it = test.CBeginRev(); it != test.CEndRev(); ++it )
    {
        ::printf( "%i\n", *it );
    }

    int temp = 5;
    int &ref = temp;

    TypeDesc < int [] >::type t2 = 5;
    ::printf( "%s\n", TypeDesc < CVec < i32 > [] >::is_pod ? "is_pod == true" : "is_pod == false" );

    ::printf( "%i\n", t2 );*/

    //Files::EnumFilesRecursively( "D:\\music\\", "*.wma", EnumFilesCallback, 0 );

    /*CTC tc;
    tc.Set();
    for( ; ; )
    {
        CThread::SleepCurrent( 1 );
        ::printf( "\b\b\b\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b" );
        ::printf( "%.4lf", tc.Get64() );
    }*/

    //system( "Cls" );

    //char first[] = "310e120c1b4d635e5d5a06135d270a1f534811720a12130c4f295e55565301001c0f";
    //char second[] = "Robot Roller-Derby Disco Dodgeball";

    //for( int i = 0; second[ i ]; ++i )
    //{
    //    //Funcs::Swap( &first[ i * 2 ], &first[ i * 2 + 1 ] );
    //    char one[ 32 ], two[ 32 ], three[ 32 ], four[ 32 ], indexBits[ 32 ];
    //    char conv[ 3 ] = { first[ i * 2 ], first[ i * 2 + 1 ] };
    //    ui32 inted = Funcs::StrHexToInt < ui32 >( conv );
    //    Funcs::IntToStrBin < i8 >( first[ i * 2 ], one );
    //    Funcs::IntToStrBin < i8 >( first[ i * 2 + 1 ], two );
    //    Funcs::IntToStrBin < i8 >( inted, four );
    //    Funcs::IntToStrBin < i8 >( second[ i ], three );
    //    Funcs::IntToStrBin < i8 >( i, indexBits );
    //    printf( "%2u/%s %c/0x%2x/%3u/%s = %c/0x%2x/%3u/%s %3i %i\n", i, indexBits, (inted == 10 ? '?' : inted), inted, inted, four, second[ i ], second[ i ], second[ i ], three, (i32)second[ i ] - (i32)inted, (i32)second[ i ] ^ (i32)inted );
    //}

    /*char str[] = "4f04421157430d";
    int mults[] = { 111, 109, 49, 49, 49, 54, 99, 97 };

    for( int i = 0; str[ i ]; i += 2 )
    {
        ::printf( "%c", Funcs::StrHexToInt < ui32 >( &str[ i ], 2 ) ^ mults[ i / 2 ] );
    }

    ::printf( "\n" );*/

    //GetSizes();

    //GLFuncsCreate();

    //Fix( TextFixerMode::codeSpaces );

    //Words();

    //UniquePtr < DeleteShit, DeleteShit::Deleter >( DeleteShit::Create() )->Perform();

#if 0
    CStr str( "fuck" );
    str += " hello";
    str += " shiifhv";
    str = str + "sd";
    str = str + str;
    str = 'c' + str + "sdf" + 'c';
    str.Erase( 5, 3 );
    ::printf( "what we have: %s\nis_static: %i\nlen: %u\n", str.CStr(), str.IsStatic(), str.Len() );

    {
        CVectorStatic < i32, 128, ui8 > vec;
        vec.PushBack( 50 );
        vec.PushBack( 54 );
        CVector < i32, Reservator::Simple < ui8 >, Allocator::Simple < i32 >, ui8 > vec2;
        vec2.PushBackArr( &vec[ 0 ], vec.Size() );

        ::printf( "%u\n", vec2.Size() );
        ::printf( "%i\n", vec2[ 0 ] );

        ::printf( "%u\n%u\n", sizeof(vec), sizeof(vec2) );
    }
#endif

#if 0
    const ui32 count = 512;
    typedef i8 ntype;
    ntype test[ count ];
    packiarr_static < count, 4, true, packiarr_OutOfRangeIgnore, false, true > arr;
    //packiarr_dynamic < 5, ntype, packiarr_OutOfRangeIgnore, false, true > arr( count );
    for( ui32 index = 0; index < count; ++index )
    {
        ntype what = rand() % 16 - 8;
        test[ index ] = what;
        arr.Set( index, what );
    }
    bool is_errors = false;
    for( ui32 index = 0; index < count; ++index )
    {
        if( test[ index ] != arr.Get( index ) )
        {
            is_errors = true;
            ::printf( "ERROR: " );
        }
        ::printf( "test %i arr %i\n", test[ index ], arr.Get( index ) );
    }
    ::printf( "%s\n", is_errors ? "there is errors" : "no errors" );

    ::printf( "%u\n", Funcs::StrDecToUI32( "2515 fucked", uiw_max, ' ' ) );
#endif

#if 0
    CTC tc;
    tc.Set();
    f32 writeTime, readTime;
    f32 cwriteTime, creadTime;

    filehdl file = FileIO::Open( "test.txt", FileIO::FileMode::Write | FileIO::FileMode::Create, 0 );
    if( !file.IsValid() )
    {
        ::printf( "can't create test.txt\n" );
        ::getchar();
        return 1;
    }

    FileIO::WriteBufferSizeSet( file, 2048 );

    for( ui32 index = 0; index < 100000; ++index )
    {
        FileIO::Write( file, "test", 4 );
    }

    FileIO::SStats stats;
    FileIO::StatsGet( file, &stats );
    //::printf( "stats 0:\n" );
    //PrintStats( &stats );

    FileIO::Close( file );

    writeTime = tc.Get32Set();

    file = FileIO::Open( "test.txt", FileIO::FileMode::Read | FileIO::FileMode::SequentialScan, 0 );
    if( !file.IsValid() )
    {
        ::printf( "can't open test.txt\n" );
        ::getchar();
        return 1;
    }

    FileIO::ReadBufferSizeSet( file, 2048 );

    char *buf = new char[ 100000 * 4 ];
    FileIO::Read( file, buf, 100000 * 4, 0 );
    /*for( ui32 index = 0; index < 100000; ++index )
    {
        char a_buf[ 4 ];
        if( !FileIO::Read( file, a_buf, 4, 0 ) )
        {
            ::printf( "reading failed\n" );
        }
        else
        {
            //::printf( "%.4s", a_buf );
        }
    }*/

    FileIO::StatsGet( file, &stats );
    //::printf( "stats 1:\n" );
    //PrintStats( &stats );

    FileIO::Close( file );

    readTime = tc.Get32Set();

    FILE *f = fopen( "test", "wb" );
    if( !f )
    {
        ::printf( "c can't create test.txt\n" );
        ::getchar();
        return 1;
    }

    for( ui32 index = 0; index < 100000; ++index )
    {
        fwrite( "test", 1, 4, f );
    }

    fclose( f );

    cwriteTime = tc.Get32Set();

    f = fopen( "test.txt", "rb" );
    if( !f )
    {
        ::printf( "c can't open test.txt\n" );
        ::getchar();
        return 1;
    }

    buf = new char[ 100000 * 4 ];
    fread( buf, 1, 100000 * 4, f );
    /*for( ui32 index = 0; index < 100000; ++index )
    {
        char a_buf[ 4 ];
        if( !fread( a_buf, 1, 4, f ) )
        {
            ::printf( "reading failed\n" );
        }
        else
        {
            //::printf( "%.4s", a_buf );
        }
    }*/

    fclose( f );

    creadTime = tc.Get32Set();

    ::printf( "write time = %.8f\nread time = %.8f\n", writeTime, readTime );
    ::printf( "cwrite time = %.8f\ncread time = %.8f\n", cwriteTime, creadTime );
#endif

    /*cf16 < 25, 1, true > cf;
    cf.FromF32Clamp( 8.652343 );

    ::printf( "%f\n", cf.ToF32() );

    f16 f;
    f.FromI32Clamp( -346263 );
    ::printf( "%i\n", f.ToI32() );

    char a_buf[ 2048 ];
    ::printf( "%s\n", Funcs::UIToDecStrAlt( 2556, a_buf, sizeof(a_buf) ) );*/

    /*::srand( ::GetTickCount() );

    f32 orF = Funcs::RandomF32();
    f16 f( orF );
    f32 faF = f.ToF32();
    ::printf( "original = %f\nfaked = %f\n", orF, faF );*/

    /*char a_buf[ 1024 ];
    f32 fval = 500.f;
    i32 val = *(i32 *)&fval;
    Funcs::IntToStrBin( val, a_buf, true, false );
    ::printf( "%s\n", a_buf );
    VC( Funcs::PrintToStr, a_buf, 1023, "%f", fval );
    ::printf( "%s\n", a_buf );*/

    /*const char *cp_bits = "0_01111110_00000000000000000000000";
    ::printf( "bits num = %u\n", _StrLen( cp_bits ) - 2 );
    i32 val = Funcs::StrBinToInt < i32 >( cp_bits );
    f32 fval = *(f32 *)&val;
    ::printf( "%f\n", fval );*/

    /*m4x4 o_proj;
    LiceMath::Projection( &o_proj, 75, 1, 1000, 1920.f / 1080.f );
    m4x3 o_rot;
    LiceMath::M4x3RotateXYZ( &o_rot, 0, f32_pi, 1.f );
    m4x3 o_trans;
    LiceMath::M4x3Translate3D( &o_trans, 123.5, 46, 5.f );

    m4x4 o_projNew;
    LiceMath::M4x3AsM4x4LastIdenMultM4x4( &o_projNew, &o_rot, &o_proj );

    Out4x4( "proj = ", &o_projNew );

    LiceMath::M4x3AsM4x4LastIdenMultM4x4( &o_projNew, &o_trans, &o_projNew );
    Out4x4( "proj2 = ", &o_projNew );*/

#if 0
    packiarr < 32, 2, i8, true > arr;
    for( ui32 index = 0; index < arr.Cells(); ++index )
    {
        arr.Set( index, index );
    }
    for( ui32 index = 0; index < arr.Cells(); ++index )
    {
        ::printf( "%u  =  %u\n", index, arr.Get( index ) );
    }
#endif

#if 0
    vec3 o_scale( 1, 1, 1 );
    vec3 o_rot( 1, 1, 0 );
    vec3 o_translate( 0, 0, 0 );

    m4x3 o_transform;
    LiceMath::M4x3Scale3DRotateXYZTranslate3DVec( &o_transform, &o_scale, &o_rot, &o_translate );
    vec3 o_pos;
    LiceMath::Vec3TransformCoordByM4x3( &o_pos, &vec3( 12, 43, 123 ), &o_transform );
    OutVec3( "after transform = ", &o_pos );

    m4x3 o_matRef;
    LiceMath::M4x3Scale3DRotateXYZTranslate3DVec( &o_matRef, &o_scale, &o_rot, &o_translate );
    m4x4 o_matRef4;
    LiceMath::M4x4InverseTranspose( &o_matRef4, &m4x4( o_matRef ) );
    LiceMath::M4x4TransposeInplace( &o_matRef4 );
    o_matRef = o_matRef4;

    Out4x4( "ref = ", &o_matRef4 );
    vec3 o_posRev;
    LiceMath::Vec3TransformCoordByM4x4( &o_posRev, &o_pos, &o_matRef4 );
    OutVec3( "after refed = ", &o_posRev );

    LiceMath::M4x3Scale3DRotateXYZTranslate3DVec( &o_matRef, &o_scale, &o_rot, &o_translate );
    m4x4 o_test;
    LiceMath::M4x4InverseTranspose4x3( &o_test, &o_matRef );
    LiceMath::M4x4TransposeInplace( &o_test );
    Out4x4( "test = ", &o_test );
    LiceMath::Vec3TransformCoordByM4x4( &o_posRev, &o_pos, &o_test );
    OutVec3( "after tested = ", &o_posRev );
#endif

#if 0

    /*NOINLINE ui32 ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );
    NOINLINE ui32 ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );
    NOINLINE ui32 ExtractExtensionFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );*/

    char a_buf[ 2048 ];
    ui32 len;
    if( (len = Files::ExtractPathFromString( "C:\\Program Files (x86)\\Mass Effect 3\\Binaries\\Win32\\MassEffect3.exe", a_buf )) != 0 )
    {
        ::printf( "test 1 true, result %s len %u\n", a_buf, len );
    }
    else
    {
        ::printf( "test 1 false\n" );
    }

    if( (len = Files::ExtractNameFromString( "C:\\Program Files (x86)\\Mass Effect 3\\Binaries\\Win32\\MassEffect3.exe", a_buf )) != 0 )
    {
        ::printf( "test 2 true, result %s len %u\n", a_buf, len );
    }
    else
    {
        ::printf( "test 2 false\n" );
    }

    if( (len = Files::ExtractNameWOExtFromString( "C:\\Program Files (x86)\\Mass Effect 3\\Binaries\\Win32\\MassEffect3.exe", a_buf )) != 0 )
    {
        ::printf( "test 2 true, result %s len %u\n", a_buf, len );
    }
    else
    {
        ::printf( "test 2 false\n" );
    }

    if( (len = Files::ExtractExtensionFromString( "C:\\Program Files (x86)\\Mass Effect 3\\Binaries\\Win32\\MassEffect3.exe", a_buf )) != 0 )
    {
        ::printf( "test 4 true, result %s len %u\n", a_buf, len );
    }
    else
    {
        ::printf( "test 4 false\n" );
    }

    CLogger *po_logger = LoggersManager::CreateLogger( "teskjhsdabiuaysdgfiujasgdfuywqaegfiuyweghqrfuy3ebwruyvb34fh34qfioyuweghqioyg suadfasdawerf3qw4f34f34wfw34efwe4rfq343q43t", true );
    po_logger->AddDirection( Dir );
    po_logger->AddDirection( Dir2 );

    po_logger->Message( CLogger::Tag::debug, "test message %s %[4]f %u\n", "hey hey hey", 14545.123414f, 9999 );

    po_logger->Message( CLogger::Tag::info, "my name is %s\n", po_logger->NameGet() );
#endif

#if 0
    ::printf( "%f\n", LiceMath::NormalizeRadian( rand() ) );

    /*m4x3 o_m( 1, 2, 3,
              4, 5, 6,
              7, 8, 9,
              1, 2, 3 );
    m3x4 *po_m = LiceMath::M4x3TransposeInplaceAs3x4( &o_m );


    o_m = m4x3( 1, 2, 3,
              4, 5, 6,
              7, 8, 9,
              1, 2, 3 );
    m3x4 o_m333;
    LiceMath::M4x3Transpose( &o_m333, &o_m );
    Out3x4( "LiceMath::M4x3Transpose( &o_m333, &o_m );", &o_m333 );

    o_m = m4x3( 1, 2, 3,
                4, 5, 6,
                7, 8, 9,
                1, 2, 3 );
    m4x3 out;
    LiceMath::M4x3MultM4x3AsM4x4LastIden( &out, &o_m, &o_m );
    Out4x3( "LiceMath::M4x3MultM4x3AsM4x4LastIden( &out, &o_m, &o_m );", &out );

    m4x3 o_w3;
    LiceMath::M4x3Scale3DRotateXYZ( &o_w3, 2, 3, 1, 1.4, -0.4, 1 );
    LiceMath::M4x3Translate3DTransform( &o_w3, 500, 999, -500 );
    Out4x3( "complex 3", &o_w3 );

    m4x4 o_w4;
    LiceMath::M4x4Scale3D( &o_w4, 2, 3, 1 );
    LiceMath::M4x4RotateXYZTransformLastIden( &o_w4, 1.4, -0.4, 1 );
    LiceMath::M4x4Translate3DTransformLastIden( &o_w4, 500, 999, -500 );
    Out4x4( "complex 4", &o_w4 );

    LiceMath::M4x4TransposeInplace( &o_w4 );
    Out4x4( "LiceMath::M4x4TransposeInplace( &o_w4 );", &o_w4 );

    LiceMath::M4x4Transpose( &o_w4, &m4x4( o_w3 ) );
    Out4x4( "LiceMath::M4x4Transpose( &o_w4, &m4x4( o_w3 ) );", &o_w4 );

    f128color o_col = f128color( f96color() );
    f96color coco( o_col );
    coco = (f96color)o_col;
    o_col = (f128color)coco;

    bool oh;
    oh && oh;

    i32 val = Funcs::Min( 4.3f, 4.3f );

    m2x2 one( 1, 2, 3, 4 );
    m2x2 two( 4, 3, 2, 1 );

    LiceMath::M2x2MultM2x2Inplace( &one, &two );
    ::printf( "%.2f  %.2f\n%.2f  %.2f", one.e00, one.e01, one.e10, one.e11 );

    ::srand( ::GetTickCount() );

    ::printf( "%f\n", Funcs::RandomF32() );

    for( int index = 0; index < 1000000; ++index )
    {
        int res = Funcs::RandomRangeI32( -555, 555 );
        if( res > 555 || res < -555 )
        {
            ::FatalAppExitA( 1, "FAIL" );
        }
    }

    ::printf( "%i\n", Funcs::RandomRangeI32( -555, 555 ) );*/

    for( volatile f32 index = 0; index < 10000000;  )
    {
        index += Funcs::RandomF32();
    }

    const ui32 how_much = 1000000;

    volatile f32 res;
    f32 cou = 0.f;
    ui32 i = Funcs::RandomRangeUI32( 0, 2 );

    vec3 o_scalev( rand(), rand(), rand() );
    vec3 o_rotv( rand(), rand(), rand() );
    vec3 o_translatev( rand(), rand(), rand() );

    tcs tc;
    TC::Set( &tc );

    for( ui32 index = 0; index < how_much; ++index )
    {
        o_rotv.x += cou;
        o_rotv.y += cou;
        o_rotv.z += cou;

        m4x4 o_scale, o_rot, o_trans, o_final;
        LiceMath::M4x4Scale3D( &o_scale, o_scalev.x, o_scalev.y, o_scalev.z );
        LiceMath::M4x4RotateXYZ( &o_rot, o_rotv.x, o_rotv.y, o_rotv.z );
        LiceMath::M4x4Translate3D( &o_trans, o_translatev.x, o_translatev.y, o_translatev.z );
        LiceMath::M4x4MultM4x4( &o_final, &o_scale, &o_rot );
        LiceMath::M4x4MultM4x4Inplace( &o_final, &o_trans );

        cou += o_final[ i ][ i ];
    }

    res = cou;

    ::printf( "4x4 unopt TIME: %f\n", TC::Get32( &tc ) );

    TC::Set( &tc );

    for( ui32 index = 0; index < how_much; ++index )
    {
        o_rotv.x += cou;
        o_rotv.y += cou;
        o_rotv.z += cou;

        m4x4 o_final;
        LiceMath::M4x4Scale3D( &o_final, o_scalev.x, o_scalev.y, o_scalev.z );
        LiceMath::M4x4RotateXYZTransformLastIden( &o_final, o_rotv.x, o_rotv.y, o_rotv.z );
        LiceMath::M4x4Translate3DTransformLastIden( &o_final, o_translatev.x, o_translatev.y, o_translatev.z );

        cou += o_final[ i ][ i ];
    }

    res = cou;

    ::printf( "4x4 opt TIME: %f\n", TC::Get32( &tc ) );

    TC::Set( &tc );

    for( ui32 index = 0; index < how_much; ++index )
    {
        o_rotv.x += cou;
        o_rotv.y += cou;
        o_rotv.z += cou;

        m4x3 o_final;
        LiceMath::M4x3Scale3D( &o_final, o_scalev.x, o_scalev.y, o_scalev.z );
        LiceMath::M4x3RotateXYZTransform( &o_final, o_rotv.x, o_rotv.y, o_rotv.z );
        LiceMath::M4x3Translate3DTransform( &o_final, o_translatev.x, o_translatev.y, o_translatev.z );

        cou += o_final[ i ][ i ];
    }

    res = cou;

    ::printf( "4x3 unopt TIME: %f\n", TC::Get32( &tc ) );

    TC::Set( &tc );

    for( ui32 index = 0; index < how_much; ++index )
    {
        o_rotv.x += cou;
        o_rotv.y += cou;
        o_rotv.z += cou;

        m4x3 o_final;
        LiceMath::M4x3Scale3DRotateXYZTranslate3DVec( &o_final, &o_scalev, &o_rotv, &o_translatev );

        cou += o_final[ i ][ i ];
    }

    res = cou;

    ::printf( "4x3 opt TIME: %f\n", TC::Get32( &tc ) );
#endif

#if 0
    char d_buf[ 1024 ];
    Funcs::Int32ToBinStr( true, true, d_buf, 0xFF00FF05 );
    Funcs::Int32ToBinStr( true, true, d_buf, 0xFFAAFF05 );
    ::printf( "%s\n", d_buf );

    ::printf( "%.8X\n", Funcs::BinStrToI32( d_buf + 2 ) );

    char a_buf[ 1024 ];
    VC( Funcs::PrintToStr, a_buf, 1023, "%r\n", 0.4444445f );
    ::printf( "%s\n", a_buf );

    CFramedStore < i32, i32, 0, 0, 128 > o_store;

    for( uiw index = 0; index < 1024; ++index )
    {
        o_store.Add( index + 1 );
    }

    uiw frame = 0, index = 0;
    for( uiw enu = 0; enu < o_store.Size(); ++enu )
    {
        ::printf( "%i = %i\n", enu, o_store.Enumerate( &frame, &index ) );
    }
    ::printf( "frames: %i\n", o_store.Frames() );

    for( uiw enu = 0; enu < o_store.Frames() * o_store.FrameSize(); ++enu )
    {
        ::printf( "%i = %i\n", enu, o_store[ enu ] );
    }

    o_store.TotalClear();

    frame = 0, index = 0;
    for( uiw enu = 0; enu < o_store.Size(); ++enu )
    {
        ::printf( "%i = %i\n", enu, o_store.Enumerate( &frame, &index ) );
    }
    ::printf( "frames: %i\n", o_store.Frames() );

    const SDummy co_dom;
    SDummy o_dom;
    o_dom = co_dom;

    CVector < i32 > o_one;
    for( i32 index = 0; index < 100; ++index )
    {
        o_one.Push( index );
    }

    o_one.Erase( 50, 10 );
    o_one[ 5 ] = 34234;

    i32 arr[ 5 ] = { 5, 4, 3, 2, 1 };
    o_one.InsertArr( 10, arr, 5 );

    //Func( o_one );

    for( ui32 index = 0; index < o_one.Size(); ++index )
    {
        ::printf( "%i\n", o_one[ index ] );
    }

    o_one.Clear();

    {
        CObjectVector < SDummy > o_two;
        o_two.Resize( 10 );
    }

    /*int *p = new int;

    SError o_err;
    VirtualMem::PageMode_t pageMode = VirtualMem::ProtectGet( p, VirtualMem::PageSize(), &o_err );
    if( pageMode == VirtualMem::PageMode::Error )
    {
        ::printf( "fail %u with error %s\n", __LINE__, o_err.cp_desc );
        ::getchar();
        return 1;
    }

    ::printf( "mode %u\n", pageMode );

    *p = 99;

    if( !VirtualMem::ProtectSet( p, 4, VirtualMem::PageMode::Read ) )
    {
        ::printf( "fail %u\n", __LINE__ );
        ::getchar();
        return 1;
    }

    ::printf( "value %i\n", *p );

    pageMode = VirtualMem::ProtectGet( p, VirtualMem::PageSize(), 0 );
    if( pageMode == VirtualMem::PageMode::Error )
    {
        ::printf( "fail %u\n", __LINE__ );
        ::getchar();
        return 1;
    }

    ::printf( "mode %u\n", pageMode );

    if( !VirtualMem::ProtectSet( p, 4, VirtualMem::PageMode::Read | VirtualMem::PageMode::Write ) )
    {
        ::printf( "fail %u\n", __LINE__ );
        ::getchar();
        return 1;
    }*/

    ::printf( "1. %i\n2. %i\n3. %i\n", Funcs::RoundUIDownTo( 8000, 4096 ), Funcs::RoundUIUpTo( 9999, 4096 ), Funcs::RoundUIToNearest( 5000, 4096 ) );
#endif
    ::printf( "done" );
    while( ::getchar() != 'e' );
    //::getchar();
    return 0;
}