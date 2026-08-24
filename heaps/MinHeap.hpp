#pragma once
#include <stdexcept>
#include "../arrays/MyVector.hpp"

// MinHeap<T>: complete binary tree stored in a flat array, where
// parent <= both children always (the "heap property").
//
// HOW IT WORKS
// Because the tree is COMPLETE (every level full except possibly the
// last, filled left to right), we can store it in an array with no
// pointers at all: for a node at index i, its children live at
// 2i+1 and 2i+2, and its parent at (i-1)/2. Insert appends at the end
// then "sifts up" (swaps with parent while smaller than parent).
// extractMin removes the root, moves the LAST element to the root,
// then "sifts down" (swaps with the smaller child while larger than it).
//
// Complexity: insert O(log n), extractMin O(log n), peek O(1),
//             buildHeap from n elements O(n) (not n log n — see note).
template <typename T>
class MinHeap {
private:
    MyVector<T> data_;

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }

    void siftUp(int i) {
        while (i > 0 && data_[i] < data_[parent(i)]) {
            std::swap(data_[i], data_[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int n = data_.size();
        while (true) {
            int smallest = i;
            if (left(i) < n && data_[left(i)] < data_[smallest]) smallest = left(i);
            if (right(i) < n && data_[right(i)] < data_[smallest]) smallest = right(i);
            if (smallest == i) break;
            std::swap(data_[i], data_[smallest]);
            i = smallest;
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

    void extractMin() {
        if (data_.empty()) throw std::runtime_error("extractMin on empty heap");
        data_[0] = data_[data_.size() - 1];
        data_.pop_back();
        if (!data_.empty()) siftDown(0);
    }

    // Turns an arbitrary array into a valid heap in O(n) by sifting
    // down from the last non-leaf node backward to the root — this is
    // faster than inserting n elements one by one (O(n log n)) because
    // most nodes are near the bottom, where siftDown does very little
    // work.
    void buildHeap(const MyVector<T>& arr) {
        data_ = arr;
        for (int i = data_.size() / 2 - 1; i >= 0; i--) siftDown(i);
    }

    int size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
};
