#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MOD = 998244353;
const int MAXV = 100005;

long long power(long long base, long long exp, int mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int spf[MAXV];
int phi[MAXV];
int mu[MAXV];
vector<int> primes;

void sieve() {
    mu[1] = 1;
    for (int i = 2; i < MAXV; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (p > spf[i] || (long long)i * p >= MAXV) break;
            spf[i * p] = p;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            } else {
                mu[i * p] = -mu[i];
            }
        }
    }
    phi[1] = 1;
    for (int i = 2; i < MAXV; ++i) {
        int temp = i;
        int res = i;
        while (temp > 1) {
            int p = spf[temp];
            res = res / p * (p - 1);
            while (temp % p == 0) temp /= p;
        }
        phi[i] = res;
    }
}

int f[MAXV];

void precompute_f() {
    for (int i = 1; i < MAXV; ++i) {
        int b = i;
        int n = 0, m = 0;
        while (b % 2 == 0) { b /= 2; n++; }
        while (b % 5 == 0) { b /= 5; m++; }
        f[i] = max(n, m);
        if (b > 1) {
            int k = phi[b];
            int temp_phi = b; // factorize b for ord10? No, factorize phi(b)
            int phi_val = k;
            while (phi_val > 1) {
                int p = spf[phi_val];
                while (k % p == 0 && power(10, k / p, b) == 1) k /= p;
                while (phi_val % p == 0) phi_val /= p;
            }
            f[i] += k;
        }
    }
}

long long inv(long long n) {
    return power(n, MOD - 2, MOD);
}

int C[MAXV];
long long T[MAXV];
long long prefC[MAXV];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    for (int i = 0; i < N; ++i) {
        int a;
        cin >> a;
        C[a]++;
    }

    sieve();
    precompute_f();

    for (int i = 1; i < MAXV; ++i) {
        prefC[i] = (prefC[i - 1] + C[i]) % MOD;
    }

    for (int i = 1; i < MAXV; ++i) {
        for (int j = i; j < MAXV; j += i) {
            T[i] = (T[i] + C[j]) % MOD;
        }
    }

    long long sum_int = 0;
    for (int u = 1; u < MAXV; ++u) {
        if (C[u] == 0) continue;
        long long current_u_int = N;
        for (long long p10 = 10; p10 * u < MAXV; p10 *= 10) {
            long long count_ge = (N - prefC[p10 * u - 1] + MOD) % MOD;
            current_u_int = (current_u_int + count_ge) % MOD;
        }
        sum_int = (sum_int + (long long)C[u] * current_u_int) % MOD;
    }

    long long sum_frac = 0;
    for (int d = 1; d < MAXV; ++d) {
        if (mu[d] == 0) continue;
        for (int x_prime = 1; d * x_prime < MAXV; ++x_prime) {
            int X = d * x_prime;
            if (T[X] == 0) continue;
            for (int y_prime = 1; (long long)X * y_prime < MAXV; ++y_prime) {
                int Y = d * y_prime;
                int U = X * y_prime;
                if (C[U] == 0) continue;
                
                long long term = (long long)f[Y] * C[U] % MOD;
                term = term * T[X] % MOD;
                if (mu[d] == 1) {
                    sum_frac = (sum_frac + term) % MOD;
                } else {
                    sum_frac = (sum_frac - term + MOD) % MOD;
                }
            }
        }
    }

    long long total_sum = (sum_int + sum_frac) % MOD;
    long long Q_inv = inv((long long)N * N % MOD);
    cout << (total_sum * Q_inv % MOD) << endl;

    return 0;
}
