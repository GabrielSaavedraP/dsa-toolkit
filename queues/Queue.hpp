#pragma once
#include <stdexcept>
#include "../linked_lists/SinglyLinkedList.hpp"

// Queue<T>: FIFO (First In, First Out), built on a singly linked list
// with a tail pointer. enqueue happens at the tail, dequeue at the
// head — DIFFERENT ends, which is why a linked list (not a plain
// array) is the natural fit: no shifting is ever needed.
//
// Complexity: enqueue O(1), dequeue O(1), front O(1).
template <typename T>
class Queue {
private:
    SinglyLinkedList<T> data_;

public:
    void enqueue(const T& val) { data_.push_back(val); }

    void dequeue() {
        if (data_.empty()) throw std::runtime_error("dequeue on empty queue");
        data_.pop_front();
    }

    T& front() {
        if (data_.empty()) throw std::runtime_error("front on empty queue");
        return data_.front();
    }

    bool empty() const { return data_.empty(); }
    int size() const { return data_.size(); }
};
