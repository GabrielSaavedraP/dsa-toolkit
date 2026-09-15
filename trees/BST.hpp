#include <iostream>
using namespace std;

template <typename data_type>
struct BST {
    struct TreeNode {
        data_type value;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;

        TreeNode(data_type value = data_type(),
            TreeNode* left = nullptr,
            TreeNode* right = nullptr,
            TreeNode* parent = nullptr
            ) : value(value), left(left), right(right), parent(parent) {}
    };

    TreeNode* root;

    BST() {
        root = nullptr;
    }

    bool search(data_type target) {
        TreeNode* current = root;
        while (current != nullptr) {
            if (current->value == target) {
                return true;
            }
            if (current->value > target) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }
};
