#include <stdio.h>
#include <stdlib.h>

const int MOD = 998244353;

int main() {
    long long N;
    int K;
    if (scanf("%lld %d", &N, &K) != 2) return 0;

    int *dp = (int *)malloc((K + 1) * sizeof(int));
    for (int q = 1; q <= K; q++) dp[q] = 1;
    dp[0] = 0;

    int limit = (N < (long long)K) ? (int)N : K;

    for (int i = 2; i <= limit; i++) {
        int *new_dp_diff = (int *)calloc((K + 2), sizeof(int));
        for (int q_old = 1; q_old <= K; q_old++) {
            if (dp[q_old] == 0) continue;
            
            int low = q_old - (q_old - 1) / i;
            int high = q_old + (K - q_old) / i;
            
            if (low <= K) {
                new_dp_diff[low] = (new_dp_diff[low] + dp[q_old]) % MOD;
                if (high + 1 <= K) {
                    new_dp_diff[high + 1] = (new_dp_diff[high + 1] - dp[q_old] + MOD) % MOD;
                }
            }
        }
        int curr = 0;
        for (int q = 1; q <= K; q++) {
            curr = (curr + new_dp_diff[q]) % MOD;
            dp[q] = curr;
        }
        free(new_dp_diff);
    }

    long long total = 0;
    for (int q = 1; q <= K; q++) {
        total = (total + dp[q]) % MOD;
    }
    printf("%lld\n", total);

    free(dp);
    return 0;
}
