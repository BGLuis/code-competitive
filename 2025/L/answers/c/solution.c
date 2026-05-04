#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double l, r;
} Interval;

int compare_intervals(const void *a, const void *b) {
    Interval *i1 = (Interval *)a;
    Interval *i2 = (Interval *)b;
    if (fabs(i1->l - i2->l) > 1e-11) {
        if (i1->l < i2->l) return -1;
        return 1;
    }
    if (i1->r < i2->r) return -1;
    if (i1->r > i2->r) return 1;
    return 0;
}

int compare_double(const void *a, const void *b) {
    double d1 = *(double *)a;
    double d2 = *(double *)b;
    if (fabs(d1 - d2) < 1e-11) return 0;
    if (d1 < d2) return -1;
    return 1;
}

int bit_arr[100005];
int m_size;

void update(int i, int val) {
    for (; i <= m_size; i += i & -i) bit_arr[i] += val;
}

int query(int i) {
    int res = 0;
    for (; i > 0; i -= i & -i) res += bit_arr[i];
    return res;
}

int find_first_ge(double *arr, int size, double target) {
    int low = 0, high = size - 1;
    int res = size;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] >= target - 1e-13) {
            res = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return res;
}

int main() {
    int N;
    double A, B;
    if (scanf("%d %lf %lf", &N, &A, &B) != 3) return 0;

    Interval *intervals = malloc(N * sizeof(Interval));
    int count = 0;
    for (int i = 0; i < N; i++) {
        double m, c;
        if (scanf("%lf %lf", &m, &c) != 2) break;
        // Solving Ax^2 - mx + (B - c) = 0 for intersection points
        double disc = m * m - 4.0 * A * (B - c);
        if (disc > 1e-11) {
            double sd = sqrt(disc);
            intervals[count].l = (m - sd) / (2.0 * A);
            intervals[count].r = (m + sd) / (2.0 * A);
            count++;
        }
    }

    if (count == 0) {
        printf("0\n");
        free(intervals);
        return 0;
    }

    qsort(intervals, count, sizeof(Interval), compare_intervals);

    double *allR = malloc(count * sizeof(double));
    for (int i = 0; i < count; i++) allR[i] = intervals[i].r;
    qsort(allR, count, sizeof(double), compare_double);

    int m_unique = 0;
    if (count > 0) {
        m_unique = 1;
        for (int i = 1; i < count; i++) {
            if (fabs(allR[i] - allR[m_unique - 1]) > 1e-11) {
                allR[m_unique++] = allR[i];
            }
        }
    }
    m_size = m_unique;

    long long ans = 0;
    double eps = 1e-11;
    for (int i = 0; i < count; i++) {
        // Interlacing condition counting using sweep-line
        int idxL = find_first_ge(allR, m_unique, intervals[i].l + eps);
        int idxR = find_first_ge(allR, m_unique, intervals[i].r - eps);
        
        if (idxR > idxL) {
            ans += query(idxR) - query(idxL);
        }
        
        int rIdx = find_first_ge(allR, m_unique, intervals[i].r - 1e-13);
        update(rIdx + 1, 1);
    }

    printf("%lld\n", ans);

    free(intervals); free(allR);
    return 0;
}
