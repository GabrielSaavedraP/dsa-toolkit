#pragma once
#include <stdexcept>

// CircularLinkedList<T>: singly linked, but the last node points back
// to the first instead of to nullptr. `tail_->next == head_` always.
// Useful for round-robin scheduling problems, Josephus-style problems.
//
// Complexity: push_back/push_front O(1) with tail pointer.
//             traversal/find O(n), but you must track a stopping
//             condition yourself (there's no natural nullptr to stop at).
//             eraseAfter O(1) -- this is the operation Josephus-style
//             elimination problems need: you already have a pointer to
//             the PREDECESSOR of the node you want to remove (because
//             that's how you're walking the ring), so removal is just
//             relinking, no search needed.
template <typename T>
class CircularLinkedList {
public:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

private:
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
    // (copy ctor/operator= omitted for brevity -- same pattern as
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

    // Removes the node RIGHT AFTER `prev` from the ring, and returns
    // a pointer to the node that is now after `prev` (so traversal
    // can continue without re-finding its place). If the removed
    // node was the last one in the ring, the list becomes empty and
    // this returns nullptr.
    //
    // Special case: if the ring has exactly 1 node, `prev` and the
    // node to remove are the SAME node (prev->next == prev) -- handle
    // that before calling this in general traversal code.
    Node* eraseAfter(Node* prev) {
        Node* target = prev->next;
        if (target == tail_) tail_ = prev; // removing the tail: prev becomes new tail
        if (target == prev) {
            // ring had exactly one node: prev == target == tail_
            delete target;
            tail_ = nullptr;
            size_ = 0;
            return nullptr;
        }
        prev->next = target->next;
        delete target;
        size_--;
        return prev->next;
    }

    Node* head() const { return tail_ ? tail_->next : nullptr; }
    Node* tailNode() const { return tail_; }
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
