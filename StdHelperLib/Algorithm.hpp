#ifndef __ALGORITHM_HPP__
#define __ALGORITHM_HPP__

namespace StdLib
{ namespace Algorithm
  {
      template < typename iter, typename type > iter Find( iter start, iter end, const type &val )
      {
          while( start != end )
          {
              if( *start == val )
              {
                  return start;
              }
              ++start;
          }
          return end;
      }

      template < typename iter, typename type, typename cmp > iter Find( iter start, iter end, const type &val, const cmp &comparator )
      {
          while( start != end )
          {
              if( comparator( *start, val ) )
              {
                  return start;
              }
              ++start;
          }
          return end;
      }
  }
}

#endif __ALGORITHM_HPP__