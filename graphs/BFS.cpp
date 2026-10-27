#include<vector>
#include<string>
#include<queue>
#include<iostream>
using namespace::std;

int BFS(int sx, int sy, int tx, int ty, int n, int m, vector<string> &s) {
    vector<int> dx = {1, -1, 0, 0};
    vector<int> dy = {0, 0, 1, -1};
    vector<vector<int>> D(n, vector<int>(m, -1));
    queue<pair<int, int>> Q;
    Q.emplace(sx, sy);
    D[sx][sy] = 0;
    while (not Q.empty()) {
        int ux, uy;
        tie(ux, uy) = Q.front();
        Q.pop();
        for (int k = 0; k < 4; ++k) {
            int vx = ux + dx[k];
            int vy = uy + dy[k];
            if (vx < 0 or vy < 0 or vx >= n or vy >= m) continue;
            if (s[vx][vy] == '#') continue;
            if (D[vx][vy] != -1) continue;
            D[vx][vy] = D[ux][uy] + 1;
            Q.emplace(vx, vy);
        }
    }
    return D[tx][ty];
}
