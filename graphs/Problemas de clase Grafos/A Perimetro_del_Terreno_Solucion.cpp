#include<bits/stdc++.h>
using namespace::std;

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    vector<string> s(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    vector<int> cnt_deg(5, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (s[i][j] == '.') continue;
            int cur_deg = 0;
            for (int k = 0; k < 4; ++k) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x < 0 or y < 0 or x >= n or y >= m) continue;
                if (s[x][y] == '#') {
                    ++cur_deg;
                }
            }
            ++cnt_deg[cur_deg];
        }
    }
    int res = 0;
    for (int i = 0; i <= 4; ++i) {
        res += cnt_deg[i] * (4 - i);
    }
    cout << res;
    for (int i = 0; i <= 4; ++i) {
        cout << " " << cnt_deg[i];
    }
    cout << '\n';
    return 0;
}