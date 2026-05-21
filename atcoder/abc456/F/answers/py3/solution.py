import sys

# Increase recursion depth just in case (though we don't use recursion here)
sys.setrecursionlimit(2000)

def get_M_00(N, L, A):
    res = [0] * (N + 2)
    if L <= 0:
        return res
    
    INF = 10**18
    num_blocks = (N + 2 * L) // L + 2
    max_size = num_blocks * L
    
    T0 = [0] * max_size
    T1 = [INF] * max_size
    T2 = [INF] * max_size
    T3 = [0] * max_size
    
    for i in range(2, N + 1):
        T0[i] = A[i]
        T1[i] = A[i]
        T2[i] = 0
        T3[i] = INF
        
    pref0 = [0] * max_size
    pref1 = [0] * max_size
    pref2 = [0] * max_size
    pref3 = [0] * max_size
    
    suff0 = [0] * max_size
    suff1 = [0] * max_size
    suff2 = [0] * max_size
    suff3 = [0] * max_size
    
    for b in range(num_blocks):
        start = b * L
        end = (b + 1) * L - 1
        
        pref0[start] = T0[start]
        pref1[start] = T1[start]
        pref2[start] = T2[start]
        pref3[start] = T3[start]
        
        for x in range(start + 1, end + 1):
            tx0, tx1, tx2, tx3 = T0[x], T1[x], T2[x], T3[x]
            p0, p1, p2, p3 = pref0[x-1], pref1[x-1], pref2[x-1], pref3[x-1]
            pref0[x] = min(tx0 + p0, tx1 + p2)
            pref1[x] = min(tx0 + p1, tx1 + p3)
            pref2[x] = min(tx2 + p0, tx3 + p2)
            pref3[x] = min(tx2 + p1, tx3 + p3)
            
        suff0[end] = T0[end]
        suff1[end] = T1[end]
        suff2[end] = T2[end]
        suff3[end] = T3[end]
        
        for x in range(end - 1, start - 1, -1):
            s0, s1, s2, s3 = suff0[x+1], suff1[x+1], suff2[x+1], suff3[x+1]
            tx0, tx1, tx2, tx3 = T0[x], T1[x], T2[x], T3[x]
            suff0[x] = min(s0 + tx0, s1 + tx2)
            suff1[x] = min(s0 + tx1, s1 + tx3)
            suff2[x] = min(s2 + tx0, s3 + tx2)
            suff3[x] = min(s2 + tx1, s3 + tx3)
            
    for i in range(1, N - L + 1):
        mid = ((i + L) // L) * L
        if mid == i + 1:
            res[i] = suff0[i + 1]
        else:
            p0, p1, p2, p3 = pref0[i+L], pref1[i+L], pref2[i+L], pref3[i+L]
            s0, s1, s2, s3 = suff0[i+1], suff1[i+1], suff2[i+1], suff3[i+1]
            res[i] = min(p0 + s0, p1 + s2)
            
    return res

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    INF = 10**18
    iterator = iter(input_data)
    
    try:
        T_cases = int(next(iterator))
    except StopIteration:
        return
        
    out = []
    for _ in range(T_cases):
        try:
            N = int(next(iterator))
            K = int(next(iterator))
        except StopIteration:
            break
            
        A = [INF] * (N + 2)
        for i in range(1, N + 1):
            A[i] = int(next(iterator))
            
        if K == 1:
            ans = min(A[1:N+1])
            out.append(str(ans))
            continue
            
        M_K_minus_1 = get_M_00(N, K - 1, A)
        M_K_minus_2 = get_M_00(N, K - 2, A)
        
        global_ans = INF
        for v in range(K, N + 1):
            i = v - K + 1
            term1 = A[i] + M_K_minus_1[i]
            term2 = A[i-1] + M_K_minus_2[i+1] + A[i+1]
            
            val = term1 if term1 < term2 else term2
            if val < global_ans:
                global_ans = val
                
        out.append(str(global_ans))
        
    print('\n'.join(out))

if __name__ == "__main__":
    solve()
