#pragma once
#include <stdexcept>

// MyArray<T>: FIXED-size array with heap-allocated storage.
// Use this when the problem gives you a hard capacity n up front and
// you don't need growth (e.g. "arreglo de capacidad fija n").
//
// Complexity: operator[] O(1). No resizing logic at all — that's the
// whole point versus MyVector.
template <typename T>
class MyArray {
private:
    T* data_;
    int size_;

public:
    explicit MyArray(int n) : data_(new T[n]), size_(n) {}

    MyArray(const MyArray& other) : data_(new T[other.size_]), size_(other.size_) {
        for (int i = 0; i < size_; i++) data_[i] = other.data_[i];
    }

    MyArray& operator=(const MyArray& other) {
        if (this == &other) return *this;
        delete[] data_;
        size_ = other.size_;
        data_ = new T[size_];
        for (int i = 0; i < size_; i++) data_[i] = other.data_[i];
        return *this;
    }

    ~MyArray() { delete[] data_; }

    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
    int size() const { return size_; }
};
