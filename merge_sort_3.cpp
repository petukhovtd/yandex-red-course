#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
     std::size_t size = std::distance( range_begin, range_end );
     if( size < 2 )
     {
          return;
     }
     std::size_t partSize = size / 3;
     auto vec = std::vector< typename RandomIt::value_type >( std::make_move_iterator( range_begin ),
                                                              std::make_move_iterator( range_end ) );
     auto p1_begin = begin( vec );
     auto p1_end = p1_begin + partSize;
     auto p2_begin = p1_end;
     auto p2_end = p2_begin + partSize;
     auto p3_begin = p2_end;
     auto p3_end = end( vec );

     MergeSort( p1_begin, p1_end );
     MergeSort( p2_begin, p2_end );
     MergeSort( p3_begin, p3_end );

     auto tmp = std::vector< typename RandomIt::value_type >();
     std::merge( std::make_move_iterator( p1_begin ), std::make_move_iterator( p1_end ),
                 std::make_move_iterator( p2_begin ), std::make_move_iterator( p2_end ),
                 std::back_inserter( tmp ) );

     std::merge( std::make_move_iterator( begin( tmp ) ), std::make_move_iterator( end( tmp ) ),
                 std::make_move_iterator( p3_begin ), std::make_move_iterator( p3_end ),
                 range_begin );
}

void TestIntVector() {
     vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
     MergeSort(begin(numbers), end(numbers));
     ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
     TestRunner tr;
     RUN_TEST(tr, TestIntVector);
     return 0;
}