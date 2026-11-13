// B. Red Completa
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

using ll = long long;
constexpr ll INF = 1LL << 62;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll p, q, mod;
    if (!(cin >> n >> p >> q >> mod)) return 0;

    // pair compara primero tiempo y luego número de carreteras.
    using Cost = pair<ll, int>;
    vector<Cost> dist(n + 1, {INF, 0});
    vector<bool> finalized(n + 1, false);
    dist[1] = {0, 0};

    // Grafo completo. Calcular los pesos al vuelo cuesta O(n²) y usa O(n) memoria.
    for (int step = 0; step < n; ++step) {
        int u = -1;
        for (int v = 1; v <= n; ++v) {
            if (!finalized[v] && (u == -1 || dist[v] < dist[u])) u = v;
        }
        if (u == n) break;
        finalized[u] = true;

        for (int v = 1; v <= n; ++v) {
            if (finalized[v]) continue;
            ll weight = ((u + v) * p + u * v * q) % mod + 1;
            Cost candidate = {dist[u].first + weight, dist[u].second + 1};
            if (candidate < dist[v]) dist[v] = candidate;
        }
    }
    cout << dist[n].first << ' ' << dist[n].second << '\n';
}
