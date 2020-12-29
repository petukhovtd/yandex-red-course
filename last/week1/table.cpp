#include "test_runner.h"

using namespace std;

template< typename T >
class Table
{
public:
     Table( size_t row, size_t col )
     {
          if( row == 0 || col == 0 )
          {
               return;
          }
          table_ = std::vector< std::vector< T > >( row, std::vector< T >( col ));
     }

     std::pair< size_t, size_t > Size() const
     {
          if( table_.empty() )
          {
               return { 0, 0 };
          }
          return { table_.size(), table_.front().size() };
     }

     void Resize( size_t row, size_t col )
     {
          if( row == 0 || col == 0 )
          {
               table_.clear();
               return;
          }

          table_.resize( row );
          for( std::vector< T >& line: table_ )
          {
               line.resize( col );
          }
     }

     std::vector< T > const& operator[]( size_t index ) const
     {
          return table_[ index ];
     }

     std::vector< T >& operator[]( size_t index )
     {
          return table_[ index ];
     }

private:
     std::vector< std::vector< T > >  table_;
};

void TestTable() {
     Table<int> t(1, 1);
     ASSERT_EQUAL(t.Size().first, 1u);
     ASSERT_EQUAL(t.Size().second, 1u);
     t[0][0] = 42;
     ASSERT_EQUAL(t[0][0], 42);
     t.Resize(3, 4);
     ASSERT_EQUAL(t.Size().first, 3u);
     ASSERT_EQUAL(t.Size().second, 4u);

     t.Resize( 2, 3 );
     {
          Table<int> t( 5, 0);
          ASSERT_EQUAL(t.Size().first, 0);
          ASSERT_EQUAL(t.Size().second, 0);
     }
}

int main() {
     TestRunner tr;
     RUN_TEST(tr, TestTable);
     return 0;
}