import sys

def solve():
    # Read all input from standard input
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    S = input_data[0]
    
    dp = [0, 0, 0]  # dp[0] for 'a', dp[1] for 'b', dp[2] for 'c'
    MOD = 998244353
    
    for c in S:
        idx = ord(c) - ord('a')
        ways = (1 + dp[(idx + 1) % 3] + dp[(idx + 2) % 3]) % MOD
        dp[idx] = (dp[idx] + ways) % MOD
        
    ans = sum(dp) % MOD
    print(ans)

if __name__ == "__main__":
    solve()
