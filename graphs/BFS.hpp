#pragma once
#include "GraphList.hpp"
#include "../queues/Queue.hpp"
#include "../arrays/MyVector.hpp"

// Breadth-First Search: explores the graph level by level (all
// distance-1 vertices before any distance-2 vertex), using a Queue
// (FIFO) to guarantee that order. Marking a vertex "visited" the
// MOMENT it's enqueued (not when dequeued) is what prevents the same
// vertex from being added to the queue multiple times.
//
// Complexity: O(V + E) — every vertex enqueued once, every edge examined once.
// Use BFS for: shortest path in an UNWEIGHTED graph, connected components.
inline MyVector<int> bfs(const GraphList& g, int start) {
    MyVector<bool> visited;
    for (int i = 0; i < g.numVertices(); i++) visited.push_back(false);

    MyVector<int> order;
    Queue<int> q;

    visited[start] = true;
    q.enqueue(start);

    while (!q.empty()) {
        int u = q.front();
        q.dequeue();
        order.push_back(u);

        for (const Edge& e : g.neighbors(u)) {
            if (!visited[e.to]) {
                visited[e.to] = true; // mark on enqueue, not on dequeue
                q.enqueue(e.to);
            }
        }
    }
    return order;
}
