#pragma once
#include "GraphList.hpp"
#include "../heaps/MinHeap.hpp"
#include "../arrays/MyVector.hpp"

// Prim's algorithm: also builds a Minimum Spanning Tree, but grows a
// SINGLE tree outward from a start vertex, always adding the cheapest
// edge that connects the current tree to a new vertex outside it —
// as opposed to Kruskal, which considers edges globally regardless of
// connectivity to a single growing tree.
//
// HOW IT WORKS: a MinHeap of (weight, vertex) pairs. Pop the cheapest
// entry; if that vertex is already in the tree, skip it (a "lazy
// deletion" — cheaper than trying to update/remove stale entries in
// the heap). Otherwise, add it to the tree and push all its edges to
// still-outside neighbors.
//
// Complexity: O(E log E) — each edge can be pushed to the heap once,
// each push/pop is O(log E).
struct HeapEntry {
    int weight, vertex;
    HeapEntry(int w = 0, int v = 0) : weight(w), vertex(v) {}
    bool operator<(const HeapEntry& other) const { return weight < other.weight; }
};

inline int primMST(const GraphList& g, int start) {
    MyVector<bool> inTree;
    for (int i = 0; i < g.numVertices(); i++) inTree.push_back(false);

    MinHeap<HeapEntry> pq;
    pq.insert(HeapEntry(0, start));

    int totalWeight = 0;

    while (!pq.empty()) {
        HeapEntry cur = pq.peek();
        pq.extractMin();

        if (inTree[cur.vertex]) continue; // lazy deletion: skip stale entry
        inTree[cur.vertex] = true;
        totalWeight += cur.weight;

        for (const Edge& e : g.neighbors(cur.vertex)) {
            if (!inTree[e.to]) pq.insert(HeapEntry(e.weight, e.to));
        }
    }
    return totalWeight;
}
