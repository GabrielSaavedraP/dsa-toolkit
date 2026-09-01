#pragma once
#include <stdexcept>
#include <iostream>

// DoublyLinkedList<T>: like SinglyLinkedList, but each node also keeps
// a `prev` pointer. That extra pointer is what makes pop_back and
// backward traversal O(1)/O(n)-without-restarting-from-head, instead
// of needing a full walk from head like in the singly linked version.
//
// Complexity: push_front/push_back/pop_front/pop_back O(1).
//             insert_at/erase_at/find/Delete O(n).
using namespace std;

template <typename T>
class DoublyLinkedList {
public:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
    };

private:
    Node* head_;
    Node* tail_;
    int size_;

public:
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    void push_front(const T& val) {
        Node* n = new Node(val);
        n->next = head_;
        if (head_) head_->prev = n;
        head_ = n;
        if (!tail_) tail_ = n;
        size_++;
    }

    void push_back(const T& val) {
        Node* n = new Node(val);
        n->prev = tail_;
        if (tail_) tail_->next = n;
        tail_ = n;
        if (!head_) head_ = n;
        size_++;
    }

    void pop_front() {
        if (!head_) return;
        Node* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr; else tail_ = nullptr;
        delete old;
        size_--;
    }

    void pop_back() {
        if (!tail_) return;
        Node* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr; else head_ = nullptr;
        delete old;
        size_--;
    }

    // Elimina la PRIMERA aparición de val en O(N)
    void Delete(const T& val) {
        Node* cur = head_;
        while (cur) {
            if (cur->value == val) {
                if (cur->prev) cur->prev->next = cur->next; else head_ = cur->next;
                if (cur->next) cur->next->prev = cur->prev; else tail_ = cur->prev;
                delete cur;
                size_--;
                return;
            }
            cur = cur->next;
        }
    }

    int size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void print() const {
        Node* cur = head_;
        bool first = true;
        while (cur) {
            if (!first) cout << " ";
            cout << cur->value;
            first = false;
            cur = cur->next;
        }
        cout << "\n";
    }
};