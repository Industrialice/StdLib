#include <StdCoreLib.hpp>
#include <FileIO.hpp>
#include <CVector.hpp>
#include <Files.hpp>
#include <stdio.h>

using namespace StdLib;

static uiw GlobalFixes;

static void MakeFixes( const char *pnn )
{
	uiw localFixes = GlobalFixes;

	FileIO::CFile file( pnn, FileIO::OpenMode::OpenExisting, FileIO::ProcMode::Read | FileIO::ProcMode::SequentialScan, 0 );
	if( !file.IsOpened() )
	{
		::printf( "failed to open %s to read\n", pnn );
		return;
	}

	uiw size = file.SizeGet();
	const uiw originalSize = size;
	char *buf = new char[ size ];
	char *bufEnd = buf + size;
	CVec < char > to;	

	char *bufDel = buf;

	file.Read( buf, size, 0 );

	file.Close();

	#define next( offset ) (byte)(size > offset ? buf[ offset ] : '\0')

	for( uiw skip = 1; buf < bufEnd; size -= skip, buf += skip, skip = 1 )
	{
		if( next( 0 ) == '\n' )
		{
			while( to.Size() && to.Back() == ' ' )
			{
				++GlobalFixes;
				to.PopBack();
			}
			to.PushBack( '\n' );
			continue;
		}
		if( next( 1 ) == '\0' )
		{
			while( to.Size() && (to.Back() == ' ' || to.Back() == '\n') )
			{
				++GlobalFixes;
				to.PopBack();
			}
		}
		if( next( 0 ) == '\r' )
		{
			++GlobalFixes;
			continue;
		}
		if( next( 0 ) == '\t' )
		{
			++GlobalFixes;
			continue;
		}
		if( next( 0 ) == ' ' && next( 1 ) == ' ' )
		{
			++GlobalFixes;
			continue;
		}
		if( next( 0 ) == ' ' )
		{
			if( to.Size() && to.Back() == '\n' )
			{
				continue;
			}
		}
		if( next( 0 ) == '\n' && next( 1 ) == '\n' && next( 2 ) == '\n' )
		{
			++GlobalFixes;
			continue;
		}
		if( next( 0 ) == 0xC3 )
		{
			ui32 offset = 0;
			for( ; offset <= 'z' - 'a'; ++offset )
			{
				if( next( 1 ) == 0xA0 + offset )
				{
					break;
				}
			}
			if( offset <= 'z' - 'a' )
			{
				++GlobalFixes;
				to.PushBack( 'a' + offset );
				skip = 2;
				continue;
			}
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x99 && next( 2 ) == 0xAB )  //  ♫
		{
			++GlobalFixes;
			to.PushBack( '@' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x98 )  //  ‘
		{
			++GlobalFixes;
			to.PushBack( '\'' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x99 )  //  ’
		{
			++GlobalFixes;
			to.PushBack( '\'' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x9C )  //  “
		{
			++GlobalFixes;
			to.PushBack( '"' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x9D )  //  ”
		{
			++GlobalFixes;
			to.PushBack( '"' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x93 )  //  –
		{
			++GlobalFixes;
			to.PushBack( '-' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0x94 )  //  —
		{
			++GlobalFixes;
			to.PushBack( '-' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x87 && next( 2 ) == 0x94 )  //  ⇔
		{
			++GlobalFixes;
			to.PushBack( '<' );
			to.PushBack( '-' );
			to.PushBack( '>' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xE2 && next( 1 ) == 0x80 && next( 2 ) == 0xA6 )  //  …
		{
			++GlobalFixes;
			to.PushBack( '.' );
			to.PushBack( '.' );
			to.PushBack( '.' );
			skip = 3;
			continue;
		}
		if( next( 0 ) == 0xC2 && next( 1 ) == 0xA0 )
		{
			++GlobalFixes;
			to.PushBack( ' ' );
			skip = 2;
			continue;
		}
		to.PushBack( next( 0 ) );
	}

	uiw startIndex = 0;
	while( char *search = Funcs::StrStrAdv( to.Data() + startIndex, "\n ", to.Size() - startIndex ) )
	{
		startIndex = search - to.Data();
		to.Erase( startIndex + 1, 1 );
		++GlobalFixes;
	}

	while( to.Size() && to.Back() == '\n' )
	{
		to.PopBack();
		++GlobalFixes;
	}

	if( to.Size() >= 3 )
	{
		for( uiw index = 0; index < to.Size() - 2; ++index )
		{
			if( to[ index ] == '\n' && to[ index + 1 ] == '\n' && to[ index + 2 ] == '\n' )
			{
				to.Erase( index, 1 );
				--index;
				++GlobalFixes;
			}
		}
	}

	if( localFixes != GlobalFixes )
	{
		file.Open( pnn, FileIO::OpenMode::CreateAlways, FileIO::ProcMode::Write, 0 );
		if( !file.IsOpened() )
		{
			::printf( "failed to open %s to write\n", pnn );
			return;
		}

		file.Write( to.Data(), to.Size() );

		file.Close();
	}

	delete[] bufDel;

	if( localFixes != GlobalFixes )
	{
		::printf( "fixed %u places in %s, size was %u now %u dt %u\n", GlobalFixes - localFixes, pnn, originalSize, to.Size(), originalSize - to.Size() );
	}
	else
	{
		::printf( "no fixes for %s\n", pnn );
	}
}

void Fix()
{
	printf( "file or folder: " );
	char buf[ 256 ];
	scanf( "%s", buf );

	Files::CFileEnumInfo info;
	
	for( bln result = Files::EnumFirstFile( &info, buf, "*.*" ); result; result = Files::EnumNextFile( &info ) )
	{
		if( !info.IsFolder() )
		{
			MakeFixes( info.PNN() );
		}
	}

	::printf( "overall fixes %u\n", GlobalFixes );
}