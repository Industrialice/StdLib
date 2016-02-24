#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FileMapping.hpp"

FileMapping::Private::MappingStruct FileMapping::Private::FileMapping_Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error )
{
	ASSUME( file );

	MappingStruct retStruct = { 0, 0, NULL };
	mappingError o_error;
	HANDLE mapping;
	LARGE_INTEGER sizeToMap;
	DWORD protect;
	LPVOID memory;
	DWORD desiredAccess;
	LARGE_INTEGER fileOffset;

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

	protect = (is_writeCopy || ((file->ProcModeGet() & FileIO::ProcMode::Write) == 0)) ? PAGE_WRITECOPY : PAGE_READWRITE;  //  PAGE_WRITECOPY is the same as PAGE_READONLY
	sizeToMap.QuadPart = size;
	mapping = ::CreateFileMappingW( ((FileIO::Private::CFileBasis *)file)->handle, 0, protect, sizeToMap.HighPart, sizeToMap.LowPart, NULL );
	if( mapping == NULL )
	{
		o_error = mappingError( Error::Unknown(), "CreateFileMappingW failed" );
		goto toExit;
	}

	if( file->ProcModeGet() & FileIO::ProcMode::Append )
	{
		offset += ((FileIO::Private::CFileBasis *)file)->offsetToStart;
	}
	fileOffset.QuadPart = offset;

	if( is_writeCopy )
	{
		desiredAccess = FILE_MAP_COPY;
	}
	else
	{
		if( file->ProcModeGet() & FileIO::ProcMode::Write )
		{
			desiredAccess = FILE_MAP_WRITE;
		}
		else
		{
			desiredAccess = FILE_MAP_READ;
		}
	}
	memory = ::MapViewOfFile( mapping, desiredAccess, fileOffset.HighPart, fileOffset.LowPart, size );
	if( memory == 0 )
	{
		BOOL result = ::CloseHandle( mapping );
		ASSUME( result );
		o_error = mappingError( Error::Unknown(), "MapViewOfFile failed" );
		goto toExit;
	}

	o_error = Error::Ok();
	retStruct.memory = memory;
	retStruct.size = size;
	retStruct.handle = mapping;

toExit:
	DSA( error, o_error );
	return retStruct;
}

void FileMapping::Private::FileMapping_Destroy( MappingStruct *mapping )
{
	if( mapping->memory )
	{
		ASSUME( mapping->handle != NULL );
		BOOL result = ::UnmapViewOfFile( mapping->memory );
		ASSUME( result );
		mapping->memory = 0;
		result = ::CloseHandle( mapping->handle );
		ASSUME( result );
		mapping->handle = NULL;
	}
}

#endif