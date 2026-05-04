#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int N, M;
int adj[1005][1005];
int adj_size[1005];
int sensor_id[1005];

typedef struct {
    uint64_t bits[8]; // 512 bits to cover N <= 500
} Mask;

typedef struct MaskNode {
    Mask m;
    struct MaskNode *next;
} MaskNode;

MaskNode *failed_masks[1005];
int path_arr[1005];
int path_len = 0;

void set_bit(Mask *m, int bit) {
    m->bits[bit / 64] |= (1ULL << (bit % 64));
}

bool test_bit(const Mask *m, int bit) {
    return (m->bits[bit / 64] & (1ULL << (bit % 64))) != 0;
}

bool is_subset(const Mask *current, const Mask *failed) {
    for (int i = 0; i < 8; i++) {
        if ((current->bits[i] & failed->bits[i]) != failed->bits[i]) return false;
    }
    return true;
}

bool dfs(int u, Mask forbidden) {
    if (u == 2 * N) {
        path_arr[path_len++] = u;
        return true;
    }

    MaskNode *curr = failed_masks[u];
    while (curr) {
        if (is_subset(&forbidden, &curr->m)) return false;
        curr = curr->next;
    }

    for (int i = 0; i < adj_size[u]; i++) {
        int v = adj[u][i];
        int s = sensor_id[v];
        if (test_bit(&forbidden, s)) continue;

        Mask next_forbidden = forbidden;
        set_bit(&next_forbidden, s);

        if (dfs(v, next_forbidden)) {
            path_arr[path_len++] = u;
            return true;
        }
    }

    MaskNode *node = malloc(sizeof(MaskNode));
    node->m = forbidden;
    node->next = failed_masks[u];
    failed_masks[u] = node;
    return false;
}

int main() {
    if (scanf("%d %d", &N, &M) != 2) return 0;
    for (int i = 1; i <= N; i++) {
        sensor_id[i] = i;
        sensor_id[i + N] = i;
    }
    for (int i = 0; i < M; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) break;
        adj[u][adj_size[u]++] = v;
    }

    Mask initial;
    for(int i=0; i<8; i++) initial.bits[i] = 0;
    set_bit(&initial, sensor_id[1]);

    if (dfs(1, initial)) {
        printf("%d\n", path_len);
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%d%c", path_arr[i], (i == 0 ? '\n' : ' '));
        }
    } else {
        printf("*\n");
    }

    return 0;
}
