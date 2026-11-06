#include<vector>
#include<iostream>
#include<stack>
using namespace::std;

const int N = 500000 + 5;

int n, m;
int vis[N];
vector<int> G[N];
stack<int> current_path;
vector<pair<int,int>> edges;

bool has_cycle(int u) {
    vis[u] = 1;
    for (int &e : G[u]) {
        int v = edges[e].second;
        if (vis[v] == 2) continue;
        if (vis[v] == 1) {
            vector<int> res;
            int at = u;
            while (not current_path.empty() && edges[current_path.top()].first != v) {
                res.emplace_back(current_path.top());
                current_path.pop();
            }
            res.emplace_back(current_path.top());
            reverse(res.begin(), res.end());
            cout << res.size() + 1 << '\n';
            for (int &x : res) {
                cout << x << '\n';
            }
            return true;
        }
        current_path.emplace(e);
        if (has_cycle(v)) {
            return true;
        }
    }
}


int main() {
    cin.tie(0) -> sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
        G[u].emplace_back(i);
    }
    for (int i = 0; i < n; ++i) {
        if (vis[i]) continue;
        if (has_cycle(i)) {
            return 0;
        }
    }
    cout << -1 << '\n';
    return 0;
}