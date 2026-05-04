import sys

def solve():
    # Reading all input data efficiently
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    n = int(input_data[0])
    k = int(input_data[1])
    
    # If only one king is present, the bounding box always moves with it, 
    # maintaining an area of 1.
    if n == 1:
        sys.stdout.write("1\n")
        return
        
    kings = []
    ptr = 2
    r_min = float('inf')
    r_max = float('-inf')
    c_min = float('inf')
    c_max = float('-inf')
    
    # Identify the initial bounding box
    for _ in range(n):
        r = int(input_data[ptr])
        c = int(input_data[ptr+1])
        ptr += 2
        kings.append((r, c))
        if r < r_min: r_min = r
        if r > r_max: r_max = r
        if c < c_min: c_min = c
        if c > c_max: c_max = c
        
    h0 = r_max - r_min + 1
    w0 = c_max - c_min + 1
    
    ans = h0 * w0
    
    # Strategy 1: Expand the bounding box using a single king to push 
    # two boundaries simultaneously (a corner move).
    for r, c in kings:
        # Distances to the current boundaries
        au = r_max - r
        ad = r - r_min
        br = c_max - c
        bl = c - c_min
        
        # Try moving this king towards each of the 4 potential corners
        # Up-Right
        ans = max(ans, (h0 + max(0, k - au)) * (w0 + max(0, k - br)))
        # Up-Left
        ans = max(ans, (h0 + max(0, k - au)) * (w0 + max(0, k - bl)))
        # Down-Right
        ans = max(ans, (h0 + max(0, k - ad)) * (w0 + max(0, k - br)))
        # Down-Left
        ans = max(ans, (h0 + max(0, k - ad)) * (w0 + max(0, k - bl)))
        
    # Strategy 2: Use the total K moves to expand height and width 
    # independently using two different kings (orthogonal move).
    # Area = (h0 + x) * (w0 + (k - x))
    x_opt = (w0 + k - h0) // 2
    for x in [0, k, x_opt, x_opt + 1]:
        if 0 <= x <= k:
            ans = max(ans, (h0 + x) * (w0 + k - x))
            
    sys.stdout.write(str(ans) + "\n")

if __name__ == "__main__":
    solve()
