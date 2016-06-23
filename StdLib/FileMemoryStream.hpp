#ifndef __FILE_MEMORY_STREAM_HPP__
#define __FILE_MEMORY_STREAM_HPP__

#include "MemoryStreamInterface.hpp"
#include "FileInterface.hpp"

namespace StdLib
{
	class FileMemoryStream final : public FileInterface
	{
		MemoryStreamInterface *_stream;
		uiw _offset;  //  _startOffset + current offset
		uiw _startOffset;  //  can be non-zero in append mode
		FileProcMode::mode_t _procMode;

		FileMemoryStream( const FileMemoryStream & );
		FileMemoryStream &operator = ( const FileMemoryStream & );

	public:
		typedef CTError < const char * > fileError;

		FileMemoryStream();
		FileMemoryStream( MemoryStreamInterface *stream, FileProcMode::mode_t procMode, fileError *error = 0 );
		bln Open( MemoryStreamInterface *stream, FileProcMode::mode_t procMode, fileError *error = 0 );

		virtual void Close() override;
		virtual bln IsOpened() const override;

		virtual bln Read( void *target, ui32 len, ui32 *readed = 0 ) override;
		virtual bln Write( const void *source, ui32 len, ui32 *written = 0 ) override;

		virtual bln Flush() override;
		virtual bln IsBufferingSupported() const override;
		virtual bln BufferSet( ui32 size, void *buffer ) override;
		virtual ui32 BufferSizeGet() const override;
		virtual const void *BufferGet() const override;

		virtual bln IsSeekSupported() const override;
		virtual i64 OffsetGet( FileOffsetMode::mode_t offsetMode = FileOffsetMode::FromBegin, CError *error = 0 ) override;
		virtual i64 OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error = 0 ) override;

		virtual ui64 SizeGet( CError *error = 0 ) const override;
		virtual bln SizeSet( ui64 newSize, CError *error = 0 ) override;

		virtual FileProcMode::mode_t ProcModeGet() const override;
		virtual FileCacheMode::mode_t CacheModeGet() const override;
	};
}

#endif
