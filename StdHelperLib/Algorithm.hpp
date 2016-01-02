#ifndef __ALGORITHM_HPP__
#define __ALGORITHM_HPP__

#include <algorithm>  //  TODO: temp

namespace StdLib
{ namespace Algorithm
  {
      namespace Comparator
      {
          struct LeftLess
          {
              template < typename X > bln operator () ( const X &left, const X &right ) const
              {
                  return left < right;
              }
          };

          struct LeftEqual
          {
              template < typename X > bln operator () ( const X &left, const X &right ) const
              {
                  return left == right;
              }
          };

          struct LeftGreater
          {
              template < typename X > bln operator () ( const X &left, const X &right ) const
              {
                  return left > right;
              }
          };
      }

      template < typename iter, typename type, typename cmp > inline iter Find( iter start, iter end, const type &val, const cmp &comparator )
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

      template < typename iter, typename type > inline iter Find( iter start, iter end, const type &val )
      {
          return Find( start, end, val, Comparator::LeftEqual() );
      }

      template < typename iter > inline uiw Distance( const iter &first, const iter &last )
      {
          return Iterator::_IterDist< iter, iter::iteratorType >::Dist( first, last );
      }

      template < typename iter, typename comparator > inline void Sort( iter begin, iter end, const comparator &cmp )
      {
          std::sort( begin, end, cmp );  //  TODO: temp
      }

      template < typename iter > inline void Sort( iter begin, iter end )
      {
          std::sort( begin, end );  //  TODO: temp
      }
      
      //  TODO: unoptimized
      template < typename leftIter, typename rightIter, typename comparator > inline bln Equals( leftIter leftBegin, leftIter leftEnd, rightIter rightBegin, rightIter rightEnd, const comparator &cmp )
      {
          for( ; leftBegin != leftEnd && rightBegin != rightEnd; ++leftBegin, ++rightBegin )
          {
              if( !cmp( *leftBegin, *rightBegin ) )
              {
                  return false;
              }
          }
          
          return (leftBegin == leftEnd) && (rightBegin == rightEnd);
      }

      template < typename leftIter, typename rightIter > inline bln Equals( leftIter leftBegin, leftIter leftEnd, rightIter rightBegin, rightIter rightEnd )
      {
          return Equals( leftBegin, leftEnd, rightBegin, rightEnd, Comparator::LeftEqual() );
      }

      //  TODO: backward-only iterators are unsupported
      template < typename IterType > void Reverse( IterType begin, IterType end )
      {
          if( begin != end )
          {
              --end;
              for( ; begin != end; --end, ++begin )
              {
                  std::swap( *begin, *end );
              }
          }
      }

      template < typename TargetIter, typename SourceIter > void Copy( TargetIter target, SourceIter sourceBegin, SourceIter sourceEnd )
      {
          for( ; sourceBegin != sourceEnd; ++target, ++sourceBegin )
          {
              *target = *sourceBegin;
          }
      }
  }
}

#endif __ALGORITHM_HPP__