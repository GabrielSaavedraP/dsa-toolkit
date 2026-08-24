#pragma once
#include <utility> // std::swap — a utility function, not a container, so it's allowed

// DSU (Disjoint Set Union / Union-Find): tracks a partition of {0..n-1}
// into disjoint groups, supporting two operations extremely fast:
// "which group is x in?" (find) and "merge x's group with y's group"
// (union). This is the backbone of Kruskal's algorithm and of
// connected-components-style problems.
//
// HOW IT WORKS
// parent_[i] points toward i's "representative" (root of its tree).
// find(x) walks parent_[x] until it reaches a node that is its own
// parent (the root). PATH COMPRESSION: while walking up, we re-point
// every visited node directly to the root, so future finds are
// faster. UNION BY RANK: when merging two trees, attach the
// SHALLOWER tree under the deeper one's root, to avoid trees growing
// tall in the first place.
//
// Complexity: find / union   O(alpha(n)) amortized — alpha is the
// inverse Ackermann function, which is <= 4 for any n you'll ever see
// in practice. Effectively O(1).
class DSU {
private:
    int* parent_;
    int* rank_;
    int n_;

public:
    explicit DSU(int n) : n_(n) {
        parent_ = new int[n];
        rank_ = new int[n];
        for (int i = 0; i < n; i++) { parent_[i] = i; rank_[i] = 0; }
    }
    ~DSU() { delete[] parent_; delete[] rank_; }

    int find(int x) {
        if (parent_[x] != x) parent_[x] = find(parent_[x]); // path compression
        return parent_[x];
    }

    // Returns false if x and y were already in the same group (useful
    // for cycle detection in Kruskal: an edge that would union two
    // already-connected vertices creates a cycle, so skip it).
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;

        if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
};
