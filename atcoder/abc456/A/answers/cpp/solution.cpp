#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }
};

bool used_edges_mat[1005][1005];

void solve() {
    int N;
    if (!(cin >> N)) return;

    int P;
    cin >> P;
    vector<int> L(P);
    for (int i = 0; i < P; ++i) cin >> L[i];

    int Q;
    cin >> Q;
    vector<int> R(Q);
    for (int i = 0; i < Q; ++i) cin >> R[i];

    if (N % 2 == 0) {
        if (N > 2) {
            cout << "N" << endl;
            return;
        } else {
            // N = 2, S = 2.
            if (P + Q > 2) { cout << "N" << endl; return; }
            if (P >= 2 && L[0] == L[1]) { cout << "N" << endl; return; }
            if (Q >= 2 && R[0] == R[1]) { cout << "N" << endl; return; }
            if (P >= 1 && Q >= 1 && L[0] == R[0]) { cout << "N" << endl; return; }
            cout << "Y" << endl;
            return;
        }
    }

    long long S = (long long)N * (N - 1) / 2 + 1;
    if (P + Q > S) {
        cout << "N" << endl;
        return;
    }

    for (int i = 0; i < P - 1; ++i) if (L[i] == L[i+1]) { cout << "N" << endl; return; }
    for (int i = 0; i < Q - 1; ++i) if (R[i] == R[i+1]) { cout << "N" << endl; return; }

    if (P > 0 && Q > 0 && L[0] != R.back()) {
        cout << "N" << endl;
        return;
    }

    int used_count = 0;
    for (int i = 0; i < P - 1; ++i) {
        int u = L[i], v = L[i+1];
        if (u > v) swap(u, v);
        if (used_edges_mat[u][v]) { cout << "N" << endl; return; }
        used_edges_mat[u][v] = true;
        used_count++;
    }
    for (int i = 0; i < Q - 1; ++i) {
        int u = R[i], v = R[i+1];
        if (u > v) swap(u, v);
        if (used_edges_mat[u][v]) { cout << "N" << endl; return; }
        used_edges_mat[u][v] = true;
        used_count++;
    }

    long long E_prime = (S - 1) - used_count;
    int u_target, v_target;
    if (P > 0 && Q > 0) { u_target = L.back(); v_target = R[0]; }
    else if (P > 0) { u_target = L.back(); v_target = L[0]; }
    else if (Q > 0) { u_target = R.back(); v_target = R[0]; }
    else { u_target = 1; v_target = 1; }

    if (E_prime == 0) {
        cout << (u_target == v_target ? "Y" : "N") << endl;
        return;
    }

    DSU dsu(N);
    vector<bool> has_edge(N + 1, false);
    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            if (!used_edges_mat[i][j]) {
                dsu.unite(i, j);
                has_edge[i] = has_edge[j] = true;
            }
        }
    }

    int edge_comp = -1;
    for (int i = 1; i <= N; ++i) {
        if (has_edge[i]) {
            int c = dsu.find(i);
            if (edge_comp == -1) edge_comp = c;
            else if (edge_comp != c) { cout << "N" << endl; return; }
        }
    }

    if (edge_comp != -1) {
        if (dsu.find(u_target) != edge_comp || dsu.find(v_target) != edge_comp) {
            cout << "N" << endl;
            return;
        }
    }

    cout << "Y" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
