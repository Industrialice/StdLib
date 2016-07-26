#include "PreHeader.hpp"
#include "FileMemoryStream.hpp"

FileMemoryStream::FileMemoryStream() : _stream( 0 )
{}

FileMemoryStream::FileMemoryStream( MemoryStreamInterface *stream, FileProcMode::mode_t procMode, fileError *error )
{
	this->Open( stream, procMode, error );
}

bln FileMemoryStream::Open( MemoryStreamInterface *stream, FileProcMode::mode_t procMode, fileError *error )
{
	ASSUME( stream && procMode );

	_stream = 0;
	_offset = 0;
	_startOffset = 0;

	if( (procMode & (FileProcMode::Read | FileProcMode::Write)) == 0 )
	{
		DSA( error, fileError( Error::InvalidArgument(), "Neither FileProcMode::Read, nor FileProcMode::Write is set" ) );
		return false;
	}

	if( procMode & FileProcMode::Read )
	{
		if( stream->IsReadable() == false )
		{
			DSA( error, fileError( Error::NoAccess(), "requested read proc mode, but the stream is not readable" ) );
			return false;
		}
	}
	if( procMode & FileProcMode::Write )
	{
		if( stream->IsWritable() == false )
		{
			DSA( error, fileError( Error::NoAccess(), "requested write proc mode, but the stream is not readable" ) );
			return false;
		}
	}
	if( (procMode & FileProcMode::WriteAppend) == FileProcMode::WriteAppend )
	{
		_offset = _startOffset = stream->Size();
	}

	_stream = stream;
	_procMode = procMode;

	DSA( error, Error::Ok() );

	return true;
}

void FileMemoryStream::Close()
{
	_stream = 0;
}

bln FileMemoryStream::IsOpened() const
{
	return _stream != 0;
}

bln FileMemoryStream::Read( void *target, ui32 len, ui32 *readed )
{
	ASSUME( _stream && (_procMode & FileProcMode::Read) );
	uiw diff = _offset <= _stream->Size() ? _stream->Size() - _offset : 0;
	len = Funcs::Min < uiw >( len, diff );
	if( _offset + len < _offset )  //  overflow
	{
		len = uiw_max - _offset;
	}
	_MemCpy( target, _stream->Memory() + _offset, len );
	_offset += len;
	DSA( readed, len );
	return true;
}

bln FileMemoryStream::Write( const void *source, ui32 len, ui32 *written )
{
	ASSUME( _stream && (_procMode & FileProcMode::Write) );
	uiw writeEnd = _offset + len;
	if( writeEnd < _offset )  //  overflow
	{
		writeEnd = uiw_max;
		len = uiw_max - _offset;
	}
	if( writeEnd > _stream->Size() )
	{
		uiw newSize = _stream->Resize( writeEnd );
		if( newSize <= _offset )
		{
			len = 0;
		}
		else
		{
			len = newSize - _offset;
		}
	}
	_MemCpy( _stream->Memory() + _offset, source, len );
	_offset += len;
	DSA( written, len );
	return true;
}

bln FileMemoryStream::Flush()
{
	ASSUME( _stream );
	return true;
}

bln FileMemoryStream::IsBufferingSupported() const
{
	return false;
}

bln FileMemoryStream::BufferSet( ui32 size, void *buffer )
{
	return false;
}

ui32 FileMemoryStream::BufferSizeGet() const
{
	return 0;
}

const void *FileMemoryStream::BufferGet() const
{
	return 0;
}

bln FileMemoryStream::IsSeekSupported() const
{
	return true;
}

i64 FileMemoryStream::OffsetGet( FileOffsetMode::mode_t offsetMode, CError *error )
{
	ASSUME( _stream );
	DSA( error, Error::Ok() );
	if( offsetMode == FileOffsetMode::FromBegin )
	{
		return _offset - _startOffset;
	}
	else if( offsetMode == FileOffsetMode::FromCurrent )
	{
		return 0;
	}
	else  //  if( offsetMode == FileOffsetMode::FromEnd )
	{
		ASSUME( offsetMode == FileOffsetMode::FromEnd );
		return _offset - _stream->Size();
	}
}

i64 FileMemoryStream::OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error )
{
	ASSUME( _stream );

	DSA( error, Error::Ok() );

	if( offset <= 0 )  //  backward
	{
		if( offsetMode == FileOffsetMode::FromBegin )
		{
			_offset = _startOffset;
		}
		else if( offsetMode == FileOffsetMode::FromCurrent )
		{
			if( _offset + offset < _startOffset )  //  underflow
			{
				_offset = _startOffset;
			}
			else
			{
				_offset += offset;
			}
		}
		else  //  if( offsetMode == FileOffsetMode::FromEnd )
		{
			ASSUME( offsetMode == FileOffsetMode::FromEnd );

			if( _stream->Size() + offset < _startOffset )  //  underflow
			{
				_offset = _startOffset;
			}
			else
			{
				_offset = _stream->Size() + offset;
			}
		}
	}
	else  //  forward
	{
		if( offsetMode == FileOffsetMode::FromBegin )
		{
			uiw diff = uiw_max - _startOffset;
			if( offset >= diff )  //  overflow
			{
				_offset = uiw_max;
			}
			else
			{
				_offset = _startOffset + offset;
			}
		}
		else if( offsetMode == FileOffsetMode::FromCurrent )
		{
			uiw diffToOverflow = uiw_max - _offset;
			if( offset > diffToOverflow )  //  overflow
			{
				_offset = uiw_max;
			}
			else
			{
				_offset += offset;
			}
		}
		else  //  if( offsetMode == FileOffsetMode::FromEnd )
		{
			ASSUME( offsetMode == FileOffsetMode::FromEnd );

			uiw diffToOverflow = uiw_max - _stream->Size();
			if( offset >= diffToOverflow )  //  overflow
			{
				_offset = uiw_max;
			}
			else
			{
				_offset = _stream->Size() + offset;
			}
		}
	}

	return _offset - _startOffset;
}

ui64 FileMemoryStream::SizeGet( CError *error ) const
{
	ASSUME( _stream );
	DSA( error, Error::Ok() );
	return _stream->Size() - _startOffset;
}

bln FileMemoryStream::SizeSet( ui64 newSize, CError *error )
{
	ASSUME( _stream );
	DSA( error, Error::Ok() );
	newSize += _startOffset;
	if( newSize > _startOffset )
	{
		if( _stream->Resize( newSize ) == newSize )
		{
			return true;
		}
	}
	DSA( error, Error::OutOfMemory() );
	return false;
}

FileProcMode::mode_t FileMemoryStream::ProcModeGet() const
{
	ASSUME( _stream );
	return _procMode;
}

FileCacheMode::mode_t FileMemoryStream::CacheModeGet() const
{
	ASSUME( _stream );
	return FileCacheMode::Default;
}

#ifdef MOVE_SUPPORTED
	FileMemoryStream::FileMemoryStream( FileMemoryStream &&source )
	{
		this->_stream = source._stream;
		source._stream = 0;
		this->_offset = source._offset;
		this->_procMode = source._procMode;
		this->_startOffset = source._startOffset;
	}
	
	FileMemoryStream &FileMemoryStream::operator = ( FileMemoryStream &&source )
	{
		ASSUME( this != &source );
		this->_stream = source._stream;
		source._stream = 0;
		this->_offset = source._offset;
		this->_procMode = source._procMode;
		this->_startOffset = source._startOffset;
		return *this;
	}
#endif