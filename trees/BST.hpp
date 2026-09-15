#include <bits/stdc++.h>
using namespace std;

template <typename data_type>
struct BST {
    struct TreeNode {
        data_type data;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        TreeNode(data_type data = data_type(),
            TreeNode* left = nullptr,
            TreeNode* right = nullptr,
            TreeNode* parent = nullptr
            ) : data(data), left(left), right(right), parent(parent) {}
    };

    TreeNode* root;

    BST() {
        root = new TreeNode();
    }
};