#pragma once
#include <stdexcept>

// BPlusTree<T>: like a B-Tree, but with two key differences that
// matter a lot in practice:
//   1) ALL actual values live in the LEAVES. Internal nodes only
//      store "routing" keys used to decide which child to descend
//      into — they never hold real data themselves.
//   2) Leaves are LINKED together in a chain (leaf->next), so once
//      you've found where a range starts, you can walk forward
//      through sorted order without ever going back up the tree.
//      This is exactly why databases use B+ Trees for range queries
//      (e.g. "all rows with 10 <= id <= 50").
//
// HOW IT WORKS (insert)
// Same "split on the way down when full" strategy as BTree. The one
// difference: when a LEAF splits, the middle key is COPIED (not
// moved) up into the parent, because that key must still physically
// exist in a leaf to keep the leaf chain complete.
//
// NOTE: like BTree.hpp, this implements insert + search only.
// Deletion (borrow/merge across the leaf chain) is a natural extension
// of the same techniques but is left out to keep this exam-usable.
//
// Complexity: search / insert   O(log_t n), same shape as BTree, but
// with better constants for RANGE queries thanks to the leaf chain.
template <typename T>
class BPlusTree {
private:
    struct Node {
        bool isLeaf;
        int t;
        int numKeys;
        T* keys;
        Node** children; // used only if !isLeaf
        Node* next;       // used only if isLeaf (the leaf chain)

        Node(int minDegree, bool leaf) : isLeaf(leaf), t(minDegree), numKeys(0), next(nullptr) {
            keys = new T[2 * t];
            children = leaf ? nullptr : new Node*[2 * t + 1];
            if (children) for (int i = 0; i <= 2 * t; i++) children[i] = nullptr;
        }
        ~Node() {
            delete[] keys;
            delete[] children;
        }
    };

    Node* root_;
    int t_;

    // Splits a full child of `parent` at index i. Returns nothing; the
    // routing key is inserted directly into parent.
    void splitChild(Node* parent, int i) {
        Node* full = parent->children[i];
        Node* sibling = new Node(t_, full->isLeaf);

        if (full->isLeaf) {
            // Leaf split: right half moves to sibling, middle key is
            // COPIED up (it still lives in the sibling leaf too).
            int mid = t_;
            sibling->numKeys = full->numKeys - mid;
            for (int j = 0; j < sibling->numKeys; j++) sibling->keys[j] = full->keys[mid + j];
            full->numKeys = mid;

            sibling->next = full->next;
            full->next = sibling;

            for (int j = parent->numKeys; j >= i + 1; j--) parent->children[j + 1] = parent->children[j];
            parent->children[i + 1] = sibling;
            for (int j = parent->numKeys - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];
            parent->keys[i] = sibling->keys[0]; // copy up, don't remove from leaf
            parent->numKeys++;
        } else {
            // Internal split: standard B-Tree style, middle key MOVES up.
            int mid = t_;
            T midKey = full->keys[mid];
            sibling->numKeys = full->numKeys - mid - 1;
            for (int j = 0; j < sibling->numKeys; j++) sibling->keys[j] = full->keys[mid + 1 + j];
            for (int j = 0; j <= sibling->numKeys; j++) sibling->children[j] = full->children[mid + 1 + j];
            full->numKeys = mid;

            for (int j = parent->numKeys; j >= i + 1; j--) parent->children[j + 1] = parent->children[j];
            parent->children[i + 1] = sibling;
            for (int j = parent->numKeys - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];
            parent->keys[i] = midKey;
            parent->numKeys++;
        }
    }

    void insertNonFull(Node* node, const T& val) {
        if (node->isLeaf) {
            int i = node->numKeys - 1;
            while (i >= 0 && val < node->keys[i]) { node->keys[i + 1] = node->keys[i]; i--; }
            node->keys[i + 1] = val;
            node->numKeys++;
            return;
        }
        int i = 0;
        while (i < node->numKeys && val >= node->keys[i]) i++;
        if (node->children[i]->numKeys == maxKeys(node->children[i])) {
            splitChild(node, i);
            if (val >= node->keys[i]) i++;
        }
        insertNonFull(node->children[i], val);
    }

    int maxKeys(Node* n) const { return n->isLeaf ? 2 * t_ - 1 : 2 * t_; }

    Node* findLeaf(Node* node, const T& val) const {
        if (node->isLeaf) return node;
        int i = 0;
        while (i < node->numKeys && val >= node->keys[i]) i++;
        return findLeaf(node->children[i], val);
    }

    void clearRec(Node* node) {
        if (!node) return;
        if (!node->isLeaf) for (int i = 0; i <= node->numKeys; i++) clearRec(node->children[i]);
        delete node;
    }

public:
    explicit BPlusTree(int minDegree) : root_(nullptr), t_(minDegree) {
        if (minDegree < 2) throw std::invalid_argument("minimum degree t must be >= 2");
    }
    ~BPlusTree() { clearRec(root_); }

    bool search(const T& val) const {
        if (!root_) return false;
        Node* leaf = findLeaf(root_, val);
        for (int i = 0; i < leaf->numKeys; i++) if (leaf->keys[i] == val) return true;
        return false;
    }

    void insert(const T& val) {
        if (!root_) { root_ = new Node(t_, true); root_->keys[0] = val; root_->numKeys = 1; return; }

        if (root_->numKeys == maxKeys(root_)) {
            Node* newRoot = new Node(t_, false);
            newRoot->children[0] = root_;
            root_ = newRoot;
            splitChild(newRoot, 0);
            insertNonFull(newRoot, val);
        } else {
            insertNonFull(root_, val);
        }
    }

    // Range query [lo, hi]: find the starting leaf, then walk the leaf
    // chain forward. This is the whole reason B+ Trees exist.
    template <typename F>
    void rangeQuery(const T& lo, const T& hi, F visit) const {
        if (!root_) return;
        Node* leaf = findLeaf(root_, lo);
        while (leaf) {
            for (int i = 0; i < leaf->numKeys; i++) {
                if (leaf->keys[i] >= lo && leaf->keys[i] <= hi) visit(leaf->keys[i]);
            }
            leaf = leaf->next;
        }
    }
};
