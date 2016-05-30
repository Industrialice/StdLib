#ifndef __FILE_INTERFACE_HPP__
#define __FILE_INTERFACE_HPP__

#include "Error.hpp"

namespace StdLib
{
	namespace FileProcMode
	{
		CONSTS_OPED( mode_t,
				Read = BIT( 1 ),
				Write = BIT( 2 ),
				WriteAppend = BIT( 3 ) | BIT( 2 ) )  //  makes existing part of the file virtually invisible( isn't reported with Size calls, isn't accessible through offset sets )
	}

	namespace FileCacheMode
	{
		CONSTS_OPED( mode_t,
			Default = 0,
			LinearRead = BIT( 1 ),  //  requires ProcMode::Read, can't be used with RandomRead
			RandomRead = BIT( 2 ),  //  requires ProcMode::Read, can't be used with LinearRead
			DisableSystemWriteCache = BIT( 3 ) )  //  requires ProcMode::Write
	}

	namespace FileOffsetMode
	{
		CONSTS( mode_t,
				FromBegin,
				FromCurrent,
				FromEnd )
	}

	struct FileInterface
	{
		virtual ~FileInterface() {}

		virtual void Close() = 0;
		virtual bln IsOpened() const = 0;

		virtual bln Read( void *target, ui32 len, ui32 *readed = 0 ) = 0;
		virtual bln Write( const void *source, ui32 len, ui32 *written = 0 ) = 0;

		virtual bln Flush() = 0;
		virtual bln IsBufferingSupported() const = 0;
		virtual bln BufferSet( ui32 size, void *buffer ) = 0;  //  will reject this call if buffering isn't supported,   pass null as a buffer to use an auto allocated buffer, pass 0 as a size to disable buffering
		virtual ui32 BufferSizeGet() const = 0;
		virtual const void *BufferGet() const = 0;  //  will return 0 if there's no buffer

		virtual bln IsSeekSupported() const = 0;
		virtual i64 OffsetGet( FileOffsetMode::mode_t offsetMode = FileOffsetMode::FromBegin, CError *error = 0 ) = 0;  //  returns 0 on fail
		virtual i64 OffsetSet( FileOffsetMode::mode_t offsetMode, i64 offset, CError *error = 0 ) = 0;  //  -1 on error, current offset from the begginning on ok, will Flush

		virtual ui64 SizeGet( CError *error = 0 ) const = 0;  //  returns 0 on fail
		virtual bln SizeSet( ui64 newSize, CError *error = 0 ) = 0;  //  if the file is extended, the extended part's content is undefined

		virtual FileProcMode::mode_t ProcModeGet() const = 0;
		virtual FileCacheMode::mode_t CacheModeGet() const = 0;
	};
}

#endif