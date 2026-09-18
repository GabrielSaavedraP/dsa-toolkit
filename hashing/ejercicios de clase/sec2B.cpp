#include <iostream>
#include <vector>
using namespace std;

// Tabla hash con encadenamiento (chaining), usando solo vector como STL permitido.
// Cada cubeta es un vector<Par>; colisiones se resuelven agregando a la misma cubeta.


//=====================================HASH STRINGS==================================
// Descomenta este bloque (y comenta el _hash de long long de abajo) cuando
// el problema use claves tipo string (ej. "cuenta cuántas veces aparece
// cada palabra", "verifica si dos strings son anagramas").
/*
*/
//==============================================================================
template <typename TipoClave, typename TipoValor>
struct my_map {

    struct Par {
        TipoClave clave;
        TipoValor valor;
        Par(const TipoClave& k, const TipoValor& v) : clave(k), valor(v) {}
    };

    int num_cubetas;
    int total;
    vector<vector<Par>> cubetas;

    my_map(int cubetas_iniciales = 8) : num_cubetas(cubetas_iniciales), total(0) {
        cubetas.resize(num_cubetas);
    }

    // ========================================================
    // ========================================================

    // USAR CUANDO: necesitas leer O escribir un valor asociado a una
    // clave, sin que te importe si ya existía. Es el caso más común en
    // Codeforces: "contar frecuencia de cada elemento" (m[x]++), "sumar
    // valores por categoría" (m[categoria] += valor), o simplemente
    // guardar/actualizar un dato por clave. Si la clave es nueva, la
    // crea sola con valor por defecto (0 para int, "" para string, etc.)
    // — por eso `m[x]++` funciona directo sin chequear antes si x existe.
    TipoValor& operator[](const TipoClave& clave) {
        if ((double)(total + 1) / num_cubetas > 0.75) {
            resize(num_cubetas * 2);
        }

        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        if (j == (int)cubetas[i].size()) {
            cubetas[i].emplace_back(clave, TipoValor());
            ++total;
        }

        return cubetas[i][j].valor;
    }

    // USAR CUANDO: el problema pide "elimina la clave x" explícitamente,
    // o cuando estás procesando una ventana deslizante (sliding window) y
    // necesitas sacar elementos que ya no están en el rango actual. NO la
    // uses solo para "poner un valor en 0" — para eso basta con
    // m[x] = 0, que es más simple y no reduce el tamaño de la tabla.
    void erase(const TipoClave& clave) {
        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        if (j != (int)cubetas[i].size()) {
            if (j + 1 < (int)cubetas[i].size()) {
                swap(cubetas[i][j], cubetas[i].back());
            }
            cubetas[i].pop_back();
            --total;
        }
    }

    // USAR CUANDO: solo necesitas saber SI existe la clave, sin querer
    // crearla si no está. Importante: NO uses `if (m[x] > 0)` para esto,
    // porque operator[] CREA la clave con valor 0 si no existía (efecto
    // secundario no deseado que infla el tamaño de la tabla). has_key es
    // la forma segura de "solo consultar" — típico en "verifica si ya
    // visitaste este estado" (backtracking, BFS con estados complejos) o
    // "detecta si hay un elemento repetido en el arreglo".
    bool has_key(const TipoClave& clave) const {
        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        return j != (int)cubetas[i].size();
    }

    // Función interna: no la llamas directo en tus problemas, la usan
    // operator[]/erase/has_key para saber en qué cubeta buscar.
    int _hash(const string& clave) const {
        const int BASE = 311;
        const int MOD = 1e9 + 7;

        int h = 0;
        for (char c : clave) {
            h = (1LL * h * BASE + (c + 1)) % MOD;
        }

        int indice = h % num_cubetas;
        if (indice < 0) indice += num_cubetas;
        return indice;
    }

    // ========================================================
    // ========================================================

    // Función interna: se dispara sola desde operator[] cuando el load
    // factor sube demasiado. No la llamas directo, salvo que ya sepas
    // de antemano cuántos elementos vas a insertar y quieras reservar
    // espacio una sola vez al inicio (evita varios rehash intermedios
    // en problemas con n grande, ej. n=10^6).
    void resize(int nuevo_num_cubetas) {
        vector<vector<Par>> cubetas_viejas = cubetas;

        num_cubetas = nuevo_num_cubetas;
        cubetas.clear();
        cubetas.resize(num_cubetas);
        total = 0;

        for (int i = 0; i < (int)cubetas_viejas.size(); ++i) {
            for (int j = 0; j < (int)cubetas_viejas[i].size(); ++j) {
                (*this)[cubetas_viejas[i][j].clave] = cubetas_viejas[i][j].valor;
            }
        }
    }
    // --------------------------------------------------------

    // USAR CUANDO: el problema pregunta directamente "¿cuántos elementos
    // distintos hay?" — como en el main de abajo, donde cada m[x]=1
    // solo cuenta como elemento nuevo la primera vez que aparece x.
    int size() const { return total; }

    // USAR CUANDO: necesitas chequear si ya has guardado algo antes de
    // hacer otra operación (ej. "si la tabla está vacía, inicializa algo").
    bool empty() const { return total == 0; }

    // USAR CUANDO: estás depurando tu solución y quieres ver cómo se
    // distribuyeron las claves entre cubetas (detectar si tu hash está
    // colisionando demasiado). Normalmente NO se usa en la solución
    // final que envías al juez, solo mientras desarrollas.
    void print() {
        for (int i = 0; i < num_cubetas; ++i) {
            cout << "Bucket " << i << ":\n";
            for (auto& par : cubetas[i]) {
                cout << par.clave << " --> " << par.valor << "\n";
            }
            cout << "End bucket\n";
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int casos_de_prueba;
    if (!(cin >> casos_de_prueba)) return 0;

    while (casos_de_prueba--) {
        // En cada tablero usamos un mapa nuevo
        my_map<string, int> registro_vistos(100);
        bool es_valido = true;

        for (int fila = 0; fila < 9; ++fila) {
            string linea;
            cin >> linea; // Leemos la fila completa de 9 caracteres

            for (int columna = 0; columna < 9; ++columna) {
                char valor = linea[columna];

                // Las casillas vacías se ignoran
                if (valor == '.') continue;

                // Formamos las 3 claves únicas para fila, columna y caja 3x3
                string clave_fila = "f_" + to_string(fila) + "_" + valor;
                string clave_columna = "c_" + to_string(columna) + "_" + valor;
                string clave_caja = "b_" + to_string(fila / 3) + "_" + to_string(columna / 3) + "_" + valor;

                // Si cualquiera de las 3 marcas ya existía en el mapa, violó las reglas
                if (registro_vistos.has_key(clave_fila) ||
                    registro_vistos.has_key(clave_columna) ||
                    registro_vistos.has_key(clave_caja)) {
                    es_valido = false;
                    }

                // Registramos las tres marcas en el mapa
                registro_vistos[clave_fila] = 1;
                registro_vistos[clave_columna] = 1;
                registro_vistos[clave_caja] = 1;
            }
        }

        if (es_valido) {
            cout << "SI\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}

//PARA INSERTAR (dentro de un for): m[nums[i]] = 1;
/*

m[clave] = 1; → "Solo quiero saber si esta clave existe".
m[clave] = i; → "Necesito recordar la posición de esta clave".
m[clave]++; → "Necesito contar cuántas veces aparece esta clave".


SIEMPRE EN CODEFORCES PARA LEER DENTRO DEL FOR:
int x;
cin >> x;
*/