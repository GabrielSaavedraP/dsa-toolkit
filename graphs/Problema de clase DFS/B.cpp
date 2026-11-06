#include<bits/stdc++.h>
using namespace::std;

const int N = 200000 + 5;

int T;
int in[N];
int out[N];
vector<int> G[N];

void add_edge(int u, int v) {
    G[u].emplace_back(v);
    G[v].emplace_back(u);
}

void DFS(int u, int p = -1) {
    in[u] = ++T;
    for (int &v : G[u]) {
        if (v == p) continue;
        DFS(v, u);
    }
    out[u] = ++T;
}

bool is_ancestor(int u, int v) {
    return in[u] <= in[v] and out[v] <= out[u];
}

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        add_edge(u, v);
    }
    DFS(1); // DFS desde la raiz
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << (is_ancestor(u, v) ? "SI" : "NO") << '\n';
    }
    return 0;
}