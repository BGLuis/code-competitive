import sys

def solve():
    # Read all input from standard input
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    S = input_data[0]
    
    total = 0
    mod = 998244353
    L = 1
    for i in range(1, len(S)):
        if S[i] == S[i - 1]:
            contribution = (L * (L + 1)) // 2
            total = (total + contribution) % mod
            L = 1
        else:
            L += 1
            
    contribution = (L * (L + 1)) // 2
    total = (total + contribution) % mod
    
    print(total)

if __name__ == '__main__':
    solve()
