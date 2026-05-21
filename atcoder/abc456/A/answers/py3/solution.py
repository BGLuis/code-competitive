import sys

# Increase recursion depth for deep DSU
sys.setrecursionlimit(2000000)

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    N = int(input_data[0])
    
    ptr = 1
    P = int(input_data[ptr])
    ptr += 1
    L = []
    for _ in range(P):
        L.append(int(input_data[ptr]))
        ptr += 1
        
    Q = int(input_data[ptr])
    ptr += 1
    R = []
    for _ in range(Q):
        R.append(int(input_data[ptr]))
        ptr += 1
        
    if N % 2 == 0:
        if N > 2:
            print("N")
            return
        else: # N = 2
            # S = 2, full sequence must be [1, 2] or [2, 1]
            if P + Q > 2:
                print("N")
                return
            # Valid partials check: no adjacent same, no duplicate edge
            if P >= 2 and L[0] == L[1]: print("N"); return
            if Q >= 2 and R[0] == R[1]: print("N"); return
            if P >= 1 and Q >= 1 and L[0] == R[0]: print("N"); return
            print("Y")
            return
            
    # N is odd
    S = N * (N - 1) // 2 + 1
    if P + Q > S:
        print("N")
        return
        
    for i in range(P - 1):
        if L[i] == L[i+1]:
            print("N")
            return
    for i in range(Q - 1):
        if R[i] == R[i+1]:
            print("N")
            return
            
    if P > 0 and Q > 0 and L[0] != R[-1]:
        print("N")
        return
        
    used_edges = set()
    def get_edge(u, v):
        return (u, v) if u < v else (v, u)
        
    for i in range(P - 1):
        e = get_edge(L[i], L[i+1])
        if e in used_edges:
            print("N")
            return
        used_edges.add(e)
        
    for i in range(Q - 1):
        e = get_edge(R[i], R[i+1])
        if e in used_edges:
            print("N")
            return
        used_edges.add(e)
        
    E_prime = (S - 1) - len(used_edges)
    
    u, v = -1, -1
    if P > 0 and Q > 0:
        u, v = L[-1], R[0]
    elif P > 0:
        u, v = L[-1], L[0]
    elif Q > 0:
        u, v = R[-1], R[0]
    else:
        u, v = 1, 1
        
    if E_prime == 0:
        print("Y" if u == v else "N")
        return
        
    # Check connected components in G' (complement of used_edges)
    parent = list(range(N + 1))
    def find(i):
        root = i
        while parent[root] != root:
            root = parent[root]
        while parent[i] != root:
            parent[i], i = root, parent[i]
        return root

    def union(i, j):
        root_i = find(i)
        root_j = find(j)
        if root_i != root_j:
            parent[root_i] = root_j
            return True
        return False

    has_edge = [False] * (N + 1)
    for i in range(1, N + 1):
        for j in range(i + 1, N + 1):
            e = (i, j)
            if e not in used_edges:
                union(i, j)
                has_edge[i] = True
                has_edge[j] = True
                
    edge_comp = -1
    for i in range(1, N + 1):
        if has_edge[i]:
            c = find(i)
            if edge_comp == -1:
                edge_comp = c
            elif edge_comp != c:
                print("N")
                return
                
    if edge_comp != -1:
        if find(u) != edge_comp or find(v) != edge_comp:
            print("N")
            return
            
    print("Y")

if __name__ == "__main__":
    solve()
