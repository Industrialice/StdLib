#include "PreHeader.hpp"
#include "FileCFILEStream.hpp"
#include "Files.hpp"

#ifdef WINDOWS
	#define fopen _wfopen
	#define ftell _ftelli64_nolock
	#define fseek _fseeki64_nolock
#endif

FileCFILEStream::~FileCFILEStream()
{
	this->Close();
}

FileCFILEStream::FileCFILEStream() : _file( 0 )
{}

FileCFILEStream::FileCFILEStream( const FilePath &path, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode, fileError *error ) : _file( 0 )
{
	this->Open( path, openMode, procMode, FileCacheMode::Default, error );
}

bln FileCFILEStream::Open( const FilePath &path, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode, fileError *error )
{
	this->Close();

	Nullable < bln > is_fileExists = Files::IsExists( path );
	if( is_fileExists.IsNull() )
	{
		DSA( error, fileError( Error::UnknownError(), "failed to check file's existense" ) );
		return false;
	}

	if( openMode == FileOpenMode::OpenExisting )
	{
		if( !is_fileExists )
		{
			DSA( error, Error::DoesNotExist() );
			return false;
		}
	}

	if( openMode == FileOpenMode::CreateNew )
	{
		if( is_fileExists )
		{
			DSA( error, Error::AlreadyExists() );
			return false;
		}
	}

	if( openMode == FileOpenMode::CreateAlways )
	{
		if( !(procMode & FileProcMode::Write) )
		{
			DSA( error, fileError( Error::InvalidArgument(), "FileOpenMode::CreateAlways cannot be used without FileProcMode::Write" ) );
			return false;
		}
	}

	TCStr < pathChar > procModeStr;

	if( (procMode & FileProcMode::Read) && (procMode & FileProcMode::Write) )
	{
		procModeStr += is_fileExists ? PLATFORM_PATH( "r+" ) : PLATFORM_PATH( "w+" );
	}
	else if( procMode & FileProcMode::Read )
	{
		procModeStr += PLATFORM_PATH( "r" );
	}
	else
	{
		ASSUME( procMode & FileProcMode::Write );
		procModeStr += PLATFORM_PATH( "w" );
	}

	procModeStr += PLATFORM_PATH( "b" );

	_file = fopen( path.PlatformPath(), procModeStr.CStr() );

	if( !_file )
	{
		DSA( error, fileError( Error::UnknownError(), "fopen has failed" ) );
		return false;
	}

	if( (cacheMode & FileCacheMode::DisableSystemWriteCache) && (procMode & FileProcMode::Write) )
	{
		if( setvbuf( (FILE *)_file, 0, _IONBF, 0 ) != 0 )
		{
			DSA( error, fileError( Error::UnknownError(), "setvbuf has failed, cannot disable caching" ) );
			fclose( (FILE *)_file );
			_file = 0;
			return false;
		}
	}

	_procMode = procMode;
	_openMode = openMode;
	_cacheMode = cacheMode;
	_offsetToStart = 0;
	_bufferSize = 0;
	_customBufferPtr = 0;

	if( FileProcMode::WriteAppend & procMode )
	{
		_offsetToStart = ftell( (FILE *)_file );
		if( _offsetToStart == -1 )
		{
			DSA( error, fileError( Error::UnknownError(), "ftell has failed" ) );
			return false;
		}
	}

	DSA( error, Error::Ok() );
	return true;
}

void FileCFILEStream::Close()
{
	if( _file )
	{
		fclose( (FILE *)_file );
	}
	_file = 0;
}

bln FileCFILEStream::IsOpened() const
{
	return _file != 0;
}

bln FileCFILEStream::Read( void *target, ui32 len, ui32 *readed )
{
	ASSUME( IsOpened() );
	ui32 actuallyReaded = fread( target, 1, len, (FILE *)_file );
	DSA( readed, actuallyReaded );
	return true;
}

bln FileCFILEStream::Write( const void *source, ui32 len, ui32 *written )
{
	ASSUME( IsOpened() );
	ui32 actuallyWritten = fwrite( source, 1, len, (FILE *)_file );
	DSA( written, actuallyWritten );
	return true;
}

bln FileCFILEStream::Flush()
{
	ASSUME( IsOpened() );
	return fflush( (FILE *)_file ) == 0;
}

bln FileCFILEStream::IsBufferingSupported() const
{
	ASSUME( IsOpened() );
	return (_cacheMode & FileCacheMode::DisableSystemWriteCache) ? false : true;
}

bln FileCFILEStream::BufferSet( ui32 size, void *buffer )
{
	ASSUME( IsOpened() );

	if( !this->IsBufferingSupported() )
	{
		return false;
	}

	if( fflush( (FILE *)_file ) != 0 )
	{
		return false;
	}
	if( setvbuf( (FILE *)_file, (char *)buffer, _IOFBF, size ) != 0 )
	{
		return false;
	}

	_bufferSize = size;
	_customBufferPtr = buffer;

	return true;
}

ui32 FileCFILEStream::BufferSizeGet() const
{
	ASSUME( IsOpened() );
	return _bufferSize;
}

const void *FileCFILEStream::BufferGet() const
{
	ASSUME( IsOpened() );
	return _customBufferPtr;
}

bln FileCFILEStream::IsSeekSupported() const
{
	ASSUME( IsOpened() );
	return true;
}

i64 FileCFILEStream::OffsetGet( FileOffsetMode::mode_t offsetMode, CError *error )
{
	ASSUME( IsOpened() );

	if( offsetMode == FileOffsetMode::FromCurrent )
	{
		DSA( error, Error::Ok() );
		return 0;
	}

	DSA( error, Error::UnknownError() );

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return 0;
	}

	if( offsetMode == FileOffsetMode::FromBegin )
	{
		DSA( error, Error::Ok() );
		return currentOffset - _offsetToStart;
	}

	ASSUME( offsetMode == FileOffsetMode::FromEnd );

	if( fseek( (FILE *)_file, 0, SEEK_END ) != 0 )
	{
		return 0;
	}

	i64 fileEnd = ftell( (FILE *)_file );
	if( fileEnd == -1 )
	{
		return 0;
	}

	i64 offsetDiff = currentOffset - fileEnd;

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return 0;
	}

	DSA( error, Error::Ok() );
	return offsetDiff;
}

i64 FileCFILEStream::OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error )
{
	ASSUME( IsOpened() );
	DSA( error, Error::UnknownError() );

	offset += _offsetToStart;

	if( offsetMode == FileOffsetMode::FromBegin )
	{
		if( fseek( (FILE *)_file, offset, SEEK_SET ) == 0 )
		{
			DSA( error, Error::Ok() );
			return ftell( (FILE *)_file );
		}
	}

	if( offsetMode == FileOffsetMode::FromCurrent )
	{
		if( fseek( (FILE *)_file, offset, SEEK_CUR ) == 0 )
		{
			DSA( error, Error::Ok() );
			return ftell( (FILE *)_file );
		}
	}

	if( offsetMode == FileOffsetMode::FromEnd )  //  we need this check if an error occured in prior conditions
	{
		if( fseek( (FILE *)_file, offset, SEEK_END ) == 0 )
		{
			DSA( error, Error::Ok() );
			return ftell( (FILE *)_file );
		}
	}

	return -1;
}

ui64 FileCFILEStream::SizeGet( CError *error ) const
{
	ASSUME( IsOpened() );

	DSA( error, Error::UnknownError() );

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return 0;
	}

	if( fseek( (FILE *)_file, 0, SEEK_END ) != 0 )
	{
		return 0;
	}

	i64 endOfFile = ftell( (FILE *)_file );
	if( endOfFile == -1 )
	{
		return 0;
	}

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return 0;
	}

	DSA( error, Error::Ok() );
	return (ui64)(endOfFile - _offsetToStart);
}

bln FileCFILEStream::SizeSet( ui64 newSize, CError *error )
{
	ASSUME( IsOpened() );
	
	DSA( error, Error::UnknownError() );

	newSize += _offsetToStart;

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return false;
	}

	if( fseek( (FILE *)_file, newSize, SEEK_SET ) != 0 )
	{
		return false;
	}

	if( fputc( '\0', (FILE *)_file ) != 0 )
	{
		return false;
	}

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return false;
	}

	DSA( error, Error::Ok() );
	return true;
}

FileProcMode::mode_t FileCFILEStream::ProcModeGet() const
{
	ASSUME( IsOpened() );
	return _procMode;
}

FileCacheMode::mode_t FileCFILEStream::CacheModeGet() const
{
	ASSUME( IsOpened() );
	return FileCacheMode::Default;
}

FileOpenMode::mode_t FileCFILEStream::OpenModeGet() const
{
	ASSUME( IsOpened() );
	return _openMode;
}

FileCFILEStream::FileCFILEStream( FileCFILEStream &&source )
{
	this->_file = source._file;
	source._file = 0;
	this->_offsetToStart = source._offsetToStart;
	this->_openMode = source._openMode;
	this->_procMode = source._procMode;
}
	
FileCFILEStream &FileCFILEStream::operator = ( FileCFILEStream &&source )
{
	ASSUME( this != &source );
	this->_file = source._file;
	source._file = 0;
	this->_offsetToStart = source._offsetToStart;
	this->_openMode = source._openMode;
	this->_procMode = source._procMode;
	return *this;
}