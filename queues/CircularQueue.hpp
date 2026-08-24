#pragma once
#include <stdexcept>

// CircularQueue<T>: FIXED-capacity FIFO backed by a raw array, where
// front/rear indices WRAP AROUND using modulo instead of shifting
// elements. This avoids the O(n) shift a naive array-based queue
// would need on every dequeue.
//
// HOW IT WORKS: we keep `front_`, `count_`, and `capacity_`. The
// "rear" slot for the next enqueue is (front_ + count_) % capacity_.
// We track `count_` explicitly (rather than comparing front_==rear_)
// because that comparison is ambiguous between "empty" and "full".
//
// Complexity: enqueue/dequeue/front O(1). Fixed memory, no resizing.
template <typename T>
class CircularQueue {
private:
    T* data_;
    int capacity_;
    int front_;
    int count_;

public:
    explicit CircularQueue(int capacity)
        : data_(new T[capacity]), capacity_(capacity), front_(0), count_(0) {}

    ~CircularQueue() { delete[] data_; }

    bool full() const { return count_ == capacity_; }
    bool empty() const { return count_ == 0; }
    int size() const { return count_; }

    void enqueue(const T& val) {
        if (full()) throw std::runtime_error("enqueue on full circular queue");
        int rear = (front_ + count_) % capacity_;
        data_[rear] = val;
        count_++;
    }

    void dequeue() {
        if (empty()) throw std::runtime_error("dequeue on empty circular queue");
        front_ = (front_ + 1) % capacity_;
        count_--;
    }

    T& front() {
        if (empty()) throw std::runtime_error("front on empty circular queue");
        return data_[front_];
    }
};
