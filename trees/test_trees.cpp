#include <iostream>
#include "BST.hpp"
#include "TreeTraversals.hpp"
using namespace std;

int main() {
    BST<int> t;
    int vals[] = {50, 30, 70, 20, 40, 60, 80};
    for (int v : vals) t.insert(v);

    cout << "inorder (sorted): "; inorder(t.root(), [](int v){ cout << v << " "; }); cout << "\n";
    cout << "preorder: "; preorder(t.root(), [](int v){ cout << v << " "; }); cout << "\n";
    cout << "level-order: "; levelorder(t.root(), [](int v){ cout << v << " "; }); cout << "\n";

    cout << "search(60)=" << t.search(60) << " search(99)=" << t.search(99) << "\n";
    t.remove(30); // node with two children
    cout << "after remove(30) inorder: "; inorder(t.root(), [](int v){ cout << v << " "; }); cout << "\n";
    cout << "height=" << t.height() << " size=" << t.size() << "\n";

    return 0;
}
