#ifndef __VIRTUAL_MEMORY_HPP__
#define __VIRTUAL_MEMORY_HPP__

namespace StdLib
{
	namespace VirtualMemory
	{
		enum class PageMode : ui8
		{
			Write = BitPos( 0 ),
			Read = BitPos( 1 ),
			Execute = BitPos( 2 )
		}; CONSTS_OPS( PageMode );

		//  TODO: resize, realloc, POSIX has mremap
		EXTERNALD WARN_IF_UNUSED void *Reserve( uiw size );
		EXTERNALD bool Commit( void *p_mem, uiw size, PageMode mode );
		EXTERNALD WARN_IF_UNUSED void *Alloc( uiw size, PageMode mode );
		EXTERNALD bool Free( void *p_mem );
		EXTERNALD CResult < PageMode > ProtectGet( const void *p_mem, uiw size );  //  PageMode::Error if pages have different protection, always PageMode::Error on POSIX
		EXTERNALD bool ProtectSet( void *p_mem, uiw size, PageMode mode );  //  can commit uncommited memory
		EXTERNALS ui32 PageSize();
		EXTERNALS CResult < ui32 > LargePageSize();

		inline CError<> Error_InconsistentProtection() { return CError<>( Error::_MaxDefaultError + 0, "VIRTUAL_MEM", "INCONSISTENT_PROTECTION" ); }
	}
}

#endif