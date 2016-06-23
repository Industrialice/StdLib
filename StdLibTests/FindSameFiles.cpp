#include <StdAbstractionLib.hpp>
#include <Files.hpp>

using namespace StdLib;

struct FileInfo
{
	FilePath path;
	ui32 size;
};

static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
	CVec < FileInfo > *arg = (CVec < FileInfo > *)argument;
	arg->Append( { info->PNN(), (ui32)info->FileSize() } );
}

void PrintSameFiles( const wchar_t *pnn )
{
	CVec < FileInfo > filesInfos;

	Files::EnumFilesRecursively( pnn, L"*.*", false, EnumFilesCallback, &filesInfos );

	for( uiw index = 0; index < filesInfos.Size(); ++index )
	{
		const FileInfo &info = filesInfos[ index ];

		auto it = std::find_if( filesInfos.begin() + index + 1, filesInfos.end(), [info]( const FileInfo &test ) { return info.size == test.size; } );
		if( it != filesInfos.end() )
		{
			::wprintf( L"file %ws has the same size as %ws\n", info.path.PlatformPath(), it->path.PlatformPath() );
		}
	}

	::printf( "done enumerating\n" );
}