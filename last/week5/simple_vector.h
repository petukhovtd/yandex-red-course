#pragma once

#include <cstdlib>
#include <cstring>
#include <algorithm>

template< typename T >
class SimpleVector
{
public:
     SimpleVector()
               : data_( nullptr )
               , size_( 0 )
               , cap_( 0 )
     {}

     explicit SimpleVector( size_t size )
               : data_( new T[size] )
               , size_( size )
               , cap_( size )
     {}

     SimpleVector( SimpleVector const& other )
               : data_( new T[other.cap_] )
               , size_( other.size_ )
               , cap_( other.cap_ )
     {
          std::copy( other.begin(), other.end(), begin() );
     }

     void operator=( SimpleVector const& other )
     {
          if( other.size_ <= cap_ )
          {
               std::copy( other.begin(), other.end(), begin() );
               size_ = other.size_;
          }
          else
          {
               SimpleVector< T > tmp( other );
               std::swap( tmp.data_, data_ );
               std::swap( tmp.size_, size_ );
               std::swap( tmp.cap_, cap_ );
          }
     }

     ~SimpleVector()
     {
          delete[] data_;
     }

     T& operator[]( size_t index )
     {
          return data_[ index ];
     }

     T* begin()
     {
          return data_;
     }

     T* end()
     {
          return data_ + size_;
     }

     T* begin() const
     {
          return data_;
     }

     T* end() const
     {
          return data_ + size_;
     }

     size_t Size() const
     {
          return size_;
     }

     size_t Capacity() const
     {
          return cap_;
     }

     void PushBack( const T& value )
     {
          if( size_ >= cap_ )
          {
               auto new_cap = cap_ == 0? 1: 2 * cap_;
               auto new_data = new T[new_cap];
               copy( begin(), end(), new_data );
               delete[] data_;
               data_ = new_data;
               cap_ = new_cap;
          }
          data_[ size_++ ] = value;
     }

private:
     T* data_;
     size_t size_;
     size_t cap_;
};
