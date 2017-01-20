#ifndef __VIRTUAL_MEMORY_HPP__
#define __VIRTUAL_MEMORY_HPP__

namespace StdLib
{
	namespace VirtualMemory
	{
		enum class PageMode : ui8
		{
			Write = BIT( 0 ),
			Read = BIT( 1 ),
			Execute = BIT( 2 )
		}; CONSTS_OPS( PageMode );

		//  TODO: resize, realloc
		EXTERNALD void *Reserve( uiw size );
		EXTERNALD bln Commit( void *p_mem, uiw size, PageMode mode );
		EXTERNALD void *Alloc( uiw size, PageMode mode );
		EXTERNALD bln Free( void *p_mem );
		EXTERNALD CResult < PageMode > ProtectGet( const void *p_mem, uiw size );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
		EXTERNALD bln ProtectSet( void *p_mem, uiw size, PageMode mode );  //  can commit uncommited memory
		EXTERNALS ui32 PageSize();
		EXTERNALS CResult < ui32 > LargePageSize();

		inline CError<> Error_InconsistentProtection() { return CError<>( Error::_MaxDefaultError + 0, "VIRTUAL_MEM", "INCONSISTENT_PROTECTION" ); }
	}
}

#endif