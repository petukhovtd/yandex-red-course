#ifndef SYNCHRONIZED_H
#define SYNCHRONIZED_H
#include <mutex>


// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template< typename T >
class Synchronized
{
public:
     explicit Synchronized( T initial = T() )
     : value( std::move( initial ) )
     {}

     struct Access
     {
          T& ref_to_value;
          std::lock_guard< std::mutex > lockGuard;

          Access( T& value, std::mutex& m )
          : ref_to_value( value )
          , lockGuard( m )
          {}
     };

     Access GetAccess()
     {
          return Access( value, m );
     }

private:
     T value;
     std::mutex m;
};

#endif
