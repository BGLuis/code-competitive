import sys

def solve():
    # Using sys.stdin.read().split() for faster input processing
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    n = int(input_data[0])
    # The difficulties follow N in the input stream
    
    count = 0
    curr_max = -1
    for i in range(1, n + 1):
        d = int(input_data[i])
        if d > curr_max:
            count += 1
            curr_max = d
            
    sys.stdout.write(str(count) + "\n")

if __name__ == "__main__":
    solve()
