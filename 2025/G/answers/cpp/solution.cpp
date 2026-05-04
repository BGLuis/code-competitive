#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef __int128_t int128;
typedef long long ll;

const int128 INF_128 = (int128)2e30; // Large enough for any flow

int N, M, P_cables;
ll K_limit;
ll C_cap[305][305];
int R_rate[305][305];
bool horizontal_cable[305][305]; // (i, j) - (i, j+1)
bool vertical_cable[305][305];   // (i, j) - (i+1, j)

struct Edge {
    int to;
    ll static_weight; // -1 if dynamic node capacity
    int node_r, node_c;
};

vector<vector<Edge>> adj;
int S_star = 0, T_sink = 1;

int get_region(int r, int c) {
    if (r == 0 || c == M) return S_star;
    if (r == N || c == 0) return T_sink;
    return 2 + (r - 1) * (M - 1) + (c - 1);
}

int128 solve_flow(ll t) {
    int total_nodes = adj.size();
    vector<int128> dist(total_nodes, INF_128);
    priority_queue<pair<int128, int>, vector<pair<int128, int>>, greater<pair<int128, int>>> pq;

    dist[S_star] = 0;
    pq.push({0, S_star});

    while (!pq.empty()) {
        int128 d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == T_sink) return d;

        for (auto &e : adj[u]) {
            int128 w;
            if (e.static_weight == -1) {
                w = (int128)C_cap[e.node_r][e.node_c] + (int128)t * R_rate[e.node_r][e.node_c];
            } else {
                w = e.static_weight;
            }
            if (dist[u] + w < dist[e.to]) {
                dist[e.to] = dist[u] + w;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    return dist[T_sink];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M >> P_cables >> K_limit)) return 0;

    for (int i = 1; i <= N; i++) for (int j = 1; j <= M; j++) cin >> C_cap[i][j];
    for (int i = 1; i <= N; i++) for (int j = 1; j <= M; j++) cin >> R_rate[i][j];

    for (int i = 0; i < P_cables; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 == x2) {
            horizontal_cable[x1][min(y1, y2)] = true;
        } else {
            vertical_cable[min(x1, x2)][y1] = true;
        }
    }

    int face_count = (N - 1) * (M - 1);
    int node_count = N * M;
    adj.resize(2 + face_count + node_count);

    // 1. Dual edges for missing cables
    // Vertical grid edges (i, j) - (i+1, j) separate face(i, j-1) and face(i, j)
    for (int i = 1; i < N; i++) {
        for (int j = 1; j <= M; j++) {
            if (!vertical_cable[i][j]) {
                int r1 = get_region(i, j - 1);
                int r2 = get_region(i, j);
                if (r1 != r2) {
                    adj[r1].push_back({r2, 0, 0, 0});
                    adj[r2].push_back({r1, 0, 0, 0});
                }
            }
        }
    }
    // Horizontal grid edges (i, j) - (i, j+1) separate face(i-1, j) and face(i, j)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j < M; j++) {
            if (!horizontal_cable[i][j]) {
                int r1 = get_region(i - 1, j);
                int r2 = get_region(i, j);
                if (r1 != r2) {
                    adj[r1].push_back({r2, 0, 0, 0});
                    adj[r2].push_back({r1, 0, 0, 0});
                }
            }
        }
    }

    // 2. Dual nodes for grid nodes
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            int v_gate = 2 + face_count + (i - 1) * M + (j - 1);
            vector<int> regions;
            regions.push_back(get_region(i - 1, j - 1));
            regions.push_back(get_region(i - 1, j));
            regions.push_back(get_region(i, j - 1));
            regions.push_back(get_region(i, j));
            sort(regions.begin(), regions.end());
            regions.erase(unique(regions.begin(), regions.end()), regions.end());

            for (int r : regions) {
                adj[r].push_back({v_gate, -1, i, j});
                adj[v_gate].push_back({r, 0, 0, 0});
            }
        }
    }

    ll low = 0, high = K_limit;
    while (high - low > 3) {
        ll m1 = low + (high - low) / 3;
        ll m2 = high - (high - low) / 3;
        if (solve_flow(m1) < solve_flow(m2)) low = m1;
        else high = m2;
    }

    int128 max_f = 0;
    for (ll t = low; t <= high; t++) {
        int128 cur = solve_flow(t);
        if (cur > max_f) max_f = cur;
    }

    string s = "";
    if (max_f == 0) s = "0";
    else {
        while (max_f > 0) {
            s += (char)('0' + (ll)(max_f % 10));
            max_f /= 10;
        }
        reverse(s.begin(), s.end());
    }
    cout << s << endl;

    return 0;
}
