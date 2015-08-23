#include <StdCoreLib.hpp>
#include <FileIO.hpp>
#include <stdio.h>
#include <CVector.hpp>

using namespace StdLib;

void GLFuncsCreate()
{
	FileIO::CFile file( "gl.h", FileIO::OpenMode::OpenExisting, FileIO::ProcMode::Read | FileIO::ProcMode::SequentialScan, 0 );
	if( !file.IsOpened() )
	{
		::printf( "failed to open gl.h\n" );
		return;
	}

	uiw size = file.SizeGet();
	CVec < char > buf( size + 1 );

	file.Read( buf.Data(), size, 0 );
	for( uiw index = 0; index < buf.Size() - 1; ++index )
	{
		if( buf[ index ] == '\r' || buf[ index ] == '\t' )
		{
			buf.Erase( index, 1 );
			--index;
		}
	}
	buf.Back() = '\n';

	file.Close();

	file.Open( "gl.c", FileIO::OpenMode::CreateAlways, FileIO::ProcMode::Write, 0 );
	if( !file.IsOpened() )
	{
		::printf( "failed to create gl.c\n" );
		return;
	}

	file.BufferSet( 0, 2048 );

	file.Write( "#include <gl.h>\n\n", _StrLen( "#include <gl.h>\n\n" ) );

	char *search = buf.Data();
	char *str;

	CVec < CVec < char > > funcNames;

	while( (search < buf.Data() + buf.Size()) && (str = _StrStr( search, "extern " )) )
	{
		search = _StrChr( str, '\n' ) + 1;

		str += sizeof("extern ") - 1;
		uiw len = _StrChr( str, '\n' ) - str;
		if( *str == '\"' )  //  skip extern "C"
		{
			continue;
		}

		char *spaced = _StrChr( str, ' ' );
		ASSUME( spaced );
		++spaced;
		if( spaced[ 0 ] != 'g' || spaced[ 1 ] != 'l' )
		{
			continue;
		}

		char *par = _StrChr( spaced, '(' );
		ASSUME( par );
		funcNames.PushBackNum();
		funcNames.Back().Assign < false >( spaced, par - spaced );
		funcNames.Back().PushBack( '\0' );

		file.Write( str, spaced - str );
		file.Write( "(*__stdcall ", _StrLen( "(*__stdcall " ) );
		file.Write( funcNames.Back().Data(), funcNames.Back().Size() - 1 );
		file.Write( ")", 1 );
		file.Write( par, search - 1 - par );
		file.Write( "\n", 1 );
	}

	file.Write( "\n", 1 );
	file.Write( "void InitializeGLFuncs()\n", _StrLen( "void InitializeGLFuncs()\n" ) );
	file.Write( "{\n", 2 );
	for( uiw index = 0; index < funcNames.Size(); ++index )
	{
		char buf[ 1024 ];
		VC( Funcs::PrintToStr, buf,, 1023, 0, "    %s = wglGetProcAddress(\"%s\");\n", funcNames[ index ].Data(), funcNames[ index ].Data() );
		file.Write( buf, _StrLen( buf ) );
	}
	file.Write( "}", 1 );

	::printf( "done\n" );
}