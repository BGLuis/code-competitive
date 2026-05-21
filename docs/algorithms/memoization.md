# Memoization (Top-Down Dynamic Programming)

## Conceptual Explanation

**Memoization** is a top-down optimization technique used in dynamic programming. It speeds up recursive algorithms by storing the results of expensive function calls (subproblems) in a cache and returning the cached result when the same inputs occur again.

This approach is highly beneficial when a recursive solution evaluates the exact same state multiple times, leading to an exponential time complexity if left unoptimized.

### Top-Down vs. Bottom-Up (Tabulation)
- **Top-Down (Memoization)**: We start solving the problem at the high level, recursively breaking it down into smaller subproblems. We only compute subproblems when they are visited.
  - *Pros*: Natural to write; only computes states that are actually reachable/needed.
  - *Cons*: Call stack overhead (recursion); potential Stack Overflow for deep recursions.
- **Bottom-Up (Tabulation)**: We solve all possible subproblems iteratively, beginning with the base cases.
  - *Pros*: No recursion overhead; easy to optimize space.
  - *Cons*: Might compute values for states that are never actually needed for the final answer.

### Key Components of Memoization

1. **State representation**: Identify the parameters that uniquely define a recursive state (e.g., indices `i` and `j`).
2. **Memo Table / Cache**: A data structure (usually a multi-dimensional array or a hash map/dictionary) initialized with dummy/unvisited indicator values (such as `-1`).
3. **Base Cases**: Conditions where recursion terminates and returns a fixed value.
4. **Cache Check**: At the start of the recursive function, check if the state's result is already in the cache. If yes, return it immediately.
5. **Recursion & Storage**: If the state is unvisited, perform the transitions recursively, store the final result in the cache, and then return it.

---

## Complexity Analysis

Let's evaluate the complexity using the **Edit Distance** problem (minimum operations to convert string $A$ of length $N$ to string $B$ of length $M$):
- **Time Complexity**: $\mathcal{O}(N \cdot M)$. There are exactly $(N+1) \times (M+1)$ unique subproblems. With memoization, each state is computed exactly once in $\mathcal{O}(1)$ time. Without memoization, the recursion complexity would be $\mathcal{O}(3^{N+M})$.
- **Space Complexity**:
  - **Memo Table**: $\mathcal{O}(N \cdot M)$ to store the cache.
  - **Call Stack**: $\mathcal{O}(N + M)$ due to recursion depth.

---

## C++ Implementation

Below is a robust and modern C++ class solving the **Edit Distance** problem using top-down recursion with a memoization grid.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class EditDistanceMemoized {
private:
    std::string s1;
    std::string s2;
    std::vector<std::vector<int>> memo;

    int compute_distance(int i, int j) {
        // Base Cases
        if (i == 0) return j; // Must insert all remaining chars of s2
        if (j == 0) return i; // Must delete all remaining chars of s1

        // Check if already computed
        if (memo[i][j] != -1) {
            return memo[i][j];
        }

        // Transition logic
        if (s1[i - 1] == s2[j - 1]) {
            // Characters match, no operation needed
            memo[i][j] = compute_distance(i - 1, j - 1);
        } else {
            // Perform operations and take the minimum:
            int insert_op = compute_distance(i, j - 1);     // Insert into s1
            int delete_op = compute_distance(i - 1, j);     // Delete from s1
            int replace_op = compute_distance(i - 1, j - 1); // Replace char

            memo[i][j] = 1 + std::min({insert_op, delete_op, replace_op});
        }

        return memo[i][j];
    }

public:
    EditDistanceMemoized(const std::string& str1, const std::string& str2) 
        : s1(str1), s2(str2) {
        int n = s1.length();
        int m = s2.length();
        // Initialize memo grid with -1 indicating unvisited states
        memo.assign(n + 1, std::vector<int>(m + 1, -1));
    }

    int get_result() {
        return compute_distance(s1.length(), s2.length());
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List

class EditDistanceMemoized:
    def __init__(self, s1: str, s2: str):
        self.s1 = s1
        self.s2 = s2
        self.n = len(s1)
        self.m = len(s2)
        # Initialize memo table with -1
        self.memo: List[List[int]] = [[-1] * (self.m + 1) for _ in range(self.n + 1)]

    def get_result(self) -> int:
        return self._compute_distance(self.n, self.m)

    def _compute_distance(self, i: int, j: int) -> int:
        # Base Cases
        if i == 0:
            return j
        if j == 0:
            return i

        # Cache check
        if self.memo[i][j] != -1:
            return self.memo[i][j]

        # Transition logic
        if self.s1[i - 1] == self.s2[j - 1]:
            self.memo[i][j] = self._compute_distance(i - 1, j - 1)
        else:
            insert_op = self._compute_distance(i, j - 1)
            delete_op = self._compute_distance(i - 1, j)
            replace_op = self._compute_distance(i - 1, j - 1)
            self.memo[i][j] = 1 + min(insert_op, delete_op, replace_op)

        return self.memo[i][j]
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    std::string s1 = "intention";
    std::string s2 = "execution";

    EditDistanceMemoized solver(s1, s2);
    int dist = solver.get_result();

    std::cout << "Minimum Edit Distance between \"" << s1 
              << "\" and \"" << s2 << "\": " 
              << dist << " (Expected: 5)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    s1 = "intention"
    s2 = "execution"

    solver = EditDistanceMemoized(s1, s2)
    dist = solver.get_result()

    print(f"Minimum Edit Distance between \"{s1}\" and \"{s2}\": {dist} (Expected: 5)")
```
