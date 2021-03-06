#include <Files.hpp>
#include <TimeMoment.hpp>

using namespace StdLib;

struct EnumStats
{
	ui32 enumeratedFolders;
	ui32 enumeratedFiles;
};

static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
	EnumStats *stats = (EnumStats *)argument;
	if( info->IsFolder() )
	{
		++stats->enumeratedFolders;
	}
	else
	{
		++stats->enumeratedFiles;
	}
}

void FileEnumBenchmark()
{
	TimeMoment tc( TimeMoment::CreateCurrent() );
	EnumStats stats = {};
	Files::EnumFilesRecursively( L"C:\\Program Files", L"*.*", true, EnumFilesCallback, &stats );
	::printf( "enumerated %u files and %u folders for %f seconds\n", stats.enumeratedFiles, stats.enumeratedFolders, TimeMoment::CreateCurrent().SinceSec32( tc ) );
}