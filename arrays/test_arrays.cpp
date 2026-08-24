#include <iostream>
#include "MyVector.hpp"
#include "MyArray.hpp"
using namespace std;

int main() {
    MyVector<int> v;
    for (int i = 1; i <= 5; i++) v.push_back(i * 10);
    v.insert(2, 999);   // [10,20,999,30,40,50]
    v.erase(0);         // [20,999,30,40,50]

    cout << "MyVector after insert/erase: ";
    for (int x : v) cout << x << " ";
    cout << "\n";

    MyVector<int> copy = v; // exercises copy constructor
    copy.push_back(777);
    cout << "original size=" << v.size() << " copy size=" << copy.size() << " (must differ)\n";

    MyArray<int> a(5);
    for (int i = 0; i < 5; i++) a[i] = i * i;
    cout << "MyArray: ";
    for (int i = 0; i < a.size(); i++) cout << a[i] << " ";
    cout << "\n";

    return 0;
}
