#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef __int128_t int128;

typedef struct {
    int num, den;
} Rational;

typedef struct {
    Rational coord;
    int type;
    int id;
} Event;

typedef struct {
    int h, l, u;
    long long m;
} Cleaner;

int compare_rational(Rational a, Rational b) {
    long long p1 = (long long)a.num * b.den;
    long long p2 = (long long)b.num * a.den;
    if (p1 < p2) return -1;
    if (p1 > p2) return 1;
    return 0;
}

int compare_events(const void *a, const void *b) {
    Event *e1 = (Event *)a;
    Event *e2 = (Event *)b;
    int cmp = compare_rational(e1->coord, e2->coord);
    if (cmp != 0) return cmp;
    return e1->type - e2->type;
}

int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

long long bit_cnt[100005];
long long bit_sum[100005];
int nh;

void update(int idx, long long val_m, long long val_mh) {
    for (; idx <= nh; idx += idx & -idx) {
        bit_cnt[idx] += val_m;
        bit_sum[idx] += val_mh;
    }
}

long long get_min_sumH(long long s, int *distinct_h) {
    int idx = 0;
    long long current_cnt = 0;
    long long current_sum = 0;
    int i = 1;
    while (i <= nh) i <<= 1;
    for (i >>= 1; i > 0; i >>= 1) {
        int next_idx = idx + i;
        if (next_idx <= nh && current_cnt + bit_cnt[next_idx] < s) {
            idx = next_idx;
            current_cnt += bit_cnt[idx];
            current_sum += bit_sum[idx];
        }
    }
    long long remaining = s - current_cnt;
    current_sum += remaining * distinct_h[idx];
    return current_sum;
}

long long gcd_long(long long a, long long b) {
    while (b) { a %= b; long long tmp = a; a = b; b = tmp; }
    return a;
}

int main() {
    int N;
    long long S, K;
    if (scanf("%d %lld %lld", &N, &S, &K) != 3) return 0;

    Cleaner *cleaners = malloc(N * sizeof(Cleaner));
    Event *events = malloc(2 * N * sizeof(Event));
    int *h_vals = malloc(N * sizeof(int));
    int cleaner_count = 0;
    int event_count = 0;

    for (int i = 0; i < N; i++) {
        int h, l, u;
        scanf("%d %d %d", &h, &l, &u);
        long long m = K / h;
        if (m >= 1) {
            cleaners[cleaner_count].h = h;
            cleaners[cleaner_count].l = l;
            cleaners[cleaner_count].u = u;
            cleaners[cleaner_count].m = m;
            events[event_count].coord = (Rational){l, h};
            events[event_count].type = 0;
            events[event_count].id = cleaner_count;
            event_count++;
            events[event_count].coord = (Rational){u, h};
            events[event_count].type = 1;
            events[event_count].id = cleaner_count;
            event_count++;
            h_vals[cleaner_count] = h;
            cleaner_count++;
        }
    }

    if (cleaner_count == 0) {
        printf("*\n");
        free(cleaners); free(events); free(h_vals);
        return 0;
    }

    qsort(h_vals, cleaner_count, sizeof(int), compare_int);
    nh = 0;
    if (cleaner_count > 0) {
        nh = 1;
        for (int i = 1; i < cleaner_count; i++) {
            if (h_vals[i] != h_vals[nh - 1]) {
                h_vals[nh++] = h_vals[i];
            }
        }
    }

    qsort(events, event_count, sizeof(Event), compare_events);

    long long best_num = -1, best_den = 1;

    int i = 0;
    while (i < event_count) {
        Rational current_coord = events[i].coord;
        int j = i;
        while (j < event_count && compare_rational(events[j].coord, current_coord) == 0) {
            if (events[j].type == 0) {
                Cleaner c = cleaners[events[j].id];
                int *ptr = bsearch(&c.h, h_vals, nh, sizeof(int), compare_int);
                int h_idx = (int)(ptr - h_vals) + 1;
                update(h_idx, c.m, c.m * c.h);
            }
            j++;
        }

        long long total_cnt = 0;
        for (int idx = nh; idx > 0; idx -= idx & -idx) total_cnt += bit_cnt[idx];

        if (total_cnt >= S) {
            long long sumH = get_min_sumH(S, h_vals);
            long long cur_num = (long long)current_coord.num * sumH;
            long long cur_den = current_coord.den;
            if (best_num == -1 || (int128)cur_num * best_den < (int128)best_num * cur_den) {
                best_num = cur_num;
                best_den = cur_den;
            }
        }

        j = i;
        while (j < event_count && compare_rational(events[j].coord, current_coord) == 0) {
            if (events[j].type == 1) {
                Cleaner c = cleaners[events[j].id];
                int *ptr = bsearch(&c.h, h_vals, nh, sizeof(int), compare_int);
                int h_idx = (int)(ptr - h_vals) + 1;
                update(h_idx, -c.m, -c.m * c.h);
            }
            j++;
        }
        i = j;
    }

    if (best_num == -1) {
        printf("*\n");
    } else {
        long long common = gcd_long(best_num, best_den);
        printf("%lld %lld\n", best_num / common, best_den / common);
    }

    free(cleaners); free(events); free(h_vals);
    return 0;
}
