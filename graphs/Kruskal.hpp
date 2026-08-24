#pragma once
#include <algorithm>
#include "GraphList.hpp"
#include "DSU.hpp"
#include "../arrays/MyVector.hpp"

// Kruskal's algorithm: builds a Minimum Spanning Tree by GREEDILY
// considering edges from lightest to heaviest, adding an edge only if
// it connects two vertices that aren't already connected (checked via
// DSU — that's exactly what union-find is for here).
//
// HOW IT WORKS: sort all edges by weight ascending (std::sort is fine
// here — it's a utility algorithm, not a data structure). Walk them in
// order; DSU.unite(u,v) returns false if u,v are already in the same
// component (adding this edge would create a cycle) — skip those,
// keep the rest. Stop once you've added V-1 edges.
//
// Complexity: O(E log E) for the sort, dominates the O(E * alpha(V))
// DSU operations. Works on graphs given as an edge list.
struct WeightedEdge {
    int u, v, weight;
    WeightedEdge(int a = 0, int b = 0, int w = 0) : u(a), v(b), weight(w) {}
    bool operator<(const WeightedEdge& other) const { return weight < other.weight; }
};

inline MyVector<WeightedEdge> kruskalMST(int numVertices, MyVector<WeightedEdge> edges) {
    // std::sort works fine directly on MyVector because it exposes
    // begin()/end() pointer iterators.
    std::sort(edges.begin(), edges.end());

    DSU dsu(numVertices);
    MyVector<WeightedEdge> mst;

    for (int i = 0; i < edges.size() && mst.size() < numVertices - 1; i++) {
        if (dsu.unite(edges[i].u, edges[i].v)) mst.push_back(edges[i]);
    }
    return mst;
}
