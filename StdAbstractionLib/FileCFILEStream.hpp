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
		FileOpenMode::mode_t _openMode;
		FileProcMode::mode_t _procMode;
		FileCacheMode::mode_t _cacheMode;
		i64 _offsetToStart;  //  used only when you're using ProcMode::Append, the file will be opened as usual, then the offset will be added so you can't work with the existing part of the file
		ui32 _bufferSize;
		void *_customBufferPtr;

		FileCFILEStream( const FileCFILEStream & );
		FileCFILEStream &operator = ( const FileCFILEStream & );

	public:
		typedef CTError < const char * > fileError;

		~FileCFILEStream();
		FileCFILEStream();
		FileCFILEStream( const FilePath &path, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode = FileCacheMode::Default, fileError *error = 0 );
		bln Open( const FilePath &path, FileOpenMode::mode_t openMode, FileProcMode::mode_t procMode, FileCacheMode::mode_t cacheMode = FileCacheMode::Default, fileError *error = 0 );

		virtual void Close() override;
		virtual bln IsOpened() const override;

		virtual bln Read( void *target, ui32 len, ui32 *readed = 0 ) override;
		virtual bln Write( const void *source, ui32 len, ui32 *written = 0 ) override;

		virtual bln Flush() override;
		virtual bln IsBufferingSupported() const override;
		virtual bln BufferSet( ui32 size, void *buffer = 0 ) override;
		virtual ui32 BufferSizeGet() const override;
		virtual const void *BufferGet() const override;

		virtual bln IsSeekSupported() const override;
		virtual i64 OffsetGet( FileOffsetMode::mode_t offsetMode = FileOffsetMode::FromBegin, CError *error = 0 ) override;
		virtual i64 OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error = 0 ) override;

		virtual ui64 SizeGet( CError *error = 0 ) const override;
		virtual bln SizeSet( ui64 newSize, CError *error = 0 ) override;

		virtual FileProcMode::mode_t ProcModeGet() const override;
		virtual FileCacheMode::mode_t CacheModeGet() const override;

		FileOpenMode::mode_t OpenModeGet() const;

	#ifdef MOVE_SUPPORTED
		FileCFILEStream( FileCFILEStream &&source );
		FileCFILEStream &operator = ( FileCFILEStream &&source );
	#endif
	};
}

#endif