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

		EXTERNALS MappingStruct Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error );
		EXTERNALS void Destroy( MappingStruct *mapping );
	}

	//  Note that file must be opened as readable, writable is optional
	//  If you pass is_writeCopy, you can write even if file isn't writable( actual file will not be affected by your changes )
	//  Pass uiw_max as size to map the whole file
	//  You can close the file right after mapping was created
	class Mapping
	{
		Private::MappingStruct mappingStruct;

		Mapping( const Mapping & );
		Mapping &operator = ( const Mapping & );

	public:
		~Mapping()
		{
			Private::Destroy( &mappingStruct );
		}

		Mapping()
		{
			mappingStruct.memory = 0;
		}

		Mapping( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error = 0 )
		{
			mappingStruct = Private::Create( file, offset, size, is_writeCopy, error );
		}

		void Create( FileIO::CFile *file, uiw offset, uiw size, bln is_writeCopy, mappingError *error = 0 )
		{
			Private::Destroy( &mappingStruct );
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

		const void *Memory() const
		{
			return mappingStruct.memory;
		}

		uiw Size() const
		{
			ASSUME( IsCreated() );
			return mappingStruct.size;
		}

	#ifdef MOVE_SUPPORTED
		Mapping( Mapping &&source ) : mappingStruct( source.mappingStruct )
		{
			source.mappingStruct.memory = 0;
		}

		Mapping &operator = ( Mapping &&source )
		{
			ASSUME( this != &source );
			this->mappingStruct = source.mappingStruct;
			source.mappingStruct.memory = 0;
		}
	#endif
	};
}

}

#endif