# Matrix Multiplication and Exponentiation

## Conceptual Explanation
Matrix arithmetic, specifically **Matrix Multiplication** and **Matrix Exponentiation**, is a highly versatile tool in competitive programming. It is predominantly used to:
1. **Solve Linear Recurrences in $O(D^3 \log N)$**: For a linear recurrence $F_n = c_1 F_{n-1} + c_2 F_{n-2} + \dots + c_d F_{n-d}$, we can express the transition as:
   $$\begin{pmatrix} F_n \\ F_{n-1} \\ \vdots \\ F_{n-d+1} \end{pmatrix} = M \cdot \begin{pmatrix} F_{n-1} \\ F_{n-2} \\ \vdots \\ F_{n-d} \end{pmatrix}$$
   where $M$ is the $D \times D$ transition matrix. By repeated multiplication, the state at step $N$ is obtained via $M^{N-d} \cdot \vec{v}_{start}$.
2. **Count Paths in a Graph**: If $A$ is the adjacency matrix of a graph, then the element $(A^K)_{i, j}$ represents the number of paths of exact length $K$ from vertex $i$ to vertex $j$.

### Matrix Multiplication
Let $A$ be a matrix of size $N \times M$ and $B$ be a matrix of size $M \times P$. Their product $C = A \times B$ is an $N \times P$ matrix where:
$$C_{i, j} = \sum_{k=0}^{M-1} A_{i, k} \times B_{k, j}$$

In competitive programming, calculations are almost always performed modulo a large prime (e.g., $10^9 + 7$).

### Matrix Exponentiation
Using the **binary exponentiation** (divide-and-conquer) technique, we can compute the $K$-th power of a square matrix $M$ in $O(D^3 \log K)$ matrix multiplications:
- If $K = 0$, $M^0 = I$ (the Identity matrix).
- If $K$ is odd, $M^K = M \times M^{K-1}$.
- If $K$ is even, $M^K = (M^{K/2})^2$.

---

## Complexity Analysis
- **Matrix Multiplication ($N \times M$ by $M \times P$)**:
  - **Time Complexity**: $O(N \cdot M \cdot P)$ using the standard triply nested loop. For square matrices of size $D$, it is $O(D^3)$.
  - **Space Complexity**: $O(N \cdot P)$ to store the result.
- **Matrix Exponentiation ($D \times D$ matrix to power $K$)**:
  - **Time Complexity**: $O(D^3 \log K)$ since binary exponentiation performs at most $2 \log_2 K$ matrix multiplications.
  - **Space Complexity**: $O(D^2)$ to store intermediate powers.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

// A generic Matrix class optimized for competitive programming (incorporating modulo)
class Matrix {
public:
    int rows, cols;
    std::vector<std::vector<long long>> mat;
    long long mod;

    Matrix(int r, int c, long long m = 1e9 + 7, long long default_val = 0)
        : rows(r), cols(c), mod(m), mat(r, std::vector<long long>(c, default_val)) {}

    // Static function to create an Identity Matrix
    static Matrix identity(int n, long long m = 1e9 + 7) {
        Matrix I(n, n, m);
        for (int i = 0; i < n; ++i) {
            I.mat[i][i] = 1;
        }
        return I;
    }

    // Multiply two matrices modulo mod
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::runtime_error("Dimension mismatch for matrix multiplication.");
        }
        Matrix res(rows, other.cols, mod);
        for (int i = 0; i < rows; ++i) {
            for (int k = 0; k < cols; ++k) {
                if (mat[i][k] == 0) continue; // Optimization for sparse elements
                for (int j = 0; j < other.cols; ++j) {
                    res.mat[i][j] = (res.mat[i][j] + mat[i][k] * other.mat[k][j]) % mod;
                }
            }
        }
        return res;
    }

    // Binary Exponentiation of a square matrix
    Matrix power(long long exp) const {
        if (rows != cols) {
            throw std::runtime_error("Matrix must be square to perform exponentiation.");
        }
        Matrix res = Matrix::identity(rows, mod);
        Matrix base = *this;
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << mat[i][j] << (j == cols - 1 ? "" : " ");
            }
            std::cout << "\n";
        }
    }
};
```

---

## Python Implementation

```python
from typing import List


class Matrix:
    def __init__(
        self,
        rows: int,
        cols: int,
        mod: int = 10**9 + 7,
        default_val: int = 0,
    ):
        self.rows = rows
        self.cols = cols
        self.mod = mod
        self.mat = [[default_val] * cols for _ in range(rows)]

    @classmethod
    def identity(cls, n: int, mod: int = 10**9 + 7) -> "Matrix":
        """Creates an n x n identity matrix."""
        I = cls(n, n, mod)
        for i in range(n):
            I.mat[i][i] = 1
        return I

    def __mul__(self, other: "Matrix") -> "Matrix":
        """Performs matrix multiplication modulo mod."""
        if self.cols != other.rows:
            raise ValueError("Dimension mismatch for matrix multiplication.")

        res = Matrix(self.rows, other.cols, self.mod)
        for i in range(self.rows):
            for k in range(self.cols):
                val = self.mat[i][k]
                if val == 0:
                    continue  # Optimization
                for j in range(other.cols):
                    res.mat[i][j] = (
                        res.mat[i][j] + val * other.mat[k][j]
                    ) % self.mod
        return res

    def power(self, exp: int) -> "Matrix":
        """Computes self^exp modulo mod using binary exponentiation."""
        if self.rows != self.cols:
            raise ValueError(
                "Matrix must be square to perform exponentiation."
            )

        res = Matrix.identity(self.rows, self.mod)
        base = self
        curr_exp = exp

        while curr_exp > 0:
            if curr_exp & 1:
                res = res * base
            base = base * base
            curr_exp >>= 1

        return res

    def __str__(self) -> str:
        return "\n".join(" ".join(map(str, row)) for row in self.mat)
```

---

## Usage Examples

### C++ Example (Fibonacci in $O(\log N)$)
```cpp
// Compute the N-th Fibonacci number modulo 10^9 + 7
// F_0 = 0, F_1 = 1, F_2 = 1, ...
long long fibonacci(long long N) {
    if (N == 0) return 0;
    if (N == 1) return 1;

    // Transition matrix M:
    // [1 1]
    // [1 0]
    Matrix M(2, 2);
    M.mat[0][0] = 1; M.mat[0][1] = 1;
    M.mat[1][0] = 1; M.mat[1][1] = 0;

    Matrix M_power = M.power(N - 1);
    
    // The vector is [F_1, F_0]^T = [1, 0]^T
    // Result F_N is M_power[0][0]*F_1 + M_power[0][1]*F_0 = M_power[0][0]
    return M_power.mat[0][0];
}

int main() {
    long long N = 10;
    std::cout << "F_" << N << " mod 10^9+7 = " << fibonacci(N) << " (expected 55)\n";

    N = 1000000000000LL; // 10^12
    std::cout << "F_" << N << " mod 10^9+7 = " << fibonacci(N) << "\n";

    return 0;
}
```

### Python Example (Fibonacci in $O(\log N)$)
```python
def fibonacci(N: int, mod: int = 10**9 + 7) -> int:
    if N == 0:
        return 0
    if N == 1:
        return 1

    # Transition matrix
    M = Matrix(2, 2, mod)
    M.mat[0][0] = 1
    M.mat[0][1] = 1
    M.mat[1][0] = 1
    M.mat[1][1] = 0

    M_power = M.power(N - 1)
    return M_power.mat[0][0]


if __name__ == "__main__":
    N = 10
    print(f"F_{N} mod 10^9+7 = {fibonacci(N)} (expected 55)")

    N = 1000000000000  # 10^12
    print(f"F_{N} mod 10^9+7 = {fibonacci(N)}")
```
