#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
    int N;
    ll K;
    if (scanf("%d %lld", &N, &K) != 2) return 0;

    if (N == 1) {
        printf("1\n");
        return 0;
    }

    ll *rs = malloc(N * sizeof(ll));
    ll *cs = malloc(N * sizeof(ll));
    ll r_min = 2e9, r_max = -2e9, c_min = 2e9, c_max = -2e9;
    for (int i = 0; i < N; i++) {
        scanf("%lld %lld", &rs[i], &cs[i]);
        if (rs[i] < r_min) r_min = rs[i];
        if (rs[i] > r_max) r_max = rs[i];
        if (cs[i] < c_min) c_min = cs[i];
        if (cs[i] > c_max) c_max = cs[i];
    }

    ll h0 = r_max - r_min + 1;
    ll w0 = c_max - c_min + 1;
    ll ans = h0 * w0;

    for (int i = 0; i < N; i++) {
        ll r = rs[i], c = cs[i];
        ll au = r_max - r;
        ll ad = r - r_min;
        ll br = c_max - c;
        ll bl = c - c_min;

        ans = MAX(ans, (h0 + MAX(0LL, K - au)) * (w0 + MAX(0LL, K - br)));
        ans = MAX(ans, (h0 + MAX(0LL, K - au)) * (w0 + MAX(0LL, K - bl)));
        ans = MAX(ans, (h0 + MAX(0LL, K - ad)) * (w0 + MAX(0LL, K - br)));
        ans = MAX(ans, (h0 + MAX(0LL, K - ad)) * (w0 + MAX(0LL, K - bl)));
    }

    ll x_opt = (w0 + K - h0) / 2;
    ll candidates[] = {0LL, K, x_opt, x_opt + 1};
    for (int i = 0; i < 4; i++) {
        ll x = candidates[i];
        if (x >= 0 && x <= K) {
            ans = MAX(ans, (h0 + x) * (w0 + K - x));
        }
    }

    printf("%lld\n", ans);

    free(rs); free(cs);
    return 0;
}
