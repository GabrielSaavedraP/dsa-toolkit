#include <iostream>
#include "Queue.hpp"
#include "CircularQueue.hpp"
using namespace std;

int main() {
    Queue<int> q;
    q.enqueue(1); q.enqueue(2); q.enqueue(3);
    cout << "front=" << q.front() << " size=" << q.size() << "\n";
    q.dequeue();
    cout << "after dequeue, front=" << q.front() << "\n";

    CircularQueue<int> cq(3);
    cq.enqueue(10); cq.enqueue(20); cq.enqueue(30);
    cout << "circular full=" << cq.full() << "\n";
    cq.dequeue();
    cq.enqueue(40); // wraps around
    cout << "circular front=" << cq.front() << " size=" << cq.size() << "\n";

    return 0;
}
