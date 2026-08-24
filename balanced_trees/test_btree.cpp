#include <iostream>
#include "BTree.hpp"
using namespace std;

int main() {
    BTree<int> bt(2); // minimum degree t=2 -> classic "2-3-4 tree" shape
    int vals[] = {10, 20, 5, 6, 12, 30, 7, 17, 1, 2, 25, 40, 50};
    for (int v : vals) bt.insert(v);

    for (int v : {6, 15, 40, 99}) {
        cout << "search(" << v << ") = " << bt.search(v) << "\n";
    }
    return 0;
}
