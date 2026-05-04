import sys
from math import sqrt
from bisect import bisect_left

def solve():
    # Reading input efficiently
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    ptr = 0
    N = int(input_data[ptr]); ptr += 1
    A = float(input_data[ptr]); ptr += 1
    B = float(input_data[ptr]); ptr += 1
    
    intervals = []
    for _ in range(N):
        m = float(input_data[ptr]); ptr += 1
        c = float(input_data[ptr]); ptr += 1
        
        # Solving Ax^2 - mx + (B - c) = 0 for intersection points
        disc = m * m - 4.0 * A * (B - c)
        if disc > 1e-11:
            sd = sqrt(disc)
            l = (m - sd) / (2.0 * A)
            r = (m + sd) / (2.0 * A)
            intervals.append((l, r))
            
    if not intervals:
        sys.stdout.write("0\n")
        return
        
    # Intervals are sorted by L to facilitate sweep-line counting of interlacing pairs
    intervals.sort()
    
    # Collect all R values for discretization and Fenwick tree
    all_R = sorted(list(set(r for l, r in intervals)))
    m_size = len(all_R)
    
    bit = [0] * (m_size + 1)
    
    def update(i, val):
        while i <= m_size:
            bit[i] += val
            i += i & (-i)
            
    def query(i):
        res = 0
        while i > 0:
            res += bit[i]
            i -= i & (-i)
        return res
        
    ans = 0
    EPS = 1e-11
    
    for l, r in intervals:
        # A pair of intervals (L_j, R_j) and (L_i, R_i) interlaces if L_j < L_i < R_j < R_i.
        # Since we process intervals in increasing order of L_i, L_j < L_i is already satisfied.
        # We need to count previously added R_j such that R_j is in the range (L_i, R_i).
        
        # Find index range in the discretized R list
        il = bisect_left(all_R, l + EPS) + 1
        ih = bisect_left(all_R, r - EPS) # largest index where all_R[idx] < r - EPS (1-based index)
        
        if ih >= il:
            ans += query(ih) - query(il - 1)
            
        # Add the current R to the Fenwick tree
        r_idx = bisect_left(all_R, r - EPS) + 1
        update(r_idx, 1)
            
    sys.stdout.write(str(ans) + "\n")

if __name__ == "__main__":
    solve()
