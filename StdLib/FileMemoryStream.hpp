#ifndef __FILE_MEMORY_STREAM_HPP__
#define __FILE_MEMORY_STREAM_HPP__

#include "MemoryStreamInterface.hpp"
#include "FileInterface.hpp"

namespace StdLib
{
	class FileMemoryStream : public FileInterface
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

		virtual void Close() override final;
		virtual bln IsOpened() const override final;

		virtual bln Read( void *target, ui32 len, ui32 *readed = 0 ) override final;
		virtual bln Write( const void *source, ui32 len, ui32 *written = 0 ) override final;

		virtual bln Flush() override final;
		virtual bln IsBufferingSupported() const override final;
		virtual bln BufferSet( ui32 size, void *buffer ) override final;
		virtual ui32 BufferSizeGet() const override final;
		virtual const void *BufferGet() const override final;

		virtual bln IsSeekSupported() const override final;
		virtual i64 OffsetGet( FileOffsetMode::mode_t offsetMode = FileOffsetMode::FromBegin, CError *error = 0 ) override final;
		virtual i64 OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error = 0 ) override final;

		virtual ui64 SizeGet( CError *error = 0 ) const override final;
		virtual bln SizeSet( ui64 newSize, CError *error = 0 ) override final;

		virtual FileProcMode::mode_t ProcModeGet() const override final;
		virtual FileCacheMode::mode_t CacheModeGet() const override final;
	};
}

#endif
