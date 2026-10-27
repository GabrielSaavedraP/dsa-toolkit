#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<int>> G(n+1, vector<int>());
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    for (int i = 1; i <= n; i++) {
        sort(G[i].begin(), G[i].end());
    }
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int u, k;
            cin >> u >> k;
            cout << (G[u].size() >= k ? G[u][k-1] : -1) << '\n';
        }
        else {
            int u, x;
            cin >> u >> x;
            cout << upper_bound(G[u].begin(), G[u].end(), x) - G[u].begin() << '\n';
        }
    }
}

