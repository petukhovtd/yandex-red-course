#pragma once

#include <cstdlib>
#include <cstring>

template<typename T>
class SimpleVector {
public:
    SimpleVector()
            : data(nullptr), end_(nullptr), cap_(nullptr) {}

    explicit SimpleVector(size_t size) {
        data = new T[size];
        end_ = data + size;
        cap_ = end_;
    }

    ~SimpleVector() {
        delete[] data;
    }

    T &operator[](size_t index) {
        return data[index];
    }

    T* begin() {
        return data;
    }

    T* end() {
        return end_;
    }

    size_t Size() const {
        return static_cast< size_t >( end_ - data );
    }

    size_t Capacity() const {
        return cap_ - data;
    }

    void PushBack(const T &value) {
        if( !data ) {
            data = new T[1];
            *data = value;
            end_ = data + 1;
            cap_ = end_;
            return;
        }

        if( end_ == cap_ ) {
            Alloc();
        }
        *end_ = value;
        end_++;
    }

private:
    void Alloc() {
        size_t size = Size();
        size_t newCap = Capacity() * 2;
        T* newData = new T[newCap];
        memcpy(newData, data, size * sizeof(T));
        delete[] data;
        data = newData;
        end_ = data + size;
        cap_ = data + newCap;
    }

private:
    T* data;
    T* end_;
    T* cap_;
};