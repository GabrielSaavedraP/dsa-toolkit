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
        int cnt;
        int altura;
        Nodo(const T& d, Nodo* p = nullptr) :
            dato(d), izq(nullptr), der(nullptr), padre(p), cnt(1), altura(0) {}
    };

    Nodo* raiz;
    int modo;
    bool repetidos;

    Arbol(int modo = AVL, bool repetidos = false) : raiz(nullptr), modo(modo), repetidos(repetidos) {}

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
                if (igual(actual -> dato, valor)) {
                    if (repetidos) actual -> cnt++;
                    return actual;
                }
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

    Nodo* encontrar(const T& valor) {
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (valor < actual -> dato) actual = actual -> izq;
            else if (actual -> dato < valor) actual = actual -> der;
            else return actual;
        }
        return nullptr;
    }

    Nodo* minimo(Nodo* u) { while (u != nullptr && u -> izq != nullptr) u = u -> izq; return u; }
    Nodo* maximo(Nodo* u) { while (u != nullptr && u -> der != nullptr) u = u -> der; return u; }
    Nodo* minimo() { return minimo(raiz); }
    Nodo* maximo() { return maximo(raiz); }

    void eliminar(const T& valor) {
        Nodo* u = encontrar(valor);
        if (u == nullptr) return;
        if (u -> cnt > 1) { u -> cnt--; return; }
        if (u -> izq != nullptr && u -> der != nullptr) {
            Nodo* s = minimo(u -> der);
            u -> dato = s -> dato;
            u -> cnt = s -> cnt;
            u = s;
        }
        Nodo* hijo = (u -> izq != nullptr) ? u -> izq : u -> der;
        Nodo* padre = u -> padre;
        reemplazar_hijo(u, hijo);
        delete u;
        if (modo == AVL) rebalancear_avl_hacia_arriba(padre);
    }
};





int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    int K;
    cin >> n ;
    cin >> K;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    Arbol<int> A(AVL, true);
    long long total = 0;
    int mejor = 0;
    int izquierda = 0;

    for (int derecha = 0; derecha < n; derecha++) {
        A.insertar(a[derecha]);
        while ((long long)A.maximo() -> dato - A.minimo() -> dato > K) {
            A.eliminar(a[izquierda]);
            izquierda++;
        }
        total += derecha - izquierda + 1;
        if (derecha - izquierda + 1 > mejor) mejor = derecha - izquierda + 1;
    }
    cout << total << " " << mejor << "\n";
}