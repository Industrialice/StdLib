#include "PreHeader.hpp"
#include "Files.hpp"

uiw Files::EnumFiles( const FilePath &path, const FilePath &mask, bool is_reportFolders, EnumFilesCallback callback, void *argument )
{
	uiw enumerated = 0;
	CFileEnumInfo info;
	if( !EnumFirstFile( &info, path, mask ).Ok() )
	{
		return enumerated;
	}

	do
	{
		if( is_reportFolders || info.IsFolder() == false )
		{
			callback( &info, argument );
			++enumerated;
		}
	} while( EnumNextFile( &info ).Ok() );

	return enumerated;
}

uiw Files::EnumFilesRecursively( const FilePath &path, const FilePath &mask, bool is_reportFolders, EnumFilesCallback callback, void *argument )
{
	uiw enumerated = 0;
	std::unique_ptr < CFileEnumInfo > info( new CFileEnumInfo );
	if( !EnumFirstFile( info.get(), path, PLATFORM_PATH( "*.*" ) ).Ok() )
	{
		return enumerated;
	}

	do
	{
		if( info->IsFolder() )
		{
			if( is_reportFolders )
			{
				callback( info.get(), argument );
				++enumerated;
			}
			enumerated += EnumFilesRecursively( info->PNN(), mask, is_reportFolders, callback, argument );
		}
	} while( EnumNextFile( info.get() ).Ok() );

	if( !EnumFirstFile( info.get(), path, mask ).Ok() )
	{
		return enumerated;
	}

	do
	{
		if( info->IsFolder() == false )
		{
			callback( info.get(), argument );
			++enumerated;
		}
	} while( EnumNextFile( info.get() ).Ok() );

	return enumerated;
}