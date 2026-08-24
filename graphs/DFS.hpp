#pragma once
#include "GraphList.hpp"
#include "../stacks/Stack.hpp"
#include "../arrays/MyVector.hpp"

// Depth-First Search: dives as deep as possible along one path before
// backtracking. The recursive version uses the CALL STACK implicitly;
// the iterative version below uses an explicit Stack — both do the
// same traversal, recursion is just easier to read.
//
// Complexity: O(V + E), same reasoning as BFS.
// Use DFS for: cycle detection, topological sort, connected components,
// exploring all paths (combined with backtracking).
inline void dfsRec(const GraphList& g, int u, MyVector<bool>& visited, MyVector<int>& order) {
    visited[u] = true;
    order.push_back(u);
    for (const Edge& e : g.neighbors(u)) {
        if (!visited[e.to]) dfsRec(g, e.to, visited, order);
    }
}

inline MyVector<int> dfsRecursive(const GraphList& g, int start) {
    MyVector<bool> visited;
    for (int i = 0; i < g.numVertices(); i++) visited.push_back(false);
    MyVector<int> order;
    dfsRec(g, start, visited, order);
    return order;
}

inline MyVector<int> dfsIterative(const GraphList& g, int start) {
    MyVector<bool> visited;
    for (int i = 0; i < g.numVertices(); i++) visited.push_back(false);

    MyVector<int> order;
    Stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();
        if (visited[u]) continue; // may be pushed more than once before being visited
        visited[u] = true;
        order.push_back(u);

        for (const Edge& e : g.neighbors(u)) {
            if (!visited[e.to]) s.push(e.to);
        }
    }
    return order;
}
