# Quadratic Optimization (Single Variable)

## Conceptual Explanation
Quadratic optimization involves finding the input $x$ that minimizes or maximizes a quadratic function:
$$f(x) = a x^2 + b x + c$$
In competitive programming, we frequently solve this subject to various constraints:
1. **Unconstrained**:
   - If $a > 0$, $f(x)$ is convex (opens upward) and has a unique global minimum.
   - If $a < 0$, $f(x)$ is concave (opens downward) and has a unique global maximum.
   - The extremum (vertex) occurs at:
     $$x^* = -\frac{b}{2a}$$
2. **Interval Constraints ($x \in [L, R]$)**:
   - Since $f(x)$ is strictly monotonic on either side of the vertex $x^*$, the optimal value of $x$ inside an interval $[L, R]$ must be one of:
     - The vertex $x^*$ (if $L \le x^* \le R$).
     - The boundaries $L$ or $R$.
3. **Integer Constraints**:
   - When $x$ must be an integer, $x^*$ may be fractional. The optimal integer must be one of:
     - The closest integers to the vertex: $\lfloor x^* \rfloor$ or $\lceil x^* \rceil$ (if they lie within $[L, R]$).
     - The boundaries $L$ or $R$.

---

## Complexity Analysis
- **Time Complexity**: $O(1)$ since all operations are direct arithmetic calculations (vertex evaluation, boundary comparisons).
- **Space Complexity**: $O(1)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iomanip>

struct OptimizationResultReal {
    double best_x;
    double best_val;
};

struct OptimizationResultInt {
    long long best_x;
    long long best_val;
};

// 1. Optimize real x in [L, R] for f(x) = a*x^2 + b*x + c
OptimizationResultReal optimize_quadratic_real(double a, double b, double c, double L, double R, bool find_max = false) {
    if (L > R) {
        throw std::invalid_argument("Interval L must be <= R");
    }

    auto f = [=](double x) { return a * x * x + b * x + c; };

    // Candidates are the boundaries
    double best_x = L;
    double best_val = f(L);

    auto update = [&](double x) {
        if (x >= L && x <= R) {
            double val = f(x);
            if (find_max) {
                if (val > best_val) {
                    best_val = val;
                    best_x = x;
                }
            } else {
                if (val < best_val) {
                    best_val = val;
                    best_x = x;
                }
            }
        }
    };

    update(R);

    // Candidate: the vertex (if a is non-zero)
    if (std::abs(a) > 1e-9) {
        double vertex = -b / (2.0 * a);
        update(vertex);
    }

    return {best_x, best_val};
}

// 2. Optimize integer x in [L, R] for f(x) = a*x^2 + b*x + c
OptimizationResultInt optimize_quadratic_integer(double a, double b, double c, long long L, long long R, bool find_max = false) {
    if (L > R) {
        throw std::invalid_argument("Interval L must be <= R");
    }

    auto f = [=](long long x) { return a * x * x + b * x + c; };

    long long best_x = L;
    double best_val = f(L);

    auto update = [&](long long x) {
        if (x >= L && x <= R) {
            double val = f(x);
            if (find_max) {
                if (val > best_val) {
                    best_val = val;
                    best_x = x;
                }
            } else {
                if (val < best_val) {
                    best_val = val;
                    best_x = x;
                }
            }
        }
    };

    update(R);

    // Candidate: nearest integers around the vertex
    if (std::abs(a) > 1e-9) {
        double vertex = -b / (2.0 * a);
        long long v1 = static_cast<long long>(std::floor(vertex));
        long long v2 = static_cast<long long>(std::ceil(vertex));
        update(v1);
        update(v2);
    }

    return {best_x, static_cast<long long>(best_val)};
}
```

---

## Python Implementation

```python
import math
from typing import NamedTuple


class OptimizationResultReal(NamedTuple):
    best_x: float
    best_val: float


class OptimizationResultInt(NamedTuple):
    best_x: int
    best_val: int


def optimize_quadratic_real(
    a: float, b: float, c: float, L: float, R: float, find_max: bool = False
) -> OptimizationResultReal:
    """Finds real x in [L, R] optimizing f(x) = a*x^2 + b*x + c."""
    if L > R:
        raise ValueError("Interval L must be <= R")

    def f(x: float) -> float:
        return a * x * x + b * x + c

    best_x = L
    best_val = f(L)

    def update(x: float):
        nonlocal best_x, best_val
        if L <= x <= R:
            val = f(x)
            if find_max:
                if val > best_val:
                    best_val = val
                    best_x = x
            else:
                if val < best_val:
                    best_val = val
                    best_x = x

    update(R)

    # Check vertex candidate
    if abs(a) > 1e-9:
        vertex = -b / (2.0 * a)
        update(vertex)

    return OptimizationResultReal(best_x, best_val)


def optimize_quadratic_integer(
    a: float, b: float, c: float, L: int, R: int, find_max: bool = False
) -> OptimizationResultInt:
    """Finds integer x in [L, R] optimizing f(x) = a*x^2 + b*x + c."""
    if L > R:
        raise ValueError("Interval L must be <= R")

    def f(x: int) -> float:
        return a * x * x + b * x + c

    best_x = L
    best_val = f(L)

    def update(x: int):
        nonlocal best_x, best_val
        if L <= x <= R:
            val = f(x)
            if find_max:
                if val > best_val:
                    best_val = val
                    best_x = x
            else:
                if val < best_val:
                    best_val = val
                    best_x = x

    update(R)

    # Check nearest integers to the vertex
    if abs(a) > 1e-9:
        vertex = -b / (2.0 * a)
        v1 = math.floor(vertex)
        v2 = math.ceil(vertex)
        update(v1)
        update(v2)

    return OptimizationResultInt(best_x, int(best_val))
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    std::cout << std::fixed << std::setprecision(6);

    // 1. Real minimization of f(x) = 2*x^2 - 8*x + 5 in interval [0.0, 5.0]
    // Vertex is at x = -(-8)/(2*2) = 2.0, which lies in [0, 5].
    // Minimum value should be f(2) = 2*(4) - 16 + 5 = -3.
    double a = 2.0, b = -8.0, c = 5.0;
    auto res_real = optimize_quadratic_real(a, b, c, 0.0, 5.0, false);
    std::cout << "Real Min (in [0, 5]): x = " << res_real.best_x 
              << ", value = " << res_real.best_val << " (expected x=2.0, val=-3.0)\n";

    // 2. Integer maximization of f(x) = -3*x^2 + 10*x + 1 in interval [0, 10]
    // Vertex is at x = -10/(2*-3) = 1.6666...
    // The closest integers are 1 and 2.
    // f(1) = -3 + 10 + 1 = 8.
    // f(2) = -12 + 20 + 1 = 9.
    // Maximum integer should be x = 2 with value 9.
    auto res_int = optimize_quadratic_integer(-3.0, 10.0, 1.0, 0, 10, true);
    std::cout << "Integer Max (in [0, 10]): x = " << res_int.best_x 
              << ", value = " << res_int.best_val << " (expected x=2, val=9)\n";

    // 3. Real optimization with vertex outside interval [4.0, 10.0]
    // Minimum of f(x) = 2*x^2 - 8*x + 5. Vertex is 2.0 (outside [4, 10]).
    // Minimum must be at the left boundary L = 4.0. f(4) = 32 - 32 + 5 = 5.
    auto res_outside = optimize_quadratic_real(a, b, c, 4.0, 10.0, false);
    std::cout << "Real Min (in [4, 10]): x = " << res_outside.best_x 
              << ", value = " << res_outside.best_val << " (expected x=4.0, val=5.0)\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. Real Minimization: f(x) = 2*x^2 - 8*x + 5 in [0.0, 5.0] (expected x=2.0, val=-3.0)
    a, b, c = 2.0, -8.0, 5.0
    res_real = optimize_quadratic_real(a, b, c, 0.0, 5.0, find_max=False)
    print(
        f"Real Min: x = {res_real.best_x:.4f}, value = {res_real.best_val:.4f}"
    )

    # 2. Integer Maximization: f(x) = -3*x^2 + 10*x + 1 in [0, 10] (expected x=2, val=9)
    res_int = optimize_quadratic_integer(-3.0, 10.0, 1.0, 0, 10, find_max=True)
    print(f"Integer Max: x = {res_int.best_x}, value = {res_int.best_val}")

    # 3. Vertex outside boundary: Min in [4.0, 10.0] (expected x=4.0, val=5.0)
    res_out = optimize_quadratic_real(a, b, c, 4.0, 10.0, find_max=False)
    print(f"Outside Min: x = {res_out.best_x:.4f}, value = {res_out.best_val:.4f}")
```
