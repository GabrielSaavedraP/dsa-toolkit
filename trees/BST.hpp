#pragma once
#include <algorithm>

// BST<T>: Binary Search Tree. Invariant maintained at every node:
// everything in the left subtree < node < everything in the right
// subtree. That invariant is what makes search O(h) instead of O(n) —
// at each step you eliminate one whole subtree.
//
// Complexity (h = height of tree):
//   insert / search / remove   O(h)  ->  O(log n) balanced, O(n) worst case
//   (worst case: inserting sorted data with no rebalancing degenerates
//    into a linked list — this is exactly the motivation for AVL trees)
template <typename T>
class BST {
public:
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node(const T& v) : value(v), left(nullptr), right(nullptr) {}
    };

private:
    Node* root_;
    int size_;

    Node* insertRec(Node* node, const T& val) {
        if (!node) { size_++; return new Node(val); }
        if (val < node->value) node->left = insertRec(node->left, val);
        else if (val > node->value) node->right = insertRec(node->right, val);
        // else: duplicate, do nothing
        return node;
    }

    Node* minNode(Node* node) const {
        while (node->left) node = node->left;
        return node;
    }

    Node* removeRec(Node* node, const T& val) {
        if (!node) return nullptr;
        if (val < node->value) { node->left = removeRec(node->left, val); return node; }
        if (val > node->value) { node->right = removeRec(node->right, val); return node; }

        // node->value == val: this is the node to delete
        if (!node->left) { Node* r = node->right; delete node; size_--; return r; }
        if (!node->right) { Node* l = node->left; delete node; size_--; return l; }

        // Two children: replace value with in-order successor (smallest
        // in right subtree), then delete that successor instead.
        Node* succ = minNode(node->right);
        node->value = succ->value;
        node->right = removeRec(node->right, succ->value);
        return node;
    }

    bool searchRec(Node* node, const T& val) const {
        if (!node) return false;
        if (val == node->value) return true;
        return val < node->value ? searchRec(node->left, val) : searchRec(node->right, val);
    }

    int heightRec(Node* node) const {
        if (!node) return -1;
        return 1 + std::max(heightRec(node->left), heightRec(node->right));
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    BST() : root_(nullptr), size_(0) {}
    ~BST() { clearRec(root_); }

    void insert(const T& val) { root_ = insertRec(root_, val); }
    void remove(const T& val) { root_ = removeRec(root_, val); }
    bool search(const T& val) const { return searchRec(root_, val); }
    int height() const { return heightRec(root_); }
    int size() const { return size_; }
    Node* root() const { return root_; }
};
