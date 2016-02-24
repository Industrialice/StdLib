#include "PreHeader.hpp"

#ifdef POSIX

#include <sys/mman.h>

#include "FileMapping.hpp"

FileMapping::Private::MappingStruct FileMapping::Private::FileMapping_Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error )
{
	ASSUME( file );

	MappingStruct retStruct = { 0, 0 };
	mappingError o_error;
	void *memory;
	int prot;
	int flags = is_writeCopy ? MAP_PRIVATE : MAP_SHARED;
	offset += ((FileIO::Private::CFileBasis *)file)->offsetToStart;

	if( !file->IsOpened() )
	{
		o_error = mappingError( Error::InvalidArgument(), "File isn't opened" );
		goto toExit;
	}
	if( (file->ProcModeGet() & FileIO::ProcMode::Read) == 0 )
	{
		o_error = mappingError( Error::InvalidArgument(), "File isn't readable( it must be )" );
		goto toExit;
	}

	if( size == uiw_max )
	{
		size = file->SizeGet();
	}

	prot = PROT_READ;
	if( file->ProcModeGet() & FileIO::ProcMode::Write )
	{
		prot |= PROT_WRITE;
	}

	memory = ::mmap( 0, size, prot, flags, ((FileIO::Private::CFileBasis *)file)->handle, offset );
	if( memory == 0 )
	{
		o_error = mappingError( Error::Unknown(), "mmap failed" );
		goto toExit;
	}

	o_error = Error::Ok();
	retStruct.memory = memory;
	retStruct.size = size;

toExit:
	DSA( error, o_error );
	return retStruct;
}

void FileMapping::Private::FileMapping_Destroy( MappingStruct *mapping )
{
	if( mapping->memory )
	{
		int result = ::munmap( mapping->memory, mapping->size );
		ASSUME( result == 0 );
	}
}

#endif