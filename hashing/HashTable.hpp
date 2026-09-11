#include <iostream>
#include <vector>
using namespace std;

// Tabla hash con encadenamiento (chaining), usando solo vector como STL permitido.
// Cada cubeta es un vector<Par>; colisiones se resuelven agregando a la misma cubeta.
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

    // Borra intercambiando con el último elemento de la cubeta (O(1), evita shift)
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

    bool has_key(const TipoClave& clave) const {
        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        return j != (int)cubetas[i].size();
    }

    // Hash polinomial dígito por dígito. Normaliza negativos (si no, caen todos en cubeta 0).
    int _hash(TipoClave clave) const {
        const int BASE = 311;
        const int MOD = 1e9 + 7;

        if (clave < 0) clave = -clave;

        int h = 0;
        while (clave > 0) {
            int digito = clave % 10;
            h = (1LL * h * BASE + (digito + 1)) % MOD;
            clave /= 10;
        }

        return h % num_cubetas;
    }

    // Duplica cubetas y reinserta todo (evita que una cubeta crezca sin control)
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

    int size() const { return total; }
    bool empty() const { return total == 0; }

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