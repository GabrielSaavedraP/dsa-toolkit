#pragma once
#include "../arrays/MyVector.hpp"

// GraphList: adjacency list representation. For each vertex, we store
// a small list of (neighbor, weight) edges — no std::vector<vector<>>
// or std::unordered_map involved, just MyVector of MyVector<Edge>.
//
// HOW IT WORKS: adj_[u] is the list of all edges leaving u. This is
// the right representation for SPARSE graphs (few edges relative to
// V^2) — checking "what are u's neighbors" is O(degree(u)) instead of
// O(V) like it would be scanning a full matrix row.
//
// Complexity: addEdge O(1) amortized, iterate neighbors of u O(degree(u)),
// space O(V + E)  (vs O(V^2) for a matrix — the key trade-off vs GraphMatrix).
struct Edge {
    int to;
    int weight;
    Edge(int t = 0, int w = 1) : to(t), weight(w) {}
};

class GraphList {
private:
    MyVector<MyVector<Edge>> adj_;
    int numVertices_;
    bool directed_;

public:
    GraphList(int numVertices, bool directed = false)
        : numVertices_(numVertices), directed_(directed) {
        for (int i = 0; i < numVertices; i++) adj_.push_back(MyVector<Edge>());
    }

    void addEdge(int u, int v, int weight = 1) {
        adj_[u].push_back(Edge(v, weight));
        if (!directed_) adj_[v].push_back(Edge(u, weight));
    }

    const MyVector<Edge>& neighbors(int u) const { return adj_[u]; }
    int numVertices() const { return numVertices_; }
    bool isDirected() const { return directed_; }
};
