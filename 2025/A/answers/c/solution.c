#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int parent[1005];
bool used_edges_mat[1005][1005];
bool has_edge[1005];

int find(int i) {
    if (parent[i] == i) return i;
    return parent[i] = find(parent[i]);
}

void unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) parent[root_i] = root_j;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    int P;
    if (scanf("%d", &P) != 1) return 0;
    int *L = (int *)malloc(P * sizeof(int));
    for (int i = 0; i < P; i++) scanf("%d", &L[i]);

    int Q;
    if (scanf("%d", &Q) != 1) return 0;
    int *R = (int *)malloc(Q * sizeof(int));
    for (int i = 0; i < Q; i++) scanf("%d", &R[i]);

    if (N % 2 == 0) {
        if (N > 2) {
            printf("N\n");
        } else {
            // N = 2, S = 2.
            if (P + Q > 2) { printf("N\n"); }
            else if (P >= 2 && L[0] == L[1]) { printf("N\n"); }
            else if (Q >= 2 && R[0] == R[1]) { printf("N\n"); }
            else if (P >= 1 && Q >= 1 && L[0] == R[0]) { printf("N\n"); }
            else { printf("Y\n"); }
        }
        free(L); free(R); return 0;
    }

    long long S = (long long)N * (N - 1) / 2 + 1;
    if (P + Q > S) {
        printf("N\n");
        free(L); free(R); return 0;
    }

    for (int i = 0; i < P - 1; i++) if (L[i] == L[i+1]) { printf("N\n"); free(L); free(R); return 0; }
    for (int i = 0; i < Q - 1; i++) if (R[i] == R[i+1]) { printf("N\n"); free(L); free(R); return 0; }

    if (P > 0 && Q > 0 && L[0] != R[Q-1]) {
        printf("N\n");
        free(L); free(R); return 0;
    }

    int used_count = 0;
    for (int i = 0; i < P - 1; i++) {
        int u = L[i], v = L[i+1];
        if (u > v) swap(&u, &v);
        if (used_edges_mat[u][v]) { printf("N\n"); free(L); free(R); return 0; }
        used_edges_mat[u][v] = true;
        used_count++;
    }
    for (int i = 0; i < Q - 1; i++) {
        int u = R[i], v = R[i+1];
        if (u > v) swap(&u, &v);
        if (used_edges_mat[u][v]) { printf("N\n"); free(L); free(R); return 0; }
        used_edges_mat[u][v] = true;
        used_count++;
    }

    long long E_prime = (S - 1) - used_count;
    int u_target, v_target;
    if (P > 0 && Q > 0) { u_target = L[P-1]; v_target = R[0]; }
    else if (P > 0) { u_target = L[P-1]; v_target = L[0]; }
    else if (Q > 0) { u_target = R[Q-1]; v_target = R[0]; }
    else { u_target = 1; v_target = 1; }

    if (E_prime == 0) {
        if (u_target == v_target) printf("Y\n"); else printf("N\n");
        free(L); free(R); return 0;
    }

    for (int i = 1; i <= N; i++) parent[i] = i;
    for (int i = 1; i <= N; i++) {
        for (int j = i + 1; j <= N; j++) {
            if (!used_edges_mat[i][j]) {
                unite(i, j);
                has_edge[i] = has_edge[j] = true;
            }
        }
    }

    int edge_comp = -1;
    for (int i = 1; i <= N; i++) {
        if (has_edge[i]) {
            int c = find(i);
            if (edge_comp == -1) edge_comp = c;
            else if (edge_comp != c) { printf("N\n"); free(L); free(R); return 0; }
        }
    }

    if (edge_comp != -1) {
        if (find(u_target) != edge_comp || find(v_target) != edge_comp) {
            printf("N\n");
            free(L); free(R); return 0;
        }
    }

    printf("Y\n");
    free(L); free(R);
    return 0;
}
