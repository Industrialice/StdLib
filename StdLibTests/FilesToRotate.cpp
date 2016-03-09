#include <StdAbstractionLib.hpp>
#include <Files.hpp>

#include <D3DX11.h>
#pragma comment( lib, "d3dx11.lib" )

using namespace StdLib;

namespace
{
	uiw ImageIndex;
}

static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
	if( info->PNN().Extension() == L"gif" )
	{
		return;
	}

	D3DX11_IMAGE_INFO image_info;
	if( ::D3DX11GetImageInfoFromFileW( info->PlatformPNN(), NULL, &image_info, NULL ) != S_OK )
	{
		::wprintf( L"failed to retrieve info for %s\n", info->PlatformPNN() );
	}
	else
	{
		FilePath target = L"C:\\Users\\Slice\\Desktop\\ds103images\\";
		if( image_info.Width > image_info.Height )
		{
			target += L"rotated\\";
		}
		else
		{
			target += L"normal\\";
		}
		char buf[ 128 ];
		Funcs::IntToStrDec( ImageIndex, buf );
		wchar_t buf2[ 128 ];
		::mbstowcs( buf2, buf, 127 );
		target += buf2;
		target += L".";
		target += info->PNN().Extension();

		CError error;
		if( !Files::CopyFileTo( info->PNN(), target, true, &error ) )
		{
			::wprintf( L"failed to copy image %ls, ", info->PlatformPNN() );
			::printf( "reason %s\n", error.Description() );
		}
		else
		{
			::wprintf( L"copy image %ls, ", info->PlatformPNN() );
			::printf( "reason %s\n", error.Description() );
		}
	}

	++ImageIndex;
}

void CopyFilesToRotate()
{
	FilePath imagesPath( L"D:\\Pictures" );
	Files::EnumFiles( imagesPath, L"*.*", false, EnumFilesCallback, 0 );
}