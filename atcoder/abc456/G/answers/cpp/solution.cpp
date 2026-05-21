#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int MOD = 998244353;

vector<long long> fact;
vector<long long> invFact;
vector<long long> power_of_2;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = res * base % MOD;
        base = base * base % MOD;
        exp /= 2;
    }
    return res;
}

void precompute(int n) {
    fact.resize(n + 5, 1);
    invFact.resize(n + 5, 1);
    power_of_2.resize(n + 5, 1);
    
    for (int i = 1; i <= n + 4; ++i) {
        fact[i] = fact[i - 1] * i % MOD;
        power_of_2[i] = power_of_2[i - 1] * 2 % MOD;
    }
    
    invFact[n + 4] = power(fact[n + 4], MOD - 2);
    for (int i = n + 3; i >= 0; --i) {
        invFact[i] = invFact[i + 1] * (i + 1) % MOD;
    }
}

long long C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

long long compute_f(int L, int x) {
    if (x >= L) return power_of_2[L];
    long long ans = 0;
    int limit = (L + 1) / (x + 2);
    for (int k = 0; k <= limit; ++k) {
        int M = L - k * (x + 1);
        long long term1 = (M >= k) ? C(M, k) * power_of_2[M - k] % MOD : 0;
        long long term2 = (M >= k - 1) ? C(M, k - 1) * power_of_2[M - k + 1] % MOD : 0;
        long long S = (term1 + term2) % MOD;
        if (k % 2 == 1) {
            ans = (ans - S + MOD) % MOD;
        } else {
            ans = (ans + S) % MOD;
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    if (!(cin >> N)) return 0;
    string S;
    if (!(cin >> S)) return 0;
    
    precompute(N + 5);
    
    vector<int> p;
    p.push_back(-1);
    for (int i = 0; i < N; ++i) {
        if (S[i] == 'x') {
            p.push_back(i);
        }
    }
    p.push_back(N);
    
    vector<int> cnt(N + 2, 0);
    vector<int> unique_lengths;
    for (size_t i = 0; i < p.size() - 1; ++i) {
        int L = p[i + 1] - p[i] - 1;
        if (L > 0) {
            if (cnt[L] == 0) {
                unique_lengths.push_back(L);
            }
            cnt[L]++;
        }
    }
    
    sort(unique_lengths.begin(), unique_lengths.end());
    
    vector<long long> sum_L(N + 2, 0);
    long long curr_sum = 0;
    for (int L = 0; L <= N + 1; ++L) {
        curr_sum += (long long)cnt[L] * L;
        sum_L[L] = curr_sum;
    }
    
    vector<long long> F(N + 1, 1);
    for (int x = 0; x <= N; ++x) {
        F[x] = power_of_2[sum_L[x]];
        for (int L : unique_lengths) {
            if (L <= x) continue;
            long long val = compute_f(L, x);
            F[x] = F[x] * power(val, cnt[L]) % MOD;
        }
    }
    
    for (int k = 1; k <= N; ++k) {
        long long ans = (F[k] - F[k - 1] + MOD) % MOD;
        cout << ans << "\n";
    }
    
    return 0;
}
