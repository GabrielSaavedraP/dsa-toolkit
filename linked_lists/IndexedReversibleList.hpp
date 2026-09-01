#pragma once

// IndexedReversibleList: a doubly linked list specialized for problems
// where (a) you need to jump straight to "the node holding label X"
// without walking the list, and (b) you need to reverse the WHOLE
// list in O(1) instead of O(n).
//
// HOW IT WORKS
// 1) Direct access: we keep pos_[label] = Node*, a plain array indexed
//    by label, so "find the node for box X" is O(1) instead of O(n).
// 2) O(1) reversal: instead of physically flipping every node's
//    pointers when asked to reverse (O(n)), we keep the physical
//    links `l`/`r` FIXED and add a single global boolean `reversed_`.
//    All "logical left/right" queries just swap which physical field
//    they read depending on that flag. Reversing the whole list is
//    then just `reversed_ = !reversed_` -- O(1), no pointers touched.
// 3) detach/insert helpers work purely in PHYSICAL terms (they don't
//    care about `reversed_` at all) -- that symmetry is what keeps
//    moveBefore/moveAfter simple: only THOSE two functions need to
//    check the flag, to decide which physical operation implements
//    the requested logical one.
//
// Complexity: access by label O(1), moveBefore/moveAfter/swap O(1),
// reverse O(1), full traversal O(n).
#include <iostream>

using namespace std;

class IndexedReversibleList {
public:
    struct Node {
        int label;
        Node* l;
        Node* r;
        Node(int lab) : label(lab), l(nullptr), r(nullptr) {}
    };

private:
    Node** pos_;
    Node* front_;
    Node* back_;
    bool reversed_;
    int n_;

    Node* logicalLeft(Node* x) const { return reversed_ ? x->r : x->l; }
    Node* logicalRight(Node* x) const { return reversed_ ? x->l : x->r; }

    void detach(Node* x) {
        if (x->l) x->l->r = x->r; else front_ = x->r;
        if (x->r) x->r->l = x->l; else back_ = x->l;
        x->l = x->r = nullptr;
    }

    void insertPhysicallyBefore(Node* pivot, Node* x) {
        x->r = pivot;
        x->l = pivot->l;
        if (pivot->l) pivot->l->r = x; else front_ = x;
        pivot->l = x;
    }

    void insertPhysicallyAfter(Node* pivot, Node* x) {
        x->l = pivot;
        x->r = pivot->r;
        if (pivot->r) pivot->r->l = x; else back_ = x;
        pivot->r = x;
    }

public:
    explicit IndexedReversibleList(int n) : front_(nullptr), back_(nullptr), reversed_(false), n_(n) {
        pos_ = new Node*[n + 1];
        Node* prev = nullptr;
        for (int i = 1; i <= n; i++) {
            Node* node = new Node(i);
            pos_[i] = node;
            if (!prev) front_ = node;
            else { prev->r = node; node->l = prev; }
            prev = node;
        }
        back_ = prev;
    }

    void moveBefore(int X, int Y) {
        Node* x = pos_[X];
        Node* y = pos_[Y];
        if (logicalLeft(y) == x) return;
        detach(x);
        if (!reversed_) insertPhysicallyBefore(y, x);
        else insertPhysicallyAfter(y, x);
    }

    void moveAfter(int X, int Y) {
        Node* x = pos_[X];
        Node* y = pos_[Y];
        if (logicalRight(y) == x) return;
        detach(x);
        if (!reversed_) insertPhysicallyAfter(y, x);
        else insertPhysicallyBefore(y, x);
    }

    void swapBoxes(int X, int Y) {
        Node* nx = pos_[X];
        Node* ny = pos_[Y];
        int tmp = nx->label;
        nx->label = ny->label;
        ny->label = tmp;
        pos_[X] = ny;
        pos_[Y] = nx;
    }

    void reverseAll() { reversed_ = !reversed_; }

    template <typename F>
    void forEachLogical(F f) const {
        Node* cur = reversed_ ? back_ : front_;
        while (cur) {
            f(cur->label);
            cur = logicalRight(cur);
        }
    }
};