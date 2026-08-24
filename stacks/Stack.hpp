#pragma once
#include <stdexcept>
#include "../arrays/MyVector.hpp"

// Stack<T>: LIFO (Last In, First Out), built on top of MyVector.
// push/pop/top all happen at the SAME end (the back), which is exactly
// why they're all O(1) amortized — no shifting required, unlike a
// stack built on the FRONT of an array.
//
// Complexity: push O(1) amortized, pop O(1), top O(1).
template <typename T>
class Stack {
private:
    MyVector<T> data_;

public:
    void push(const T& val) { data_.push_back(val); }

    void pop() {
        if (data_.empty()) throw std::runtime_error("pop on empty stack");
        data_.pop_back();
    }

    T& top() {
        if (data_.empty()) throw std::runtime_error("top on empty stack");
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    int size() const { return data_.size(); }
};
