#pragma once
#include <stdexcept>

// SinglyLinkedList<T>: each node points only to the next one.
//
// HOW IT WORKS
// Unlike an array, nodes are NOT contiguous in memory -- each Node is
// its own heap allocation holding a value and a pointer to the next
// Node. This is why push_front is O(1) (no shifting needed, just
// relink pointers) but random access A[i] is O(n) (you must walk
// node by node from head).
//
// Complexity:
//   push_front / pop_front      O(1)
//   push_back                    O(1) with tail pointer (else O(n))
//   insert_at(i) / erase_at(i)   O(n)  (walk to position i)
//   search                       O(n)
template <typename T>
class SinglyLinkedList {
public:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

private:
    Node* head_;
    Node* tail_;
    int size_;

    // Deep-copy helper shared by copy ctor and operator=
    void copyFrom(const SinglyLinkedList& other) {
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
    SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    SinglyLinkedList(const SinglyLinkedList& other) { copyFrom(other); }
    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this == &other) return *this;
        clear();
        copyFrom(other);
        return *this;
    }
    ~SinglyLinkedList() { clear(); }

    void push_front(const T& val) {
        Node* n = new Node(val);
        n->next = head_;
        head_ = n;
        if (!tail_) tail_ = n; // list was empty
        size_++;
    }

    void push_back(const T& val) {
        Node* n = new Node(val);
        if (!head_) { head_ = tail_ = n; }
        else { tail_->next = n; tail_ = n; }
        size_++;
    }

    void pop_front() {
        if (!head_) throw std::runtime_error("pop_front on empty list");
        Node* old = head_;
        head_ = head_->next;
        if (!head_) tail_ = nullptr; // list became empty
        delete old;
        size_--;
    }

    void insert_at(int pos, const T& val) {
        if (pos < 0 || pos > size_) throw std::out_of_range("insert_at");
        if (pos == 0) { push_front(val); return; }
        Node* cur = head_;
        for (int i = 0; i < pos - 1; i++) cur = cur->next;
        Node* n = new Node(val);
        n->next = cur->next;
        cur->next = n;
        if (n->next == nullptr) tail_ = n;
        size_++;
    }

    void erase_at(int pos) {
        if (pos < 0 || pos >= size_) throw std::out_of_range("erase_at");
        if (pos == 0) { pop_front(); return; }
        Node* cur = head_;
        for (int i = 0; i < pos - 1; i++) cur = cur->next;
        Node* target = cur->next;
        cur->next = target->next;
        if (target == tail_) tail_ = cur;
        delete target;
        size_--;
    }

    // Returns index of first match, or -1.
    int find(const T& val) const {
        Node* cur = head_;
        int idx = 0;
        while (cur) {
            if (cur->value == val) return idx;
            cur = cur->next;
            idx++;
        }
        return -1;
    }

    T& at(int pos) {
        Node* cur = head_;
        for (int i = 0; i < pos; i++) cur = cur->next;
        return cur->value;
    }

    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    T& front() { return head_->value; }
    T& back() { return tail_->value; }
    Node* headNode() const { return head_; }

    template <typename F>
    void forEach(F f) const {
        Node* cur = head_;
        while (cur) { f(cur->value); cur = cur->next; }
    }
};
