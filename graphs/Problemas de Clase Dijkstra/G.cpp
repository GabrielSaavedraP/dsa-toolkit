// G. Calculadora Cara
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

using ll = long long;
constexpr ll INF = 1LL << 62;

// Estados numerados de 0 a states-1. El estado inicial tiene costo 0.
// expand(u, cost, relax) enumera las transiciones válidas.
// relax(v, total) recibe el costo TOTAL hasta v.
// Cada transición debe tener costo no negativo y preservar el orden:
// llegar antes o con menor costo al mismo estado debe ser igual o mejor.
// Con target, solo la distancia a ese destino se garantiza al terminar.
template <class Expand>
vector<ll> dijkstra(int states, int start, Expand expand, int target = -1) {
    using Entry = pair<ll, int>; // {costo acumulado, estado}
    priority_queue<Entry, vector<Entry>, greater<Entry>> pending;
    vector<ll> dist(states, INF);
    dist[start] = 0;
    pending.emplace(0, start);

    while (!pending.empty()) {
        const ll cost = pending.top().first;
        const int u = pending.top().second;
        pending.pop();

        if (cost != dist[u]) continue; // La cola aún contiene una distancia anterior.
        if (u == target) break;

        expand(u, cost, [&](int v, ll total) {
            if (total < dist[v]) {
                dist[v] = total;
                pending.emplace(total, v);
            }
        });
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll plus_cost, minus_cost, double_cost;
    if (!(cin >> n >> plus_cost >> minus_cost >> double_cost)) return 0;
    const int limit = 2 * n;

    // El número mostrado es el estado. El 0 queda sin usar.
    const auto dist = dijkstra(limit + 1, 1, [&](int x, ll cost, auto relax) {
        if (x < limit) relax(x + 1, cost + plus_cost);
        if (x > 1) relax(x - 1, cost + minus_cost);
        if (x <= limit / 2) relax(2 * x, cost + double_cost);
    }, n);

    cout << dist[n] << '\n';
}
