#pragma once
#include <stdexcept>

// BTree<T>: generalizes a BST so each node holds UP TO (2t-1) keys and
// has UP TO 2t children, not just 1 key / 2 children. This makes the
// tree much SHALLOWER for the same number of elements — the classic
// use case is disk-backed indexes, where each node = one disk page and
// you want to minimize the number of page reads (= tree height).
//
// HOW IT WORKS (insert)
// We always insert into a leaf. Before descending into any FULL child
// (one already holding 2t-1 keys), we SPLIT it in advance ("insert on
// the way down") — that guarantees the node we finally insert into
// always has room, so we never need to re-split all the way back up
// afterward. Splitting a full node pushes its MIDDLE key up into the
// parent and divides the remaining keys into two half-full siblings.
//
// NOTE: This toolkit implements insert + search only. Deletion in a
// B-Tree needs several additional merge/borrow cases and is rarely
// asked for directly in an exam that also covers B+ Trees — if you
// need it, budget extra time to implement it following the same
// "fix problems on the way down" strategy used here for insert.
//
// Complexity (t = minimum degree, n = number of keys):
//   search / insert   O(t * log_t(n))  ->  effectively O(log n)
//   height             O(log_t(n))  (much flatter than a BST for large t)
template <typename T>
class BTree {
private:
    struct Node {
        T* keys;
        Node** children;
        int numKeys;
        bool isLeaf;
        int t;

        Node(int minDegree, bool leaf) : numKeys(0), isLeaf(leaf), t(minDegree) {
            keys = new T[2 * t - 1];
            children = new Node*[2 * t];
            for (int i = 0; i < 2 * t; i++) children[i] = nullptr;
        }
        ~Node() {
            delete[] keys;
            delete[] children;
        }
    };

    Node* root_;
    int t_; // minimum degree

    void splitChild(Node* parent, int i) {
        Node* full = parent->children[i];
        Node* sibling = new Node(t_, full->isLeaf);
        sibling->numKeys = t_ - 1;

        for (int j = 0; j < t_ - 1; j++) sibling->keys[j] = full->keys[j + t_];
        if (!full->isLeaf) {
            for (int j = 0; j < t_; j++) sibling->children[j] = full->children[j + t_];
        }

        T midKey = full->keys[t_ - 1];
        full->numKeys = t_ - 1;

        for (int j = parent->numKeys; j >= i + 1; j--) parent->children[j + 1] = parent->children[j];
        parent->children[i + 1] = sibling;

        for (int j = parent->numKeys - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];
        parent->keys[i] = midKey;
        parent->numKeys++;
    }

    void insertNonFull(Node* node, const T& val) {
        int i = node->numKeys - 1;
        if (node->isLeaf) {
            while (i >= 0 && val < node->keys[i]) { node->keys[i + 1] = node->keys[i]; i--; }
            node->keys[i + 1] = val;
            node->numKeys++;
        } else {
            while (i >= 0 && val < node->keys[i]) i--;
            i++;
            if (node->children[i]->numKeys == 2 * t_ - 1) {
                splitChild(node, i);
                if (val > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], val);
        }
    }

    bool searchRec(Node* node, const T& val) const {
        if (!node) return false;
        int i = 0;
        while (i < node->numKeys && val > node->keys[i]) i++;
        if (i < node->numKeys && val == node->keys[i]) return true;
        if (node->isLeaf) return false;
        return searchRec(node->children[i], val);
    }

    void clearRec(Node* node) {
        if (!node) return;
        if (!node->isLeaf) for (int i = 0; i <= node->numKeys; i++) clearRec(node->children[i]);
        delete node;
    }

public:
    explicit BTree(int minDegree) : root_(nullptr), t_(minDegree) {
        if (minDegree < 2) throw std::invalid_argument("minimum degree t must be >= 2");
    }
    ~BTree() { clearRec(root_); }

    bool search(const T& val) const { return searchRec(root_, val); }

    void insert(const T& val) {
        if (!root_) { root_ = new Node(t_, true); root_->keys[0] = val; root_->numKeys = 1; return; }

        if (root_->numKeys == 2 * t_ - 1) {
            Node* newRoot = new Node(t_, false);
            newRoot->children[0] = root_;
            root_ = newRoot;
            splitChild(newRoot, 0);
            insertNonFull(newRoot, val);
        } else {
            insertNonFull(root_, val);
        }
    }
};
