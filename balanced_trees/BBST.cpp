#include <iostream>
#include <vector>
using namespace std;

// ##########################################################################################
//     [NO TOCAR]       codigo que sirve para CUALQUIER enunciado. no editar.
//     [QUIZAS TOCAR]   lo cambias SOLO si el enunciado lo pide (cada uno dice cuando).
//     [USAR]           funciones que LLAMAS desde el main. Nombres = lo que dice el enunciado.
//
//  COMO SE USA EN EL EXAMEN:
//     1) Elige el arbol:   Arbol<int> A(AVL);   Arbol<int> A(ROJINEGRO);   Arbol<int> A(SIN_BALANCE);
//                          Arbol<int> A(AVL, true);   <- true = acepta repetidos (guarda un contador)
//     2) Si cada elemento tiene varios campos, haz un struct con operator< SOLO sobre la clave.
//     3) En el main: lee los datos y llama a las funciones [USAR]. Ve la CHULETA al final del archivo.
// ##########################################################################################


// ==========================================================================================
// [NO TOCAR]
// ==========================================================================================
enum Modo { SIN_BALANCE, AVL, ROJINEGRO };

// Sirve para las sumas y para "el mas cercano". int y long long -> su valor. Otros tipos -> 0.
// [QUIZAS TOCAR]: solo si necesitas sumar/medir distancias con otro tipo (double, char...).
//                 Agrega una linea:  inline long long como_numero(char x) { return x; }
template<typename T> long long como_numero(const T&) { return 0; }
inline long long como_numero(int x) { return x; }
inline long long como_numero(long long x) { return x; }


template<typename T>
struct Arbol {

    // ======================================================================================
    // [NO TOCAR]  Nodo
    // ======================================================================================
    struct Nodo {
        T dato;
        Nodo* izq;
        Nodo* der;
        Nodo* padre;
        int cnt;
        int tam;
        long long suma;
        int altura;
        bool rojo;
        Nodo(const T& d, Nodo* p = nullptr) :
            dato(d), izq(nullptr), der(nullptr), padre(p),
            cnt(1), tam(1), suma(como_numero(d)), altura(0), rojo(false) {}
    };

    // ======================================================================================
    // [QUIZAS TOCAR]  Configuracion (normalmente se fija en el main, al declarar el arbol)
    // ======================================================================================
    Nodo* raiz;
    int modo;                       // SIN_BALANCE / AVL / ROJINEGRO
    bool repetidos;                 // true = multiconjunto (cuenta copias). false = ignora repetidos
    bool reemplazo_por_predecesor;  // eliminar nodo con 2 hijos: false = usa SUCESOR (lo comun)
                                    //   si el enunciado dice "reemplazar por el PREDECESOR": A.reemplazo_por_predecesor = true;
    long long rotaciones;           // cuenta cada rotacion. Para "cuantas rotaciones hubo": A.rotaciones = 0; ...operacion...; leer A.rotaciones

    Arbol(int modo = AVL, bool repetidos = false) :
        raiz(nullptr), modo(modo), repetidos(repetidos), reemplazo_por_predecesor(false), rotaciones(0) {}

    // ======================================================================================
    // [NO TOCAR]  Auxiliares internos
    // ======================================================================================
    bool igual(const T& a, const T& b) { return !(a < b) && !(b < a); }
    int mayor(int a, int b) { return a > b ? a : b; }
    bool es_rojo(Nodo* u) { return u != nullptr && u -> rojo; }   // nulo cuenta como negro

    int altura(Nodo* u) { return u == nullptr ? -1 : u -> altura; }
    int tam(Nodo* u) { return u == nullptr ? 0 : u -> tam; }
    long long suma(Nodo* u) { return u == nullptr ? 0 : u -> suma; }

    // FB(v) = h(izq) - h(der).
    // [QUIZAS TOCAR]: si el enunciado define FB = h(der) - h(izq), NO cambies nada de la logica
    //                 (los rebalanceos son simetricos); solo afecta lo que muestres en dibujar().
    int factor_balance(Nodo* u) { return u == nullptr ? 0 : altura(u -> izq) - altura(u -> der); }

    // recalcula altura/tam/suma de u a partir de sus hijos
    void actualizar(Nodo* u) {
        u -> tam = u -> cnt + tam(u -> izq) + tam(u -> der);
        u -> suma = como_numero(u -> dato) * u -> cnt + suma(u -> izq) + suma(u -> der);
        u -> altura = 1 + mayor(altura(u -> izq), altura(u -> der));
    }
    void actualizar_hacia_arriba(Nodo* u) {
        while (u != nullptr) { actualizar(u); u = u -> padre; }
    }

    // pone v en el lugar de u dentro del padre de u (o como raiz). No borra u.
    void reemplazar_hijo(Nodo* u, Nodo* v) {
        if (u -> padre == nullptr) raiz = v;
        else if (u -> padre -> izq == u) u -> padre -> izq = v;
        else u -> padre -> der = v;
        if (v != nullptr) v -> padre = u -> padre;
    }

    // ======================================================================================
    // ROTACIONES   [NO TOCAR el codigo]  /  [USAR] desde el main si el enunciado dice "rota en x"
    // ======================================================================================
    // Cada una devuelve el nodo que SUBIO. Mantienen punteros a padre, raiz y alturas de los 2 nodos movidos.
    // (En SIN_BALANCE no se actualizan alturas de los ancestros; si las necesitas: recalcular_todo().)
    //
    // USAR CUANDO: "rotar a la derecha en y" (y baja, su hijo izquierdo sube)
    //    y          x
    //   x C  -->   A y
    //  A B           B C
    Nodo* rotar_derecha(Nodo* y) {
        Nodo* x = y -> izq;
        Nodo* B = x -> der;
        reemplazar_hijo(y, x);
        x -> der = y;  y -> padre = x;
        y -> izq = B;  if (B != nullptr) B -> padre = y;
        actualizar(y); actualizar(x);
        ++rotaciones;
        return x;
    }

    // USAR CUANDO: "rotar a la izquierda en x" (x baja, su hijo derecho sube)
    Nodo* rotar_izquierda(Nodo* x) {
        Nodo* y = x -> der;
        Nodo* B = y -> izq;
        reemplazar_hijo(x, y);
        y -> izq = x;  x -> padre = y;
        x -> der = B;  if (B != nullptr) B -> padre = x;
        actualizar(x); actualizar(y);
        ++rotaciones;
        return y;
    }

    // ======================================================================================
    // [NO TOCAR]  Rebalanceo AVL (casos LL, RR, LR, RL)
    // ======================================================================================
    // Nota: con FB == 0 en el hijo pesado (solo pasa al eliminar) hace 1 rotacion simple. Es lo estandar.
    Nodo* rebalancear_avl(Nodo* u) {
        actualizar(u);
        int fb = factor_balance(u);
        if (fb > 1) {
            if (factor_balance(u -> izq) < 0) rotar_izquierda(u -> izq);   // LR
            return rotar_derecha(u);                                       // LL
        }
        if (fb < -1) {
            if (factor_balance(u -> der) > 0) rotar_derecha(u -> der);     // RL
            return rotar_izquierda(u);                                     // RR
        }
        return u;
    }

    // sube hasta la raiz rebalanceando cada nodo del camino
    void rebalancear_avl_hacia_arriba(Nodo* u) {
        while (u != nullptr) {
            u = rebalancear_avl(u);
            u = u -> padre;
        }
    }

    // ======================================================================================
    // [NO TOCAR]  Rebalanceo ROJINEGRO (version estandar tipo CLRS)
    // ======================================================================================
    // [QUIZAS TOCAR]: si tu curso usa OTRA variante (left-leaning, 2-3-4, otra regla de colores),
    //                 esta parte no sirve tal cual y habria que reescribirla.

    // n = nodo nuevo (rojo). Arregla "dos rojos seguidos" mirando al tio.
    void arreglar_insercion_rb(Nodo* n) {
        while (n != raiz && n -> padre -> rojo) {
            Nodo* p = n -> padre;
            Nodo* a = p -> padre;
            if (p == a -> izq) {
                Nodo* t = a -> der;
                if (es_rojo(t)) {                       // tio rojo: recolorear y subir
                    p -> rojo = false; t -> rojo = false; a -> rojo = true;
                    n = a;
                }
                else {                                  // tio negro: rotar
                    if (n == p -> der) { n = p; rotar_izquierda(n); p = n -> padre; }
                    p -> rojo = false; a -> rojo = true;
                    rotar_derecha(a);
                }
            }
            else {
                Nodo* t = a -> izq;
                if (es_rojo(t)) {
                    p -> rojo = false; t -> rojo = false; a -> rojo = true;
                    n = a;
                }
                else {
                    if (n == p -> izq) { n = p; rotar_derecha(n); p = n -> padre; }
                    p -> rojo = false; a -> rojo = true;
                    rotar_izquierda(a);
                }
            }
        }
        raiz -> rojo = false;
    }

    // x = hijo que reemplazo al nodo negro eliminado (puede ser nulo), px = su padre.
    void arreglar_eliminacion_rb(Nodo* x, Nodo* px) {
        while (x != raiz && !es_rojo(x)) {
            if (x == px -> izq) {
                Nodo* w = px -> der;                    // hermano
                if (es_rojo(w)) {
                    w -> rojo = false; px -> rojo = true;
                    rotar_izquierda(px);
                    w = px -> der;
                }
                if (!es_rojo(w -> izq) && !es_rojo(w -> der)) {
                    w -> rojo = true;
                    x = px; px = x -> padre;
                }
                else {
                    if (!es_rojo(w -> der)) {
                        w -> izq -> rojo = false; w -> rojo = true;
                        rotar_derecha(w);
                        w = px -> der;
                    }
                    w -> rojo = px -> rojo; px -> rojo = false; w -> der -> rojo = false;
                    rotar_izquierda(px);
                    x = raiz;
                }
            }
            else {
                Nodo* w = px -> izq;
                if (es_rojo(w)) {
                    w -> rojo = false; px -> rojo = true;
                    rotar_derecha(px);
                    w = px -> izq;
                }
                if (!es_rojo(w -> izq) && !es_rojo(w -> der)) {
                    w -> rojo = true;
                    x = px; px = x -> padre;
                }
                else {
                    if (!es_rojo(w -> izq)) {
                        w -> der -> rojo = false; w -> rojo = true;
                        rotar_izquierda(w);
                        w = px -> izq;
                    }
                    w -> rojo = px -> rojo; px -> rojo = false; w -> izq -> rojo = false;
                    rotar_derecha(px);
                    x = raiz;
                }
            }
        }
        if (x != nullptr) x -> rojo = false;
    }


    // ##################################################################################
    //  A PARTIR DE AQUI: [USAR]  -  las funciones que llamas desde el main, por PATRON
    //  (el cuerpo de cada una es [NO TOCAR] salvo donde diga [QUIZAS TOCAR])
    // ##################################################################################


    // ======================================================================================
    // PATRON 1: CONJUNTO / DICCIONARIO  ("llega x", "sale x", "¿esta x?")
    // ======================================================================================

    // USAR CUANDO: "inserta x" / "llega x" / "agrega x" / "registra x".
    // Devuelve el Nodo donde quedo el dato (el nuevo, o el que ya existia).
    //   - repetidos=false: si ya estaba, no hace nada.
    //   - repetidos=true : si ya estaba, suma 1 a su contador.
    // [QUIZAS TOCAR]: si el enunciado dice "los repetidos se insertan como NODOS DISTINTOS (a la derecha)",
    //                 hay que tocar el bloque  if (igual(...))  de abajo. Si dice "se ignoran" o "se cuentan", no.
    Nodo* insertar(const T& valor) {
        Nodo* nuevo = nullptr;
        if (raiz == nullptr) {
            nuevo = raiz = new Nodo(valor);
        }
        else {
            Nodo* actual = raiz;
            while (nuevo == nullptr) {
                if (igual(actual -> dato, valor)) {
                    if (repetidos) { actual -> cnt++; actualizar_hacia_arriba(actual); }
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
        actualizar_hacia_arriba(nuevo -> padre);
        if (modo == AVL) rebalancear_avl_hacia_arriba(nuevo -> padre);
        if (modo == ROJINEGRO) {
            nuevo -> rojo = true;
            arreglar_insercion_rb(nuevo);
            actualizar_hacia_arriba(nuevo);     // las rotaciones pueden cambiar alturas de ancestros
        }
        return nuevo;
    }

    // USAR CUANDO: "elimina x" / "sale x" / "vende x" / "cancela x". Si x no existe no hace nada.
    //   - con repetidos=true quita UNA sola copia.
    //   - nodo con 2 hijos: se reemplaza por su SUCESOR (o PREDECESOR si reemplazo_por_predecesor = true).
    //   - nodo con 0 o 1 hijo: el hijo (si hay) toma su lugar.
    void eliminar(const T& valor) {
        Nodo* u = encontrar(valor);
        if (u == nullptr) return;
        if (u -> cnt > 1) { u -> cnt--; actualizar_hacia_arriba(u); return; }
        if (u -> izq != nullptr && u -> der != nullptr) {
            Nodo* s = reemplazo_por_predecesor ? predecesor(u) : sucesor(u);
            u -> dato = s -> dato;
            u -> cnt = s -> cnt;
            u = s;
        }
        Nodo* hijo = (u -> izq != nullptr) ? u -> izq : u -> der;
        Nodo* padre = u -> padre;
        bool era_negro = !u -> rojo;
        reemplazar_hijo(u, hijo);
        delete u;
        actualizar_hacia_arriba(padre);
        if (modo == AVL) rebalancear_avl_hacia_arriba(padre);
        if (modo == ROJINEGRO && era_negro) {
            arreglar_eliminacion_rb(hijo, padre);
            actualizar_hacia_arriba(padre);
        }
    }

    // USAR CUANDO: necesitas el Nodo para leer/modificar su carga. nullptr si no esta.
    Nodo* encontrar(const T& valor) {
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (valor < actual -> dato) actual = actual -> izq;
            else if (actual -> dato < valor) actual = actual -> der;
            else return actual;
        }
        return nullptr;
    }

    // USAR CUANDO: "¿esta x?" / "existe x?"
    bool buscar(const T& valor) { return encontrar(valor) != nullptr; }

    bool vacio() { return raiz == nullptr; }
    int cuantos_hay() { return tam(raiz); }            // total de elementos (cuenta repetidos)

    // USAR CUANDO: "altura del arbol". Aqui: un solo nodo = 0, vacio = -1.
    // [QUIZAS TOCAR]: si el enunciado dice que una hoja tiene altura 1, imprime altura_arbol() + 1.
    int altura_arbol() { return altura(raiz); }


    // ======================================================================================
    // PATRON 2: EXTREMOS Y VECINOS DE UN NODO  ("el menor", "el mayor", "el siguiente de x")
    // ======================================================================================
    // Todas devuelven Nodo* (nullptr si no existe). El valor esta en ->dato.

    Nodo* minimo(Nodo* u) { while (u != nullptr && u -> izq != nullptr) u = u -> izq; return u; }
    Nodo* maximo(Nodo* u) { while (u != nullptr && u -> der != nullptr) u = u -> der; return u; }
    Nodo* minimo() { return minimo(raiz); }            // USAR CUANDO: "el menor actual"
    Nodo* maximo() { return maximo(raiz); }            // USAR CUANDO: "el mayor actual"

    // USAR CUANDO: "el siguiente mayor a x" y x SI esta en el arbol:  sucesor(encontrar(x))
    Nodo* sucesor(Nodo* x) {
        if (x -> der != nullptr) return minimo(x -> der);
        Nodo* y = x -> padre;
        while (y != nullptr && y -> der == x) { x = y; y = y -> padre; }
        return y;
    }
    // USAR CUANDO: "el anterior menor a x" y x SI esta en el arbol:  predecesor(encontrar(x))
    Nodo* predecesor(Nodo* x) {
        if (x -> izq != nullptr) return maximo(x -> izq);
        Nodo* y = x -> padre;
        while (y != nullptr && y -> izq == x) { x = y; y = y -> padre; }
        return y;
    }


    // ======================================================================================
    // PATRON 3: VECINOS POR VALOR  (x NO tiene que estar en el arbol)
    //   "el mas cercano a x", "primer valor >= x", "el ultimo valor < x", "piso / techo"
    // ======================================================================================
    // Todas devuelven Nodo* (nullptr si no hay). El valor esta en ->dato.

    Nodo* siguiente_o_igual(const T& x) {      // el menor elemento  >= x
        Nodo* u = raiz; Nodo* res = nullptr;
        while (u != nullptr) { if (u -> dato < x) u = u -> der; else { res = u; u = u -> izq; } }
        return res;
    }
    Nodo* siguiente_estricto(const T& x) {     // el menor elemento  >  x
        Nodo* u = raiz; Nodo* res = nullptr;
        while (u != nullptr) { if (x < u -> dato) { res = u; u = u -> izq; } else u = u -> der; }
        return res;
    }
    Nodo* anterior_o_igual(const T& x) {       // el mayor elemento  <= x
        Nodo* u = raiz; Nodo* res = nullptr;
        while (u != nullptr) { if (x < u -> dato) u = u -> izq; else { res = u; u = u -> der; } }
        return res;
    }
    Nodo* anterior_estricto(const T& x) {      // el mayor elemento  <  x
        Nodo* u = raiz; Nodo* res = nullptr;
        while (u != nullptr) { if (u -> dato < x) { res = u; u = u -> der; } else u = u -> izq; }
        return res;
    }

    // USAR CUANDO: "el elemento mas cercano a x" (solo T = int / long long).
    // [QUIZAS TOCAR]: en empate de distancias devuelve el MENOR. Si el enunciado pide el MAYOR,
    //                 cambia el  <=  de la ultima linea por  <
    Nodo* el_mas_cercano(const T& x) {
        Nodo* a = anterior_o_igual(x);
        Nodo* b = siguiente_o_igual(x);
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        long long dist_a = como_numero(x) - como_numero(a -> dato);
        long long dist_b = como_numero(b -> dato) - como_numero(x);
        return dist_a <= dist_b ? a : b;
    }


    // ======================================================================================
    // PATRON 4: RANKING / CONTEO / K-ESIMO / RANGOS   (todo O(log n))
    //   "cuantos son menores que x", "en que posicion queda x", "el k-esimo", "cuantos entre l y r"
    // ======================================================================================

    // USAR CUANDO: "¿a cuantos les gano?", "cuantos son estrictamente menores que x" (x puede no estar)
    int cuantos_menores_que(const T& x) {
        int res = 0; Nodo* u = raiz;
        while (u != nullptr) {
            if (u -> dato < x) { res += tam(u -> izq) + u -> cnt; u = u -> der; }
            else u = u -> izq;
        }
        return res;
    }
    // USAR CUANDO: "cuantos son menores o iguales a x"
    int cuantos_menores_o_iguales(const T& x) {
        int res = 0; Nodo* u = raiz;
        while (u != nullptr) {
            if (x < u -> dato) u = u -> izq;
            else { res += tam(u -> izq) + u -> cnt; u = u -> der; }
        }
        return res;
    }
    // USAR CUANDO: "cuantos hay entre l y r inclusive" (cuenta repetidos)
    // [QUIZAS TOCAR]: rango abierto/semiabierto -> usa directamente cuantos_menores_que /
    //                 cuantos_menores_o_iguales (ej. [l, r) = cuantos_menores_que(r) - cuantos_menores_que(l))
    int cuantos_en_rango(const T& l, const T& r) {
        return cuantos_menores_o_iguales(r) - cuantos_menores_que(l);
    }
    // USAR CUANDO: "posicion de x si ordeno todo" (1-indexado)
    int posicion_de(const T& x) { return cuantos_menores_que(x) + 1; }

    // USAR CUANDO: "el k-esimo menor" (k desde 1, cuenta repetidos). nullptr si k esta fuera de rango.
    // [QUIZAS TOCAR]: "el k-esimo MAYOR" = el_k_esimo(cuantos_hay() - k + 1).  Si k es desde 0, usa k + 1.
    Nodo* el_k_esimo(int k) {
        Nodo* u = raiz;
        while (u != nullptr) {
            int ti = tam(u -> izq);
            if (k <= ti) u = u -> izq;
            else if (k <= ti + u -> cnt) return u;
            else { k -= ti + u -> cnt; u = u -> der; }
        }
        return nullptr;
    }


    // ======================================================================================
    // PATRON 5: SUMAS EN RANGO   (solo T = int / long long; devuelven long long)
    //   "suma de los precios entre l y r"
    // ======================================================================================
    long long suma_menores_que(const T& x) {
        long long res = 0; Nodo* u = raiz;
        while (u != nullptr) {
            if (u -> dato < x) { res += suma(u -> izq) + como_numero(u -> dato) * u -> cnt; u = u -> der; }
            else u = u -> izq;
        }
        return res;
    }
    long long suma_menores_o_iguales(const T& x) {
        long long res = 0; Nodo* u = raiz;
        while (u != nullptr) {
            if (x < u -> dato) u = u -> izq;
            else { res += suma(u -> izq) + como_numero(u -> dato) * u -> cnt; u = u -> der; }
        }
        return res;
    }
    // USAR CUANDO: "suma de los valores entre l y r inclusive" (cuenta repetidos)
    long long suma_en_rango(const T& l, const T& r) {
        return suma_menores_o_iguales(r) - suma_menores_que(l);
    }
    long long suma_total() { return suma(raiz); }


    // ======================================================================================
    // PATRON 6: ARBOL DADO EN EL ENUNCIADO + VERIFICAR  ("¿es BST?", "¿es AVL?", "¿es rojinegro?")
    // ======================================================================================

    // [NO TOCAR]  lista de nodos SIN recursion (cada padre aparece ANTES que sus descendientes)
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

    // recalcula altura/tam/suma de TODO el arbol sin recursion
    void recalcular_todo() {
        vector<Nodo*> orden = lista_de_nodos();
        for (int i = (int)orden.size() - 1; i >= 0; --i) actualizar(orden[i]);
    }

    // USAR CUANDO: "el nodo i tiene clave k[i], hijo izq l[i], hijo der r[i] (0 = no hay)".
    // Vectores 1-indexados (tamano n+1). El nodo 1 es la raiz. Deja nodos[i] = puntero al nodo i.
    // Declara el arbol como SIN_BALANCE para que no "arregle" nada.
    // [QUIZAS TOCAR]: si la raiz no es el nodo 1, cambia  raiz = nodos[1]  por el nodo que diga el enunciado.
    //                 Si los vectores vienen 0-indexados, sumales 1 al leer.
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

    // USAR CUANDO: "¿es un BST?"  Iterativo: aguanta cadenas de 2e5 nodos. (valida inorder estrictamente creciente)
    // [QUIZAS TOCAR]: si el enunciado permite claves repetidas en el BST, cambia  !(previo->dato < u->dato)
    //                 por  (u->dato < previo->dato)
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

    // USAR CUANDO: "¿es un AVL?"  (|FB| <= 1 en todos los nodos). Iterativo y O(n).
    // OJO: NO revisa que sea BST. Haz primero es_bst() y luego es_avl().
    bool es_avl() {
        vector<Nodo*> orden = lista_de_nodos();
        for (int i = (int)orden.size() - 1; i >= 0; --i) {
            actualizar(orden[i]);
            int fb = factor_balance(orden[i]);
            if (fb > 1 || fb < -1) return false;
        }
        return true;
    }

    // [NO TOCAR] devuelve la altura negra, o -1 si viola la regla "rojo sin hijo rojo" o "misma altura negra"
    int altura_negra(Nodo* u) {
        if (u == nullptr) return 0;
        if (u -> rojo && (es_rojo(u -> izq) || es_rojo(u -> der))) return -1;
        int a = altura_negra(u -> izq), b = altura_negra(u -> der);
        if (a == -1 || b == -1 || a != b) return -1;
        return a + (u -> rojo ? 0 : 1);
    }
    // USAR CUANDO: "¿es rojinegro?" (requiere colores en los nodos; recursivo: pensado para arboles construidos por insertar)
    bool es_rojinegro() { return !es_rojo(raiz) && altura_negra(raiz) != -1; }

    // altura recalculada a mano (recursiva). Solo para depurar.
    int altura_real(Nodo* u) { return u == nullptr ? -1 : 1 + mayor(altura_real(u -> izq), altura_real(u -> der)); }

    // USAR CUANDO: terminas con muchos casos de prueba y quieres liberar memoria (sin recursion)
    void liberar() {
        vector<Nodo*> orden = lista_de_nodos();
        for (int i = 0; i < (int)orden.size(); ++i) delete orden[i];
        raiz = nullptr;
    }


    // ======================================================================================
    // PATRON 7: RECORRIDOS Y DIBUJO  ("imprime el preorden", "dibuja el arbol")
    // ======================================================================================
    // [NO TOCAR]  recursivos (OK para arboles balanceados)
    void inorder(Nodo* u) { if (u == nullptr) return; inorder(u -> izq); cout << u -> dato << " "; inorder(u -> der); }
    void preorder(Nodo* u) { if (u == nullptr) return; cout << u -> dato << " "; preorder(u -> izq); preorder(u -> der); }
    void postorder(Nodo* u) { if (u == nullptr) return; postorder(u -> izq); postorder(u -> der); cout << u -> dato << " "; }

    // USAR CUANDO: el enunciado pide el recorrido (inorder sale ordenado)
    // [QUIZAS TOCAR]: separador (" ") y salto de linea, segun el formato de salida que pida el enunciado.
    void imprimir_inorder() { inorder(raiz); cout << "\n"; }
    void imprimir_preorder() { preorder(raiz); cout << "\n"; }
    void imprimir_postorder() { postorder(raiz); cout << "\n"; }

    // USAR CUANDO: quieres VER el arbol (gira 90°: raiz a la izquierda, arriba = derecha). Solo para depurar.
    // AVL muestra [FB]; ROJINEGRO muestra [R] o [N].  (Requiere que T se pueda imprimir con cout.)
    void dibujar(Nodo* u, int nivel) {
        if (u == nullptr) return;
        dibujar(u -> der, nivel + 1);
        for (int i = 0; i < nivel; ++i) cout << "      ";
        cout << u -> dato;
        if (modo == AVL) cout << " [" << factor_balance(u) << "]";
        if (modo == ROJINEGRO) cout << (u -> rojo ? " [R]" : " [N]");
        cout << "\n";
        dibujar(u -> izq, nivel + 1);
    }
    void dibujar() { dibujar(raiz, 0); cout << "-----\n"; }
};


// ##########################################################################################
//  PATRONES LISTOS (envuelven un Arbol; los usas como una "caja" con nombres del enunciado)
// ##########################################################################################

// ==========================================================================================
// PATRON 8: DICCIONARIO ORDENADO  clave -> valor     ("cuantas veces aparece x", "precio de x", "nota de x")
// ==========================================================================================
// USO:  Mapa<int,int> m;      m[x]++;      m[x] = 5;      if (m.existe(x)) ...      m.quitar(x);
// Lo que NO esta en el diccionario se crea con valor 0 (V()) la primera vez que haces m[x].
// OJO: no guardes una referencia  V& r = m[x]  y luego llames a quitar(): puede quedar invalida.
// [QUIZAS TOCAR]: si necesitas recorrer en orden o vecinos de claves, usa  m.A  (es el Arbol interno).
template<typename K, typename V>
struct Mapa {
    struct Par {
        K clave; V valor;
        bool operator<(const Par& o) const { return clave < o.clave; }   // el orden es SOLO por clave
    };
    Arbol<Par> A;
    Mapa(int modo = AVL) : A(modo) {}

    V& operator[](const K& k) { return A.insertar({k, V()}) -> dato.valor; }   // crea si no existe
    bool existe(const K& k) { return A.buscar({k, V()}); }
    V valor_de(const K& k, const V& si_no_existe = V()) {                      // lee SIN crear
        typename Arbol<Par>::Nodo* e = A.encontrar({k, V()});
        return e == nullptr ? si_no_existe : e -> dato.valor;
    }
    void quitar(const K& k) { A.eliminar({k, V()}); }
    int cuantas_claves() { return A.cuantos_hay(); }
};


// ==========================================================================================
// PATRON 9: FRECUENCIAS / VOTOS / MODA EN VIVO
//   "llega un voto para x", "se retira un voto de x", "¿cual va ganando?"
// ==========================================================================================
// USO:  Frecuencias<int> f;   f.agregar(x);   f.quitar(x);
//       if (f.hay_algo()) { f.el_mas_frecuente() ; f.veces_del_mas_frecuente(); }
// [QUIZAS TOCAR]: la regla de DESEMPATE esta en Rank::operator<  (abajo).
//    - hoy: gana el que tiene mas veces; en empate, la clave MENOR.
//    - si en empate debe ganar la clave MAYOR, cambia  clave < o.clave  por  o.clave < clave
//    - si quieres el MENOS frecuente, usa  ranking.maximo()  en vez de  ranking.minimo()
template<typename K>
struct Frecuencias {
    struct Rank {
        int veces; K clave;
        bool operator<(const Rank& o) const {
            if (veces != o.veces) return veces > o.veces;   // mas veces = "menor" = primero
            return clave < o.clave;                         // desempate
        }
    };
    Mapa<K, int> cuenta;     // clave -> cuantas veces
    Arbol<Rank> ranking;     // ordenado por (veces desc, clave asc). El minimo() es el que va ganando

    Frecuencias(int modo = AVL) : cuenta(modo), ranking(modo) {}

    void agregar(const K& k) {
        int& c = cuenta[k];
        if (c > 0) ranking.eliminar({c, k});
        c++;
        ranking.insertar({c, k});
    }
    // si k no tiene votos no hace nada
    void quitar(const K& k) {
        int& c = cuenta[k];
        if (c == 0) return;
        ranking.eliminar({c, k});
        c--;
        if (c > 0) ranking.insertar({c, k});
    }
    int frecuencia_de(const K& k) { return cuenta.valor_de(k, 0); }
    bool hay_algo() { return !ranking.vacio(); }
    K el_mas_frecuente() { return ranking.minimo() -> dato.clave; }
    int veces_del_mas_frecuente() { return ranking.minimo() -> dato.veces; }
};


// ==========================================================================================
// PATRON 10: INTERVALOS QUE NO SE SOLAPAN   (reservas, agenda, sala, tareas)
//   "pide la sala de s a e: si choca se rechaza", "cancela la reserva que empieza en s",
//   "¿que reserva ocupa el minuto t?"
// ==========================================================================================
// USO:  Agenda ag;
//       if (ag.reservar(s, e)) ...        // true = aceptada (se guarda), false = choca (no se guarda)
//       ag.cancelar(s);                   // cancela la que EMPIEZA en s
//       Intervalo* r = ag.que_ocupa(t);   // nullptr si esta libre; si no, r->ini y r->fin
// [QUIZAS TOCAR]: tipo de intervalo.
//    - Agenda ag;           -> semiabierto [ini, fin): [10,20) y [20,30) NO chocan   (lo mas comun)
//    - Agenda ag(AVL, true) -> cerrado [ini, fin]:     [10,20] y [20,30] SI chocan
struct Intervalo {
    long long ini, fin;
    bool operator<(const Intervalo& o) const { return ini < o.ini; }   // la clave es el inicio
};
struct Agenda {
    Arbol<Intervalo> A;
    int extra;                       // 0 = semiabierto, 1 = cerrado
    Agenda(int modo = AVL, bool cerrado = false) : A(modo), extra(cerrado ? 1 : 0) {}

    bool esta_libre(long long ini, long long fin) {
        Arbol<Intervalo>::Nodo* a = A.anterior_o_igual({ini, 0});     // la que empieza antes (o igual)
        if (a != nullptr && a -> dato.fin + extra > ini) return false;
        Arbol<Intervalo>::Nodo* b = A.siguiente_estricto({ini, 0});   // la que empieza despues
        if (b != nullptr && b -> dato.ini < fin + extra) return false;
        return true;
    }
    bool reservar(long long ini, long long fin) {
        if (!esta_libre(ini, fin)) return false;
        A.insertar({ini, fin});
        return true;
    }
    void cancelar(long long ini) { A.eliminar({ini, 0}); }
    Intervalo* que_ocupa(long long t) {
        Arbol<Intervalo>::Nodo* a = A.anterior_o_igual({t, 0});
        if (a != nullptr && a -> dato.fin + extra > t) return &a -> dato;
        return nullptr;
    }
};


// ##########################################################################################
//  si el enunciado dice...  ->  llama a
// ##########################################################################################
//   inserta / llega / registra x ............ A.insertar(x)
//   elimina / sale / vende x ................ A.eliminar(x)
//   ¿esta x? ................................ A.buscar(x)
//   el menor / el mayor ..................... A.minimo()->dato / A.maximo()->dato
//   siguiente / anterior de x (x esta) ...... A.sucesor(A.encontrar(x)) / A.predecesor(A.encontrar(x))
//   primer valor >= x / > x ................. A.siguiente_o_igual(x) / A.siguiente_estricto(x)
//   ultimo valor <= x / < x ................. A.anterior_o_igual(x) / A.anterior_estricto(x)
//   el mas cercano a x ...................... A.el_mas_cercano(x)       (ojo con nullptr si esta vacio)
//   cuantos son menores que x / ranking ..... A.cuantos_menores_que(x) / A.posicion_de(x)
//   cuantos hay entre l y r ................. A.cuantos_en_rango(l, r)       (con repetidos: Arbol A(AVL, true))
//   el k-esimo menor ........................ A.el_k_esimo(k)->dato
//   suma de valores entre l y r ............. A.suma_en_rango(l, r)          (con repetidos: Arbol A(AVL, true))
//   rotar a la izq / der en el nodo x ....... A.rotar_izquierda(nodo) / A.rotar_derecha(nodo)   [con construir(...)]
//   cuantas rotaciones hubo ................. A.rotaciones = 0; operacion; A.rotaciones
//   altura del arbol ........................ A.altura_arbol()
//   arbol dado + "¿es BST? ¿es AVL?" ........ A.construir(k, l, r, nodos); A.es_bst(); A.es_avl()
//   construir un AVL/rojinegro insertando ... Arbol<int> A(AVL) / A(ROJINEGRO); A.insertar(...) en orden
//   diccionario clave->valor ................ Mapa<K,V> m;  m[x]++;  m.existe(x);  m.quitar(x)
//   votos / frecuencias / moda en vivo ...... Frecuencias<K> f; f.agregar(x); f.quitar(x); f.el_mas_frecuente()
//   reservas / intervalos sin solapar ....... Agenda ag; ag.reservar(s,e); ag.cancelar(s); ag.que_ocupa(t)
//   alturas min/max de un AVL con n nodos ... NO es un arbol: es matematica. N(h)=N(h-1)+N(h-2)+1
//
//   Si T es struct: operator< SOLO sobre la clave. Los otros campos son carga libre (los puedes modificar
//   con  A.encontrar(x)->dato.campo  o con el Nodo* que devuelve insertar).
// ##########################################################################################


int main() {
    ios::sync_with_stdio(false);     // lectura/escritura rapida. Usa "\n" en vez de endl
    cin.tie(nullptr);
    int n;
    cin >> n;
    Arbol<int> A(AVL);               // AVL / ROJINEGRO / SIN_BALANCE segun el enunciado
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        A.insertar(x);
    }
    A.dibujar();
    A.imprimir_preorder();
    A.imprimir_inorder();
    A.imprimir_postorder();
    return 0;
}
