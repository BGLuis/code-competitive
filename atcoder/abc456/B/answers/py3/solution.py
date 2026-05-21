import sys

def solve():
    input_data = sys.stdin.read().split()
    if len(input_data) < 18:
        return
    
    A = []
    idx = 0
    for _ in range(3):
        row = []
        for _ in range(6):
            row.append(int(input_data[idx]))
            idx += 1
        A.append(row)
        
    match_count = 0
    for i in range(6):
        for j in range(6):
            for k in range(6):
                x = A[0][i]
                y = A[1][j]
                z = A[2][k]
                
                # Check if {x, y, z} matches {4, 5, 6}
                vals = sorted([x, y, z])
                if vals == [4, 5, 6]:
                    match_count += 1
                    
    print(f"{match_count / 216.0:.10f}")

if __name__ == "__main__":
    solve()
