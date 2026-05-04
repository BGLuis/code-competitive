#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    long long p;
    long long e;
} Factor;

int compare_factors(const void *a, const void *b) {
    long long diff = ((Factor *)a)->p - ((Factor *)b)->p;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

Factor factors[200];
int factor_count = 0;

void add_factor(long long p, long long e) {
    for (int i = 0; i < factor_count; i++) {
        if (factors[i].p == p) {
            factors[i].e += e;
            return;
        }
    }
    factors[factor_count].p = p;
    factors[factor_count].e = e;
    factor_count++;
}

void factorize(long long n) {
    for (long long i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            add_factor(i, 1);
            n /= i;
        }
    }
    if (n > 1) add_factor(n, 1);
}

int main() {
    char s[1005];
    if (scanf("%s", s) != 1) return 0;
    int n = strlen(s);

    if (n <= 15) {
        long long val = atoll(s);
        factorize(val);
    } else {
        char prefix[15];
        strncpy(prefix, s, 14);
        prefix[14] = '\0';
        long long p = atoll(prefix);
        int l = n - 14;
        factorize(p);
        add_factor(2, l);
        add_factor(5, l);
    }

    qsort(factors, factor_count, sizeof(Factor), compare_factors);

    int final_count = 0;
    for (int i = 0; i < factor_count; i++) if (factors[i].e > 0) final_count++;

    printf("%d\n", final_count);
    for (int i = 0; i < factor_count; i++) {
        if (factors[i].e > 0) {
            printf("%lld %lld\n", factors[i].p, factors[i].e);
        }
    }

    return 0;
}
