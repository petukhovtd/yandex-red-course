#include <cstdint>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
     SimpleVector()
     : data_( nullptr )
     , size_( 0 )
     , capacity_( 0 )
     {};

     explicit SimpleVector(size_t size)
     : data_( new T[size])
     , size_( size )
     , capacity_( size )
     {}

     ~SimpleVector()
     {
          delete [] data_;
     }

     T& operator[](size_t index)
     {
          return data_[index];
     }

     T* begin()
     {
          return data_;
     }

     T* end()
     {
          return data_ + size_;
     }

     size_t Size() const
     {
          return size_;
     }

     size_t Capacity() const
     {
          return capacity_;
     }

     void PushBack(const T& value)
     {
          if( size_ == capacity_ )
          {
               Alloc();
          }
          data_[size_++] = value;
     }

     void PushBack(T&& value)
     {
          if( size_ == capacity_ )
          {
               Alloc();
          }
          data_[size_++] = T(std::move(value));
     }

private:
     void Alloc()
     {
          size_t newCapacity = capacity_ == 0? 1: capacity_ * 2;
          T* newData = new T[newCapacity];
          std::copy( std::make_move_iterator(begin()), std::make_move_iterator(end()), newData );
          delete [] data_;
          data_ = newData;
          capacity_ = newCapacity;
     }
     // При необходимости перегрузите
     // существующие публичные методы

private:
     T* data_;
     size_t size_;
     size_t capacity_;
};