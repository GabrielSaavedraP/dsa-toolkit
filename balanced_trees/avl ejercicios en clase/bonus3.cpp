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

    Nodo* encontrar(const T& valor) {
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (valor < actual -> dato) actual = actual -> izq;
            else if (actual -> dato < valor) actual = actual -> der;
            else return actual;
        }
        return nullptr;
    }

    bool buscar(const T& valor) { return encontrar(valor) != nullptr; }

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

    void recalcular_todo() {
        vector<Nodo*> orden = lista_de_nodos();
        for (int i = (int)orden.size() - 1; i >= 0; --i) actualizar(orden[i]);
    }

    void construir(const vector<T>& claves, const vector<int>& L, const vector<int>& R, vector<Nodo*>& nodos) {
        int n = (int)claves.size() - 1;
        nodos.assign(n + 1, nullptr);
        for (int i = 1; i <= n; ++i) nodos[i] = new Nodo(claves[i]);
        for (int i = 1; i <= n; ++i) {
            if (L[i] != 0) { nodos[i] -> izq = nodos[L[i]]; nodos[L[i]] -> padre = nodos[i]; }
            if (R[i] != 0) { nodos[i] -> der = nodos[R[i]]; nodos[R[i]] -> padre = nodos[i]; }
        }
        raiz = nodos[1];
        recalcular_todo();
    }

    bool es_bst() {
        vector<Nodo*> pila;
        Nodo* u = raiz;
        Nodo* previo = nullptr;
        while (u != nullptr || !pila.empty()) {
            while (u != nullptr) { pila.push_back(u); u = u -> izq; }
            u = pila.back(); pila.pop_back();
            if (previo != nullptr && !(previo -> dato < u -> dato)) return false;
            previo = u;
            u = u -> der;
        }
        return true;
    }

    bool es_avl() {
        if (!es_bst()) return false;
        vector<Nodo*> nodos = lista_de_nodos();
        for (Nodo* u : nodos) {
            int fb = factor_balance(u);
            if (fb < -1 || fb > 1) return false;
        }
        return true;
    }

    void inorder(Nodo* u) { if (u == nullptr) return; inorder(u -> izq); cout << u -> dato << " "; inorder(u -> der); }
    void imprimir_inorder() { inorder(raiz); cout << "\n"; }
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
    bool existe(const K& k) { return A.buscar({k, V()}); }
};

int main() {
    int n;
    cin >> n;
    vector<int> preOrder(n + 1), postOrder(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> preOrder[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> postOrder[i];
    }

    Arbol<int> A(SIN_BALANCE);

    vector<Arbol<int>::Nodo*> pila;
    int siguientePost = 1;
    bool posible = true;
    for (int i = 1; i <= n && posible; i++) {
        Arbol<int>::Nodo* nuevo = new Arbol<int>::Nodo(preOrder[i]);

        if (pila.empty()) {
            if (i > 1) posible = false;

            else A.raiz = nuevo;
        }


        else {
            Arbol<int>::Nodo* tope = pila.back();
            if (tope -> izq == nullptr) tope -> izq = nuevo;
            else if (tope -> der == nullptr) tope -> der = nuevo;
            else posible = false;
            nuevo -> padre = tope;
        }

        pila.push_back(nuevo);




        while (!pila.empty() && pila.back() -> dato == postOrder[siguientePost]) {
            Arbol<int>::Nodo* tope = pila.back();
            if ((tope -> izq == nullptr) != (tope -> der == nullptr)) posible = false;
            pila.pop_back();
            siguientePost++;
        }
    }
    if (!pila.empty() || siguientePost != n + 1) posible = false;
    if (!posible) cout << "IMPOSIBLE\n";
    else A.imprimir_inorder();
}