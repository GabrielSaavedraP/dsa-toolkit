// I. Tanque de Combustible
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

using Graph = vector<vector<pair<int, ll>>>; // {vecino, peso}

Graph read_graph(int n, int m, bool directed, int first_vertex) {
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll weight;
        cin >> u >> v >> weight;
        u -= first_vertex;
        v -= first_vertex;
        g[u].emplace_back(v, weight);
        if (!directed) g[v].emplace_back(u, weight);
    }
    return g;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, capacity;
    if (!(cin >> n >> m >> capacity)) return 0;
    vector<ll> price(n);
    for (ll& value : price) cin >> value;
    const Graph g = read_graph(n, m, false, 1);

    // id = ciudad * levels + combustible restante.
    // Todas las cantidades de combustible en la última ciudad llevan
    // a un mismo destino artificial con costo adicional cero.
    const int levels = capacity + 1;
    const int target = n * levels;
    const auto dist = dijkstra(target + 1, 0, [&](int state, ll cost, auto relax) {
        const int city = state / levels;
        const int fuel = state % levels;
        if (city == n - 1) {
            relax(target, cost);
            return;
        }
        if (fuel < capacity) relax(state + 1, cost + price[city]);
        for (const auto& edge : g[city]) {
            if (edge.second <= fuel) {
                const int remaining = fuel - static_cast<int>(edge.second);
                relax(edge.first * levels + remaining, cost);
            }
        }
    }, target);

    cout << (dist[target] == INF ? -1 : dist[target]) << '\n';
}
