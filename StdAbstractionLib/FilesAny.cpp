#include "PreHeader.hpp"
#include "Files.hpp"

void Files::EnumFiles( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument )
{
	CFileEnumInfo info;
	if( !EnumFirstFile( &info, path, mask ) )
	{
		return;
	}

	do
	{
		if( is_reportFolders || info.IsFolder() == false )
		{
			callback( &info, argument );
		}
	} while( EnumNextFile( &info ) );
}

void Files::EnumFilesRecursively( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument )
{
	UniquePtr < CFileEnumInfo > info( new CFileEnumInfo );
	if( !EnumFirstFile( info, path, PLATFORM_PATH( "*.*" ) ) )
	{
		return;
	}

	do
	{
		if( info->IsFolder() )
		{
			if( is_reportFolders )
			{
				callback( info, argument );
			}
			EnumFilesRecursively( info->PNN(), mask, is_reportFolders, callback, argument );
		}
	} while( EnumNextFile( info ) );

	if( !EnumFirstFile( info, path, mask ) )
	{
		return;
	}

	do
	{
		if( info->IsFolder() == false )
		{
			callback( info, argument );
		}
	} while( EnumNextFile( info ) );
}