#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string S;
    if (!(cin >> S)) return 0;

    long long dp[3] = {0, 0, 0};
    long long MOD = 998244353;

    for (char c : S) {
        int idx = c - 'a';
        // The number of new valid subsequences ending at this position is:
        // 1 (subsequence consisting of just this character)
        // + sum of valid subsequences ending at previous positions with a different character.
        long long ways = (1 + dp[(idx + 1) % 3] + dp[(idx + 2) % 3]) % MOD;
        dp[idx] = (dp[idx] + ways) % MOD;
    }

    long long ans = (dp[0] + dp[1] + dp[2]) % MOD;
    cout << ans << "\n";

    return 0;
}
