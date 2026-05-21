# Dynamic Programming (DP)

## Conceptual Explanation

**Dynamic Programming (DP)** is a powerful algorithmic paradigm used to solve optimization and counting problems by breaking them down into simpler, overlapping subproblems. It is highly applicable to problems that exhibit two main properties:

1. **Optimal Substructure**: The optimal solution to the problem can be constructed efficiently from the optimal solutions to its subproblems.
2. **Overlapping Subproblems**: The recursive solution involves solving the exact same subproblems repeatedly.

### Tabulation vs. Memoization

- **Tabulation (Bottom-Up)**: Solves subproblems first (usually starting from the base cases), filling up a table (array/matrix) step-by-step. It is iterative and avoids the overhead of function calls and stack space.
- **Memoization (Top-Down)**: Solves the problem recursively, but caches (memoizes) the results of subproblems to avoid redundant calculations.

*Note: This document focuses primarily on **Tabulation** and associated optimization techniques, as Memoization is detailed in a separate guide.*

### Design Steps for a DP Solution

1. **Define the State**: Determine what parameters uniquely describe a subproblem. For example, `dp[i][j]` could represent the length of the longest common subsequence of the prefixes `A[0...i-1]` and `B[0...j-1]`.
2. **Establish the State Transition Relation**: Find how the solution to a state relates to previous states.
3. **Identify Base Cases**: Determine the initial states whose values are known without transitions.
4. **Determine the Order of Evaluation**: Ensure that when computing the value of a state, all the states it depends on have already been computed.
5. **Space Optimization**: Identify if the entire history of the DP table is needed. Often, to compute the current row/state, we only need the previous row/state, allowing us to reduce space complexity (e.g., from $\mathcal{O}(N^2)$ to $\mathcal{O}(N)$).

### Typical Competitive Programming Use Cases

- Knapsack Problems (0/1, unbounded).
- Substring/Subsequence Alignment (LCS, Edit Distance, LIS).
- Interval DP (Matrix Chain Multiplication, Polygon Triangulation).
- DP on Trees (finding independent sets, path sums).
- Bitmask DP (Hamiltonian paths, assignment problems on small sizes).

---

## Complexity Analysis

Let $N$ and $M$ be the dimensions of the state space.
- **Time Complexity**: $\mathcal{O}(\text{Number of States} \times \text{Transition Time per State})$. For standard Longest Common Subsequence (LCS), this is $\mathcal{O}(N \cdot M)$ states with $\mathcal{O}(1)$ transitions, resulting in $\mathcal{O}(N \cdot M)$ time.
- **Space Complexity**:
  - **Standard Tabulation**: $\mathcal{O}(N \cdot M)$ to store the full 2D grid of states.
  - **Space Optimized**: $\mathcal{O}(\min(N, M))$ by keeping only the current and previous rows in memory, which is sufficient for transitions depending only on the immediate row above.

---

## C++ Implementation

Below is a robust C++ implementation of the **Longest Common Subsequence (LCS)** problem. It includes both the standard 2D DP formulation and the space-optimized 1D array approach.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class LCS {
public:
    // 1. Standard Bottom-Up Tabulation (O(N * M) Space)
    static int solve_standard(const std::string& s1, const std::string& s2) {
        int n = s1.length();
        int m = s2.length();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[n][m];
    }

    // 2. Space Optimized Bottom-Up Tabulation (O(min(N, M)) Space)
    static int solve_space_optimized(const std::string& s1, const std::string& s2) {
        // Ensure s1 is shorter or equal to s2 to optimize space to O(min(N, M))
        if (s1.length() > s2.length()) {
            return solve_space_optimized(s2, s1);
        }

        int n = s1.length(); // shorter
        int m = s2.length(); // longer

        std::vector<int> prev(n + 1, 0);
        std::vector<int> curr(n + 1, 0);

        for (int j = 1; j <= m; ++j) {
            for (int i = 1; i <= n; ++i) {
                if (s1[i - 1] == s2[j - 1]) {
                    curr[i] = prev[i - 1] + 1;
                } else {
                    curr[i] = std::max(curr[i - 1], prev[i]);
                }
            }
            prev = curr; // Move current row to previous
        }

        return prev[n];
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints and space optimization.

```python
from typing import List

class LCS:
    @staticmethod
    def solve_standard(s1: str, s2: str) -> int:
        """
        Computes the LCS of s1 and s2 using standard O(N * M) space.
        """
        n, m = len(s1), len(s2)
        dp = [[0] * (m + 1) for _ in range(n + 1)]

        for i in range(1, n + 1):
            for j in range(1, m + 1):
                if s1[i - 1] == s2[j - 1]:
                    dp[i][j] = dp[i - 1][j - 1] + 1
                else:
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

        return dp[n][m]

    @staticmethod
    def solve_space_optimized(s1: str, s2: str) -> int:
        """
        Computes the LCS of s1 and s2 using space-optimized O(min(N, M)) space.
        """
        if len(s1) > len(s2):
            s1, s2 = s2, s1

        n, m = len(s1), len(s2)
        prev = [0] * (n + 1)
        curr = [0] * (n + 1)

        for j in range(1, m + 1):
            for i in range(1, n + 1):
                if s1[i - 1] == s2[j - 1]:
                    curr[i] = prev[i - 1] + 1
                else:
                    curr[i] = max(curr[i - 1], prev[i])
            prev = curr[:]  # Deep copy current row to previous row
            
        return prev[n]
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    std::string s1 = "abcde";
    std::string s2 = "ace";

    int lcs_len_1 = LCS::solve_standard(s1, s2);
    int lcs_len_2 = LCS::solve_space_optimized(s1, s2);

    std::cout << "LCS of " << s1 << " and " << s2 << ":\n";
    std::cout << "Standard DP space length: " << lcs_len_1 << " (Expected: 3)\n";
    std::cout << "Optimized DP space length: " << lcs_len_2 << " (Expected: 3)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    s1 = "abcde"
    s2 = "ace"

    lcs_len_1 = LCS.solve_standard(s1, s2)
    lcs_len_2 = LCS.solve_space_optimized(s1, s2)

    print(f"LCS of {s1} and {s2}:")
    print(f"Standard DP space length: {lcs_len_1} (Expected: 3)")
    print(f"Optimized DP space length: {lcs_len_2} (Expected: 3)")
```
