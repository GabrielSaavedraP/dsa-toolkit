#include <iostream>
using namespace std;

// my_map<key_type, value_type>: tabla hash con encadenamiento separado (separate chaining).
//
// CÓMO FUNCIONA:
// Mantenemos un arreglo de `m` "baldes" o buckets (chains[0..m-1]). Cada bucket es la
// CABEZA de su propia pequeña lista enlazada de nodos Entry. Para buscar/insertar una clave:
//   1) Se aplica la función hash a la clave para obtener el índice del bucket (0 a m-1) mediante _hash().
//   2) Se recorre la cadena de ese bucket comparando las claves una por una.
//   3) Si se encuentra, se retorna/actualiza. Si no se encuentra (se llega al final,
//      cur == nullptr), ocurre un "fallo de búsqueda". Para operator[], creamos un
//      nodo Entry completamente nuevo y lo colocamos como la nueva cabeza de esa cadena.
// Que dos claves distintas caigan en el MISMO bucket se llama "colisión". En lugar de evitarlo,
// simplemente dejamos que la cadena de ese bucket crezca. Esa es la idea del "encadenamiento separado":
// separar las claves que colisionan en su propia lista en lugar de sobrescribirse.
//
// CÓMO IMPLEMENTARLO (paso a paso):
//   a) Definir un nodo Entry pequeño: clave (key), valor (value) y un puntero al siguiente
//      Entry en la MISMA cadena del bucket (es un nodo de lista simplemente enlazada estándar).
//   b) Asignar `chains` como un arreglo de `m` punteros (Entry*), todos inicializados en nullptr
//      (m cadenas vacías).
//   c) _hash(key): convierte la clave en un número en el rango [0, m) mediante una función determinista.
//      Aquí se procesa dígito por dígito (ya que la clave es entera), multiplicando por un valor B
//      para distribuir mejor los distintos patrones de dígitos entre los buckets.
//   d) operator[](key): aplica la función hash para hallar el bucket correcto, luego RECORRE la cadena
//      con un puntero (`cur = chains[i]; while (cur && ...) cur = cur->next;`), comparando
//      `cur->key != key` en cada paso. Si recorre toda la lista sin encontrarlo, antepone (prepend)
//      un nuevo Entry a ese bucket (el `next` del nuevo nodo apunta a la cabeza antigua, y el bucket
//      pasa a apuntar al nuevo nodo). Anteponer es $O(1)$, no hay necesidad de ir hasta el final.
//   e) has_key(key): recorrido idéntico al anterior, pero solo retorna una respuesta booleana
//      (cur == nullptr significa "no encontrado").
//   f) Incluir un destructor que recorra y elimine cada Entry de cada bucket, además de hacer
//      `delete[] chains`. De lo contrario, cada nodo creado con `new` causaría una fuga de memoria.
//
// Complejidad: operator[] / has_key es $O(1)$ en promedio (asumiendo que el hash distribuye las
// claves de manera uniforme en los m buckets, manteniendo cadenas cortas), pero $O(n)$ en el peor
// caso si todas las claves colisionan en un solo bucket (por una mala función hash o un 'm' muy pequeño).
// Esta versión NO escala de tamaño automáticamente cuando las cadenas crecen. Si requieres esa
// garantía, se debe añadir un paso de rehash (duplicar 'm' y reinsertar todo cuando el factor de
// carga sea elevado).
template <typename key_type, typename value_type>
struct my_map {
    struct Entry {
        key_type key;
        value_type value;
        Entry* next;
        Entry(const key_type& k, const value_type& v, Entry* nxt)
            : key(k), value(v), next(nxt) {}
    };

    int m;
    Entry** chains; // Arreglo de m cabezas de listas enlazadas (nodos propios, sin usar std::vector)

    my_map(int m) : m(m) {
        chains = new Entry*[m];
        for (int i = 0; i < m; ++i) chains[i] = nullptr;
    }

    ~my_map() {
        for (int i = 0; i < m; ++i) {
            Entry* cur = chains[i];
            while (cur) { Entry* nxt = cur->next; delete cur; cur = nxt; }
        }
        delete[] chains;
    }

    value_type& operator[](const key_type& key) {
        int chain_position = _hash(key);
        Entry* cur = chains[chain_position];
        while (cur != nullptr && cur->key != key) cur = cur->next;
        if (cur == nullptr) {
            // Anteponer (prepend): O(1), no es necesario llegar al final de la cadena
            chains[chain_position] = new Entry(key, value_type(), chains[chain_position]);
            cur = chains[chain_position];
        }
        return cur->value;
    }

    bool has_key(const key_type& key) const {
        Entry* cur = chains[_hash(key)];
        while (cur != nullptr && cur->key != key) cur = cur->next;
        return cur != nullptr;
    }

    int _hash(key_type key) const {
        // Para enteros: mezcla dígito por dígito con un multiplicador B
        const int B = 311;
        const int MOD = 1e9 + 7;
        int hash_value = 0;
        while (key > 0) {
            int d = key % 10;
            hash_value = (1ll * hash_value * B + (d + 1)) % MOD;
            key /= 10;
        }
        return hash_value % m;
    }

    void print() {
        for (int i = 0; i < m; ++i) {
            cout << "Bucket " << i << ": " << endl;
            for (Entry* cur = chains[i]; cur != nullptr; cur = cur->next) {
                cout << cur->key << " --> " << cur->value << endl;
            }
            cout << "Fin de bucket" << endl;
        }
    }
};

int main() {
    my_map<int, long long> hash_table(20);
    for (int i = 0; i < 100; ++i) {
        hash_table[i] = 21;
    }
    hash_table.print();
    return 0;
}