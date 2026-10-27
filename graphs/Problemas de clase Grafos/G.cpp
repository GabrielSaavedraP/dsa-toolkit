#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
using namespace std;

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int edges = 0;
    int A, B, x, y;
    cin >> A >> B >> x >> y;
    int target_in_degree = 0;
    for (int i = 0; i < A; i++) {
        for (int j = 0; j < B; j++) {
            int new_i = A;
            int new_j = B;
            if (new_i != i or new_j != j) {
                ++edges;
                if (new_i == x or new_j == y) {
                    ++target_in_degree;
                }
            }
            new_i = i;
            new_j = B;
            if (new_i != i or new_j != j) {
                ++edges;
                if (new_i == x or new_j == y) {
                    ++target_in_degree;
                }
            }
        }
    }
    cout << edges << " " << target_in_degree << '\n';

    return 0;
}

