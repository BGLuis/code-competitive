import sys

def solve():
    # Read the entire input string
    s = sys.stdin.read().strip()
    if not s:
        return
    
    score = 0
    n = len(s)
    # Iterate through each index and check for target substrings
    for i in range(n):
        if s.startswith("ha", i):
            score += 1
        if s.startswith("boooo", i):
            score -= 1
        if s.startswith("bravo", i):
            score += 3
            
    sys.stdout.write(str(score) + "\n")

if __name__ == "__main__":
    solve()
