#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>
#include <list>

using namespace std;

template< typename RandomIt >
void MakeJosephusPermutation( RandomIt first, RandomIt last, uint32_t step_size )
{
     list< typename RandomIt::value_type > pool;
     std::move( first, last, std::back_inserter( pool ) );
     size_t cur_pos = 0;
     auto curIt = pool.begin();
     while( !pool.empty() )
     {
          *( first++ ) = std::move( *curIt );
          curIt = pool.erase( curIt );
          if( pool.empty() )
          {
               break;
          }
          std::size_t old_pos = cur_pos;
          cur_pos = ( cur_pos + step_size - 1 ) % pool.size();
          if( cur_pos > old_pos )
          {
               std::size_t central = old_pos + ( pool.size() - old_pos ) / 2;
               if( cur_pos > central )
               {
                    curIt = pool.end();
                    std::advance( curIt, cur_pos - pool.size() );
               }
               else
               {
                    std::advance( curIt, cur_pos - old_pos );
               }
          }
          else
          {
               std::size_t central = old_pos / 2;
               if( cur_pos > central )
               {
                    std::advance( curIt, cur_pos - old_pos );
               }
               else
               {
                    curIt = pool.begin();
                    std::advance( curIt, cur_pos );
               }
          }
     }
}

vector< int > MakeTestVector()
{
     vector< int > numbers( 10 );
     iota( begin( numbers ), end( numbers ), 0 );
     return numbers;
}

void TestIntVector()
{
     const vector< int > numbers = MakeTestVector();
     {
          vector< int > numbers_copy = numbers;
          MakeJosephusPermutation( begin( numbers_copy ), end( numbers_copy ), 1 );
          ASSERT_EQUAL( numbers_copy, numbers );
     }
     {
          vector< int > numbers_copy = numbers;
          MakeJosephusPermutation( begin( numbers_copy ), end( numbers_copy ), 3 );
          ASSERT_EQUAL( numbers_copy, vector< int >( { 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 } ) );
     }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt
{
     int value;

     NoncopyableInt( int value )
               : value( value )
     {}

     NoncopyableInt( const NoncopyableInt& ) = delete;

     NoncopyableInt& operator=( const NoncopyableInt& ) = delete;

     NoncopyableInt( NoncopyableInt&& ) = default;

     NoncopyableInt& operator=( NoncopyableInt&& ) = default;
};

bool operator==( const NoncopyableInt& lhs, const NoncopyableInt& rhs )
{
     return lhs.value == rhs.value;
}

ostream& operator<<( ostream& os, const NoncopyableInt& v )
{
     return os << v.value;
}

void TestAvoidsCopying()
{
     vector< NoncopyableInt > numbers;
     numbers.push_back( { 1 } );
     numbers.push_back( { 2 } );
     numbers.push_back( { 3 } );
     numbers.push_back( { 4 } );
     numbers.push_back( { 5 } );

     MakeJosephusPermutation( begin( numbers ), end( numbers ), 2 );

     vector< NoncopyableInt > expected;
     expected.push_back( { 1 } );
     expected.push_back( { 3 } );
     expected.push_back( { 5 } );
     expected.push_back( { 4 } );
     expected.push_back( { 2 } );

     ASSERT_EQUAL( numbers, expected );
}

void TimeTest()
{
     vector< int > numbers( 36000 );
     iota( begin( numbers ), end( numbers ), 1 );
     {
          LOG_DURATION( "" )
          MakeJosephusPermutation( numbers.begin(), numbers.end(), 3 );
     }

}

int main()
{
     TestRunner tr;
     RUN_TEST( tr, TestIntVector );
     RUN_TEST( tr, TestAvoidsCopying );
     RUN_TEST( tr, TimeTest );
     return 0;
}