#pragma once
#include <stdexcept>
#include "../arrays/MyVector.hpp"

// MaxHeap<T>: mirror image of MinHeap — parent >= children.
// Same array-based complete-tree trick, just flipped comparisons.
// See MinHeap.hpp for the full "how it works" explanation.
//
// Complexity: insert O(log n), extractMax O(log n), peek O(1).
template <typename T>
class MaxHeap {
private:
    MyVector<T> data_;

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }

    void siftUp(int i) {
        while (i > 0 && data_[i] > data_[parent(i)]) {
            std::swap(data_[i], data_[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int n = data_.size();
        while (true) {
            int largest = i;
            if (left(i) < n && data_[left(i)] > data_[largest]) largest = left(i);
            if (right(i) < n && data_[right(i)] > data_[largest]) largest = right(i);
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

public:
    void insert(const T& val) {
        data_.push_back(val);
        siftUp(data_.size() - 1);
    }

    T peek() const {
        if (data_.empty()) throw std::runtime_error("peek on empty heap");
        return data_[0];
    }

    void extractMax() {
        if (data_.empty()) throw std::runtime_error("extractMax on empty heap");
        data_[0] = data_[data_.size() - 1];
        data_.pop_back();
        if (!data_.empty()) siftDown(0);
    }

    void buildHeap(const MyVector<T>& arr) {
        data_ = arr;
        for (int i = data_.size() / 2 - 1; i >= 0; i--) siftDown(i);
    }

    int size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
};
