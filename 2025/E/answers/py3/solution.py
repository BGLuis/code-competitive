import sys

# Increase recursion depth for deep segment tree
sys.setrecursionlimit(1000000)

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    Q = int(input_data[0])
    X = [int(input_data[i+1]) for i in range(Q)]
    
    distinct_vals = sorted(list(set(abs(x) for x in X)))
    m = len(distinct_vals)
    if m == 0:
        print(*(0 for _ in range(Q)))
        return
        
    val_to_idx = {v: i for i, v in enumerate(distinct_vals)}
    
    n_tree = 1
    while n_tree < m:
        n_tree *= 2
        
    INF = 10**16
    tree_min = [INF] * (2 * n_tree)
    tree_lazy = [0] * (2 * n_tree)
    
    # Pre-bind functions for speed
    def push(node):
        lz = tree_lazy[node]
        if lz != 0:
            l_child = 2 * node
            r_child = l_child + 1
            tree_min[l_child] += lz
            tree_lazy[l_child] += lz
            tree_min[r_child] += lz
            tree_lazy[r_child] += lz
            tree_lazy[node] = 0
            
    def update_range(node, l, r, ql, qr, val):
        if ql <= l and r <= qr:
            tree_min[node] += val
            tree_lazy[node] += val
            return
        push(node)
        mid = (l + r) // 2
        if ql <= mid:
            update_range(2 * node, l, mid, ql, qr, val)
        if qr > mid:
            update_range(2 * node + 1, mid + 1, r, ql, qr, val)
        
        m1 = tree_min[2 * node]
        m2 = tree_min[2 * node + 1]
        tree_min[node] = m1 if m1 < m2 else m2
        
    def update_point(node, l, r, idx, val):
        if l == r:
            tree_min[node] = val + tree_lazy[node]
            return
        push(node)
        mid = (l + r) // 2
        if idx <= mid:
            update_point(2 * node, l, mid, idx, val)
        else:
            update_point(2 * node + 1, mid + 1, r, idx, val)
        
        m1 = tree_min[2 * node]
        m2 = tree_min[2 * node + 1]
        tree_min[node] = m1 if m1 < m2 else m2
        
    counts = [0] * m
    current_n = 0
    results = []
    
    for x in X:
        v = abs(x)
        idx = val_to_idx[v]
        if x > 0:
            if counts[idx] == 0:
                update_point(1, 0, n_tree - 1, idx, v)
            counts[idx] += 1
            current_n += 1
            update_range(1, 0, n_tree - 1, idx, m - 1, -1)
        else:
            update_range(1, 0, n_tree - 1, idx, m - 1, 1)
            counts[idx] -= 1
            current_n -= 1
            if counts[idx] == 0:
                update_point(1, 0, n_tree - 1, idx, INF)
                
        tm = tree_min[1]
        ans = current_n
        if tm < 0:
            ans += tm
        results.append(str(ans))
        
    sys.stdout.write(" ".join(results) + "\n")

if __name__ == "__main__":
    solve()
