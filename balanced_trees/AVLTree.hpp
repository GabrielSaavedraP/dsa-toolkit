#include <iostream>
using namespace std;

// AVL Tree: BST donde |FB(v)| <= 1 en todo nodo, con h(nulo) = -1 y
// FB(v) = h(v.izq) - h(v.der). Funciona con cualquier tipo que tenga
// operator< (y operator<< si quieres imprimir): int, long long, char, string, structs.
template<typename data_type>
struct AVL {

    struct TreeNode {
        data_type data;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        int height;

        TreeNode(const data_type& data = data_type(),
                    TreeNode* left = nullptr,
                    TreeNode* right = nullptr,
                    TreeNode* parent = nullptr) :
                    data(data), left(left), right(right), parent(parent), height(0) {}
    };

    TreeNode* root;

    AVL() {
        root = nullptr;
    }

    // ========================================================
    // ALTURA Y FACTOR DE BALANCE
    // ========================================================

    int mayor(int a, int b) {
        return a > b ? a : b;
    }

    // h(nulo) = -1
    int altura(TreeNode* u) {
        return u == nullptr ? -1 : u -> height;
    }

    void actualizar_altura(TreeNode* u) {
        u -> height = 1 + mayor(altura(u -> left), altura(u -> right));
    }

    // FB(v) = h(v.izq) - h(v.der)
    int factor_balance(TreeNode* u) {
        return u == nullptr ? 0 : altura(u -> left) - altura(u -> right);
    }

    // Verifica la propiedad AVL: |FB| <= 1 en todo nodo
    bool es_avl() {
        return es_avl(root);
    }

    bool es_avl(TreeNode* u) {
        if (u == nullptr) return true;
        int fb = factor_balance(u);
        if (fb > 1 or fb < -1) return false;
        return es_avl(u -> left) and es_avl(u -> right);
    }

    // ========================================================
    // ROTACIONES (O(1), conservan el orden inorder)
    // ========================================================

    // Reemplaza a u por v como hijo de u->parent (o como raíz). No borra u.
    void reemplazar_hijo(TreeNode* u, TreeNode* v) {
        if (u -> parent == nullptr) {
            root = v;
        }
        else if (u -> parent -> left == u) {
            u -> parent -> left = v;
        }
        else {
            u -> parent -> right = v;
        }
        if (v != nullptr) {
            v -> parent = u -> parent;
        }
    }

    // rotar der. en y:  (x.izq = A, x.der = B, y.der = C)
    //   y            x
    //   x C   -->    A y
    //   A B            B C
    // Devuelve la nueva raíz del subárbol (x)
    TreeNode* rotar_derecha(TreeNode* y) {
        TreeNode* x = y -> left;
        TreeNode* B = x -> right;

        reemplazar_hijo(y, x);
        x -> right = y;
        y -> parent = x;
        y -> left = B;
        if (B != nullptr) B -> parent = y;

        actualizar_altura(y);
        actualizar_altura(x);
        return x;
    }

    // Espejo de rotar_derecha. Devuelve la nueva raíz del subárbol (y)
    TreeNode* rotar_izquierda(TreeNode* x) {
        TreeNode* y = x -> right;
        TreeNode* B = y -> left;

        reemplazar_hijo(x, y);
        y -> left = x;
        x -> parent = y;
        x -> right = B;
        if (B != nullptr) B -> parent = x;

        actualizar_altura(x);
        actualizar_altura(y);
        return y;
    }

    // ========================================================
    // REBALANCEO
    // ========================================================

    // Arregla u si |FB(u)| = 2. Devuelve la raíz del subárbol resultante.
    TreeNode* rebalancear(TreeNode* u) {
        actualizar_altura(u);
        int fb = factor_balance(u);

        if (fb > 1) {
            // LR: primero rotar izq. en el hijo, luego der. en u
            if (factor_balance(u -> left) < 0) {
                rotar_izquierda(u -> left);
            }
            return rotar_derecha(u);          // LL
        }
        if (fb < -1) {
            // RL: primero rotar der. en el hijo, luego izq. en u
            if (factor_balance(u -> right) > 0) {
                rotar_derecha(u -> right);
            }
            return rotar_izquierda(u);        // RR
        }
        return u;
    }

    // Sube desde u hasta la raíz rebalanceando cada ancestro
    void rebalancear_hacia_arriba(TreeNode* u) {
        while (u != nullptr) {
            u = rebalancear(u);
            u = u -> parent;
        }
    }

    // ========================================================
    // OPERACIONES (iguales a las del BST)
    // ========================================================

    TreeNode* find(const data_type& key) {
        TreeNode* current = root;
        while (current != nullptr) {
            if (key < current -> data) {
                current = current -> left;
            }
            else if (current -> data < key) {
                current = current -> right;
            }
            else {
                return current;
            }
        }
        return nullptr;
    }

    bool search(const data_type& key) {
        return find(key) != nullptr;
    }

    TreeNode* min_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        TreeNode* current = u;
        while (current -> left != nullptr) {
            current = current -> left;
        }
        return current;
    }

    TreeNode* max_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        TreeNode* current = u;
        while (current -> right != nullptr) {
            current = current -> right;
        }
        return current;
    }

    TreeNode* min_element() {
        return min_element(root);
    }

    TreeNode* max_element() {
        return max_element(root);
    }

    TreeNode* successor(TreeNode* x) {
        if (x -> right != nullptr) {
            return min_element(x -> right);
        }
        TreeNode* y = x -> parent;
        while (y != nullptr and y -> right == x) {
            x = y;
            y = y -> parent;
        }
        return y;
    }

    TreeNode* predecessor(TreeNode* x) {
        if (x -> left != nullptr) {
            return max_element(x -> left);
        }
        TreeNode* y = x -> parent;
        while (y != nullptr and y -> left == x) {
            x = y;
            y = y -> parent;
        }
        return y;
    }

    // Insertar como en un BST, luego rebalancear desde el padre del nodo nuevo
    void insert(const data_type& value) {
        if (root == nullptr) {
            root = new TreeNode(value);
            return;
        }
        TreeNode* current = root;
        while (current != nullptr) {
            if (not (current -> data < value) and not (value < current -> data)) return;
            if (current -> data < value) {
                if (current -> right != nullptr) {
                    current = current -> right;
                }
                else {
                    current -> right = new TreeNode(value, nullptr, nullptr, current);
                    break;
                }
            }
            else {
                if (current -> left != nullptr) {
                    current = current -> left;
                }
                else {
                    current -> left = new TreeNode(value, nullptr, nullptr, current);
                    break;
                }
            }
        }
        rebalancear_hacia_arriba(current);
    }

    void transplant(TreeNode* u, TreeNode* v) {
        reemplazar_hijo(u, v);
        delete u;
    }

    // Eliminar como en un BST, luego rebalancear desde el padre del nodo
    // que se quitó físicamente (puede haber más de una rotación)
    void erase(TreeNode* u) {
        TreeNode* p;
        if (u -> left == nullptr and u -> right == nullptr) {
            p = u -> parent;
            transplant(u, nullptr);
        }
        else if (u -> left != nullptr and u -> right != nullptr) {
            TreeNode* succ = successor(u);
            u -> data = succ -> data;
            p = succ -> parent;
            transplant(succ, succ -> right);
        }
        else {
            p = u -> parent;
            if (u -> left) transplant(u, u -> left);
            else transplant(u, u -> right);
        }
        rebalancear_hacia_arriba(p);
    }

    void erase(const data_type& key) {
        TreeNode* u = find(key);
        if (u != nullptr) erase(u);
    }

    // ========================================================
    // RECORRIDOS
    // ========================================================

    void print_inorder() {
        print_subtree_inorder(root);
        cout << endl;
    }

    void print_subtree_inorder(TreeNode* u) {
        if (u == nullptr) return;
        print_subtree_inorder(u -> left);
        cout << u -> data << " ";
        print_subtree_inorder(u -> right);
    }

    void print_preorder() {
        print_subtree_preorder(root);
        cout << endl;
    }

    void print_subtree_preorder(TreeNode* u) {
        if (u == nullptr) return;
        cout << u -> data << " ";
        print_subtree_preorder(u -> left);
        print_subtree_preorder(u -> right);
    }

    void print_postorder() {
        print_subtree_postorder(root);
        cout << endl;
    }

    void print_subtree_postorder(TreeNode* u) {
        if (u == nullptr) return;
        print_subtree_postorder(u -> left);
        print_subtree_postorder(u -> right);
        cout << u -> data << " ";
    }
};
