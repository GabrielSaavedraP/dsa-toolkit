// ============================================================================
//  NO OLVIDAR: Todos los nodos se indexan desde 0. Distancia -1 = inalcanzable.
//
//  Contenido:
//    1. bfs()            -> BFS generico sobre un grafo "implicito" (nodos 0..N-1)
//    2. bfs01()          -> BFS 0-1 (aristas de costo 0 o 1)
//    3. Grafos           -> read_graph(), bfs_graph(), build_path()
//    4. Grillas          -> read_grid(), find_cell(), bfs_grid(), cell_id()
//    5. Bipartito        -> bipartite_components()
//    6. Estados          -> bfs_states() (estado = string, pair, tuple, ...)
// ============================================================================
#include <iostream>
#include <vector>
#include <map>
using namespace std;
using ll = long long;
const int INF = 1e9;

// ----------------------------------------------------------------------------
// 1. BFS GENERICO (grafo implicito)
//    N         : cantidad de nodos (nodos 0..N-1)
//    sources   : nodos de inicio (uno solo o varios -> BFS multi-fuente)
//    neighbors : funcion  neighbors(u, push)  que llama push(v) por cada
//                vecino v de u. Ejemplo:
//                  [&](int u, auto push){ for (int v : g[u]) push(v); }
//    target    : (opcional) si lo das, se detiene al llegar a el
//    parent    : (opcional) puntero a un vector para reconstruir el camino
//    Retorna   : dist[v] = menor numero de pasos desde las fuentes, o -1
// ----------------------------------------------------------------------------
template <class NeighborFn>
vector<int> bfs(int N, const vector<int>& sources, NeighborFn neighbors,
                int target = -1, vector<int>* parent = nullptr) {
    vector<int> dist(N, -1);
    if (parent) parent->assign(N, -1);
    queue<int> q;
    for (int s : sources)
        if (dist[s] == -1) { dist[s] = 0; q.push(s); }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == target) break;
        neighbors(u, [&](int v) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                if (parent) (*parent)[v] = u;
                q.push(v);
            }
        });
    }
    return dist;
}

// ----------------------------------------------------------------------------
// 2. BFS 0-1 (cada arista cuesta 0 o 1)
//    Igual que bfs() pero push recibe el costo:  push(v, w)  con w en {0,1}.
//    Ejemplo: [&](int u, auto push){ for (auto [v,w] : g[u]) push(v, w); }
// ----------------------------------------------------------------------------
template <class NeighborFn>
vector<int> bfs01(int N, const vector<int>& sources, NeighborFn neighbors,
                  int target = -1) {
    vector<int> dist(N, INF);
    deque<int> dq;
    for (int s : sources) { dist[s] = 0; dq.push_front(s); }
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        if (u == target) break;
        neighbors(u, [&](int v, int w) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v); else dq.push_back(v);
            }
        });
    }
    for (int& d : dist) if (d == INF) d = -1;
    return dist;
}

// ----------------------------------------------------------------------------
// 3. GRAFOS (lista de adyacencia)
// ----------------------------------------------------------------------------
using Graph = vector<vector<int>>;

// Lee m aristas "u v" (1-indexadas en la entrada) y las guarda 0-indexadas.
Graph read_graph(int n, int m, bool directed = false) {
    Graph g(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v; u--; v--;
        g[u].push_back(v);
        if (!directed) g[v].push_back(u);
    }
    return g;
}

// BFS desde uno o varios nodos fuente sobre un Graph.
vector<int> bfs_graph(const Graph& g, const vector<int>& sources,
                      int target = -1, vector<int>* parent = nullptr) {
    return bfs((int)g.size(), sources,
               [&](int u, auto push) { for (int v : g[u]) push(v); },
               target, parent);
}

// Reconstruye el camino source -> target usando el vector parent de bfs().
// Retorna vacio si target es inalcanzable.
vector<int> build_path(const vector<int>& parent, int target) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// ----------------------------------------------------------------------------
// 4. GRILLAS
//    Direcciones: indice 0=arriba, 1=abajo, 2=izquierda, 3=derecha
//    Celda (r,c) <-> id = r*m + c   (usa cell_id para convertir)
// ----------------------------------------------------------------------------
using Grid = vector<string>;
const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

Grid read_grid(int n) {
    Grid grid(n);
    for (auto& row : grid) cin >> row;
    return grid;
}

inline int cell_id(int r, int c, int m) { return r * m + c; }

inline bool inside(int r, int c, int n, int m) {
    return r >= 0 && r < n && c >= 0 && c < m;
}

// Busca la primera celda con el caracter ch. Retorna {-1,-1} si no existe.
pair<int,int> find_cell(const Grid& grid, char ch) {
    for (int r = 0; r < (int)grid.size(); r++)
        for (int c = 0; c < (int)grid[r].size(); c++)
            if (grid[r][c] == ch) return {r, c};
    return {-1, -1};
}

// BFS en grilla desde una o varias celdas {r,c}. Las celdas con el caracter
// 'wall' son intransitables. Retorna dist plano: dist[cell_id(r,c,m)].
vector<int> bfs_grid(const Grid& grid, const vector<pair<int,int>>& sources,
                     char wall = '#') {
    int n = grid.size(), m = grid[0].size();
    vector<int> src;
    for (auto [r, c] : sources) src.push_back(cell_id(r, c, m));
    return bfs(n * m, src, [&](int u, auto push) {
        int r = u / m, c = u % m;
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (inside(nr, nc, n, m) && grid[nr][nc] != wall)
                push(cell_id(nr, nc, m));
        }
    });
}

// ----------------------------------------------------------------------------
// 5. BIPARTITO / COMPONENTES CONEXAS
//    Colorea con 0/1 cada componente usando BFS.
//    color[v] = 0 o 1.  components = numero de componentes conexas.
//    Retorna false si el grafo NO es bipartito (hay ciclo impar).
// ----------------------------------------------------------------------------
bool bipartite_components(const Graph& g, vector<int>& color, int& components) {
    int n = g.size();
    color.assign(n, -1);
    components = 0;
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) continue;
        components++;
        color[s] = 0;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (color[v] == -1) { color[v] = color[u] ^ 1; q.push(v); }
                else if (color[v] == color[u]) return false;
            }
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
// 6. BFS SOBRE ESTADOS ARBITRARIOS (string, pair, tuple, vector...)
//    Usalo cuando los estados no se codifican facil como un entero.
//    start     : estado inicial
//    neighbors : neighbors(state, push) con push(nuevo_estado)
//    target    : (opcional) puntero al estado objetivo para cortar antes
//    Retorna   : map estado -> distancia (si un estado no esta, es inalcanzable)
//    Ejemplo:  auto d = bfs_states(string("123405"), [&](const string& s, auto push){...});
//              int ans = d.count(goal) ? d[goal] : -1;
// ----------------------------------------------------------------------------
template <class State, class NeighborFn>
map<State, int> bfs_states(const State& start, NeighborFn neighbors,
                           const State* target = nullptr) {
    map<State, int> dist;
    queue<State> q;
    dist[start] = 0; q.push(start);
    while (!q.empty()) {
        State u = q.front(); q.pop();
        if (target && u == *target) break;
        int du = dist[u];
        neighbors(u, [&](const State& v) {
            if (!dist.count(v)) { dist[v] = du + 1; q.push(v); }
        });
    }
    return dist;
}



int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string start;
    for (int i = 0; i < 6; i++) { int x; cin >> x; start += char('0' + x); }
    string goal = "123450";
    auto neighbors = [&](const string& s, auto push) {
        int p = s.find('0');
        int r = p / 3, c = p % 3;
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (inside(nr, nc, 2, 3)) {
                string t = s;
                swap(t[p], t[nr * 3 + nc]);
                push(t);
            }
        }
    };
    auto dist = bfs_states(start, neighbors, &goal);
    cout << (dist.count(goal) ? dist[goal] : -1) << "\n";
}
