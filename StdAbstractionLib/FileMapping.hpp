#ifndef __FILE_MAPPING_HPP__
#define __FILE_MAPPING_HPP__

#include <StdCoreLib.hpp>
#include "FileIO.hpp"

namespace StdLib
{

namespace FileMapping
{
	typedef CTError < const char * > mappingError;

	namespace Private
	{
		struct MappingStruct
		{
			void *memory;
			uiw size;
			#ifdef WINDOWS
				HANDLE handle;
			#endif
		};

		EXTERNAL MappingStruct Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error );
		EXTERNAL void Destroy( MappingStruct *mapping );
	}

	//  TODO: copy, move
	//  Note that file must be opened as readable, writable is optional
	//  If you pass is_writeCopy, you can write even if file isn't writable( actual file will not be affected by your changes )
	//  Pass uiw_max as size to map the whole file
	//  You can close the file right after mapping was created
	class Mapping
	{
		Private::MappingStruct mappingStruct;

	public:
		Mapping( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error = 0 )
		{
			mappingStruct = Private::Create( file, offset, size, is_writeCopy, error );
		}

		void Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error )
		{
			Destroy( &mappingStruct );
			mappingStruct = Private::Create( file, offset, size, is_writeCopy, error );
		}

		bln IsCreated() const
		{
			return mappingStruct.memory != 0;
		}

		void *Memory()
		{
			return mappingStruct.memory;
		}

		uiw Size() const
		{
			ASSUME( IsCreated() );
			return mappingStruct.size;
		}
	};
}

}

#endif