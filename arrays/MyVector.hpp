#pragma once
#include <stdexcept>

// MyVector<T>: dynamic array, replacement for std::vector.
//
// HOW IT WORKS
// Internally it is a raw pointer `data_` to a heap-allocated block, plus
// `size_` (elements in use) and `capacity_` (allocated slots). When
// push_back would overflow the capacity, we allocate a NEW block of
// DOUBLE the capacity, copy everything over, and free the old block.
// Doubling (instead of +1) is what makes push_back O(1) AMORTIZED:
// resizes get exponentially rarer, so total cost over n pushes is O(n).
//
// Complexity:
//   operator[]                O(1)
//   push_back / pop_back      O(1) amortized
//   insert(pos) / erase(pos)  O(n)  (must shift elements)
//   find                      O(n)
template <typename T>
class MyVector {
private:
    T* data_;
    int size_;
    int capacity_;

    void grow() {
        int newCap = (capacity_ == 0) ? 1 : capacity_ * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < size_; i++) newData[i] = data_[i];
        delete[] data_;
        data_ = newData;
        capacity_ = newCap;
    }

public:
    MyVector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit MyVector(int initialCapacity)
        : data_(new T[initialCapacity]), size_(0), capacity_(initialCapacity) {}

    // Rule of Three: deep-copy constructor. Never copy the raw pointer,
    // or two objects would point to the same memory and double-free on
    // destruction.
    MyVector(const MyVector& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = (capacity_ > 0) ? new T[capacity_] : nullptr;
        for (int i = 0; i < size_; i++) data_[i] = other.data_[i];
    }

    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this; // guard against self-assignment
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = (capacity_ > 0) ? new T[capacity_] : nullptr;
        for (int i = 0; i < size_; i++) data_[i] = other.data_[i];
        return *this;
    }

    ~MyVector() { delete[] data_; }

    void push_back(const T& val) {
        if (size_ == capacity_) grow();
        data_[size_++] = val;
    }

    void pop_back() {
        if (size_ > 0) size_--; // no need to zero memory, just shrink logical size
    }

    // Insert val so it ends up at index pos, shifting the tail right.
    // Must iterate BACKWARD (from the end toward pos) or you overwrite
    // elements before you've had a chance to move them.
    void insert(int pos, const T& val) {
        if (pos < 0 || pos > size_) throw std::out_of_range("insert: pos out of range");
        if (size_ == capacity_) grow();
        for (int i = size_; i > pos; i--) data_[i] = data_[i - 1];
        data_[pos] = val;
        size_++;
    }

    // Remove the element at pos, shifting the tail left.
    // Here you iterate FORWARD safely because each write only depends
    // on a value you've already read.
    void erase(int pos) {
        if (pos < 0 || pos >= size_) throw std::out_of_range("erase: pos out of range");
        for (int i = pos; i < size_ - 1; i++) data_[i] = data_[i + 1];
        size_--;
    }

    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }

    T& front() { return data_[0]; }
    T& back() { return data_[size_ - 1]; }

    int size() const { return size_; }
    int capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    void clear() { size_ = 0; } // O(1): just resets the logical size

    // Raw pointer iterators — enough for range-based for loops.
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }
};
