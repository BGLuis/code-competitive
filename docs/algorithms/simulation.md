# Simulation

## Conceptual Explanation

**Simulation** is a common paradigm in competitive programming where the problem asks you to model a specific process or game exactly as described by a set of rules. Unlike other paradigms that search for short-cuts (like greedy or dynamic programming), simulation requires direct, precise implementation of the rules.

### Key Challenges in Simulation Problems

1. **State representation**: Designing data structures that accurately represent the system's current state (e.g. matrices, coordinate lists, structures/classes).
2. **Transition Logic**: Writing clean, robust, bug-free functions to transition the state from step $T$ to step $T+1$.
3. **Corner Cases / Boundary Conditions**: Checking bounds of arrays/grids, handling edge collisions, wrap-arounds, or terminal states.
4. **Time & Space Optimization**: In-place updates, bitwise state encoding, or coordinate compression can be required if the state is large or if there are many time-steps.

---

### In-Place Grid Simulation: Conway's Game of Life

In Conway's Game of Life, we have an $R \times C$ board of cells, where each cell is either 0 (dead) or 1 (alive). The state transitions simultaneously according to four rules:
1. Any live cell with fewer than two live neighbors dies (underpopulation).
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies (overpopulation).
4. Any dead cell with exactly three live neighbors becomes a live cell (reproduction).

#### Optimization: In-Place Encoding
To update the board in-place (without allocating an auxiliary grid), we can use the **two least significant bits** of each cell value:
- **Bit 0**: Current state (0 or 1).
- **Bit 1**: Next state (0 or 1).

During neighbors counting, we extract the current state using `cell & 1`. Once the next state is calculated, we encode it in the second bit using `cell |= (next_state << 1)`. Finally, we shift all cells right by 1 bit (`cell >>= 1`) to compile the board to the next generation in $\mathcal{O}(1)$ auxiliary space.

---

## Complexity Analysis

- **Time Complexity**: $\mathcal{O}(G \cdot R \cdot C)$, where $G$ is the number of generations (time-steps) and $R \cdot C$ is the board size. For each cell, we inspect at most 8 neighbors, which is $\mathcal{O}(1)$ work.
- **Space Complexity**:
  - **Naive**: $\mathcal{O}(R \cdot C)$ to store a copy of the grid for updates.
  - **In-Place Bit Encoding**: $\mathcal{O}(1)$ auxiliary space.

---

## C++ Implementation

Below is a complete C++ class representing the Game of Life grid with in-place bitwise simulation updates.

```cpp
#include <iostream>
#include <vector>

class GameOfLife {
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> board;

    // Helper to count active neighbors in current state (using bit 0)
    int count_alive_neighbors(int r, int c) const {
        int count = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    // Extract current state (least significant bit)
                    count += (board[nr][nc] & 1);
                }
            }
        }
        return count;
    }

public:
    explicit GameOfLife(const std::vector<std::vector<int>>& initial_grid) 
        : rows(initial_grid.size()), cols(initial_grid[0].size()), board(initial_grid) {}

    // Simulates one step of generation in-place
    void simulate_next_generation() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int alive_neighbors = count_alive_neighbors(r, c);
                int current_state = board[r][c] & 1;
                int next_state = 0;

                if (current_state == 1) {
                    if (alive_neighbors == 2 || alive_neighbors == 3) {
                        next_state = 1;
                    }
                } else {
                    if (alive_neighbors == 3) {
                        next_state = 1;
                    }
                }

                // Encode next state into the 2nd bit (bit 1)
                board[r][c] |= (next_state << 1);
            }
        }

        // Apply transition by shifting bits to the right
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                board[r][c] >>= 1;
            }
        }
    }

    const std::vector<std::vector<int>>& get_board() const {
        return board;
    }

    void print() const {
        for (const auto& row : board) {
            for (int cell : row) {
                std::cout << (cell ? "Q" : ".") << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List

class GameOfLife:
    def __init__(self, initial_grid: List[List[int]]):
        self.rows = len(initial_grid)
        self.cols = len(initial_grid[0])
        # Make a copy of the grid
        self.board = [row[:] for row in initial_grid]

    def _count_alive_neighbors(self, r: int, c: int) -> int:
        count = 0
        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue
                nr, nc = r + dr, c + dc
                if 0 <= nr < self.rows and 0 <= nc < self.cols:
                    # Check bit 0 for current state
                    count += (self.board[nr][nc] & 1)
        return count

    def simulate_next_generation(self) -> None:
        """
        Runs one iteration of Game of Life in-place using bitwise encoding.
        """
        for r in range(self.rows):
            for c in range(self.cols):
                alive_neighbors = self._count_alive_neighbors(r, c)
                current_state = self.board[r][c] & 1
                next_state = 0

                if current_state == 1:
                    if alive_neighbors in (2, 3):
                        next_state = 1
                else:
                    if alive_neighbors == 3:
                        next_state = 1

                # Store next state in bit 1
                self.board[r][c] |= (next_state << 1)

        # Shift right by 1 to make next state the current state
        for r in range(self.rows):
            for c in range(self.cols):
                self.board[r][c] >>= 1

    def print_board(self) -> None:
        for row in self.board:
            print(" ".join("Q" if cell else "." for cell in row))
        print()
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Initializing a Blider pattern
    std::vector<std::vector<int>> initial = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}
    };

    GameOfLife game(initial);

    std::cout << "Generation 0 (Initial):\n";
    game.print();

    game.simulate_next_generation();
    std::cout << "Generation 1:\n";
    game.print();

    game.simulate_next_generation();
    std::cout << "Generation 2:\n";
    game.print();

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    initial = [
        [0, 1, 0],
        [0, 0, 1],
        [1, 1, 1],
        [0, 0, 0]
    ]

    game = GameOfLife(initial)

    print("Generation 0 (Initial):")
    game.print_board()

    game.simulate_next_generation()
    print("Generation 1:")
    game.print_board()

    game.simulate_next_generation()
    print("Generation 2:")
    game.print_board()
```
