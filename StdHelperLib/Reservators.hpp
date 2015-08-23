#ifndef __RESERVATORS_HPP__
#define __RESERVATORS_HPP__

namespace StdLib 
{ namespace Reservator
  {
    template < typename count_type = uiw > struct Simple
    {
		typedef count_type count_type;

        static bln Up( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved );
            if( newSize > *reserved )
            {
                *reserved = newSize;
                return true;
            }
            return false;
        };

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            return false;
        };
    };

    template < typename count_type = uiw > struct SimpleDowning : Simple < count_type >
	{
		typedef count_type count_type;

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            if( newSize < *reserved )
            {
                *reserved = newSize;
                return true;
            }
            return false;
        };
	};
    
    template < typename count_type = uiw > struct Half
    {
		typedef count_type count_type;

        static bln Up( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved );
            if( newSize > *reserved )
            {
                *reserved = newSize + newSize / 2;
                return true;
            }
            return false;
        };

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            return false;
        };
    };
    
    template < uiw downingDiv, typename count_type = uiw > struct HalfDowning : Half < count_type >
    {
		typedef count_type count_type;

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            if( *reserved / downingDiv > newSize )
            {
                *reserved = newSize;
                return true;
            }
            return false;
        };
    };

    template < uiw fixed, typename count_type = uiw > struct Fixed
    {
		typedef count_type count_type;

        static bln Up( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved );
            if( newSize > *reserved )
            {
                *reserved = newSize + fixed;
                return true;
            }
            return false;
        };

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            return false;
        };
    };

    template < uiw fixed, typename count_type = uiw > struct FixedDowning : Fixed < fixed, count_type >
    {
		typedef count_type count_type;

        static bln Down( count_type newSize, count_type *reserved )
        {
            ASSUME( reserved && *reserved >= newSize );
            if( *reserved - newSize > fixed )
            {
                *reserved = newSize + fixed;
                return true;
            }
            return false;
        };
    };
  }
}

#endif __RESERVATORS_HPP__