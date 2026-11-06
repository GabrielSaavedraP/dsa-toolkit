#include<bits/stdc++.h>
using namespace::std;

const int N = 200000 + 1;

int T;
int n, m;
int in[N];
int out[N];
int vis[N]; // 0 -> sin visitar, 1 -> en proceso, 2 -> finalizado
int tree, back, forward_edges, cross;
vector<int> G[N];

bool is_ancestor(int u, int v) {
    return in[u] <= in[v] and out[v] <= out[u];
}

void DFS(int u) {
    vis[u] = 1;
    in[u] = ++T;
    for (int &v : G[u]) {
        if (vis[v] == 1) {
            ++back;
        }
        else if (vis[v] == 2) {
            if (in[u] < in[v]) ++forward_edges;
            else ++cross;
        }
        else {
            DFS(v);
            ++tree;
        }
    }
    out[u] = ++T;
    vis[u] = 2;
}

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        G[u].emplace_back(v);
    }
    for (int i = 1; i <= n; ++i) sort(G[i].begin(), G[i].end());
    for (int r = 1; r <= n; ++r) {
        if (vis[r]) continue;
        DFS(r);
    }
    cout << tree << " " << back << " " << forward_edges << " " << cross << "\n";
    return 0;
}