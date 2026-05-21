# Stable Quadratic Equation Solver

## Conceptual Explanation
Solving the quadratic equation $a x^2 + b x + c = 0$ is a frequent subtask in geometry and physics problems in competitive programming. While the standard formula $x = \frac{-b \pm \sqrt{D}}{2a}$ is mathematically correct, implementing it directly using floating-point types can lead to **loss of significance (catastrophic cancellation)**.

### Mathematical Analysis

#### 1. Discriminant $D$
The discriminant $D = b^2 - 4ac$ determines the nature of the roots:
- $D > 0$: Two distinct real roots.
- $D = 0$: One double real root.
- $D < 0$: Two complex conjugate roots.

#### 2. Loss of Significance (Catastrophic Cancellation)
If $b \approx \sqrt{D}$, then subtracting $b$ from $\sqrt{D}$ (which occurs in one of the roots when $b > 0$, or $-b + \sqrt{D}$) involves subtracting two nearly equal numbers. This operation discards the most significant digits, introducing large numerical precision errors.

#### 3. Stable Root Computation
To avoid catastrophic cancellation, we compute the larger magnitude root $x_1$ stably:
$$x_1 = \frac{-b - \text{sgn}(b)\sqrt{b^2 - 4ac}}{2a}$$
where $\text{sgn}(b)$ is $1$ if $b \ge 0$, and $-1$ if $b < 0$.

Since the product of the roots is $x_1 \cdot x_2 = \frac{c}{a}$ (Vieta's formula), we can calculate the second root $x_2$ directly and stably using division:
$$x_2 = \frac{c}{a \cdot x_1}$$

#### 4. Handling Edge Cases
- **Linear Case ($a = 0$)**: The equation becomes $b x + c = 0$.
  - If $b \neq 0$: One root $x = -c / b$.
  - If $b = 0$:
    - If $c = 0$: Infinitely many solutions (returns an empty set but flag/status indicates all real numbers).
    - If $c \neq 0$: No solution.

---

## Complexity Analysis
- **Time Complexity**: $O(1)$ since it only involves a few elementary algebraic operations and a single square root evaluation.
- **Space Complexity**: $O(1)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const double EPS = 1e-9;

// Representation of the solving result
struct QuadraticResult {
    enum Type {
        NO_SOLUTION,
        INFINITE_SOLUTIONS,
        ONE_REAL_ROOT,
        TWO_REAL_ROOTS,
        COMPLEX_ROOTS
    };

    Type type;
    std::vector<double> real_roots;
    // In case of complex roots: store as {real1, imag1, real2, imag2}
    std::vector<std::pair<double, double>> complex_roots;
};

// Stable quadratic solver for a*x^2 + b*x + c = 0
QuadraticResult solve_quadratic(double a, double b, double c) {
    QuadraticResult res;

    // Edge Case: Linear equation (a == 0)
    if (std::abs(a) < EPS) {
        if (std::abs(b) < EPS) {
            if (std::abs(c) < EPS) {
                res.type = QuadraticResult::INFINITE_SOLUTIONS;
            } else {
                res.type = QuadraticResult::NO_SOLUTION;
            }
        } else {
            res.type = QuadraticResult::ONE_REAL_ROOT;
            res.real_roots.push_back(-c / b);
        }
        return res;
    }

    double D = b * b - 4.0 * a * c;

    if (D < -EPS) {
        // Complex roots
        res.type = QuadraticResult::COMPLEX_ROOTS;
        double real_part = -b / (2.0 * a);
        double imag_part = std::sqrt(-D) / (2.0 * a);
        res.complex_roots.push_back({real_part, imag_part});
        res.complex_roots.push_back({real_part, -imag_part});
    } else if (std::abs(D) < EPS) {
        // One double root
        res.type = QuadraticResult::ONE_REAL_ROOT;
        res.real_roots.push_back(-b / (2.0 * a));
    } else {
        // Two distinct real roots (stably computed to prevent cancellation)
        res.type = QuadraticResult::TWO_REAL_ROOTS;
        double q = -0.5 * (b + (b >= 0 ? 1.0 : -1.0) * std::sqrt(D));
        double r1 = q / a;
        double r2 = c / q;
        
        res.real_roots.push_back(std::min(r1, r2));
        res.real_roots.push_back(std::max(r1, r2));
    }
    return res;
}
```

---

## Python Implementation

```python
from enum import Enum
import math
from typing import List, Tuple

EPS = 1e-9


class SolutionType(Enum):
    NO_SOLUTION = 0
    INFINITE_SOLUTIONS = 1
    ONE_REAL_ROOT = 2
    TWO_REAL_ROOTS = 3
    COMPLEX_ROOTS = 4


class QuadraticResult:
    def __init__(self, sol_type: SolutionType):
        self.type = sol_type
        self.real_roots: List[float] = []
        self.complex_roots: List[Tuple[float, float]] = []

    def __repr__(self) -> str:
        if self.type == SolutionType.INFINITE_SOLUTIONS:
            return "Infinite Solutions"
        if self.type == SolutionType.NO_SOLUTION:
            return "No Solution"
        if self.type == SolutionType.ONE_REAL_ROOT:
            return f"One Real Root: {self.real_roots}"
        if self.type == SolutionType.TWO_REAL_ROOTS:
            return f"Two Real Roots: {self.real_roots}"
        return f"Complex Roots: {self.complex_roots}"


def solve_quadratic(a: float, b: float, c: float) -> QuadraticResult:
    """Stably solves a*x^2 + b*x + c = 0, avoiding precision loss."""
    # Linear check
    if abs(a) < EPS:
        if abs(b) < EPS:
            if abs(c) < EPS:
                return QuadraticResult(SolutionType.INFINITE_SOLUTIONS)
            return QuadraticResult(SolutionType.NO_SOLUTION)
        res = QuadraticResult(SolutionType.ONE_REAL_ROOT)
        res.real_roots.append(-c / b)
        return res

    D = b * b - 4.0 * a * c

    if D < -EPS:
        res = QuadraticResult(SolutionType.COMPLEX_ROOTS)
        real_part = -b / (2.0 * a)
        imag_part = math.sqrt(-D) / (2.0 * a)
        res.complex_roots.append((real_part, imag_part))
        res.complex_roots.append((real_part, -imag_part))
    elif abs(D) < EPS:
        res = QuadraticResult(SolutionType.ONE_REAL_ROOT)
        res.real_roots.append(-b / (2.0 * a))
    else:
        res = QuadraticResult(SolutionType.TWO_REAL_ROOTS)
        # Stable root calculation
        sgn = 1.0 if b >= 0 else -1.0
        q = -0.5 * (b + sgn * math.sqrt(D))
        r1 = q / a
        r2 = c / q
        res.real_roots.extend(sorted([r1, r2]))

    return res
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    std::cout << std::fixed << std::setprecision(10);

    // Case 1: High precision risk (a = 1, b = 1e9, c = 1)
    // The roots are extremely asymmetric: one is close to -1e9, another is close to -1e-9.
    // Standard formula -b + sqrt(b^2 - 4ac) will calculate -1e9 + 1e9 which loses all precision.
    double a = 1.0, b = 1e9, c = 1.0;
    auto res1 = solve_quadratic(a, b, c);
    std::cout << "Solving 1*x^2 + 1e9*x + 1 = 0:\n";
    if (res1.type == QuadraticResult::TWO_REAL_ROOTS) {
        std::cout << "Root 1 (small magnitude): " << res1.real_roots[1] << "\n";
        std::cout << "Root 2 (large magnitude): " << res1.real_roots[0] << "\n";
    }

    // Case 2: Complex roots (a = 1, b = 2, c = 5 -> roots -1 + 2i, -1 - 2i)
    auto res2 = solve_quadratic(1.0, 2.0, 5.0);
    std::cout << "\nSolving 1*x^2 + 2*x + 5 = 0:\n";
    if (res2.type == QuadraticResult::COMPLEX_ROOTS) {
        std::cout << "Root 1: " << res2.complex_roots[0].first << " + " << res2.complex_roots[0].second << "i\n";
        std::cout << "Root 2: " << res2.complex_roots[1].first << " + " << res2.complex_roots[1].second << "i\n";
    }

    // Case 3: Linear equation (a = 0, b = 2, c = -4)
    auto res3 = solve_quadratic(0.0, 2.0, -4.0);
    std::cout << "\nSolving 0*x^2 + 2*x - 4 = 0:\n";
    if (res3.type == QuadraticResult::ONE_REAL_ROOT) {
        std::cout << "Root: " << res3.real_roots[0] << "\n";
    }

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # Case 1: Large asymmetry (a = 1, b = 1e9, c = 1)
    # The standard subtraction suffers from floating-point failure.
    res1 = solve_quadratic(1.0, 1e9, 1.0)
    print("Solving 1*x^2 + 1e9*x + 1 = 0:")
    print(res1)
    print(f"Product of roots (Vieta's): {res1.real_roots[0] * res1.real_roots[1]} (expected 1.0)\n")

    # Case 2: Complex Roots
    res2 = solve_quadratic(1.0, 2.0, 5.0)
    print("Solving 1*x^2 + 2*x + 5 = 0:")
    print(res2)
    print()

    # Case 3: Linear Edge Case
    res3 = solve_quadratic(0.0, 2.0, -4.0)
    print("Solving 0*x^2 + 2*x - 4 = 0:")
    print(res3)
```
