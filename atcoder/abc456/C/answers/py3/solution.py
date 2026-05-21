import sys
from math import gcd
from functools import cmp_to_key

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    ptr = 0
    N = int(input_data[ptr]); ptr += 1
    S = int(input_data[ptr]); ptr += 1
    K = int(input_data[ptr]); ptr += 1
    
    cleaners = []
    events = []
    distinct_H_set = set()
    
    for i in range(N):
        h = int(input_data[ptr]); ptr += 1
        l = int(input_data[ptr]); ptr += 1
        u = int(input_data[ptr]); ptr += 1
        m = K // h
        if m >= 1:
            cid = len(cleaners)
            cleaners.append((h, l, u, m))
            events.append(((l, h), 0, cid))
            events.append(((u, h), 1, cid))
            distinct_H_set.add(h)
            
    if not cleaners:
        print("*")
        return
        
    distinct_H = sorted(list(distinct_H_set))
    h_to_idx = {h: i + 1 for i, h in enumerate(distinct_H)}
    N_h = len(distinct_H)
    
    bit_cnt = [0] * (N_h + 1)
    bit_sum = [0] * (N_h + 1)
    
    def update(idx, val_m, val_mh):
        while idx <= N_h:
            bit_cnt[idx] += val_m
            bit_sum[idx] += val_mh
            idx += idx & (-idx)
            
    def get_min_sumH(s):
        idx = 0
        current_cnt = 0
        current_sum = 0
        for i in range(N_h.bit_length() - 1, -1, -1):
            next_idx = idx + (1 << i)
            if next_idx <= N_h and current_cnt + bit_cnt[next_idx] < s:
                idx = next_idx
                current_cnt += bit_cnt[idx]
                current_sum += bit_sum[idx]
        
        remaining = s - current_cnt
        # The next index is idx + 1, which corresponds to distinct_H[idx]
        current_sum += remaining * distinct_H[idx]
        return current_sum

    def cmp_coords(a, b):
        v1_num, v1_den = a[0]
        v2_num, v2_den = b[0]
        p1 = v1_num * v2_den
        p2 = v2_num * v1_den
        if p1 < p2: return -1
        if p1 > p2: return 1
        if a[1] < b[1]: return -1
        if a[1] > b[1]: return 1
        return 0

    events.sort(key=cmp_to_key(cmp_coords))
    
    best_num = -1
    best_den = 1
    
    def is_better(n1, d1, n2, d2):
        if n2 == -1: return True
        return n1 * d2 < n2 * d1
    
    i = 0
    while i < len(events):
        current_coord = events[i][0]
        j = i
        while j < len(events) and events[j][0] == current_coord:
            if events[j][1] == 0: # START
                cid = events[j][2]
                h, l, u, m = cleaners[cid]
                update(h_to_idx[h], m, m * h)
            j += 1
            
        total_cnt = 0
        tmp_idx = N_h
        while tmp_idx > 0:
            total_cnt += bit_cnt[tmp_idx]
            tmp_idx -= tmp_idx & (-tmp_idx)
            
        if total_cnt >= S:
            sumH = get_min_sumH(S)
            cur_num = current_coord[0] * sumH
            cur_den = current_coord[1]
            if is_better(cur_num, cur_den, best_num, best_den):
                best_num = cur_num
                best_den = cur_den
        
        j = i
        while j < len(events) and events[j][0] == current_coord:
            if events[j][1] == 1: # END
                cid = events[j][2]
                h, l, u, m = cleaners[cid]
                update(h_to_idx[h], -m, -m * h)
            j += 1
        i = j

    if best_num == -1:
        print("*")
    else:
        common = gcd(best_num, best_den)
        print(f"{best_num // common} {best_den // common}")

if __name__ == "__main__":
    solve()
