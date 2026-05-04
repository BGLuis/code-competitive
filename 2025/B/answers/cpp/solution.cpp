#include <iostream>
#include <vector>

using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    int K;
    if (!(cin >> N >> K)) return 0;

    // dp[q] is the number of sequences of length i with sum/i = q
    vector<int> dp(K + 1, 1);
    dp[0] = 0;

    int limit = (N < (long long)K) ? (int)N : K;

    for (int i = 2; i <= limit; ++i) {
        vector<int> new_dp_diff(K + 2, 0);
        for (int q_old = 1; q_old <= K; ++q_old) {
            if (dp[q_old] == 0) continue;
            
            // For each q_old, we need b_i = q_old + m*i in [1, K]
            // q_new = q_old + m
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
        for (int q = 1; q <= K; ++q) {
            curr = (curr + new_dp_diff[q]) % MOD;
            dp[q] = curr;
        }
    }

    long long total = 0;
    for (int q = 1; q <= K; ++q) {
        total = (total + dp[q]) % MOD;
    }
    cout << total << endl;

    return 0;
}
