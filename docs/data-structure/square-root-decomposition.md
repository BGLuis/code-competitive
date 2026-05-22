# Square Root Decomposition

## Conceptual Explanation
**Square Root Decomposition** (or Block Decomposition) is an extremely powerful and versatile technique used to process range queries and point updates. While a Segment Tree solves these in $O(\log N)$, Square Root Decomposition solves them in $O(\sqrt{N})$.
Despite the slower theoretical time complexity, it is often preferred because:
1. It is incredibly easy and fast to implement.
2. It can solve problems that are strictly impossible to model with a Segment Tree (e.g., Mo's Algorithm, or operations lacking associativity).
3. The constant factor is exceptionally low, meaning $O(\sqrt{N})$ can easily pass within time limits for $N \le 10^5$.

The core idea is to divide an array of size $N$ into blocks of size $S \approx \sqrt{N}$. We precalculate the answer (e.g., sum, minimum) for each block.
- **Point Update**: To update an element at index $i$, we simply update the element and the precalculated answer for the block containing $i$. This takes $O(1)$ time.
- **Range Query**: To query a range $[L, R]$, we sum up the individual elements for the partial blocks at the boundaries, and we directly use the precalculated answers for the fully completely enclosed blocks in the middle. Since there are at most $\sqrt{N}$ blocks and at most $\sqrt{N}$ elements in the partial boundaries, a query takes $O(\sqrt{N})$ time.

---

## Complexity Analysis
- **Block Size**: $S = \lfloor \sqrt{N} \rfloor$
- **Number of Blocks**: $\lceil N / S \rceil \approx \sqrt{N}$
- **Precomputation**: $O(N)$ time and $O(\sqrt{N})$ auxiliary space.
- **Point Update**: $O(1)$ time.
- **Range Query**: $O(\sqrt{N})$ time.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class SqrtDecomposition {
private:
    int n, block_size;
    vector<long long> a;
    vector<long long> block_sum;

public:
    SqrtDecomposition(const vector<long long>& input) {
        n = input.size();
        a = input;
        block_size = sqrt(n);
        if (block_size == 0) block_size = 1;
        
        int num_blocks = (n + block_size - 1) / block_size;
        block_sum.assign(num_blocks, 0);
        
        for (int i = 0; i < n; ++i) {
            block_sum[i / block_size] += a[i];
        }
    }

    // Update element at index i to value v in O(1)
    void update(int i, long long v) {
        int block_idx = i / block_size;
        block_sum[block_idx] = block_sum[block_idx] - a[i] + v;
        a[i] = v;
    }

    // Query sum in range [L, R] inclusive in O(sqrt(N))
    long long query(int l, int r) {
        long long sum = 0;
        int start_block = l / block_size;
        int end_block = r / block_size;

        if (start_block == end_block) {
            // Range is entirely inside a single block
            for (int i = l; i <= r; ++i) {
                sum += a[i];
            }
        } else {
            // Add suffix of the starting block
            for (int i = l; i < (start_block + 1) * block_size; ++i) {
                sum += a[i];
            }
            // Add full blocks in the middle
            for (int b = start_block + 1; b < end_block; ++b) {
                sum += block_sum[b];
            }
            // Add prefix of the ending block
            for (int i = end_block * block_size; i <= r; ++i) {
                sum += a[i];
            }
        }
        return sum;
    }
};
```

---

## Python Implementation

```python
import math
from typing import List


class SqrtDecomposition:
    def __init__(self, arr: List[int]):
        self.n = len(arr)
        self.a = list(arr)
        self.block_size = int(math.sqrt(self.n))
        if self.block_size == 0:
            self.block_size = 1

        num_blocks = (self.n + self.block_size - 1) // self.block_size
        self.block_sum = [0] * num_blocks

        for i in range(self.n):
            self.block_sum[i // self.block_size] += self.a[i]

    def update(self, i: int, v: int):
        """Updates element at index i to value v in O(1)."""
        block_idx = i // self.block_size
        self.block_sum[block_idx] = self.block_sum[block_idx] - self.a[i] + v
        self.a[i] = v

    def query(self, l: int, r: int) -> int:
        """Queries the sum in range [l, r] inclusive in O(sqrt(N))."""
        res = 0
        start_block = l // self.block_size
        end_block = r // self.block_size

        if start_block == end_block:
            # Entire range is within one block
            for i in range(l, r + 1):
                res += self.a[i]
        else:
            # Suffix of the starting block
            end_of_start_block = (start_block + 1) * self.block_size
            for i in range(l, end_of_start_block):
                res += self.a[i]
            
            # Middle full blocks
            for b in range(start_block + 1, end_block):
                res += self.block_sum[b]
                
            # Prefix of the ending block
            start_of_end_block = end_block * self.block_size
            for i in range(start_of_end_block, r + 1):
                res += self.a[i]
                
        return res
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    vector<long long> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    SqrtDecomposition sq(arr);

    cout << "Initial array: [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]\n";
    cout << "Sum [2, 7]: " << sq.query(2, 7) << " (expected 60)\n"; // 5+7+9+11+13+15 = 60
    
    cout << "Updating index 4 from 9 to 20...\n";
    sq.update(4, 20);
    
    cout << "Sum [2, 7]: " << sq.query(2, 7) << " (expected 71)\n"; // 60 - 9 + 20 = 71
    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    arr = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
    sq = SqrtDecomposition(arr)

    print("Initial array: [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]")
    print(f"Sum [2, 7]: {sq.query(2, 7)} (expected 60)")

    print("Updating index 4 from 9 to 20...")
    sq.update(4, 20)

    print(f"Sum [2, 7]: {sq.query(2, 7)} (expected 71)")
```
