#ifndef __FILE_INTERFACE_HPP__
#define __FILE_INTERFACE_HPP__

#include "Result.hpp"

namespace StdLib
{
	enum class FileProcMode : ui8
	{
		Read = BIT( 1 ),
		Write = BIT( 2 ),
		WriteAppend = BIT( 3 ) | BIT( 2 )  //  makes existing part of the file virtually invisible( isn't reported with Size calls, isn't accessible through offset sets )
	}; CONSTS_OPS( FileProcMode );

	//  mode                   already exists        doesn't exist
	//  ***                    ***                   ***
	//  CreateIfDoesNotExist   opens                 creates new & opens
	//  CreateAlways           truncates & opens     creates new & opens
	//  CreateNew              fails                 creates new & opens
	//  OpenExisting           opens                 fails

    enum class FileOpenMode : ui8
	{
        CreateIfDoesNotExist,
        CreateAlways,
		CreateNew,
        OpenExisting
	};

	enum class FileCacheMode : ui8
	{
		Default = 0,
		LinearRead = BIT( 1 ),  //  requires ProcMode::Read, can't be used with RandomRead
		RandomRead = BIT( 2 ),  //  requires ProcMode::Read, can't be used with LinearRead
		DisableSystemWriteCache = BIT( 3 ),  //  requires ProcMode::Write
		DisableSystemReadCache = BIT( 4 )  //  requires ProcMode::Read
	}; CONSTS_OPS( FileCacheMode );
		//  be aware: sometimes disabling write cache you'll also disable read cache and vise versa

	//  file content: _1_2_3#4_7_8_9
	//  current file pointer: 4( highlighted with # )
	//  OffsetGet( FileOffsetMode::FromBegin ) == 4
	//  OffsetGet( FileOffsetMode::FromCurrent ) == 0 ( always zero )
	//  OffsetGet( FileOffsetMode::FromEnd ) == -5 ( 4 - 9 )

	enum class FileOffsetMode : ui8
	{
		FromBegin,
		FromCurrent,
		FromEnd
	};

	struct FileInterface
	{
		virtual ~FileInterface() {}

		virtual void Close() = 0;
		virtual bln IsOpened() const = 0;

		virtual bln Read( void *target, ui32 len, ui32 *readed = 0 ) = 0;
		virtual bln Write( const void *source, ui32 len, ui32 *written = 0 ) = 0;

		virtual bln Flush() = 0;
		virtual bln IsBufferingSupported() const = 0;
		virtual bln BufferSet( ui32 size, std::unique_ptr < byte, void(*)(byte *) > &&buffer = std::unique_ptr < byte, void(*)(byte *) >( nullptr, [](byte *){} ) ) = 0;  //  will reject this call if buffering isn't supported, pass null as a buffer to use an auto allocated buffer, pass 0 as a size to disable buffering
		virtual ui32 BufferSizeGet() const = 0;
		virtual const void *BufferGet() const = 0;  //  will return 0 if there's no buffer

		virtual bln IsSeekSupported() const = 0;
		virtual CResult < i64 > OffsetGet( FileOffsetMode offsetMode = FileOffsetMode::FromBegin ) = 0;
		virtual CResult < i64 > OffsetSet( FileOffsetMode offsetMode, i64 offset ) = 0;

		virtual CResult < ui64 > SizeGet() const = 0;
		virtual CError<> SizeSet( ui64 newSize ) = 0;  //  if the file is extended, the extended part's content is undefined

		virtual FileProcMode ProcModeGet() const = 0;
		virtual FileCacheMode CacheModeGet() const = 0;
	};
}

#endif