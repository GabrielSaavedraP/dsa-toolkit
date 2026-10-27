#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
using namespace std;

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, q;
    cin >> n >> m >> q;
    map<pair<int, int>, int> freq;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        ++freq[make_pair(u, v)];
    }
    while (q--) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        cout << (freq.count(make_pair(u, v)) ? freq[make_pair(u, v)] : 0)<< '\n';
    }

    return 0;
}

