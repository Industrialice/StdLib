#ifndef __FILE_MAPPING_HPP__
#define __FILE_MAPPING_HPP__

#include "FileIO.hpp"

namespace StdLib
{

namespace FileMapping
{
	typedef CError < const char * > mappingError;

	namespace Private
	{
		struct MappingStruct
		{
			void *memory;
			uiw size;
			bln is_writable;
			#ifdef WINDOWS
				HANDLE handle;
			#endif
		};

		EXTERNALD MappingStruct FileMapping_Create( FileIO::CFile &file, uiw offset, uiw size, bln is_writeCopy, mappingError &error );
		EXTERNALD void FileMapping_Destroy( MappingStruct *mapping );
	}

	//  Note that the file must be opened as readable, writable is optional
	//  If you pass is_writeCopy, you can write even if the file isn't writable( the actual file will not be affected by your changes )
	//  Pass uiw_max as size to map the whole file
	//  You can close the file right after mapping was created
	//  If a mapping object is const, you can't change the mapped memory
	class Mapping
	{
		Private::MappingStruct mappingStruct;

	public:
		~Mapping()
		{
			Private::FileMapping_Destroy( &mappingStruct );
		}

		Mapping()
		{
			mappingStruct.memory = 0;
		}

		Mapping( FileIO::CFile &file, uiw offset, uiw size, bln is_writeCopy, mappingError *error = 0 )
		{
			mappingError stackedError;
			if( error == nullptr )
			{
				error = &stackedError;
			}
			mappingStruct = Private::FileMapping_Create( file, offset, size, is_writeCopy, *error );
		}

		mappingError Create( FileIO::CFile &file, uiw offset, uiw size, bln is_writeCopy )
		{
			mappingError error;
			Private::FileMapping_Destroy( &mappingStruct );
			mappingStruct = Private::FileMapping_Create( file, offset, size, is_writeCopy, error );
			return error;
		}

		bln IsOpened() const
		{
			return mappingStruct.memory != 0;
		}

		bln IsWritable() const
		{
			ASSUME( IsOpened() );
			return mappingStruct.is_writable;
		}

		void *Memory()
		{
			ASSUME( mappingStruct.is_writable );
			return mappingStruct.memory;
		}

		const void *Memory() const
		{
			return mappingStruct.memory;
		}

		const void *CMemory() const
		{
			return mappingStruct.memory;
		}

		uiw Size() const
		{
			ASSUME( IsOpened() );
			return mappingStruct.size;
		}

		MemoryStreamFixedExt CreateMemoryStream( uiw offset = 0 )
		{
			ASSUME( IsOpened() );
			if( IsWritable() )
			{
				return MemoryStreamFixedExt( Memory(), Size(), offset );
			}
			return MemoryStreamFixedExt( CMemory(), Size(), offset );
		}

		MemoryStreamFixedExt CreateMemoryStream( uiw offset = 0 ) const
		{
			ASSUME( IsOpened() );
			return MemoryStreamFixedExt( CMemory(), Size(), offset );
		}

		MemoryStreamFixedExt CCreateMemoryStream( uiw offset = 0 ) const
		{
			ASSUME( IsOpened() );
			return MemoryStreamFixedExt( CMemory(), Size(), offset );
		}

		Mapping( Mapping &&source ) : mappingStruct( source.mappingStruct )
		{
			source.mappingStruct.memory = 0;
		}

		Mapping &operator = ( Mapping &&source )
		{
			ASSUME( this != &source );
			Private::FileMapping_Destroy( &this->mappingStruct );
			this->mappingStruct = source.mappingStruct;
			source.mappingStruct.memory = 0;
			return *this;
		}
	};
}

}

#endif