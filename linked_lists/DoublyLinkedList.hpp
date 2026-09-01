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

    void copyFrom(const DoublyLinkedList& other) {
        head_ = tail_ = nullptr;
        size_ = 0;
        Node* cur = other.head_;
        while (cur) { push_back(cur->value); cur = cur->next; }
    }

    void clear() {
        Node* cur = head_;
        while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

public:
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    DoublyLinkedList(const DoublyLinkedList& other) { copyFrom(other); }
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;
        clear();
        copyFrom(other);
        return *this;
    }
    ~DoublyLinkedList() { clear(); }

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
        if (!head_) throw std::runtime_error("pop_front on empty list");
        Node* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr; else tail_ = nullptr;
        delete old;
        size_--;
    }

    void pop_back() {
        if (!tail_) throw std::runtime_error("pop_back on empty list");
        Node* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr; else head_ = nullptr;
        delete old;
        size_--;
    }

    void insert_at(int pos, const T& val) {
        if (pos < 0 || pos > size_) throw std::out_of_range("insert_at");
        if (pos == 0) { push_front(val); return; }
        if (pos == size_) { push_back(val); return; }
        Node* cur = head_;
        for (int i = 0; i < pos; i++) cur = cur->next;
        Node* n = new Node(val);
        n->prev = cur->prev;
        n->next = cur;
        cur->prev->next = n;
        cur->prev = n;
        size_++;
    }

    void erase_at(int pos) {
        if (pos < 0 || pos >= size_) throw std::out_of_range("erase_at");
        if (pos == 0) { pop_front(); return; }
        if (pos == size_ - 1) { pop_back(); return; }
        Node* cur = head_;
        for (int i = 0; i < pos; i++) cur = cur->next;
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
        size_--;
    }

    // Removes the FIRST node whose value equals `val`. No-op if not found.
    // This is the "delete x" operation from the doubly-linked-list problem.
    //
    // HOW IT WORKS: walk from head comparing values. Once found, tell
    // the node's two neighbors to point past it (if a neighbor doesn't
    // exist, it means the node was head_ or tail_, so we move that
    // pointer instead), then free it.
    void Delete(const T& val) {
        Node* cur = head_;
        while (cur) {
            if (cur->value == val) {
                if (cur->prev) cur->prev->next = cur->next; else head_ = cur->next;
                if (cur->next) cur->next->prev = cur->prev; else tail_ = cur->prev;
                delete cur;
                size_--;
                return; // only the FIRST match is removed
            }
            cur = cur->next;
        }
    }

    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    T& front() { return head_->value; }
    T& back() { return tail_->value; }
    Node* headNode() const { return head_; }
    Node* tailNode() const { return tail_; }

    void print(std::ostream& os = std::cout) const {
        Node* cur = head_;
        bool first = true;
        while (cur) {
            if (!first) os << " ";
            os << cur->value;
            first = false;
            cur = cur->next;
        }
        os << "\n";
    }
};
