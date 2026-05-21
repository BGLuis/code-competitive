# Convex Optimization (Ternary Search)

## Conceptual Explanation
In competitive programming, optimization problems often require finding the minimum or maximum of a **unimodal function** (a function that rises to a single peak and then falls, or vice-versa).
- A function is **convex** (concave upward) if its line segment between any two points lies above or on the graph. It contains a unique global minimum.
- A function is **concave** (concave downward) if its line segment between any two points lies below or on the graph. It contains a unique global maximum.

To optimize unimodal functions, we utilize **Ternary Search**. It works similarly to binary search but divides the active search interval $[L, R]$ into three equal parts using two division points, $m_1$ and $m_2$:
$$m_1 = L + \frac{R - L}{3}, \quad m_2 = R - \frac{R - L}{3}$$

By comparing the function values at $m_1$ and $m_2$:
1. **Finding Minimum of a Convex Function**:
   - If $f(m_1) < f(m_2)$, the minimum cannot be in the range $[m_2, R]$. Thus, we set $R = m_2$.
   - If $f(m_1) > f(m_2)$, the minimum cannot be in the range $[L, m_1]$. Thus, we set $L = m_1$.
   - If $f(m_1) = f(m_2)$, we can set $L = m_1$ and $R = m_2$.
2. **Finding Maximum of a Concave Function**:
   - If $f(m_1) < f(m_2)$, the maximum cannot be in the range $[L, m_1]$. Thus, we set $L = m_1$.
   - If $f(m_1) > f(m_2)$, the maximum cannot be in the range $[m_2, R]$. Thus, we set $R = m_2$.

### Floating-Point vs. Integer Domains
- **Floating-point**: The search is run for a **fixed number of iterations** (typically 80 to 100), which guarantees extremely high precision (e.g., $3^{-100} \approx 1.9 \times 10^{-48}$) and prevents infinite loops caused by floating-point representation limits.
- **Integer**: When the search space becomes small ($R - L < 3$), ternary search division points can overlap. The best practice is to narrow down the range to a small size (e.g., $R - L \le 3$) and perform a local linear scan to find the exact minimum/maximum.

---

## Complexity Analysis
- **Time Complexity**:
  - Each step reduces the search space size to $\frac{2}{3}$ of its original size.
  - To reach precision $\epsilon$ from range size $D$, it takes $\log_{1.5}(D / \epsilon)$ steps.
  - Overall time is $O(\log(D / \epsilon) \cdot T(f))$, where $T(f)$ is the evaluation time of $f$. For integers, it is $O(\log(R - L) \cdot T(f))$.
- **Space Complexity**: $O(1)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>

// 1. Ternary Search over Real Numbers (floating point)
// Finds x in [L, R] that minimizes/maximizes a unimodal function f
double ternary_search_real(double L, double R, const std::function<double(double)>& f, bool find_max = false, int iterations = 100) {
    for (int i = 0; i < iterations; ++i) {
        double m1 = L + (R - L) / 3.0;
        double m2 = R - (R - L) / 3.0;

        if (find_max) {
            if (f(m1) < f(m2)) {
                L = m1;
            } else {
                R = m2;
            }
        } else { // find min
            if (f(m1) < f(m2)) {
                R = m2;
            } else {
                L = m1;
            }
        }
    }
    return L;
}

// 2. Ternary Search over Integers (discrete domain)
// Finds the integer x in [L, R] that minimizes/maximizes unimodal function f
long long ternary_search_integer(long long L, long long R, const std::function<long long(long long)>& f, bool find_max = false) {
    while (R - L > 3) {
        long long m1 = L + (R - L) / 3;
        long long m2 = R - (R - L) / 3;

        if (find_max) {
            if (f(m1) < f(m2)) {
                L = m1;
            } else {
                R = m2;
            }
        } else { // find min
            if (f(m1) < f(m2)) {
                R = m2;
            } else {
                L = m1;
            }
        }
    }

    // Linear scan of the small remaining window [L, R]
    long long best_x = L;
    long long best_val = f(L);
    for (long long i = L + 1; i <= R; ++i) {
        long long val = f(i);
        if (find_max) {
            if (val > best_val) {
                best_val = val;
                best_x = i;
            }
        } else {
            if (val < best_val) {
                best_val = val;
                best_x = i;
            }
        }
    }
    return best_x;
}
```

---

## Python Implementation

```python
from typing import Callable


def ternary_search_real(
    l: float,
    r: float,
    f: Callable[[float], float],
    find_max: bool = False,
    iterations: int = 100,
) -> float:
    """Finds x in [l, r] that minimizes/maximizes floating-point unimodal f."""
    for _ in range(iterations):
        m1 = l + (r - l) / 3.0
        m2 = r - (r - l) / 3.0

        if find_max:
            if f(m1) < f(m2):
                l = m1
            else:
                r = m2
        else:  # find minimum
            if f(m1) < f(m2):
                r = m2
            else:
                l = m1
    return l


def ternary_search_integer(
    l: int, r: int, f: Callable[[int], int], find_max: bool = False
) -> int:
    """Finds integer x in [l, r] that minimizes/maximizes discrete unimodal f."""
    while r - l > 3:
        m1 = l + (r - l) // 3
        m2 = r - (r - l) // 3

        if find_max:
            if f(m1) < f(m2):
                l = m1
            else:
                r = m2
        else:  # find minimum
            if f(m1) < f(m2):
                r = m2
            else:
                l = m1

    # Final linear scan of remainder range [l, r]
    best_x = l
    best_val = f(l)
    for i in range(l + 1, r + 1):
        val = f(i)
        if find_max:
            if val > best_val:
                best_val = val
                best_x = i
        else:
            if val < best_val:
                best_val = val
                best_x = i
    return best_x
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 1. Continuous optimization: Min of f(x) = (x - 3.5)^2 + 4.2
    // Convex function, minimum is at x = 3.5 with value 4.2
    auto f_real = [](double x) {
        return (x - 3.5) * (x - 3.5) + 4.2;
    };

    double best_x_real = ternary_search_real(-10.0, 10.0, f_real, false);
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Real Min: x = " << best_x_real << ", f(x) = " << f_real(best_x_real) << "\n";

    // 2. Discrete optimization: Max of f(x) = -(x - 42)^2 + 100
    // Concave function, maximum is at integer x = 42 with value 100
    auto f_int = [](long long x) {
        return -(x - 42) * (x - 42) + 100;
    };

    long long best_x_int = ternary_search_integer(0, 1000, f_int, true);
    std::cout << "Integer Max: x = " << best_x_int << ", f(x) = " << f_int(best_x_int) << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. Real Optimization: Minimize f(x) = (x - 3.5)^2 + 4.2
    f_real = lambda x: (x - 3.5) ** 2 + 4.2
    x_opt_real = ternary_search_real(-10.0, 10.0, f_real, find_max=False)
    print(f"Real Optimization: min at x = {x_opt_real:.6f}, f(x) = {f_real(x_opt_real):.6f}")

    # 2. Integer Optimization: Maximize f(x) = -(x - 42)^2 + 100
    f_int = lambda x: -(x - 42) ** 2 + 100
    x_opt_int = ternary_search_integer(0, 1000, f_int, find_max=True)
    print(f"Integer Optimization: max at x = {x_opt_int}, f(x) = {f_int(x_opt_int)}")
```
