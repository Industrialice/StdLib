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
		FileProcMode _procMode;

	public:
		typedef CError < const char * > fileError;

		FileMemoryStream();
		FileMemoryStream( MemoryStreamInterface &stream, FileProcMode procMode, fileError *error = 0 );
		FileMemoryStream( FileMemoryStream &&source );
		FileMemoryStream &operator = ( FileMemoryStream &&source );
		fileError Open( MemoryStreamInterface &stream, FileProcMode procMode );

		virtual void Close() override;
		virtual bool IsOpened() const override;

		virtual bool Read( void *target, ui32 len, ui32 *readed = 0 ) override;
		virtual bool Write( const void *source, ui32 len, ui32 *written = 0 ) override;

		virtual bool Flush() override;
		virtual bool IsBufferingSupported() const override;
		virtual bool BufferSet( ui32 size, std::unique_ptr < byte, void(*)(byte *) > &&buffer = std::unique_ptr < byte, void(*)(byte *) >( nullptr, [](byte *){} ) ) override;
		virtual ui32 BufferSizeGet() const override;
		virtual const void *BufferGet() const override;

		virtual bool IsSeekSupported() const override;
		virtual CResult < i64 > OffsetGet( FileOffsetMode offsetMode = FileOffsetMode::FromBegin ) override;
		virtual CResult < i64 > OffsetSet( FileOffsetMode offsetMode, i64 offset ) override;

		virtual CResult < ui64 > SizeGet() const override;
		virtual CError<> SizeSet( ui64 newSize ) override;

		virtual FileProcMode ProcModeGet() const override;
		virtual FileCacheMode CacheModeGet() const override;
	};
}

#endif
