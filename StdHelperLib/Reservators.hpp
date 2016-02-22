#ifndef __RESERVATORS_HPP__
#define __RESERVATORS_HPP__

namespace StdLib
{ namespace Reservator
  {
    template < typename cnt_type = uiw > struct Simple
    {
        typedef cnt_type count_type;

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

    template < typename cnt_type = uiw > struct SimpleDowning : Simple < cnt_type >
    {
        typedef cnt_type count_type;

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

    template < typename cnt_type = uiw > struct Half
    {
        typedef cnt_type count_type;

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

    template < uiw downingDiv, typename cnt_type = uiw > struct HalfDowning : Half < cnt_type >
    {
        typedef cnt_type count_type;

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

    template < uiw fixed, typename cnt_type = uiw > struct Fixed
    {
        typedef cnt_type count_type;

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

    template < uiw fixed, typename cnt_type = uiw > struct FixedDowning : Fixed < fixed, cnt_type >
    {
        typedef cnt_type count_type;

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

#endif