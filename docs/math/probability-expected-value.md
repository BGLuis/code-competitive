# Probability, Expected Value, and Markov Chains

## Conceptual Explanation
Probability and **Expected Value** are essential topics in advanced competitive programming. They often appear in DP problems (Probability DP) or system-of-equation questions (Markov chains/random walks).

### Linearity of Expectation
One of the most powerful properties of expected value is **Linearity of Expectation**:
$$E[X + Y] = E[X] + E[Y]$$
This property holds **regardless of whether the random variables $X$ and $Y$ are independent**. It allows us to compute complex expectations by breaking them down into the sum of simpler, individual expectations.

### Expected Value on Markov Chains / Random Walks
A classic competitive programming problem involves a **random walk** on a state graph:
- We start at some state $S$.
- At each state $u$, we transition to state $v$ with probability $P(u \to v)$ (where $\sum_v P(u \to v) = 1$).
- We want to find the expected number of steps to reach a target terminal state $T$.

Let $E_u$ be the expected number of steps to reach the target $T$ starting from state $u$.
- For the target state $T$:
  $$E_T = 0$$
- For any other state $u \neq T$:
  $$E_u = 1 + \sum_v P(u \to v) \times E_v$$

This gives us a system of $N$ linear equations with $N$ variables ($E_0, E_1, \dots, E_{N-1}$). We can solve this system using **Gaussian Elimination** in $O(N^3)$ time!

---

## Complexity Analysis
- **System Setup**: $O(N^2)$ to populate the coefficient matrix of the linear equations.
- **Gaussian Elimination**:
  - **Time Complexity**: $O(N^3)$ using standard row reduction with partial pivoting (crucial to avoid numerical instability).
  - **Space Complexity**: $O(N^2)$ to store the coefficient matrix and the constants vector.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

const double EPS = 1e-9;

// Gaussian Elimination solver for A * x = B
// Returns a vector of solutions. Throws runtime_error if no unique solution exists.
std::vector<double> gaussian_elimination(std::vector<std::vector<double>> A, std::vector<double> B) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        A[i].push_back(B[i]); // Augment matrix
    }

    for (int i = 0; i < n; ++i) {
        // Pivot selection (partial pivoting for stability)
        int pivot = i;
        for (int j = i + 1; j < n; ++j) {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i])) {
                pivot = j;
            }
        }
        if (std::abs(A[pivot][i]) < EPS) {
            throw std::runtime_error("System does not have a unique solution.");
        }
        std::swap(A[i], A[pivot]);

        // Eliminate column elements
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                double factor = A[j][i] / A[i][i];
                for (int k = i; k <= n; ++k) {
                    A[j][k] -= factor * A[i][k];
                }
            }
        }
    }

    std::vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = A[i][n] / A[i][i];
    }
    return x;
}

// Solve expected steps on a Markov chain
// transition_prob[u][v] is probability of transitioning from u to v
// target is the terminal state T
std::vector<double> solve_expected_steps(const std::vector<std::vector<double>>& transition_prob, int target) {
    int n = transition_prob.size();
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
    std::vector<double> B(n, 0.0);

    for (int u = 0; u < n; ++u) {
        if (u == target) {
            // E_target = 0 -> 1 * E_target = 0
            A[u][u] = 1.0;
            B[u] = 0.0;
        } else {
            // E_u = 1 + sum_v P(u -> v) * E_v
            // E_u - sum_v P(u -> v) * E_v = 1
            A[u][u] = 1.0;
            for (int v = 0; v < n; ++v) {
                A[u][v] -= transition_prob[u][v];
            }
            B[u] = 1.0;
        }
    }
    return gaussian_elimination(A, B);
}
```

---

## Python Implementation

```python
from typing import List

EPS = 1e-9


def gaussian_elimination(
    A: List[List[float]], B: List[float]
) -> List[float]:
    """Solves A * x = B using Gaussian Elimination with partial pivoting."""
    n = len(A)
    # Build augmented matrix
    M = [A[i] + [B[i]] for i in range(n)]

    for i in range(n):
        # Pivot selection
        pivot = i
        for j in range(i + 1, n):
            if abs(M[j][i]) > abs(M[pivot][i]):
                pivot = j

        if abs(M[pivot][i]) < EPS:
            raise ValueError("System does not have a unique solution.")

        M[i], M[pivot] = M[pivot], M[i]

        # Elimination
        for j in range(n):
            if i != j:
                factor = M[j][i] / M[i][i]
                for k in range(i, n + 1):
                    M[j][k] -= factor * M[i][k]

    # Back substitution / solve
    x = [0.0] * n
    for i in range(n):
        x[i] = M[i][n] / M[i][i]
    return x


def solve_expected_steps(
    transition_prob: List[List[float]], target: int
) -> List[float]:
    """Solves the expected number of steps to reach target in a Markov chain."""
    n = len(transition_prob)
    A = [[0.0] * n for _ in range(n)]
    B = [0.0] * n

    for u in range(n):
        if u == target:
            A[u][u] = 1.0
            B[u] = 0.0
        else:
            A[u][u] = 1.0
            for v in range(n):
                A[u][v] -= transition_prob[u][v]
            B[u] = 1.0

    return gaussian_elimination(A, B)
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // Markov Chain Example: 3 states (0, 1, 2). Target state is 2.
    // Transition probabilities:
    // From 0: goes to 0 (prob 0.5), goes to 1 (prob 0.5)
    // From 1: goes to 0 (prob 0.2), goes to 2 (prob 0.8)
    // From 2: terminal state (1.0 self-loop or irrelevant)
    std::vector<std::vector<double>> P = {
        {0.5, 0.5, 0.0},
        {0.2, 0.0, 0.8},
        {0.0, 0.0, 1.0}
    };
    int target = 2;

    try {
        std::vector<double> E = solve_expected_steps(P, target);
        std::cout << "Expected steps to reach state " << target << " starting from:\n";
        for (int i = 0; i < E.size(); ++i) {
            std::cout << "State " << i << ": " << E[i] << " steps\n";
        }
        // Analytical Verification:
        // E_2 = 0
        // E_1 = 1 + 0.2*E_0 + 0.8*0 = 1 + 0.2*E_0
        // E_0 = 1 + 0.5*E_0 + 0.5*E_1 => 0.5*E_0 = 1 + 0.5*E_1 => E_0 = 2 + E_1
        // E_1 = 1 + 0.2*(2 + E_1) = 1.4 + 0.2*E_1 => 0.8*E_1 = 1.4 => E_1 = 1.75
        // E_0 = 2 + 1.75 = 3.75
        std::cout << "(Expected results: State 0 = 3.75, State 1 = 1.75)\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # Markov Chain: 3 states. Target is 2.
    # Transition Probabilities:
    P = [[0.5, 0.5, 0.0], [0.2, 0.0, 0.8], [0.0, 0.0, 1.0]]
    target = 2

    try:
        E = solve_expected_steps(P, target)
        print(f"Expected steps to reach state {target} starting from:")
        for i, val in enumerate(E):
            print(f"State {i}: {val:.4f} steps")
        print("(Expected results: State 0 = 3.75, State 1 = 1.75)")
    except ValueError as e:
        print(f"Error: {e}")

```
