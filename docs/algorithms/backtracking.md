# Backtracking

## Conceptual Explanation

Backtracking is a systematic algorithmic technique for solving problems recursively by trying to build a solution incrementally, one piece at a time, and removing those solutions that fail to satisfy the constraints of the problem at any point in time (hence "backtracking").

It is essentially a depth-first search (DFS) over an implicit state-space tree of configurations.

### Key Concepts

1. **Choice**: At each step, we have a set of options or choices we can make (e.g., placing a queen in a specific column of a row, choosing to include an element in a subset, or selecting the next vertex in a path).
2. **Constraints**: Conditions that the partial solution must satisfy. If a choice violates a constraint, we discard that path immediately (pruning).
3. **Goal / Base Case**: A state representing a complete, valid solution.
4. **State Restoration**: After exploring a recursive branch, we must undo the last choice to restore the previous state before moving to the next choice. This is the defining characteristic of backtracking.

### Typical Competitive Programming Use Cases

- **Generating Combinatorial Objects**: Permutations, combinations, subsets, partitions.
- **Constraint Satisfaction Problems (CSPs)**: Sudoku, N-Queens, Map Coloring.
- **Path Finding on Graphs/Grids**: Finding Hamiltonian paths, word searches in a grid.
- **Optimization**: Branch and Bound (often an extension of backtracking with lower/upper bounds to prune search).

---

## Complexity Analysis

For backtracking, complexity analysis is highly dependent on the state space and the pruning rules applied.

- **Time Complexity**:
  - *State Space*: In the worst case, backtracking explores the entire state space. For permutations of size $N$, this is $\mathcal{O}(N!)$. For subsets or grid decisions, it can be $\mathcal{O}(2^N)$ or $\mathcal{O}(3^N)$ or more.
  - *Pruning*: Efficient constraint-checking reduces the search space drastically in practice, but the theoretical upper bound remains high.
  - *Constraint Check*: Checking if a state is valid takes $\mathcal{O}(1)$ or $\mathcal{O}(N)$ depending on the representation. With optimized state arrays (bitmasks or helper boolean arrays), it is usually $\mathcal{O}(1)$.
- **Space Complexity**:
  - *Recursion Stack*: $\mathcal{O}(D)$, where $D$ is the maximum depth of the search tree (usually $N$, the size of the input).
  - *State Representation*: $\mathcal{O}(D)$ to store the current decision path.

---

## C++ Implementation

Below is a robust, clean, and modern C++ implementation solving the classic **N-Queens Problem**. It generates all possible valid arrangements of placing $N$ queens on an $N \times N$ chessboard such that no two queens attack each other.

```cpp
#include <iostream>
#include <vector>
#include <string>

class NQueensSolver {
private:
    int n;
    std::vector<std::vector<std::string>> solutions;
    
    // Tracking columns and diagonals to achieve O(1) constraint checks
    std::vector<bool> cols;
    std::vector<bool> diag1; // Top-left to bottom-right (row - col)
    std::vector<bool> diag2; // Top-right to bottom-left (row + col)

    void backtrack(int row, std::vector<std::string>& current_board) {
        // Base case: If we successfully placed queens in all rows, record solution
        if (row == n) {
            solutions.push_back(current_board);
            return;
        }

        for (int col = 0; col < n; ++col) {
            int d1_idx = row - col + (n - 1);
            int d2_idx = row + col;

            // Check constraints
            if (cols[col] || diag1[d1_idx] || diag2[d2_idx]) {
                continue; // Prune this branch
            }

            // 1. Make choice
            current_board[row][col] = 'Q';
            cols[col] = true;
            diag1[d1_idx] = true;
            diag2[d2_idx] = true;

            // 2. Explore recursively
            backtrack(row + 1, current_board);

            // 3. Undo choice (Backtrack)
            current_board[row][col] = '.';
            cols[col] = false;
            diag1[d1_idx] = false;
            diag2[d2_idx] = false;
        }
    }

public:
    explicit NQueensSolver(int n_val) : n(n_val), cols(n_val, false), diag1(2 * n_val - 1, false), diag2(2 * n_val - 1, false) {}

    std::vector<std::vector<std::string>> solve() {
        solutions.clear();
        std::vector<std::string> board(n, std::string(n, '.'));
        backtrack(0, board);
        return solutions;
    }
};
```

---

## Python Implementation

Here is the equivalent Python 3 implementation with complete type hints.

```python
from typing import List

class NQueensSolver:
    def __init__(self, n: int):
        self.n = n
        self.solutions: List[List[str]] = []
        # Tracking vectors for O(1) checks
        self.cols = [False] * n
        self.diag1 = [False] * (2 * n - 1)  # row - col
        self.diag2 = [False] * (2 * n - 1)  # row + col

    def solve(self) -> List[List[str]]:
        self.solutions.clear()
        board = [["."] * self.n for _ in range(self.n)]
        self._backtrack(0, board)
        return self.solutions

    def _backtrack(self, row: int, board: List[List[str]]) -> None:
        # Base case
        if row == self.n:
            self.solutions.append(["".join(r) for r in board])
            return

        for col in range(self.n):
            d1_idx = row - col + (self.n - 1)
            d2_idx = row + col

            # Check constraints
            if self.cols[col] or self.diag1[d1_idx] or self.diag2[d2_idx]:
                continue

            # 1. Make choice
            board[row][col] = "Q"
            self.cols[col] = True
            self.diag1[d1_idx] = True
            self.diag2[d2_idx] = True

            # 2. Explore
            self._backtrack(row + 1, board)

            # 3. Undo choice (Backtrack)
            board[row][col] = "."
            self.cols[col] = False
            self.diag1[d1_idx] = False
            self.diag2[d2_idx] = False
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    int n = 4;
    NQueensSolver solver(n);
    std::vector<std::vector<std::string>> solutions = solver.solve();

    std::cout << "Total solutions for " << n << "-Queens: " << solutions.size() << "\n\n";
    for (size_t i = 0; i < solutions.size(); ++i) {
        std::cout << "Solution #" << i + 1 << ":\n";
        for (const std::string& row : solutions[i]) {
            std::cout << row << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    n = 4
    solver = NQueensSolver(n)
    solutions = solver.solve()

    print(f"Total solutions for {n}-Queens: {len(solutions)}\n")
    for idx, board in enumerate(solutions):
        print(f"Solution #{idx + 1}:")
        for row in board:
            print(row)
        print()
```
