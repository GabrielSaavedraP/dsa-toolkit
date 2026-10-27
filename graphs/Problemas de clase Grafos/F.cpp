#include <iostream>
#include <map>
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

    vector<bool> used(n+1, false);
    while (q--) {
        int u, v;
        cin >> u >> v;
        if (G[u].size() > G[v].size()) swap(u, v);
        for (int &x : G[u]) {
            used[x] = true;
        }
        int res = 0;
        for (int &x : G[v]) {
            if (used[x]) ++res;
        }
        for (int &x : G[u]) {
            used[x] = false;
        }
        cout << res << '\n';
    }
    return 0;
}

