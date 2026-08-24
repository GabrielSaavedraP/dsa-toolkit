#pragma once
#include "../queues/Queue.hpp"

// Free functions for the 4 classic traversals, generic over any node
// type that has ->value, ->left, ->right (matches BST<T>::Node and
// AVLTree<T>::Node).
//
// HOW IT WORKS
// Pre/in/post-order differ only in WHEN you visit the current node
// relative to recursing into children:
//   preorder:  visit, left, right   (root first — good for copying a tree)
//   inorder:   left, visit, right   (gives SORTED order for a BST)
//   postorder: left, right, visit   (children before parent — good for deleting a tree)
// Level-order is different in kind: it's breadth-first, so it needs a
// Queue instead of the call stack that recursion gives you for free.
//
// Complexity: all four are O(n) — every node visited exactly once.
template <typename Node, typename F>
void inorder(Node* node, F visit) {
    if (!node) return;
    inorder(node->left, visit);
    visit(node->value);
    inorder(node->right, visit);
}

template <typename Node, typename F>
void preorder(Node* node, F visit) {
    if (!node) return;
    visit(node->value);
    preorder(node->left, visit);
    preorder(node->right, visit);
}

template <typename Node, typename F>
void postorder(Node* node, F visit) {
    if (!node) return;
    postorder(node->left, visit);
    postorder(node->right, visit);
    visit(node->value);
}

template <typename Node, typename F>
void levelorder(Node* root, F visit) {
    if (!root) return;
    Queue<Node*> q;
    q.enqueue(root);
    while (!q.empty()) {
        Node* cur = q.front();
        q.dequeue();
        visit(cur->value);
        if (cur->left) q.enqueue(cur->left);
        if (cur->right) q.enqueue(cur->right);
    }
}
