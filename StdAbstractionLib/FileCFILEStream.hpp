#ifndef __FILE_C_FILE_STREAM_HPP__
#define __FILE_C_FILE_STREAM_HPP__

#include "FilePath.hpp"
#include "MemoryStreamInterface.hpp"
#include "FileInterface.hpp"

namespace StdLib
{
	class FileCFILEStream final : public FileInterface
	{
		void *_file;
		FileOpenMode _openMode;
		FileProcMode _procMode;
		FileCacheMode _cacheMode;
		i64 _offsetToStart;  //  used only when you're using ProcMode::Append, the file will be opened as usual, then the offset will be added so you can't work with the existing part of the file
		ui32 _bufferSize;
		std::unique_ptr < byte, void(*)(byte *) > _customBufferPtr = std::unique_ptr < byte, void(*)(byte *) >( nullptr, [](byte *){} );

	public:
		typedef CError < const char * > fileError;

		~FileCFILEStream();
		FileCFILEStream();
		FileCFILEStream( const FilePath &path, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode = FileCacheMode::Default, fileError *error = 0 );
		fileError Open( const FilePath &path, FileOpenMode openMode, FileProcMode procMode, FileCacheMode cacheMode = FileCacheMode::Default );

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

		FileOpenMode OpenModeGet() const;

		FileCFILEStream( FileCFILEStream &&source );
		FileCFILEStream &operator = ( FileCFILEStream &&source );
	};
}

#endif