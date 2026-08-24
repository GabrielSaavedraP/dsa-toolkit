#include <iostream>
#include "Stack.hpp"
using namespace std;

int main() {
    Stack<int> s;
    s.push(1); s.push(2); s.push(3);
    cout << "top=" << s.top() << " size=" << s.size() << "\n";
    s.pop();
    cout << "after pop, top=" << s.top() << " size=" << s.size() << "\n";
    return 0;
}
