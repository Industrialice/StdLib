#include <StdAbstractionLib.hpp>
//#include <FileIO.hpp>
#include <Files.hpp>
#include <locale.h>

using namespace StdLib;

static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
	//ASSUME( info->IsFolder() == false );
	bool is_show = false;
	for( uiw index = 0; info->PlatformPNN()[ index ]; ++index )
	{
		if( info->PlatformPNN()[ index ] < 32 || info->PlatformPNN()[ index ] > 126 || info->PlatformPNN()[ index ] == '?' )
		{
			is_show = true;
			break;
		}
	}
	//if( is_show )
	{
		::wprintf( L"%ls\n", info->PlatformPNN() );
	}
}

void FindBrokenNames()
{
	setlocale( LC_ALL, "Russian" );
	SetConsoleCP( 1251 );
	SetConsoleOutputCP( 1251 );
	Files::EnumFilesRecursively( L"D:\\Pictures", L"*.*", false, EnumFilesCallback, 0 );
}