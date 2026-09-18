#include <iostream>
#include <vector>
using namespace std;



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



int main() {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);

        string J, S;
        if (!(cin >> J >> S)) return 0;

        my_map<string, int> m(2 * J.size());

        for (int i = 0; i < (int)J.size(); ++i) {
            string letra = string(1, J[i]);
            m[letra] = 1;
        }

        int contador_joyas = 0;

        for (int i = 0; i < (int)S.size(); ++i) {
            string piedra = string(1, S[i]);
            if (m.has_key(piedra)) {
                contador_joyas++;
            }
        }

    cout << contador_joyas << "\n";

    return 0;
}
