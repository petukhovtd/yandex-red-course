#include "test_runner.h"
#include <vector>
#include <future>
#include <numeric>

using namespace std;

template< typename Iterator >
class Page
{
public:
     Page( Iterator begin, Iterator end )
               : begin_( begin )
               , end_( end )
     {}

     Iterator begin() const
     {
          return begin_;
     }

     Iterator end() const
     {
          return end_;
     }

     std::size_t size() const
     {
          return std::distance( begin_, end_ );
     }

private:
     Iterator begin_;
     Iterator end_;
};

template< typename Iterator >
class Paginator
{
     using PagesIterator = typename std::vector< Page< Iterator > >::iterator;
     using PagesConstIterator = typename std::vector< Page< Iterator > >::const_iterator;
public:
     Paginator( Iterator begin, Iterator end, size_t pageSize )
     {
          while( true )
          {
               std::size_t size = std::distance( begin, end );
               if( size == 0 )
               {
                    break;
               }
               Page page( begin, next( begin, min( pageSize, size ) ) );
               begin = page.end();
               pages_.push_back( page );
          }
     }

     PagesIterator begin()
     {
          return pages_.begin();
     }

     PagesConstIterator begin() const
     {
          return pages_.begin();
     }

     PagesIterator end()
     {
          return pages_.end();
     }

     PagesConstIterator end() const
     {
          return pages_.end();
     }

     size_t size() const
     {
          return pages_.size();
     }

private:
     std::vector< Page< Iterator > > pages_;
};

template< typename C >
auto Paginate( C& c, size_t page_size )
{
     return Paginator( begin( c ), end( c ), page_size );
}


int64_t CalculateMatrixSum( const vector< vector< int>>& matrix )
{
     std::vector< std::future< int64_t>> futures;
     for( auto const& page: Paginate( matrix, matrix.size() > 4? matrix.size() / 4: 4 ) )
     {
          futures.push_back( async( [ page ]()
                                    {
                                         int64_t res = 0;
                                         for( auto const& p: page )
                                         {
                                              res += std::accumulate( p.begin(), p.end(), 0 );
                                         }
                                         return res;
                                    } ) );
     }
     int64_t res = 0;
     for( auto& f: futures )
     {
          res += f.get();
     }
     return res;
}

void TestCalculateMatrixSum()
{
     const vector< vector< int>> matrix = {
               { 1,  2,  3,  4 },
               { 5,  6,  7,  8 },
               { 9,  10, 11, 12 },
               { 13, 14, 15, 16 }
     };
     ASSERT_EQUAL( CalculateMatrixSum( matrix ), 136 );
}

void MyTest()
{
     const vector< vector< int>> matrix = {
               { 1, 1, 1, 1 },
     };
     ASSERT_EQUAL( CalculateMatrixSum( matrix ), 4 );

     const vector< vector< int>> matrix1 = {
               { 1 },
     };
     ASSERT_EQUAL( CalculateMatrixSum( matrix1 ), 1 );

     const vector< vector< int>> matrix2 = {
               { 1,  2,  3,  4 },
               { 5,  6,  7,  8 },
               { 9,  10, 11, 12 },
               { 13, 14, 15, 16 },
               { 17, 18, 19, 20 },
     };
     ASSERT_EQUAL( CalculateMatrixSum( matrix2 ), 210 );
}

int main()
{
     TestRunner tr;
     RUN_TEST( tr, TestCalculateMatrixSum );
     RUN_TEST( tr, MyTest );
}