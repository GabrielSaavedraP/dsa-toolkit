#pragma once
#include <stdexcept>

// CircularLinkedList<T>: singly linked, but the last node points back
// to the first instead of to nullptr. `tail_->next == head_` always.
// Useful for round-robin scheduling problems, Josephus-style problems.
//
// Complexity: push_back/push_front O(1) with tail pointer.
//             traversal/find O(n), but you must track a stopping
//             condition yourself (there's no natural nullptr to stop at).
template <typename T>
class CircularLinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* tail_; // tail_->next is always head
    int size_;

    void clear() {
        if (!tail_) return;
        Node* cur = tail_->next; // = head
        for (int i = 0; i < size_; i++) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        tail_ = nullptr;
        size_ = 0;
    }

public:
    CircularLinkedList() : tail_(nullptr), size_(0) {}
    ~CircularLinkedList() { clear(); }
    // (copy ctor/operator= omitted for brevity — same pattern as
    // SinglyLinkedList::copyFrom if you need them in an exam.)

    void push_back(const T& val) {
        Node* n = new Node(val);
        if (!tail_) { n->next = n; tail_ = n; }
        else { n->next = tail_->next; tail_->next = n; tail_ = n; }
        size_++;
    }

    void push_front(const T& val) {
        Node* n = new Node(val);
        if (!tail_) { n->next = n; tail_ = n; }
        else { n->next = tail_->next; tail_->next = n; }
        size_++;
    }

    Node* head() const { return tail_ ? tail_->next : nullptr; }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Visits every node exactly once starting at head.
    template <typename F>
    void forEach(F f) const {
        if (!tail_) return;
        Node* cur = tail_->next;
        for (int i = 0; i < size_; i++) { f(cur->value); cur = cur->next; }
    }
};
