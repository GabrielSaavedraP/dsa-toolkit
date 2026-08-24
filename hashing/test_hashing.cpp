#include <iostream>
#include "HashTable.hpp"
using namespace std;

int main() {
    HashTable<int, int> h;
    for (int i = 0; i < 50; i++) h.insert(i, i * i); // forces several rehashes
    cout << "size=" << h.size() << "\n";
    cout << "get(7)=" << h.get(7) << " (expect 49)\n";
    h.remove(7);
    cout << "contains(7) after remove: " << h.contains(7) << " (expect 0)\n";

    HashTable<int, int> collisions(4); // small capacity to force collisions
    for (int i = 0; i < 10; i++) collisions.insert(i, i);
    cout << "collisions get(9)=" << collisions.get(9) << " (expect 9)\n";

    return 0;
}
