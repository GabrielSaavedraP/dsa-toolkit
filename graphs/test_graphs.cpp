#include <iostream>
#include "GraphList.hpp"
#include "GraphMatrix.hpp"
#include "BFS.hpp"
#include "DFS.hpp"
#include "DSU.hpp"
#include "Kruskal.hpp"
#include "Prim.hpp"
#include "Dijkstra.hpp"
using namespace std;

void printVec(const MyVector<int>& v, const char* label) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << "\n";
}

int main() {
    // Undirected unweighted graph for BFS/DFS: 0-1-2-3-4 with extra edges
    GraphList g(6, false);
    g.addEdge(0, 1); g.addEdge(0, 2); g.addEdge(1, 3);
    g.addEdge(2, 3); g.addEdge(3, 4); g.addEdge(4, 5);

    printVec(bfs(g, 0), "BFS from 0");
    printVec(dfsRecursive(g, 0), "DFS recursive from 0");
    printVec(dfsIterative(g, 0), "DFS iterative from 0");

    // DSU sanity check
    DSU dsu(5);
    dsu.unite(0, 1);
    dsu.unite(1, 2);
    cout << "DSU connected(0,2)=" << dsu.connected(0, 2) << " connected(0,3)=" << dsu.connected(0, 3) << "\n";

    // Weighted graph for MST (Kruskal + Prim) and Dijkstra
    GraphList wg(5, false);
    wg.addEdge(0, 1, 2);
    wg.addEdge(0, 3, 6);
    wg.addEdge(1, 2, 3);
    wg.addEdge(1, 3, 8);
    wg.addEdge(1, 4, 5);
    wg.addEdge(2, 4, 7);
    wg.addEdge(3, 4, 9);

    MyVector<WeightedEdge> edges;
    edges.push_back(WeightedEdge(0, 1, 2));
    edges.push_back(WeightedEdge(0, 3, 6));
    edges.push_back(WeightedEdge(1, 2, 3));
    edges.push_back(WeightedEdge(1, 3, 8));
    edges.push_back(WeightedEdge(1, 4, 5));
    edges.push_back(WeightedEdge(2, 4, 7));
    edges.push_back(WeightedEdge(3, 4, 9));

    MyVector<WeightedEdge> mst = kruskalMST(5, edges);
    int kruskalTotal = 0;
    cout << "Kruskal MST edges: ";
    for (const auto& e : mst) { cout << "(" << e.u << "-" << e.v << ":" << e.weight << ") "; kruskalTotal += e.weight; }
    cout << "\nKruskal total weight=" << kruskalTotal << "\n";

    int primTotal = primMST(wg, 0);
    cout << "Prim total weight=" << primTotal << " (must match Kruskal)\n";

    MyVector<int> dist = dijkstra(wg, 0);
    printVec(dist, "Dijkstra distances from 0");

    // GraphMatrix sanity check
    GraphMatrix gm(4, true);
    gm.addEdge(0, 1, 5);
    cout << "GraphMatrix hasEdge(0,1)=" << gm.hasEdge(0, 1) << " weight=" << gm.weight(0, 1)
         << " hasEdge(1,0)=" << gm.hasEdge(1, 0) << " (directed, must be 0)\n";

    return 0;
}
