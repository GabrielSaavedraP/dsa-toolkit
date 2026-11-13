// D. Dos paradas obligatorias
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

vector<ll> dijkstra_graph(const Graph& g, int start, int target = -1) {
    return dijkstra(static_cast<int>(g.size()), start, [&](int u, ll cost, auto relax) {
        for (const auto& edge : g[u]) {
            relax(edge.first, cost + edge.second);
        }
    }, target);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, a, b;
    if (!(cin >> n >> m >> a >> b)) return 0;
    --a;
    --b;
    const Graph g = read_graph(n, m, false, 1);
    const auto from_a = dijkstra_graph(g, a);
    const auto from_b = dijkstra_graph(g, b);

    // La simetría de las distancias permite resolver ambos órdenes con dos corridas.
    ll answer = INF;
    if (from_a[b] != INF) {
        if (from_a[0] != INF && from_b[n - 1] != INF)
            answer = from_a[0] + from_a[b] + from_b[n - 1];
        if (from_b[0] != INF && from_a[n - 1] != INF)
            answer = min(answer, from_b[0] + from_a[b] + from_a[n - 1]);
    }
    cout << (answer == INF ? -1 : answer) << '\n';
}
