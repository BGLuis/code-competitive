# Floating-Point Approximation and Precision

## Conceptual Explanation
In competitive programming, calculations involving real numbers (`double`, `long double` in C++, or `float` in Python) are subject to rounding and precision errors. This is because computers represent real numbers using binary floating-point representation (IEEE 754), which cannot precisely represent many decimal fractions (e.g., $0.1$ or $0.3$).

### Key Challenges and Solutions

#### 1. Precision Comparisons ($\epsilon$ Helpers)
Directly comparing two floating-point numbers using `==`, `<`, or `>` is highly prone to errors. We solve this by introducing a small tolerance parameter, **Epsilon ($\epsilon$)**, typically set between $10^{-9}$ and $10^{-12}$.
- **Equality**: $a = b \iff |a - b| < \epsilon$
- **Less than**: $a < b \iff a < b - \epsilon$
- **Greater than**: $a > b \iff a > b + \epsilon$
- **Less than or equal**: $a \le b \iff a < b + \epsilon$
- **Greater than or equal**: $a \ge b \iff a > b - \epsilon$

#### 2. Absolute vs. Relative Precision
When values are extremely large (e.g., $10^{12}$), an absolute threshold like $\epsilon = 10^{-9}$ is smaller than the spacing between representable floating-point numbers. Thus, we combine absolute and relative errors:
$$\text{Equal}(a, b) \iff |a - b| < \epsilon \times \max(1.0, |a|, |b|)$$

#### 3. Real Binary Search
When binary searching over a real domain, testing `while (R - L > eps)` can lead to infinite loops if $R$ and $L$ become so close that the difference underflow the precision limits, or if $\epsilon$ is too small. 
- **Fixed-Iteration Strategy**: Running the binary search loop for a fixed number of steps (e.g., 80 to 100 iterations). 80 steps reduce the search interval by a factor of $2^{-80} \approx 8.27 \times 10^{-25}$, which is far beyond the precision of a 64-bit `double` (about 15-17 decimal digits). This is the safest and most robust approach in competitive programming.

---

## Complexity Analysis
- **Epsilon comparisons**: $O(1)$ time and space.
- **Fixed-iteration binary search**: $O(N \cdot T(f))$ time, where $N$ is the number of iterations (typically 80-100) and $T(f)$ is the verification function cost. Space complexity is $O(1)$.

---

## C++ Implementation

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

class FloatingPoint {
public:
    static constexpr double EPS = 1e-9;

    // Check if two floating point numbers are equal (absolute threshold)
    static bool equal(double a, double b) {
        return std::abs(a - b) < EPS;
    }

    // Check if two floating point numbers are equal (relative + absolute threshold)
    static bool equal_relative(double a, double b) {
        return std::abs(a - b) < EPS * std::max({1.0, std::abs(a), std::abs(b)});
    }

    // Check if a < b
    static bool less(double a, double b) {
        return a < b - EPS;
    }

    // Check if a <= b
    static bool less_or_equal(double a, double b) {
        return a < b + EPS;
    }

    // Check if a > b
    static bool greater(double a, double b) {
        return a > b + EPS;
    }

    // Check if a >= b
    static bool greater_or_equal(double a, double b) {
        return a > b - EPS;
    }

    // Return sign: -1 if negative, 1 if positive, 0 if zero (within epsilon)
    static int sign(double x) {
        if (x < -EPS) return -1;
        if (x > EPS) return 1;
        return 0;
    }
};

// Stable binary search on real numbers using fixed iterations
// Finds the boundary x in [L, R] such that ok(x) is true
template <typename Func>
double binary_search_real(double L, double R, Func ok, int iterations = 80) {
    for (int iter = 0; iter < iterations; ++iter) {
        double mid = L + (R - L) / 2.0;
        if (ok(mid)) {
            R = mid; // or L = mid depending on problem direction
        } else {
            L = mid; // or R = mid depending on problem direction
        }
    }
    return L;
}
```

---

## Python Implementation

```python
from typing import Callable
import math


class FloatingPoint:
    EPS = 1e-9

    @classmethod
    def equal(cls, a: float, b: float) -> bool:
        """Returns True if a and b are equal within absolute EPS."""
        return abs(a - b) < cls.EPS

    @classmethod
    def equal_relative(cls, a: float, b: float) -> bool:
        """Returns True if a and b are equal within relative and absolute EPS."""
        return abs(a - b) < cls.EPS * max(1.0, abs(a), abs(b))

    @classmethod
    def less(cls, a: float, b: float) -> bool:
        """Returns True if a < b (within EPS)."""
        return a < b - cls.EPS

    @classmethod
    def less_or_equal(cls, a: float, b: float) -> bool:
        """Returns True if a <= b (within EPS)."""
        return a < b + cls.EPS

    @classmethod
    def greater(cls, a: float, b: float) -> bool:
        """Returns True if a > b (within EPS)."""
        return a > b + cls.EPS

    @classmethod
    def greater_or_equal(cls, a: float, b: float) -> bool:
        """Returns True if a >= b (within EPS)."""
        return a > b - cls.EPS

    @classmethod
    def sign(cls, x: float) -> int:
        """Returns -1 if x < -EPS, 1 if x > EPS, 0 otherwise."""
        if x < -cls.EPS:
            return -1
        if x > cls.EPS:
            return 1
        return 0


def binary_search_real(
    l: float, r: float, ok: Callable[[float], bool], iterations: int = 80
) -> float:
    """Stable floating-point binary search using a fixed number of iterations."""
    for _ in range(iterations):
        mid = l + (r - l) / 2.0
        if ok(mid):
            r = mid
        else:
            l = mid
    return l
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    double a = 0.1 + 0.2;
    double b = 0.3;

    std::cout << std::fixed << std::setprecision(17);
    std::cout << "Direct comparison (a == b): " << (a == b ? "Equal" : "Not Equal") << "\n";
    std::cout << "Epsilon comparison (equal(a, b)): " 
              << (FloatingPoint::equal(a, b) ? "Equal" : "Not Equal") << "\n";

    // Binary search to find square root of 2
    auto ok = [](double x) {
        return x * x >= 2.0;
    };
    
    double sqrt2 = binary_search_real(0.0, 2.0, ok);
    std::cout << "Square root of 2 via binary search: " << sqrt2 << "\n";
    std::cout << "Absolute difference from std::sqrt(2): " 
              << std::abs(sqrt2 - std::sqrt(2.0)) << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    a = 0.1 + 0.2
    b = 0.3

    print(f"Direct comparison (a == b): {a == b}")
    print(f"Epsilon comparison (equal(a, b)): {FloatingPoint.equal(a, b)}")

    # Binary search to find square root of 2
    ok = lambda x: x * x >= 2.0
    sqrt2 = binary_search_real(0.0, 2.0, ok)

    print(f"Square root of 2 via binary search: {sqrt2:.15f}")
    print(f"Absolute difference from math.sqrt(2): {abs(sqrt2 - math.sqrt(2.0))}")
```
