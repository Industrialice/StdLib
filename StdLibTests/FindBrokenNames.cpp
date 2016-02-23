#include <StdAbstractionLib.hpp>
//#include <FileIO.hpp>
#include <Files.hpp>
#include <locale.h>

using namespace StdLib;

void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
	ASSUME( info->IsFolder() == false );
	bln is_show = false;
	for( uiw index = 0; info->PNN()[ index ]; ++index )
	{
		if( info->PNN()[ index ] < 32 || info->PNN()[ index ] > 126 || info->PNN()[ index ] == '?' )
		{
			is_show = true;
			break;
		}
	}
	if( is_show )
	{
		::printf( "%s\n", info->PNN() );
	}
}

void FindBrokenNames()
{
	setlocale( LC_ALL, "Russian" );
	Files::EnumFilesRecursively( "D:\\Pictures", "*.*", EnumFilesCallback, 0 );
}