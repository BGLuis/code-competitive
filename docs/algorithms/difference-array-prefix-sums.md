# Difference Array and Prefix Sums

## Conceptual Explanation

**Prefix Sums** and **Difference Arrays** are two closely related techniques used to optimize range queries and range updates on arrays, transforming $\mathcal{O}(N)$ operations into $\mathcal{O}(1)$ time.

---

### 1. Prefix Sums (1D and 2D)

#### 1D Prefix Sum
Given an array $A$ of size $N$, a Prefix Sum array $P$ is constructed such that:
$$P[i] = \sum_{k=0}^{i-1} A[k] \quad \text{(using 1-based indexing for convenience)}$$

With this array, any range sum query $\sum_{i=L}^{R} A[i]$ can be answered in $\mathcal{O}(1)$ time:
$$\text{Sum}(L, R) = P[R + 1] - P[L]$$

#### 2D Prefix Sum (Grid Prefix Sum)
For a 2D grid $A$, the prefix sum table $P[i][j]$ stores the sum of all elements in the subgrid from $(0,0)$ to $(i-1, j-1)$.
Using the Principle of Inclusion-Exclusion, we construct it via:
$$P[i][j] = A[i-1][j-1] + P[i-1][j] + P[i][j-1] - P[i-1][j-1]$$

To query the sum of the subgrid with top-left corner $(r_1, c_1)$ and bottom-right corner $(r_2, c_2)$ in $\mathcal{O}(1)$ time:
$$\text{SubgridSum} = P[r_2+1][c_2+1] - P[r_1][c_2+1] - P[r_2+1][c_1] + P[r_1][c_1]$$

---

### 2. Difference Array

A **Difference Array** $D$ is used to perform multiple range update operations efficiently. If we have an array $A$, the difference array is defined as:
$$D[i] = A[i] - A[i-1] \quad (\text{with } D[0] = A[0])$$

#### Range Update in $\mathcal{O}(1)$
To add a value $X$ to all elements in the range $[L, R]$ of the array:
1. $D[L] \leftarrow D[L] + X$
2. $D[R + 1] \leftarrow D[R + 1] - X$ (if $R+1 < N$)

After executing all range updates, the final state of the array $A$ is reconstructed in $\mathcal{O}(N)$ time by taking the prefix sums (cumulative sum) of the difference array $D$:
$$A[i] = \sum_{k=0}^{i} D[k]$$

---

### Typical Competitive Programming Use Cases

- Answering numerous static subarray/subgrid sum queries (Prefix Sums).
- Performing a batch of range add/subtract queries on an array, followed by point queries (Difference Array).
- Coordinate compression problems where intervals are mapped to discrete points.

---

## Complexity Analysis

| Technique | Preprocessing Time | Update Time | Query Time | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **1D Prefix Sum** | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ (to rebuild) | $\mathcal{O}(1)$ | $\mathcal{O}(N)$ |
| **2D Prefix Sum** | $\mathcal{O}(R \cdot C)$ | $\mathcal{O}(R \cdot C)$ (to rebuild) | $\mathcal{O}(1)$ | $\mathcal{O}(R \cdot C)$ |
| **1D Difference Array** | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ per range update | $\mathcal{O}(N)$ (to compile) | $\mathcal{O}(N)$ |

---

## C++ Implementation

Here is a robust C++ implementation containing classes for 1D Prefix Sum, 2D Prefix Sum, and 1D Difference Array.

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

// 1. 1D Prefix Sum Implementation (1-based indexing internally)
class PrefixSum1D {
private:
    std::vector<long long> pref;

public:
    explicit PrefixSum1D(const std::vector<int>& arr) {
        size_t n = arr.size();
        pref.assign(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + arr[i];
        }
    }

    // Query sum of range [L, R] inclusive (0-indexed indices)
    long long query(int L, int R) const {
        if (L < 0 || R >= static_cast<int>(pref.size() - 1) || L > R) {
            throw std::out_of_range("Invalid query range");
        }
        return pref[R + 1] - pref[L];
    }
};

// 2. 2D Prefix Sum Implementation (1-based indexing internally)
class PrefixSum2D {
private:
    std::vector<std::vector<long long>> pref;

public:
    explicit PrefixSum2D(const std::vector<std::vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return;
        int rows = grid.size();
        int cols = grid[0].size();
        pref.assign(rows + 1, std::vector<long long>(cols + 1, 0));

        for (int i = 1; i <= rows; ++i) {
            for (int j = 1; j <= cols; ++j) {
                pref[i][j] = grid[i-1][j-1] 
                             + pref[i-1][j] 
                             + pref[i][j-1] 
                             - pref[i-1][j-1];
            }
        }
    }

    // Query sum of subgrid with top-left (r1, c1) and bottom-right (r2, c2)
    long long query(int r1, int c1, int r2, int c2) const {
        return pref[r2 + 1][c2 + 1] 
             - pref[r1][c2 + 1] 
             - pref[r2 + 1][c1] 
             + pref[r1][c1];
    }
};

// 3. 1D Difference Array Implementation
class DifferenceArray1D {
private:
    int n;
    std::vector<long long> diff;

public:
    explicit DifferenceArray1D(int size) : n(size), diff(size + 1, 0) {}

    // Add val to range [L, R] inclusive
    void update(int L, int R, long long val) {
        if (L < 0 || R >= n || L > R) return;
        diff[L] += val;
        diff[R + 1] -= val;
    }

    // Compile difference array back to the original representing array
    std::vector<long long> compile() const {
        std::vector<long long> arr(n, 0);
        long long current_val = 0;
        for (int i = 0; i < n; ++i) {
            current_val += diff[i];
            arr[i] = current_val;
        }
        return arr;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with complete type hints.

```python
from typing import List

class PrefixSum1D:
    def __init__(self, arr: List[int]):
        self.pref = [0] * (len(arr) + 1)
        for i in range(len(arr)):
            self.pref[i + 1] = self.pref[i] + arr[i]

    def query(self, L: int, R: int) -> int:
        """
        Query sum of subarray A[L...R] inclusive.
        """
        if not (0 <= L <= R < len(self.pref) - 1):
            raise IndexError("Query range out of bounds")
        return self.pref[R + 1] - self.pref[L]

class PrefixSum2D:
    def __init__(self, grid: List[List[int]]):
        if not grid or not grid[0]:
            self.pref = [[]]
            return
        rows = len(grid)
        cols = len(grid[0])
        self.pref = [[0] * (cols + 1) for _ in range(rows + 1)]

        for i in range(1, rows + 1):
            for j in range(1, cols + 1):
                self.pref[i][j] = (grid[i-1][j-1]
                                   + self.pref[i-1][j]
                                   + self.pref[i][j-1]
                                   - self.pref[i-1][j-1])

    def query(self, r1: int, c1: int, r2: int, c2: int) -> int:
        """
        Query sum of subgrid from top-left (r1, c1) to bottom-right (r2, c2) inclusive.
        """
        return (self.pref[r2 + 1][c2 + 1]
                - self.pref[r1][c2 + 1]
                - self.pref[r2 + 1][c1]
                + self.pref[r1][c1])

class DifferenceArray1D:
    def __init__(self, size: int):
        self.n = size
        self.diff = [0] * (size + 1)

    def update(self, L: int, R: int, val: int) -> None:
        """
        Add val to range A[L...R] inclusive.
        """
        if 0 <= L <= R < self.n:
            self.diff[L] += val
            self.diff[R + 1] -= val

    def compile(self) -> List[int]:
        """
        Reconstruct and return the updated array.
        """
        arr = [0] * self.n
        current_val = 0
        for i in range(self.n):
            current_val += self.diff[i]
            arr[i] = current_val
        return arr
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // 1. 1D Prefix Sum
    std::vector<int> arr = {2, 4, -1, 3, 5};
    PrefixSum1D p1d(arr);
    std::cout << "1D Prefix Sum [1, 3] sum: " << p1d.query(1, 3) << " (Expected: 6)\n";

    // 2. 2D Prefix Sum
    std::vector<std::vector<int>> grid = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    PrefixSum2D p2d(grid);
    std::cout << "2D Prefix Sum Subgrid [(1,1) to (2,2)] sum: " << p2d.query(1, 1, 2, 2) << " (Expected: 28)\n";

    // 3. Difference Array
    DifferenceArray1D diff(5); // Start with array [0, 0, 0, 0, 0]
    diff.update(1, 3, 5);      // Array becomes [0, 5, 5, 5, 0]
    diff.update(2, 4, 2);      // Array becomes [0, 5, 7, 7, 2]
    
    std::vector<long long> result = diff.compile();
    std::cout << "Difference Array Compiled Result: ";
    for (long long x : result) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # 1. 1D Prefix Sum
    arr = [2, 4, -1, 3, 5]
    p1d = PrefixSum1D(arr)
    print(f"1D Prefix Sum [1, 3] sum: {p1d.query(1, 3)} (Expected: 6)")

    # 2. 2D Prefix Sum
    grid = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]
    p2d = PrefixSum2D(grid)
    print(f"2D Prefix Sum Subgrid [(1,1) to (2,2)] sum: {p2d.query(1, 1, 2, 2)} (Expected: 28)")

    # 3. Difference Array
    diff = DifferenceArray1D(5)  # Start with [0, 0, 0, 0, 0]
    diff.update(1, 3, 5)         # [0, 5, 5, 5, 0]
    diff.update(2, 4, 2)         # [0, 5, 7, 7, 2]
    result = diff.compile()
    print("Difference Array Compiled Result:", result)
```
