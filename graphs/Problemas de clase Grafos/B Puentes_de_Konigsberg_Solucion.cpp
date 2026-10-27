#include<bits/stdc++.h>
using namespace::std;

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    vector<int> deg(n, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        ++deg[u];
        ++deg[v];
    }
    int cnt_odd = 0;
    for (int i = 0; i < n; ++i) {
        if (deg[i] % 2 == 1) {
            ++cnt_odd;
        }
    }
    if (cnt_odd == 0) cout << "CIRCUITO" << '\n';
    else if (cnt_odd == 2) cout << "CAMINO" << '\n';
    else cout << "IMPOSIBLE" << '\n';
    return 0;
}