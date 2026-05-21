import sys

def main():
    try:
        line = sys.stdin.read().split()
        if not line:
            return
        X = int(line[0])
        if 3 <= X <= 18:
            print("Yes")
        else:
            print("No")
    except Exception:
        pass

if __name__ == '__main__':
    main()
