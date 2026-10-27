#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    unordered_set<long long> edge_hashes;
    vector<pair<int, int>> edges;
    vector<int> in_degree(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
        edge_hashes.emplace(1ll * u * (n+1) + v);
        ++in_degree[v];
    }
    int res = 0;
    for (auto &e : edges) {
        int u, v;
        tie(u, v) = e;
        if (edge_hashes.find(1ll * v * (n+1) + u) != edge_hashes.end()) {
            ++res;
        }
    }
    int best = max_element(in_degree.begin() + 1, in_degree.end()) - in_degree.begin();
    cout << (res >> 1) << " " << best << " " << in_degree[best] << '\n';
}

