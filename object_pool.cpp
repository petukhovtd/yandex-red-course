#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate()
    {
        T* object = TryAllocate();
        if( object )
        {
            return object;
        }
        object = new T;
        allocate_.insert( object );
        return object;
    }

    T* TryAllocate()
    {
        if( pool_.empty() )
        {
            return nullptr;
        }
        T* object = pool_.front();
        pool_.pop();
        allocate_.insert( object );
        return object;
    }

    void Deallocate(T* object)
    {
        auto it = allocate_.find( object );
        if( it == allocate_.end() )
        {
            throw std::invalid_argument("");
        }
        pool_.push( *it );
        allocate_.erase( it );
    }

    ~ObjectPool()
    {
        for( auto obj: allocate_ )
        {
            delete obj;
        }
        while ( !pool_.empty() )
        {
            T* obj = pool_.front();
            delete obj;
            pool_.pop();
        }
    }

private:
    std::set< T* > allocate_;
    std::queue< T* > pool_;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}