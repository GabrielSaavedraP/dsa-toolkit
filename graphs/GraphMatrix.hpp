#pragma once

// GraphMatrix: adjacency matrix representation using a raw 2D int
// array (not std::vector<vector<int>>).
//
// HOW IT WORKS: matrix_[u][v] holds the edge weight from u to v (or a
// sentinel like 0/INF for "no edge", depending on convention — here 0
// means no edge). This makes "is there an edge u->v" O(1), at the
// cost of O(V^2) space regardless of how many edges actually exist —
// the right choice for DENSE graphs, the wrong one for sparse ones.
//
// Complexity: addEdge O(1), hasEdge O(1), iterate all neighbors of u O(V).
class GraphMatrix {
private:
    int** matrix_;
    int numVertices_;
    bool directed_;

public:
    GraphMatrix(int numVertices, bool directed = false)
        : numVertices_(numVertices), directed_(directed) {
        matrix_ = new int*[numVertices];
        for (int i = 0; i < numVertices; i++) {
            matrix_[i] = new int[numVertices];
            for (int j = 0; j < numVertices; j++) matrix_[i][j] = 0;
        }
    }

    ~GraphMatrix() {
        for (int i = 0; i < numVertices_; i++) delete[] matrix_[i];
        delete[] matrix_;
    }

    void addEdge(int u, int v, int weight = 1) {
        matrix_[u][v] = weight;
        if (!directed_) matrix_[v][u] = weight;
    }

    bool hasEdge(int u, int v) const { return matrix_[u][v] != 0; }
    int weight(int u, int v) const { return matrix_[u][v]; }
    int numVertices() const { return numVertices_; }
};
