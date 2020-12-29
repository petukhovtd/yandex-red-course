#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

template< typename T >
class Deque
{
     using Vector = std::vector< T >;
     using Iterator = typename Vector::iterator;
     using ConstIterator = typename Vector::const_iterator;

public:
     Deque() = default;

     bool Empty() const
     {
          return front_.empty() && back_.empty();
     }

     void PushFront( T item )
     {
          front_.push_back( item );
     }

     void PushBack( T item )
     {
          back_.push_back( item );
     }

     T const& Front() const
     {
          return this->operator[]( 0 );
     }

     T& Front()
     {
          return this->operator[]( 0 );
     }

     T& Back()
     {
          return this->operator[]( Size() - 1 );
     }

     T const& Back() const
     {
          return this->operator[]( Size() - 1 );
     }

     T& operator[]( size_t index )
     {
          return *GetItem( index );
     }

     T const& operator[]( size_t index ) const
     {
          return *GetItem( index );
     }

     T& At( size_t index )
     {
          CheckSize( index );
          return this->operator[]( index );
     }

     T const& At( size_t index ) const
     {
          CheckSize( index );
          return this->operator[]( index );
     }

     size_t Size() const
     {
          return front_.size() + back_.size();
     }

private:
     void CheckSize( size_t index ) const
     {
          if( index >= ( back_.size() + front_.size() ) )
          {
               throw std::out_of_range( std::to_string( index ) );
          }
     }

     ConstIterator GetItem( std::size_t index ) const
     {
          if( index < front_.size() )
          {
               return ( front_.end() - index - 1 );
          }
          return ( back_.begin() + ( index - front_.size() ));
     }

     Iterator GetItem( std::size_t index )
     {
          if( index < front_.size() )
          {
               return ( front_.end() - index - 1 );
          }
          return ( back_.begin() + ( index - front_.size() ));
     }


private:
     std::vector< T > front_;
     std::vector< T > back_;
};

int main() {
     Deque< int > deque;
     deque.PushFront( 2 );
     deque.PushFront( 1 );
     deque.PushBack( 3 );
     deque.PushBack( 4 );
     std::cout << deque.Front() << std::endl;
     std::cout << deque.Back() << std::endl;
     std::cout << deque[ 0 ] << std::endl;
     std::cout << deque[ 1 ] << std::endl;
     std::cout << deque[ 2 ] << std::endl;
     std::cout << deque[ 3 ] << std::endl;
     return 0;
}