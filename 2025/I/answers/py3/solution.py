import sys

# Increasing recursion depth to accommodate the number of floors
sys.setrecursionlimit(2000)

def solve():
    # Reading all input at once for speed
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    ptr = 0
    N = int(input_data[ptr])
    ptr += 1
    M = int(input_data[ptr])
    ptr += 1
    
    adj = [[] for _ in range(2 * N + 1)]
    for _ in range(M):
        u = int(input_data[ptr])
        ptr += 1
        v = int(input_data[ptr])
        ptr += 1
        adj[u].append(v)
        
    # Precomputing sensor IDs for each floor
    sensor_id = [0] * (2 * N + 1)
    for i in range(1, N + 1):
        sensor_id[i] = i
        sensor_id[i + N] = i
        
    # failed_masks[u] stores integers (bitsets) that led to no solution from floor u
    failed_masks = [[] for _ in range(2 * N + 1)]
    path = []
    
    def dfs(u, forbidden_mask):
        if u == 2 * N:
            path.append(u)
            return True
        
        # Pruning based on previous failed attempts at this floor with similar/subset restrictions
        for failed in failed_masks[u]:
            if (forbidden_mask & failed) == failed:
                return False
                
        for v in adj[u]:
            s = sensor_id[v]
            # Check if the sensor for the target floor is already "sealed"
            if (forbidden_mask >> s) & 1:
                continue
            
            if dfs(v, forbidden_mask | (1 << s)):
                path.append(u)
                return True
        
        # Memoize the failure
        failed_masks[u].append(forbidden_mask)
        return False

    # Start at floor 1, sensor for floor 1 is used
    initial_mask = (1 << sensor_id[1])
    if dfs(1, initial_mask):
        path.reverse()
        sys.stdout.write(str(len(path)) + "\n")
        sys.stdout.write(" ".join(map(str, path)) + "\n")
    else:
        sys.stdout.write("*\n")

if __name__ == "__main__":
    solve()
