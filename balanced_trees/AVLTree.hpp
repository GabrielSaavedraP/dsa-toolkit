#include <iostream>
#include <vector>
using namespace std;

// ============================================================================
//  my_avl_map<TipoClave, TipoValor>  -- mapa ORDENADO sobre un AVL
//  Funciona con cualquier TipoClave que tenga operator<  (int, long long,
//  char, string, pair-like structs...). Para print() la clave y el valor
//  necesitan operator<<.   Si usas string en la clave: #include <string>
//
//  Diferencia con tu my_map: aqui las claves viven ORDENADAS, asi que ademas
//  de [] / erase / has_key tienes preguntas de orden (menor, mayor, k-esimo...).
//  Todo es O(log n).
// ============================================================================
//
// ============================  CHEAT SHEET  =================================
//
//  --- LAS 3 OPERACIONES DE SIEMPRE (identicas a tu my_map) ---
//  m[clave] = 1;     // "solo quiero saber si esta clave existe"
//  m[clave] = i;     // "necesito recordar la posicion de esta clave"
//  m[clave]++;       // "necesito contar cuantas veces aparece esta clave"
//
//  --- BASICAS ---
//  m.has_key(clave)  // true/false
//  m.erase(clave)    // borra si existe
//  m.size()  m.empty()  m.clear()
//
//  --- PREGUNTAS DE ORDEN (devuelven Nodo*, nullptr si no existe) ---
//  m.minimo()                // la clave mas chica
//  m.maximo()                // la clave mas grande
//  m.mayor_o_igual(x)        // primera clave >= x   (lower_bound)
//  m.mayor(x)                // primera clave >  x   (upper_bound)
//  m.menor_o_igual(x)        // ultima  clave <= x
//  m.menor(x)                // ultima  clave <  x
//  m.siguiente(nodo)         // la clave que sigue
//  m.anterior(nodo)          // la clave previa
//
//  Uso:  auto p = m.mayor_o_igual(x);
//        if (p != nullptr) cout << p->clave << " " << p->valor;
//
//  --- ESTADISTICAS DE ORDEN ---
//  m.contar_menores(x)       // cuantas claves son < x   (rank)
//  m.kesimo(k)               // Nodo* de la k-esima clave, k desde 0
//
//  --- RECORRER EN ORDEN (de menor a mayor) ---
//  for (auto p = m.minimo(); p != nullptr; p = m.siguiente(p))
//      cout << p->clave << " " << p->valor << "\n";
//
//  --- DEBUG ---
//  m.print();        // inorder: clave --> valor
//  m.print_arbol();  // el arbol "de lado" (raiz a la izquierda)
//  m.es_avl();       // verifica la propiedad AVL
// ============================================================================

template <typename TipoClave, typename TipoValor>
struct my_avl_map {

    // ========================================================================
    // ============== NO EDITAR: NODO ==========================================
    // ========================================================================
    struct Nodo {
        TipoClave clave;
        TipoValor valor;
        Nodo* izq;
        Nodo* der;
        Nodo* padre;
        int altura;   // h(nulo) = -1, h(hoja) = 0
        int tam;      // cantidad de nodos en el subarbol (para kesimo / rank)

        Nodo(const TipoClave& k, Nodo* p = nullptr)
            : clave(k), valor(TipoValor()), izq(nullptr), der(nullptr),
              padre(p), altura(0), tam(1) {}
    };

    Nodo* raiz;

    my_avl_map() : raiz(nullptr) {}
    ~my_avl_map() { destruir(raiz); }

    // Evita copias accidentales (doble free). Pasa el mapa por referencia (&).
    my_avl_map(const my_avl_map&) = delete;
    my_avl_map& operator=(const my_avl_map&) = delete;

    // ========================================================================
    // ============== NO EDITAR: ALTURA, TAMANO Y BALANCE =====================
    // ========================================================================
    int _mayor(int a, int b) const { return a > b ? a : b; }

    int _altura(Nodo* u) const { return u == nullptr ? -1 : u->altura; }

    int _tam(Nodo* u) const { return u == nullptr ? 0 : u->tam; }

    // Recalcula altura y tamano de u a partir de sus hijos
    void _actualizar(Nodo* u) {
        u->altura = 1 + _mayor(_altura(u->izq), _altura(u->der));
        u->tam = 1 + _tam(u->izq) + _tam(u->der);
    }

    // FB(u) = h(izq) - h(der)
    int _factor_balance(Nodo* u) const {
        return u == nullptr ? 0 : _altura(u->izq) - _altura(u->der);
    }

    // ========================================================================
    // ============== NO EDITAR: ROTACIONES ===================================
    // ========================================================================

    // Reemplaza a u por v como hijo de u->padre (o como raiz). No borra u.
    void _reemplazar_hijo(Nodo* u, Nodo* v) {
        if (u->padre == nullptr) raiz = v;
        else if (u->padre->izq == u) u->padre->izq = v;
        else u->padre->der = v;
        if (v != nullptr) v->padre = u->padre;
    }

    Nodo* _rotar_derecha(Nodo* y) {
        Nodo* x = y->izq;
        Nodo* B = x->der;

        _reemplazar_hijo(y, x);
        x->der = y;
        y->padre = x;
        y->izq = B;
        if (B != nullptr) B->padre = y;

        _actualizar(y);
        _actualizar(x);
        return x;
    }

    Nodo* _rotar_izquierda(Nodo* x) {
        Nodo* y = x->der;
        Nodo* B = y->izq;

        _reemplazar_hijo(x, y);
        y->izq = x;
        x->padre = y;
        x->der = B;
        if (B != nullptr) B->padre = x;

        _actualizar(x);
        _actualizar(y);
        return y;
    }

    // ========================================================================
    // ============== NO EDITAR: REBALANCEO ===================================
    // ========================================================================

    // Arregla u si |FB(u)| = 2. Devuelve la raiz del subarbol resultante.
    Nodo* _rebalancear(Nodo* u) {
        _actualizar(u);
        int fb = _factor_balance(u);

        if (fb > 1) {
            if (_factor_balance(u->izq) < 0) _rotar_izquierda(u->izq);   // LR
            return _rotar_derecha(u);                                    // LL
        }
        if (fb < -1) {
            if (_factor_balance(u->der) > 0) _rotar_derecha(u->der);     // RL
            return _rotar_izquierda(u);                                  // RR
        }
        return u;
    }

    // Sube desde u hasta la raiz rebalanceando (y actualizando) cada ancestro
    void _rebalancear_hacia_arriba(Nodo* u) {
        while (u != nullptr) {
            u = _rebalancear(u);
            u = u->padre;
        }
    }

    // ========================================================================
    // ============== NO EDITAR: BUSQUEDA Y LIBERAR MEMORIA ===================
    // ========================================================================

    Nodo* _encontrar(const TipoClave& clave) const {
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (clave < actual->clave) actual = actual->izq;
            else if (actual->clave < clave) actual = actual->der;
            else return actual;
        }
        return nullptr;
    }

    Nodo* _minimo_de(Nodo* u) const {
        if (u == nullptr) return nullptr;
        while (u->izq != nullptr) u = u->izq;
        return u;
    }

    Nodo* _maximo_de(Nodo* u) const {
        if (u == nullptr) return nullptr;
        while (u->der != nullptr) u = u->der;
        return u;
    }

    void destruir(Nodo* u) {
        if (u == nullptr) return;
        destruir(u->izq);
        destruir(u->der);
        delete u;
    }

    // Borra fisicamente el nodo u y rebalancea desde donde quedo el hueco
    void _borrar_nodo(Nodo* u) {
        Nodo* p;
        if (u->izq == nullptr && u->der == nullptr) {          // hoja
            p = u->padre;
            _reemplazar_hijo(u, nullptr);
            delete u;
        }
        else if (u->izq != nullptr && u->der != nullptr) {     // dos hijos
            Nodo* s = siguiente(u);                            // sucesor (sin hijo izq)
            u->clave = s->clave;
            u->valor = s->valor;
            p = s->padre;
            _reemplazar_hijo(s, s->der);
            delete s;
        }
        else {                                                 // un hijo
            p = u->padre;
            Nodo* h = (u->izq != nullptr) ? u->izq : u->der;
            _reemplazar_hijo(u, h);
            delete u;
        }
        _rebalancear_hacia_arriba(p);
    }

    // ========================================================================
    // ========================================================================
    //  ZONA DE USO: ESTAS SON LAS OPERACIONES QUE VAS A LLAMAR EN main
    // ========================================================================
    // ========================================================================

    // ------------------------------------------------------------------------
    // 1) LAS 3 OPERACIONES PRINCIPALES (se comportan igual que my_map)
    //      m[clave] = 1;    m[clave] = i;    m[clave]++;
    //    Si la clave no existe se crea con valor 0 (TipoValor()).
    //    La referencia devuelta sigue valida aunque se hagan mas inserciones.
    // ------------------------------------------------------------------------
    TipoValor& operator[](const TipoClave& clave) {
        if (raiz == nullptr) {
            raiz = new Nodo(clave);
            return raiz->valor;
        }

        Nodo* actual = raiz;
        Nodo* nuevo = nullptr;
        while (nuevo == nullptr) {
            if (clave < actual->clave) {
                if (actual->izq != nullptr) actual = actual->izq;
                else { nuevo = new Nodo(clave, actual); actual->izq = nuevo; }
            }
            else if (actual->clave < clave) {
                if (actual->der != nullptr) actual = actual->der;
                else { nuevo = new Nodo(clave, actual); actual->der = nuevo; }
            }
            else {
                return actual->valor;      // ya existia
            }
        }
        _rebalancear_hacia_arriba(actual);
        return nuevo->valor;
    }

    // ------------------------------------------------------------------------
    // 2) BASICAS
    // ------------------------------------------------------------------------
    bool has_key(const TipoClave& clave) const {
        return _encontrar(clave) != nullptr;
    }

    void erase(const TipoClave& clave) {
        Nodo* u = _encontrar(clave);
        if (u != nullptr) _borrar_nodo(u);
    }

    int size() const { return _tam(raiz); }

    bool empty() const { return raiz == nullptr; }

    void clear() {
        destruir(raiz);
        raiz = nullptr;
    }

    // ------------------------------------------------------------------------
    // 3) PREGUNTAS DE ORDEN (todas devuelven Nodo*; nullptr si no hay)
    //    Se usan asi:  auto p = m.mayor_o_igual(x);
    //                  if (p != nullptr) { p->clave; p->valor; }
    // ------------------------------------------------------------------------
    Nodo* minimo() const { return _minimo_de(raiz); }
    Nodo* maximo() const { return _maximo_de(raiz); }

    // primera clave >= x   (como lower_bound)
    Nodo* mayor_o_igual(const TipoClave& x) const {
        Nodo* res = nullptr;
        Nodo* a = raiz;
        while (a != nullptr) {
            if (a->clave < x) a = a->der;
            else { res = a; a = a->izq; }
        }
        return res;
    }

    // primera clave > x    (como upper_bound)
    Nodo* mayor(const TipoClave& x) const {
        Nodo* res = nullptr;
        Nodo* a = raiz;
        while (a != nullptr) {
            if (x < a->clave) { res = a; a = a->izq; }
            else a = a->der;
        }
        return res;
    }

    // ultima clave <= x
    Nodo* menor_o_igual(const TipoClave& x) const {
        Nodo* res = nullptr;
        Nodo* a = raiz;
        while (a != nullptr) {
            if (x < a->clave) a = a->izq;
            else { res = a; a = a->der; }
        }
        return res;
    }

    // ultima clave < x
    Nodo* menor(const TipoClave& x) const {
        Nodo* res = nullptr;
        Nodo* a = raiz;
        while (a != nullptr) {
            if (a->clave < x) { res = a; a = a->der; }
            else a = a->izq;
        }
        return res;
    }

    Nodo* siguiente(Nodo* x) const {
        if (x->der != nullptr) return _minimo_de(x->der);
        Nodo* y = x->padre;
        while (y != nullptr && y->der == x) { x = y; y = y->padre; }
        return y;
    }

    Nodo* anterior(Nodo* x) const {
        if (x->izq != nullptr) return _maximo_de(x->izq);
        Nodo* y = x->padre;
        while (y != nullptr && y->izq == x) { x = y; y = y->padre; }
        return y;
    }

    // ------------------------------------------------------------------------
    // 4) ESTADISTICAS DE ORDEN
    // ------------------------------------------------------------------------

    // cuantas claves son estrictamente menores que x
    int contar_menores(const TipoClave& x) const {
        int cuenta = 0;
        Nodo* a = raiz;
        while (a != nullptr) {
            if (a->clave < x) { cuenta += 1 + _tam(a->izq); a = a->der; }
            else a = a->izq;
        }
        return cuenta;
    }

    // la k-esima clave en orden creciente, k desde 0. nullptr si k fuera de rango
    Nodo* kesimo(int k) const {
        if (k < 0 || k >= size()) return nullptr;
        Nodo* a = raiz;
        while (a != nullptr) {
            int izq = _tam(a->izq);
            if (k < izq) a = a->izq;
            else if (k == izq) return a;
            else { k -= izq + 1; a = a->der; }
        }
        return nullptr;
    }

    // ------------------------------------------------------------------------
    // 5) DEBUG
    // ------------------------------------------------------------------------
    bool es_avl() const { return _es_avl(raiz); }

    bool _es_avl(Nodo* u) const {
        if (u == nullptr) return true;
        int fb = _factor_balance(u);
        if (fb > 1 || fb < -1) return false;
        return _es_avl(u->izq) && _es_avl(u->der);
    }

    void print() const {
        for (Nodo* p = minimo(); p != nullptr; p = siguiente(p)) {
            cout << p->clave << " --> " << p->valor << "\n";
        }
    }

    // Arbol de lado: la raiz a la izquierda, hijo derecho arriba
    void print_arbol() const { _print_arbol(raiz, 0); }

    void _print_arbol(Nodo* u, int nivel) const {
        if (u == nullptr) return;
        _print_arbol(u->der, nivel + 1);
        for (int i = 0; i < nivel; ++i) cout << "    ";
        cout << u->clave << ":" << u->valor << "\n";
        _print_arbol(u->izq, nivel + 1);
    }
};


// PARA INSERTAR (dentro de un for): m[nums[i]] = 1;
/*

m[clave] = 1; → "Solo quiero saber si esta clave existe".
m[clave] = i; → "Necesito recordar la posición de esta clave".
m[clave]++;   → "Necesito contar cuántas veces aparece esta clave".

EXTRA (porque el AVL esta ordenado):
auto p = m.mayor_o_igual(x);   → "Dame la clave mas cercana que sea >= x".
auto p = m.menor(x);           → "Dame la clave mas cercana que sea <  x".
m.contar_menores(x);           → "Cuantas claves hay menores que x".
m.kesimo(k);                   → "Dame la k-esima clave en orden".

SIEMPRE EN CODEFORCES PARA LEER DENTRO DEL FOR:
int x;
cin >> x;
*/