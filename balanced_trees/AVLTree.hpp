#pragma once
#include <algorithm>

// AVLTree<T>: a BST that ALSO enforces, at every node, that the
// heights of its left and right subtrees differ by at most 1 (the
// "balance factor" is in {-1,0,1}). This guarantees height O(log n)
// ALWAYS, unlike a plain BST which can degenerate to O(n).
//
// HOW IT WORKS
// After every insert/remove, we walk back up the path we touched and
// check the balance factor at each ancestor. If it's off by more than
// 1, we fix it with a ROTATION:
//   - Left-Left case  -> single right rotation
//   - Right-Right case -> single left rotation
//   - Left-Right case  -> left rotation on child, then right rotation on node
//   - Right-Left case  -> right rotation on child, then left rotation on node
// A rotation re-parents a small number of nodes to shorten one
// subtree and lengthen another, WITHOUT breaking the BST ordering
// invariant — that's the whole trick.
//
// Complexity: insert / remove / search   O(log n) guaranteed (this is
// the entire point of AVL vs plain BST).
template <typename T>
class AVLTree {
public:
    struct Node {
        T value;
        Node* left;
        Node* right;
        int height;
        Node(const T& v) : value(v), left(nullptr), right(nullptr), height(0) {}
    };

private:
    Node* root_;

    int height(Node* n) const { return n ? n->height : -1; }
    int balanceFactor(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }
    void updateHeight(Node* n) { n->height = 1 + std::max(height(n->left), height(n->right)); }

    // Single rotation: pulls the LEFT child up to become the new root
    // of this subtree. Used to fix a left-heavy imbalance.
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Mirror image: pulls the RIGHT child up. Fixes right-heavy imbalance.
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        if (bf > 1) { // left-heavy
            if (balanceFactor(node->left) < 0) node->left = rotateLeft(node->left); // LR case
            return rotateRight(node); // LL case (or LR after fix-up above)
        }
        if (bf < -1) { // right-heavy
            if (balanceFactor(node->right) > 0) node->right = rotateRight(node->right); // RL case
            return rotateLeft(node); // RR case (or RL after fix-up above)
        }
        return node; // already balanced
    }

    Node* insertRec(Node* node, const T& val) {
        if (!node) return new Node(val);
        if (val < node->value) node->left = insertRec(node->left, val);
        else if (val > node->value) node->right = insertRec(node->right, val);
        else return node; // duplicate
        return rebalance(node);
    }

    Node* minNode(Node* node) const { while (node->left) node = node->left; return node; }

    Node* removeRec(Node* node, const T& val) {
        if (!node) return nullptr;
        if (val < node->value) node->left = removeRec(node->left, val);
        else if (val > node->value) node->right = removeRec(node->right, val);
        else {
            if (!node->left || !node->right) {
                Node* child = node->left ? node->left : node->right;
                delete node;
                return child; // may be nullptr — caller handles it
            }
            Node* succ = minNode(node->right);
            node->value = succ->value;
            node->right = removeRec(node->right, succ->value);
        }
        return rebalance(node);
    }

    bool searchRec(Node* node, const T& val) const {
        if (!node) return false;
        if (val == node->value) return true;
        return val < node->value ? searchRec(node->left, val) : searchRec(node->right, val);
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    AVLTree() : root_(nullptr) {}
    ~AVLTree() { clearRec(root_); }

    void insert(const T& val) { root_ = insertRec(root_, val); }
    void remove(const T& val) { root_ = removeRec(root_, val); }
    bool search(const T& val) const { return searchRec(root_, val); }
    int height() const { return height(root_); }
    Node* root() const { return root_; }
};
