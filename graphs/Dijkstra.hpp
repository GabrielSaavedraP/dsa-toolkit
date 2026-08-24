#pragma once
#include <climits>
#include "GraphList.hpp"
#include "../heaps/MinHeap.hpp"
#include "../arrays/MyVector.hpp"
#include "Prim.hpp" // reuse HeapEntry{weight, vertex}

// Dijkstra's algorithm: shortest path from a single source to every
// other vertex, for graphs with NON-NEGATIVE edge weights (negative
// weights break the greedy assumption below — use Bellman-Ford instead).
//
// HOW IT WORKS: maintain dist[] initialized to infinity except
// dist[start]=0. Repeatedly pop the vertex with the smallest known
// distance from a MinHeap (greedy: once popped, its distance is
// FINAL — this is only safe because all weights are non-negative, so
// nothing later could make a shorter path to it). For each neighbor,
// if going through the current vertex gives a shorter path than what
// we had, "relax" the edge: update dist[] and push the improved
// distance. Like Prim, stale heap entries are skipped lazily.
//
// Complexity: O(E log V) with a binary heap.
inline MyVector<int> dijkstra(const GraphList& g, int start) {
    int n = g.numVertices();
    MyVector<int> dist;
    for (int i = 0; i < n; i++) dist.push_back(INT_MAX);
    dist[start] = 0;

    MinHeap<HeapEntry> pq;
    pq.insert(HeapEntry(0, start));

    while (!pq.empty()) {
        HeapEntry cur = pq.peek();
        pq.extractMin();

        if (cur.weight > dist[cur.vertex]) continue; // stale entry, a better one already processed

        for (const Edge& e : g.neighbors(cur.vertex)) {
            int newDist = dist[cur.vertex] + e.weight;
            if (newDist < dist[e.to]) {
                dist[e.to] = newDist; // relax the edge
                pq.insert(HeapEntry(newDist, e.to));
            }
        }
    }
    return dist;
}
