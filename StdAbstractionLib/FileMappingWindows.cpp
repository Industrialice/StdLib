#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FileMapping.hpp"

CError StdLib_FileError();  //  from FilesWindows.cpp

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
	if( (file->ProcModeGet() & FileProcMode::Read) == 0 )
	{
		o_error = mappingError( Error::InvalidArgument(), "File isn't readable( it must be )" );
		goto toExit;
	}

	if( size == uiw_max )
	{
		size = file->SizeGet();
	}

	protect = (is_writeCopy || ((file->ProcModeGet() & FileProcMode::Write) == 0)) ? PAGE_WRITECOPY : PAGE_READWRITE;  //  PAGE_WRITECOPY is the same as PAGE_READONLY
	sizeToMap.QuadPart = size;
	mapping = ::CreateFileMappingW( ((FileIO::Private::CFileBasis *)file)->handle, 0, protect, sizeToMap.HighPart, sizeToMap.LowPart, NULL );
	if( mapping == NULL )
	{
		switch( ::GetLastError() )
		{
		case ERROR_FILE_INVALID:
			o_error = mappingError( Error::CannotOpenFile(), "CreateFileMappingW returned ERROR_FILE_INVALID" );
			break;
		default:
			o_error = mappingError( StdLib_FileError(), "CreateFileMappingW failed" );
		}
		goto toExit;
	}

	if( (file->ProcModeGet() & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
	{
		offset += ((FileIO::Private::CFileBasis *)file)->offsetToStart;
	}
	fileOffset.QuadPart = offset;

	if( is_writeCopy )
	{
		desiredAccess = FILE_MAP_COPY;
		retStruct.is_writable = true;
	}
	else
	{
		if( file->ProcModeGet() & FileProcMode::Write )
		{
			desiredAccess = FILE_MAP_WRITE;
			retStruct.is_writable = true;
		}
		else
		{
			desiredAccess = FILE_MAP_READ;
			retStruct.is_writable = false;
		}
	}
	memory = ::MapViewOfFile( mapping, desiredAccess, fileOffset.HighPart, fileOffset.LowPart, size );
	if( memory == 0 )
	{
		BOOL result = ::CloseHandle( mapping );
		ASSUME( result );
		o_error = mappingError( Error::UnknownError(), "MapViewOfFile failed" );
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