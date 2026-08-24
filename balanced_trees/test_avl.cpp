#include <iostream>
#include "AVLTree.hpp"
#include "../trees/TreeTraversals.hpp"
using namespace std;

int main() {
    AVLTree<int> t;
    // Insert in ascending order: a plain BST would degenerate into a
    // linked list here (height n). AVL must stay ~log2(n).
    for (int i = 1; i <= 15; i++) t.insert(i);

    cout << "inorder (sorted): "; inorder(t.root(), [](int v){ cout << v << " "; }); cout << "\n";
    cout << "height after inserting 1..15 ascending: " << t.height() << " (log2(15)=~3.9, must stay small)\n";

    t.remove(8);
    t.remove(1);
    cout << "after removing 8,1 -> search(8)=" << t.search(8) << " search(9)=" << t.search(9) << "\n";
    cout << "height after removals: " << t.height() << "\n";

    return 0;
}
