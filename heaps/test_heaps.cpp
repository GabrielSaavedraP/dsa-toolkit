#include <iostream>
#include "MinHeap.hpp"
#include "MaxHeap.hpp"
using namespace std;

int main() {
    MinHeap<int> minH;
    int vals[] = {5, 3, 8, 1, 9, 2};
    for (int v : vals) minH.insert(v);
    cout << "min-heap extraction order (sorted asc): ";
    while (!minH.empty()) { cout << minH.peek() << " "; minH.extractMin(); }
    cout << "\n";

    MaxHeap<int> maxH;
    for (int v : vals) maxH.insert(v);
    cout << "max-heap extraction order (sorted desc): ";
    while (!maxH.empty()) { cout << maxH.peek() << " "; maxH.extractMax(); }
    cout << "\n";

    return 0;
}
