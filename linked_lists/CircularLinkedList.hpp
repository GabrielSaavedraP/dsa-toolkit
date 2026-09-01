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
#include <iostream>

using namespace std;

template <typename T>
class CircularLinkedList {
public:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

private:
    Node* tail_; // tail_->next siempre es la cabeza (head)
    int size_;

public:
    CircularLinkedList() : tail_(nullptr), size_(0) {}

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

    // Elimina el nodo DESPUÉS de prev y retorna el nuevo nodo en esa posición
    Node* eraseAfter(Node* prev) {
        if (!prev || !tail_) return nullptr;
        Node* target = prev->next;
        if (target == tail_) tail_ = prev;
        if (target == prev) { // Había solo 1 nodo
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
};