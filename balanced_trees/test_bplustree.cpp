#include <iostream>
#include "BPlusTree.hpp"
using namespace std;

int main() {
    BPlusTree<int> bpt(2);
    int vals[] = {10, 20, 5, 6, 12, 30, 7, 17, 1, 2, 25, 40, 50, 3, 4, 8, 9, 11};
    for (int v : vals) bpt.insert(v);

    for (int v : {6, 15, 40, 99}) cout << "search(" << v << ") = " << bpt.search(v) << "\n";

    cout << "range [7,20]: ";
    bpt.rangeQuery(7, 20, [](int v){ cout << v << " "; });
    cout << "\n";

    return 0;
}
