import sys

# Set recursion depth limit just in case, though we don't use recursion
sys.setrecursionlimit(300000)

def solve():
    # Read N and S from standard input
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    N = int(input_data[0])
    S = input_data[1]
    
    MOD = 998244353
    
    # Precomputation
    fact = [1] * (N + 5)
    inv = [1] * (N + 5)
    power_of_2 = [1] * (N + 5)
    
    for i in range(1, N + 5):
        fact[i] = (fact[i - 1] * i) % MOD
        power_of_2[i] = (power_of_2[i - 1] * 2) % MOD
        
    inv[N + 4] = pow(fact[N + 4], MOD - 2, MOD)
    for i in range(N + 3, -1, -1):
        inv[i] = (inv[i + 1] * (i + 1)) % MOD
        
    def C(n, r):
        if r < 0 or r > n:
            return 0
        return fact[n] * inv[r] % MOD * inv[n - r] % MOD
        
    def compute_f(L, x):
        if x >= L:
            return power_of_2[L]
        ans = 0
        limit = (L + 1) // (x + 2)
        for k in range(limit + 1):
            M = L - k * (x + 1)
            term1 = C(M, k) * power_of_2[M - k] % MOD if M >= k else 0
            term2 = C(M, k - 1) * power_of_2[M - k + 1] % MOD if M >= k - 1 else 0
            S_val = (term1 + term2) % MOD
            if k % 2 == 1:
                ans = (ans - S_val) % MOD
            else:
                ans = (ans + S_val) % MOD
        return ans
        
    p = [-1]
    for i in range(N):
        if S[i] == 'x':
            p.append(i)
    p.append(N)
    
    cnt = [0] * (N + 2)
    unique_lengths = []
    for i in range(len(p) - 1):
        L = p[i + 1] - p[i] - 1
        if L > 0:
            if cnt[L] == 0:
                unique_lengths.append(L)
            cnt[L] += 1
            
    unique_lengths.sort()
    
    sum_L = [0] * (N + 2)
    curr_sum = 0
    for L in range(N + 2):
        curr_sum += cnt[L] * L
        sum_L[L] = curr_sum
        
    F = [1] * (N + 1)
    for x in range(N + 1):
        F[x] = power_of_2[sum_L[x]]
        for L in unique_lengths:
            if L <= x:
                continue
            val = compute_f(L, x)
            F[x] = F[x] * pow(val, cnt[L], MOD) % MOD
            
    out = []
    for k in range(1, N + 1):
        ans = (F[k] - F[k - 1]) % MOD
        out.append(str(ans))
        
    print('\n'.join(out))

if __name__ == '__main__':
    solve()
