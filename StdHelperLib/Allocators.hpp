#ifndef __ALLOCATORS_HPP__
#define __ALLOCATORS_HPP__

namespace StdLib
{ namespace Allocator
  {
    struct Simple
    {
        template < typename X >
        static X *RSTR Alloc( uiw count )
        {
            if( count )
            {
                return (X *)malloc( count * sizeof(X) );
            }
            return 0;
        }

        template < typename X >
        static X *Realloc( X *mem, uiw count )
        {
            if( mem || count )
            {
                count += count == 0;
                return (X *)realloc( mem, count * sizeof(X) );
            }
            return 0;
        }

        template < typename X >
        static bln ReallocInplaceIfPossible( X *mem, uiw count )
        {
            return false;
        }

        template < typename X >
        static void Free( X *mem )
        {
            free( mem );
        }
    };
  }
}

#endif __ALLOCATORS_HPP__