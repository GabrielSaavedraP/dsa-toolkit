#include <iostream>
#include "SinglyLinkedList.hpp"
#include "DoublyLinkedList.hpp"
#include "CircularLinkedList.hpp"
using namespace std;

int main() {
    SinglyLinkedList<int> sl;
    sl.push_back(1); sl.push_back(2); sl.push_front(0);
    sl.insert_at(2, 99); // 0,1,99,2
    cout << "singly: "; sl.forEach([](int v){ cout << v << " "; }); cout << "\n";
    sl.erase_at(2);
    cout << "after erase_at(2): "; sl.forEach([](int v){ cout << v << " "; }); cout << "\n";

    DoublyLinkedList<int> dl;
    dl.push_back(10); dl.push_back(20); dl.push_front(5);
    cout << "doubly front=" << dl.front() << " back=" << dl.back() << " size=" << dl.size() << "\n";
    dl.pop_back();
    cout << "after pop_back, back=" << dl.back() << "\n";

    CircularLinkedList<int> cl;
    cl.push_back(1); cl.push_back(2); cl.push_back(3);
    cout << "circular: "; cl.forEach([](int v){ cout << v << " "; }); cout << "\n";

    return 0;
}
