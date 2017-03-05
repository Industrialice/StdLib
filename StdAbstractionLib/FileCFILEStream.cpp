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

FileCFILEStream::FileCFILEStream( const FilePath &path, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode, fileError *error ) : _file( 0 )
{
	fileError junkError;
	if( error == nullptr )
	{
		error = &junkError;
	}
	*error = this->Open( path, openMode, procMode, FileCacheMode::Default );
}

auto FileCFILEStream::Open( const FilePath &path, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode ) -> fileError
{
	this->Close();

	CResult < bool > is_fileExists = Files::IsExists( path );
	if( !is_fileExists.Ok() )
	{
		return fileError( is_fileExists.UnwrapError(), "failed to check file's existense" );
	}

	bool fileExistenceResult = is_fileExists.Unwrap();

	if( openMode == FileOpenMode::OpenExisting )
	{
		if( !fileExistenceResult )
		{
			return Error::DoesNotExist();
		}
	}

	if( openMode == FileOpenMode::CreateNew )
	{
		if( fileExistenceResult )
		{
			return Error::AlreadyExists();
		}
	}

	if( openMode == FileOpenMode::CreateAlways )
	{
		if( !(procMode & FileProcMode::Write) )
		{
			return fileError( Error::InvalidArgument(), "FileOpenMode::CreateAlways cannot be used without FileProcMode::Write" );
		}
	}

	bool is_disableCache = false;

	if( !!(cacheMode & FileCacheMode::DisableSystemWriteCache) )
	{
		if( !!(procMode & FileProcMode::Write) )
		{
			is_disableCache = true;
		}
		else
		{
			return fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemWriteCache is used without FileProcMode::Write" );
		}
	}

	if( !!(cacheMode & FileCacheMode::DisableSystemReadCache) )
	{
		if( !!(procMode & FileProcMode::Read) )
		{
			is_disableCache = true;
		}
		else
		{
			return fileError( Error::InvalidArgument(), "FileCacheMode::DisableSystemReadCache is used without FileProcMode::Read" );
		}
	}

	TCStr < pathChar > procModeStr;

	if( !!(procMode & FileProcMode::Read) && !!(procMode & FileProcMode::Write) )
	{
		procModeStr += fileExistenceResult ? PLATFORM_PATH( "r+" ) : PLATFORM_PATH( "w+" );
	}
	else if( !!(procMode & FileProcMode::Read) )
	{
		procModeStr += PLATFORM_PATH( "r" );
	}
	else
	{
		ASSUME( !!(procMode & FileProcMode::Write) );
		procModeStr += PLATFORM_PATH( "w" );
	}

	procModeStr += PLATFORM_PATH( "b" );

	_file = fopen( path.PlatformPath(), procModeStr.CStr() );

	if( !_file )
	{
		return fileError( Error::UnknownError(), "fopen has failed" );
	}

	if( is_disableCache )
	{
		if( setvbuf( (FILE *)_file, 0, _IONBF, 0 ) != 0 )
		{
			fclose( (FILE *)_file );
			_file = 0;
			return fileError( Error::UnknownError(), "setvbuf has failed, cannot disable caching" );
		}
	}

	_procMode = procMode;
	_openMode = openMode;
	_cacheMode = cacheMode;
	_offsetToStart = 0;
	_bufferSize = 0;
	_customBufferPtr = 0;

	if( !!(FileProcMode::WriteAppend & procMode) )
	{
		_offsetToStart = ftell( (FILE *)_file );
		if( _offsetToStart == -1 )
		{
			return fileError( Error::UnknownError(), "ftell has failed" );
		}
	}

	return Error::Ok();
}

void FileCFILEStream::Close()
{
	if( _file )
	{
		fclose( (FILE *)_file );
	}
	_file = 0;
}

bool FileCFILEStream::IsOpened() const
{
	return _file != 0;
}

bool FileCFILEStream::Read( void *target, ui32 len, ui32 *readed )
{
	ASSUME( IsOpened() );
	ui32 actuallyReaded = fread( target, 1, len, (FILE *)_file );
	DSA( readed, actuallyReaded );
	return true;
}

bool FileCFILEStream::Write( const void *source, ui32 len, ui32 *written )
{
	ASSUME( IsOpened() );
	ui32 actuallyWritten = fwrite( source, 1, len, (FILE *)_file );
	DSA( written, actuallyWritten );
	return true;
}

bool FileCFILEStream::Flush()
{
	ASSUME( IsOpened() );
	return fflush( (FILE *)_file ) == 0;
}

bool FileCFILEStream::IsBufferingSupported() const
{
	ASSUME( IsOpened() );

	bool is_cachingDisabled = !!(_cacheMode & FileCacheMode::DisableSystemWriteCache) || !!(_cacheMode & FileCacheMode::DisableSystemReadCache);

	return !is_cachingDisabled;
}

bool FileCFILEStream::BufferSet( ui32 size, std::unique_ptr < byte, void(*)(byte *) > &&buffer )
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
	if( setvbuf( (FILE *)_file, (char *)buffer.get(), _IOFBF, size ) != 0 )
	{
		return false;
	}

	_bufferSize = size;
	_customBufferPtr = std::move( buffer );

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
	return _customBufferPtr.get();
}

bool FileCFILEStream::IsSeekSupported() const
{
	ASSUME( IsOpened() );
	return true;
}

CResult < i64 > FileCFILEStream::OffsetGet( FileOffsetMode offsetMode )
{
	ASSUME( IsOpened() );

	using resultType = CResult < i64 >;

	if( offsetMode == FileOffsetMode::FromCurrent )
	{
		return 0;
	}

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	if( offsetMode == FileOffsetMode::FromBegin )
	{
		return currentOffset - _offsetToStart;
	}

	ASSUME( offsetMode == FileOffsetMode::FromEnd );

	if( fseek( (FILE *)_file, 0, SEEK_END ) != 0 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	i64 fileEnd = ftell( (FILE *)_file );
	if( fileEnd == -1 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	i64 offsetDiff = currentOffset - fileEnd;

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	return offsetDiff;
}

CResult < i64 > FileCFILEStream::OffsetSet( FileOffsetMode offsetMode, i64 offset )
{
	ASSUME( IsOpened() );

	offset += _offsetToStart;

	if( offsetMode == FileOffsetMode::FromBegin )
	{
		if( fseek( (FILE *)_file, offset, SEEK_SET ) == 0 )
		{
			return ftell( (FILE *)_file );
		}
	}

	if( offsetMode == FileOffsetMode::FromCurrent )
	{
		if( fseek( (FILE *)_file, offset, SEEK_CUR ) == 0 )
		{
			return ftell( (FILE *)_file );
		}
	}

	if( offsetMode == FileOffsetMode::FromEnd )  //  we need this check if an error occured in prior conditions
	{
		if( fseek( (FILE *)_file, offset, SEEK_END ) == 0 )
		{
			return ftell( (FILE *)_file );
		}
	}

	return CResult < i64 >( 0, Error::UnknownError() );
}

CResult < ui64 > FileCFILEStream::SizeGet() const
{
	ASSUME( IsOpened() );

	using resultType = CResult < ui64 >;

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	if( fseek( (FILE *)_file, 0, SEEK_END ) != 0 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	i64 endOfFile = ftell( (FILE *)_file );
	if( endOfFile == -1 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return resultType( 0, Error::UnknownError() );
	}

	return (ui64)(endOfFile - _offsetToStart);
}

CError<> FileCFILEStream::SizeSet( ui64 newSize )
{
	ASSUME( IsOpened() );

	newSize += _offsetToStart;

	i64 currentOffset = ftell( (FILE *)_file );
	if( currentOffset == -1 )
	{
		return Error::UnknownError();
	}

	if( fseek( (FILE *)_file, newSize, SEEK_SET ) != 0 )
	{
		return Error::UnknownError();
	}

	if( fputc( '\0', (FILE *)_file ) != 0 )
	{
		return Error::UnknownError();
	}

	if( fseek( (FILE *)_file, currentOffset, SEEK_SET ) != 0 )
	{
		return Error::UnknownError();
	}

	return Error::Ok();
}

FileProcMode FileCFILEStream::ProcModeGet() const
{
	ASSUME( IsOpened() );
	return _procMode;
}

FileCacheMode FileCFILEStream::CacheModeGet() const
{
	ASSUME( IsOpened() );
	return FileCacheMode::Default;
}

FileOpenMode FileCFILEStream::OpenModeGet() const
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
	Close();
	this->_file = source._file;
	source._file = 0;
	this->_offsetToStart = source._offsetToStart;
	this->_openMode = source._openMode;
	this->_procMode = source._procMode;
	return *this;
}