#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

void solve() {
    int N, M;
    if (!(cin >> N >> M)) return;

    vector<vector<int>> adj(N);
    for (int i = 0; i < N; ++i) {
        adj[i].push_back(i); // include self-loop
    }

    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int W;
    cin >> W;

    vector<string> S(N);
    for (int i = 0; i < N; ++i) {
        cin >> S[i];
    }

    // Remove duplicates in adj
    for (int i = 0; i < N; ++i) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }

    // out_deg[u][t] and removed[u][t]
    // We can flatten this to a 1D vector of size N * W for speed and cache-friendliness.
    vector<int> out_deg(N * W, 0);
    vector<bool> removed(N * W, false);

    for (int u = 0; u < N; ++u) {
        for (int t = 0; t < W; ++t) {
            if (S[u][t] == 'x') {
                removed[u * W + t] = true;
            }
        }
    }

    // Calculate initial out-degrees
    for (int u = 0; u < N; ++u) {
        for (int t = 0; t < W; ++t) {
            if (S[u][t] == 'o') {
                int t_next = (t + 1) % W;
                int deg = 0;
                for (int v : adj[u]) {
                    if (S[v][t_next] == 'o') {
                        deg++;
                    }
                }
                out_deg[u * W + t] = deg;
            }
        }
    }

    queue<pair<int, int>> Q;
    for (int u = 0; u < N; ++u) {
        for (int t = 0; t < W; ++t) {
            if (S[u][t] == 'o' && out_deg[u * W + t] == 0) {
                removed[u * W + t] = true;
                Q.push({u, t});
            }
        }
    }

    while (!Q.empty()) {
        auto [v, t] = Q.front();
        Q.pop();

        int t_prev = (t - 1 + W) % W;
        for (int u : adj[v]) {
            int state_idx = u * W + t_prev;
            if (!removed[state_idx]) {
                out_deg[state_idx]--;
                if (out_deg[state_idx] == 0) {
                    removed[state_idx] = true;
                    Q.push({u, t_prev});
                }
            }
        }
    }

    bool possible = false;
    for (int u = 0; u < N; ++u) {
        if (!removed[u * W]) {
            possible = true;
            break;
        }
    }

    if (possible) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    if (cin >> T) {
        while (T--) {
            solve();
        }
    }
    return 0;
}
