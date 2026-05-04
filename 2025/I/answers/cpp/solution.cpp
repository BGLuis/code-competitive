#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

int N, M;
vector<int> adj[1005];
int sensor_id[1005];

struct State {
    bitset<505> forbidden;
};

vector<bitset<505>> failed_masks[1005];
vector<int> path;

bool is_forbidden(const bitset<505>& current, const bitset<505>& failed) {
    return (current & failed) == failed;
}

bool dfs(int u, bitset<505> forbidden) {
    if (u == 2 * N) {
        path.push_back(u);
        return true;
    }
    
    for (const auto& failed : failed_masks[u]) {
        if ((forbidden & failed) == failed) return false;
    }

    // Try neighbors
    for (int v : adj[u]) {
        int s = sensor_id[v];
        if (forbidden.test(s)) continue;
        
        bitset<505> next_forbidden = forbidden;
        next_forbidden.set(s);
        
        if (dfs(v, next_forbidden)) {
            path.push_back(u);
            return true;
        }
    }
    
    // Before adding, remove any existing failed masks that are supersets of this one
    // (though in DFS we usually find smaller failed masks later? No, usually larger).
    // Actually, just add it.
    failed_masks[u].push_back(forbidden);
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;
    for (int i = 1; i <= N; i++) {
        sensor_id[i] = i;
        sensor_id[i + N] = i;
    }
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    bitset<505> initial_forbidden;
    initial_forbidden.set(sensor_id[1]);
    
    if (dfs(1, initial_forbidden)) {
        reverse(path.begin(), path.end());
        cout << path.size() << endl;
        for (int i = 0; i < (int)path.size(); i++) {
            cout << path[i] << (i == (int)path.size() - 1 ? "" : " ");
        }
        cout << endl;
    } else {
        cout << "*" << endl;
    }

    return 0;
}
