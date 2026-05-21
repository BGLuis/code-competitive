import sys
from collections import deque

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    iterator = iter(input_data)
    try:
        T_cases = int(next(iterator))
    except StopIteration:
        return
    
    out = []
    for _ in range(T_cases):
        N = int(next(iterator))
        M = int(next(iterator))
        
        adj = [[i] for i in range(N)]
        for _ in range(M):
            u = int(next(iterator)) - 1
            v = int(next(iterator)) - 1
            adj[u].append(v)
            adj[v].append(u)
            
        W = int(next(iterator))
        S = [next(iterator) for _ in range(N)]
        
        # Remove duplicates from adj
        for i in range(N):
            adj[i] = list(set(adj[i]))
            
        out_deg = [0] * (N * W)
        removed = [False] * (N * W)
        
        for u in range(N):
            Su = S[u]
            u_W = u * W
            for t in range(W):
                if Su[t] == 'x':
                    removed[u_W + t] = True
                    
        # Calculate initial out_deg
        for u in range(N):
            Su = S[u]
            u_W = u * W
            for t in range(W):
                if Su[t] == 'o':
                    t_next = (t + 1) % W
                    deg = 0
                    for v in adj[u]:
                        if S[v][t_next] == 'o':
                            deg += 1
                    out_deg[u_W + t] = deg
                    
        # Find initial 0 out-deg states
        Q = deque()
        for u in range(N):
            u_W = u * W
            Su = S[u]
            for t in range(W):
                if Su[t] == 'o' and out_deg[u_W + t] == 0:
                    removed[u_W + t] = True
                    Q.append((u, t))
                    
        while Q:
            v, t = Q.popleft()
            t_prev = (t - 1) % W
            
            for u in adj[v]:
                state_idx = u * W + t_prev
                if not removed[state_idx]:
                    out_deg[state_idx] -= 1
                    if out_deg[state_idx] == 0:
                        removed[state_idx] = True
                        Q.append((u, t_prev))
                        
        possible = False
        for u in range(N):
            if not removed[u * W]:
                possible = True
                break
                
        if possible:
            out.append("Yes")
        else:
            out.append("No")
        
    sys.stdout.write('\n'.join(out) + '\n')

if __name__ == "__main__":
    solve()
