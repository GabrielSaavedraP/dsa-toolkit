#include <iostream>
#include <vector>
using namespace std;


// ==========================================================================================
// funciones sacadas de mi implementación central de BBST, disponible en mi repo: https://github.com/GabrielSaavedraP/dsa-toolkit/tree/main
// ==========================================================================================
enum Modo { SIN_BALANCE, AVL };

template<typename T>
struct Arbol {
    struct Nodo {
        T dato;
        Nodo* izq;
        Nodo* der;
        Nodo* padre;
        int altura;
        Nodo(const T& d, Nodo* p = nullptr) :
            dato(d), izq(nullptr), der(nullptr), padre(p), altura(0) {}
    };

    Nodo* raiz;
    int modo;

    Arbol(int modo = AVL) : raiz(nullptr), modo(modo) {}

    bool igual(const T& a, const T& b) { return !(a < b) && !(b < a); }
    int mayor(int a, int b) { return a > b ? a : b; }
    int altura(Nodo* u) { return u == nullptr ? -1 : u -> altura; }
    int factor_balance(Nodo* u) { return u == nullptr ? 0 : altura(u -> izq) - altura(u -> der); }

    void actualizar(Nodo* u) {
        u -> altura = 1 + mayor(altura(u -> izq), altura(u -> der));
    }

    void reemplazar_hijo(Nodo* u, Nodo* v) {
        if (u -> padre == nullptr) raiz = v;
        else if (u -> padre -> izq == u) u -> padre -> izq = v;
        else u -> padre -> der = v;
        if (v != nullptr) v -> padre = u -> padre;
    }

    Nodo* rotar_derecha(Nodo* y) {
        Nodo* x = y -> izq;
        Nodo* B = x -> der;
        reemplazar_hijo(y, x);
        x -> der = y;  y -> padre = x;
        y -> izq = B;  if (B != nullptr) B -> padre = y;
        actualizar(y); actualizar(x);
        return x;
    }

    Nodo* rotar_izquierda(Nodo* x) {
        Nodo* y = x -> der;
        Nodo* B = y -> izq;
        reemplazar_hijo(x, y);
        y -> izq = x;  x -> padre = y;
        x -> der = B;  if (B != nullptr) B -> padre = x;
        actualizar(x); actualizar(y);
        return y;
    }

    Nodo* rebalancear_avl(Nodo* u) {
        actualizar(u);
        int fb = factor_balance(u);
        if (fb > 1) {
            if (factor_balance(u -> izq) < 0) rotar_izquierda(u -> izq);
            return rotar_derecha(u);
        }
        if (fb < -1) {
            if (factor_balance(u -> der) > 0) rotar_derecha(u -> der);
            return rotar_izquierda(u);
        }
        return u;
    }

    void rebalancear_avl_hacia_arriba(Nodo* u) {
        while (u != nullptr) {
            u = rebalancear_avl(u);
            u = u -> padre;
        }
    }

    Nodo* insertar(const T& valor) {
        Nodo* nuevo = nullptr;
        if (raiz == nullptr) {
            nuevo = raiz = new Nodo(valor);
        }
        else {
            Nodo* actual = raiz;
            while (nuevo == nullptr) {
                if (igual(actual -> dato, valor)) return actual;
                if (actual -> dato < valor) {
                    if (actual -> der != nullptr) actual = actual -> der;
                    else nuevo = actual -> der = new Nodo(valor, actual);
                }
                else {
                    if (actual -> izq != nullptr) actual = actual -> izq;
                    else nuevo = actual -> izq = new Nodo(valor, actual);
                }
            }
        }
        if (modo == AVL) rebalancear_avl_hacia_arriba(nuevo -> padre);
        return nuevo;
    }

    vector<Nodo*> lista_de_nodos() {
        vector<Nodo*> orden, pila;
        if (raiz != nullptr) pila.push_back(raiz);
        while (!pila.empty()) {
            Nodo* u = pila.back(); pila.pop_back();
            orden.push_back(u);
            if (u -> izq != nullptr) pila.push_back(u -> izq);
            if (u -> der != nullptr) pila.push_back(u -> der);
        }
        return orden;
    }

    void liberar() {
        vector<Nodo*> orden = lista_de_nodos();
        for (int i = 0; i < (int)orden.size(); ++i) delete orden[i];
        raiz = nullptr;
    }

    vector<T> preorden() {
        vector<T> res; vector<Nodo*> pila;
        if (raiz != nullptr) pila.push_back(raiz);
        while (!pila.empty()) {
            Nodo* u = pila.back(); pila.pop_back();
            res.push_back(u -> dato);
            if (u -> der != nullptr) pila.push_back(u -> der);
            if (u -> izq != nullptr) pila.push_back(u -> izq);
        }
        return res;
    }
};

template<typename K, typename V>
struct Mapa {
    struct Par {
        K clave; V valor;
        bool operator<(const Par& o) const { return clave < o.clave; }
    };
    Arbol<Par> A;
    Mapa(int modo = AVL) : A(modo) {}

    V& operator[](const K& k) { return A.insertar({k, V()}) -> dato.valor; }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    Mapa<vector<int>, int> grupos;

    vector<int> respuesta(m);
    int k = 0;
    for (int i = 0; i < m; i++) {
        Arbol<int> A(SIN_BALANCE);
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            A.insertar(x);
        }
        int& g = grupos[A.preorden()];
        if (g == 0) g = ++k;
        respuesta[i] = g;
    }
    cout << k << "\n";
    for (int i = 0; i < m; i++) cout << respuesta[i] << " ";
    cout << "\n";
}