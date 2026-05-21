import sys

def power(base, exp, mod):
    res = 1
    base %= mod
    while exp > 0:
        if exp % 2 == 1: res = (res * base) % mod
        base = (base * base) % mod
        exp //= 2
    return res

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    N = int(input_data[0])
    A = list(map(int, input_data[1:]))
    
    MOD = 998244353
    MAXV = 100005
    
    C = [0] * MAXV
    for x in A:
        C[x] += 1
        
    spf = [0] * MAXV
    phi = [0] * MAXV
    mu = [0] * MAXV
    primes = []
    
    mu[1] = 1
    for i in range(2, MAXV):
        if spf[i] == 0:
            spf[i] = i
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if p > spf[i] or i * p >= MAXV:
                break
            spf[i * p] = p
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]
                
    phi[1] = 1
    for i in range(2, MAXV):
        temp = i
        res = i
        while temp > 1:
            p = spf[temp]
            res = res // p * (p - 1)
            while temp % p == 0:
                temp //= p
        phi[i] = res
        
    f = [0] * MAXV
    for i in range(1, MAXV):
        b = i
        n = 0
        m = 0
        while b % 2 == 0:
            b //= 2
            n += 1
        while b % 5 == 0:
            b //= 5
            m += 1
        f[i] = max(n, m)
        if b > 1:
            k = phi[b]
            temp_phi = k
            while temp_phi > 1:
                p = spf[temp_phi]
                # Smallest k such that 10^k == 1 mod b
                while k % p == 0 and pow(10, k // p, b) == 1:
                    k //= p
                while temp_phi % p == 0:
                    temp_phi //= p
            f[i] += k
            
    prefC = [0] * MAXV
    for i in range(1, MAXV):
        prefC[i] = (prefC[i-1] + C[i]) % MOD
        
    T = [0] * MAXV
    for i in range(1, MAXV):
        for j in range(i, MAXV, i):
            T[i] = (T[i] + C[j]) % MOD
            
    sum_int = 0
    for u in range(1, MAXV):
        if C[u] == 0: continue
        current_u_int = N
        p10 = 10
        while p10 * u < MAXV:
            count_ge = (N - prefC[p10 * u - 1]) % MOD
            current_u_int = (current_u_int + count_ge) % MOD
            p10 *= 10
        sum_int = (sum_int + C[u] * current_u_int) % MOD
        
    sum_frac = 0
    for d in range(1, MAXV):
        if mu[d] == 0: continue
        mu_val = mu[d]
        for x_prime in range(1, (MAXV - 1) // d + 1):
            X = d * x_prime
            t_val = T[X]
            if t_val == 0: continue
            for y_prime in range(1, (MAXV - 1) // X + 1):
                U = X * y_prime
                c_val = C[U]
                if c_val == 0: continue
                Y = d * y_prime
                term = (f[Y] * c_val * t_val) % MOD
                if mu_val == 1:
                    sum_frac = (sum_frac + term) % MOD
                else:
                    sum_frac = (sum_frac - term) % MOD
                    
    total_sum = (sum_int + sum_frac) % MOD
    Q_inv = pow(N * N, MOD - 2, MOD)
    print((total_sum * Q_inv) % MOD)

if __name__ == "__main__":
    solve()
