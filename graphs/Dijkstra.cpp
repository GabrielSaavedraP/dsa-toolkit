// Plantilla Dijkstra. Edita main para adaptar entrada, estados y respuesta.
// El main de ejemplo lee n m origen y aristas dirigidas u v peso (base 0).

/*
# Dijkstra · Semana 14

Copia este archivo completo. El `main` ya funciona para un grafo dirigido con vértices numerados desde cero. Cada solución A–I también es un programa C++14 independiente que puedes compilar o copiar por separado.

## Qué editar en main

1. **Entrada.** Lee el grafo y los datos adicionales. Para aristas `u v peso`, usa `read_graph(n, m, directed, first_vertex)`. Indica si las aristas son dirigidas y si la entrada empieza en 0 o 1. La función guarda los vértices desde 0. Convierte el origen y el destino a esa misma base.
2. **Estado.** Empieza con un estado por vértice. Si dos llegadas al mismo vértice tienen distintas opciones futuras, añade al estado el dato que hace la diferencia. Por ejemplo, `2 * ciudad + cupón_usado` permite guardar una distancia con cupón y otra sin cupón.
3. **Transiciones.** Para un grafo ponderado normal, llama a `dijkstra_graph(g, origen)`. Para reglas especiales, escribe la lambda `expand` en `main` y llama a `dijkstra(cantidad_estados, origen, expand)`. Cada llamada `relax(v, total)` recibe el **costo acumulado** hasta `v`.
4. **Respuesta.** Consulta la distancia al destino o combina varias distancias. `INF` indica que un estado es inalcanzable tras una corrida completa. Si solo necesitas un destino, pásalo como último argumento. En ese caso consulta únicamente su distancia.

### Ejemplo con una regla nueva

Costo mínimo de 0 a `n - 1` con un cupón que permite pagar la mitad de una arista (división entera). El cupón es opcional.

```cpp
const auto dist = dijkstra(2 * n, 0, [&](int state, ll cost, auto relax) {
    const int u = state / 2;
    const int used = state % 2;
    for (const auto& edge : g[u]) {
        const int v = edge.first;
        const ll weight = edge.second;
        relax(2 * v + used, cost + weight);
        if (used == 0) relax(2 * v + 1, cost + weight / 2);
    }
});
const ll answer = min(dist[2 * (n - 1)], dist[2 * (n - 1) + 1]);
cout << (answer == INF ? -1 : answer) << '\n';
```

Para varias fuentes, puedes crear un estado inicial artificial con transiciones de costo cero a todas ellas. Si necesitas terminar en cualquiera de varios estados, usa un destino artificial con transiciones de costo cero desde los estados válidos, como en I.

## Qué solución copiar

| Problema | Patrón |
| --- | --- |
| A. Single-Source Shortest Path | Lista de adyacencia dirigida y una corrida desde el origen. |
| B. Red Completa | Selección por escaneo en un grafo completo. Compara pares `(tiempo, tramos)` y calcula los pesos al vuelo. |
| C. Punto de Encuentro | Dos corridas y un mínimo sobre los tiempos de encuentro. |
| D. Dos paradas obligatorias | Dos corridas desde las paradas. Usa la simetría del grafo para sumar ambos órdenes. |
| E. Monopatín Par | Estado `(ciudad, paridad)`. |
| F. Semáforos | La hora de llegada determina la espera antes de cada salida. |
| G. Calculadora Cara | El número de la pantalla es el estado. Genera los vecinos al vuelo. |
| H. Segunda Ruta | Dos corridas y un escaneo de aristas para encontrar el segundo costo distinto de una ruta que permite repeticiones. |
| I. Tanque de Combustible | Estado `(ciudad, combustible)` y destino artificial. |

## Cuándo cambiar de variante

- La plantilla de heap guarda **una mejor distancia por estado**. Las transiciones deben sumar costos no negativos. Si el costo depende de la llegada, llegar antes al mismo estado debe permitir llegar al vecino igual de pronto o antes, como en F.
- Para un grafo completo, B muestra el escaneo en `O(n²)` con memoria `O(n)`. Para desempates de dos criterios, usa un par como en B.
- Si piden más de un costo por estado o restricciones sobre qué vértices se pueden repetir, revisa qué información hay que guardar. La técnica de H usa que se permiten recorridos con repeticiones.
- Usa `long long` para costos. Verifica que los totales caben por debajo de `INF` y comprueba que los tramos son alcanzables antes de sumarlos. Al multiplicar estados (por ejemplo, ciudades por niveles de combustible), verifica que el número de estados cabe en tiempo y memoria.

Para compilar el ejemplo desde la carpeta graphs:

```sh
c++ -std=c++14 -O2 Dijkstra.cpp -o /tmp/dijkstra-a
/tmp/dijkstra-a
```

[Enunciados de la Semana 14 en Codeforces](https://codeforces.com/group/nylekuuZuw/contest/731505/problems)
*/

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

    int n, m, source;
    if (!(cin >> n >> m >> source)) return 0;
    const Graph g = read_graph(n, m, true, 0);
    const auto dist = dijkstra_graph(g, source);

    for (ll cost : dist) {
        if (cost == INF) cout << "INF\n";
        else cout << cost << '\n';
    }
}
