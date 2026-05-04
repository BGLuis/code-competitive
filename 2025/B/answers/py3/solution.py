import sys

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    N = int(input_data[0])
    K = int(input_data[1])
    MOD = 998244353
    
    # dp[q] is the number of sequences of length i with sum/i = q
    dp = [0] * (K + 1)
    for q in range(1, K + 1):
        dp[q] = 1
        
    limit = min(N, K)
    for i in range(2, limit + 1):
        new_dp_diff = [0] * (K + 2)
        for q_old in range(1, K + 1):
            val = dp[q_old]
            if val == 0: continue
            
            # For each q_old, we need b_i = q_old + m*i in [1, K]
            # m*i in [1-q_old, K-q_old]
            # m in [ceil((1-q_old)/i), floor((K-q_old)/i)]
            # q_new = q_old + m
            
            low = q_old - (q_old - 1) // i
            high = q_old + (K - q_old) // i
            
            if low <= K:
                new_dp_diff[low] = (new_dp_diff[low] + val) % MOD
                if high + 1 <= K:
                    new_dp_diff[high + 1] = (new_dp_diff[high + 1] - val + MOD) % MOD
            
        curr = 0
        for q in range(1, K + 1):
            curr = (curr + new_dp_diff[q]) % MOD
            dp[q] = curr
            
    total = sum(dp) % MOD
    print(total)

if __name__ == "__main__":
    solve()
