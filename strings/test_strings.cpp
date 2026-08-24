#include <iostream>
#include "MyString.hpp"
using namespace std;

int main() {
    MyString a("hello");
    MyString b("world");
    MyString c = a + MyString(" ") + b;
    cout << "concat: " << c << " (len=" << c.length() << ")\n";

    MyString sub = c.substr(6, 5);
    cout << "substr(6,5): " << sub << "\n";

    cout << "find 'world': " << c.find(MyString("world")) << "\n";
    cout << "a == a copy? " << (a == MyString("hello") ? "yes" : "no") << "\n";

    MyString d = a; // copy ctor
    d.push_back('!');
    cout << "a=" << a << " d=" << d << " (must differ, deep copy check)\n";

    return 0;
}
