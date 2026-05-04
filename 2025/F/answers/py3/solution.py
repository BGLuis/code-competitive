import sys

def factorize(n):
    factors = {}
    d = 2
    temp = n
    while d * d <= temp:
        while temp % d == 0:
            factors[d] = factors.get(d, 0) + 1
            temp //= d
        d += 1
    if temp > 1:
        factors[temp] = factors.get(temp, 0) + 1
    return factors

def solve():
    s = sys.stdin.read().strip()
    if not s:
        return
    n = len(s)
    
    if n <= 13:
        val = int(s)
        factors = factorize(val)
    else:
        # 12 digits are enough for 10^-9 error and fast trial division in Python
        P = int(s[:12])
        L = n - 12
        factors = factorize(P)
        factors[2] = factors.get(2, 0) + L
        factors[5] = factors.get(5, 0) + L
        
    res = [(p, e) for p, e in factors.items() if e > 0]
    print(len(res))
    for p, e in res:
        print(f"{p} {e}")

if __name__ == "__main__":
    solve()
