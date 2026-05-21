# GCD and Fraction Simplification

## Conceptual Explanation
The **Greatest Common Divisor (GCD)** of two integers is the largest positive integer that divides both numbers without a remainder. GCD is the cornerstone of number theory, fractional arithmetic, and geometric coordinates.

### The Euclidean Algorithm
The Euclidean algorithm is based on the principle that the GCD of two integers $a$ and $b$ ($a \ge b$) is the same as the GCD of $b$ and $a \bmod b$:
$$\gcd(a, b) = \gcd(b, a \bmod b), \quad \text{with } \gcd(a, 0) = a$$

### The Extended Euclidean Algorithm
The Extended Euclidean algorithm computes not only $\gcd(a, b)$ but also the integer coefficients $x$ and $y$ (known as Bézout's coefficients) that satisfy **Bézout's identity**:
$$a \cdot x + b \cdot y = \gcd(a, b)$$
This is extremely useful in competitive programming to find the modular multiplicative inverse of $a$ modulo $m$ when $\gcd(a, m) = 1$. The inverse is $x \bmod m$.

### Fraction Simplification and Representation
A fraction $\frac{n}{d}$ is in its **canonical simplified form** if:
1. $d > 0$ (the denominator is strictly positive).
2. $\gcd(|n|, d) = 1$ (the numerator and denominator are coprime).

To represent fractions robustly, we implement a custom `Fraction` type supporting full rational arithmetic (addition, subtraction, multiplication, division, and comparisons) without floating-point inaccuracies.

---

## Complexity Analysis
- **GCD and Extended GCD**: $O(\log(\min(a, b)))$ time complexity using Euclidean division. Space complexity is $O(1)$ auxiliary.
- **Fraction Arithmetic**:
  - **Addition / Subtraction**: $O(\log(\min(n, d)))$ due to finding common denominators and final simplification.
  - **Multiplication / Division**: $O(\log(\min(n, d)))$ due to final simplification.
  - **Comparisons**: $O(1)$ by cross-multiplying: $\frac{a}{b} < \frac{c}{d} \iff a \cdot d < b \cdot c$.

---

## C++ Implementation

```cpp
#include <iostream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <stdexcept>

// 1. Basic GCD (implemented manually, though std::gcd exists since C++17)
long long gcd(long long a, long long b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return std::abs(a);
}

// 2. Extended Euclidean Algorithm
// Returns gcd(a, b) and finds coefficients x and y
long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

// 3. Robust Fraction Structure
struct Fraction {
    long long num;
    long long den;

    void simplify() {
        if (den == 0) {
            throw std::runtime_error("Denominator cannot be zero.");
        }
        if (num == 0) {
            den = 1;
            return;
        }
        long long g = gcd(num, den);
        num /= g;
        den /= g;
        if (den < 0) {
            num = -num;
            den = -den;
        }
    }

    Fraction(long long n = 0, long long d = 1) : num(n), den(d) {
        simplify();
    }

    Fraction operator+(const Fraction& other) const {
        long long g = gcd(den, other.den);
        long long common_den = den * (other.den / g);
        long long new_num = num * (other.den / g) + other.num * (den / g);
        return Fraction(new_num, common_den);
    }

    Fraction operator-(const Fraction& other) const {
        long long g = gcd(den, other.den);
        long long common_den = den * (other.den / g);
        long long new_num = num * (other.den / g) - other.num * (den / g);
        return Fraction(new_num, common_den);
    }

    Fraction operator*(const Fraction& other) const {
        // Cross-simplify before multiplying to prevent overflow
        long long g1 = gcd(std::abs(num), other.den);
        long long g2 = gcd(std::abs(other.num), den);
        return Fraction((num / g1) * (other.num / g2), (den / g2) * (other.den / g1));
    }

    Fraction operator/(const Fraction& other) const {
        if (other.num == 0) {
            throw std::runtime_error("Division by zero fraction.");
        }
        return *this * Fraction(other.den, other.num);
    }

    bool operator==(const Fraction& other) const {
        return num == other.num && den == other.den;
    }

    bool operator<(const Fraction& other) const {
        // Using __int128 to prevent overflow in cross-multiplication
        return static_cast<__int128>(num) * other.den < static_cast<__int128>(other.num) * den;
    }

    bool operator<=(const Fraction& other) const { return *this < other || *this == other; }
    bool operator>(const Fraction& other) const { return other < *this; }
    bool operator>=(const Fraction& other) const { return other <= *this; }
    bool operator!=(const Fraction& other) const { return !(*this == other); }
};

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    if (f.den == 1) return os << f.num;
    return os << f.num << "/" << f.den;
}
```

---

## Python Implementation

```python
import math
from typing import Tuple


def gcd(a: int, b: int) -> int:
    """Calculates greatest common divisor using Euclidean algorithm."""
    return math.gcd(a, b)


def ext_gcd(a: int, b: int) -> Tuple[int, int, int]:
    """Extended Euclidean algorithm.

    Returns (gcd, x, y) such that a*x + b*y = gcd.
    """
    if b == 0:
        return a, 1, 0
    g, x1, y1 = ext_gcd(b, a % b)
    x = y1
    y = x1 - y1 * (a // b)
    return g, x, y


class Fraction:
    def __init__(self, num: int = 0, den: int = 1):
        if den == 0:
            raise ZeroDivisionError("Denominator cannot be zero.")
        self.num = num
        self.den = den
        self._simplify()

    def _simplify(self) -> None:
        if self.num == 0:
            self.den = 1
            return
        g = math.gcd(self.num, self.den)
        self.num //= g
        self.den //= g
        if self.den < 0:
            self.num = -self.num
            self.den = -self.den

    def __add__(self, other: "Fraction") -> "Fraction":
        g = math.gcd(self.den, other.den)
        common_den = self.den * (other.den // g)
        new_num = self.num * (other.den // g) + other.num * (self.den // g)
        return Fraction(new_num, common_den)

    def __sub__(self, other: "Fraction") -> "Fraction":
        g = math.gcd(self.den, other.den)
        common_den = self.den * (other.den // g)
        new_num = self.num * (other.den // g) - other.num * (self.den // g)
        return Fraction(new_num, common_den)

    def __mul__(self, other: "Fraction") -> "Fraction":
        g1 = math.gcd(abs(self.num), other.den)
        g2 = math.gcd(abs(other.num), self.den)
        return Fraction(
            (self.num // g1) * (other.num // g2),
            (self.den // g2) * (other.den // g1),
        )

    def __truediv__(self, other: "Fraction") -> "Fraction":
        if other.num == 0:
            raise ZeroDivisionError("Division by zero fraction.")
        return self * Fraction(other.den, other.num)

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Fraction):
            return False
        return self.num == other.num and self.den == other.den

    def __lt__(self, other: "Fraction") -> bool:
        return self.num * other.den < other.num * self.den

    def __le__(self, other: "Fraction") -> bool:
        return self < other or self == other

    def __gt__(self, other: "Fraction") -> bool:
        return other < self

    def __ge__(self, other: "Fraction") -> bool:
        return other <= self

    def __ne__(self, other: object) -> bool:
        return not self.__eq__(other)

    def __str__(self) -> str:
        if self.den == 1:
            return str(self.num)
        return f"{self.num}/{self.den}"

    def __repr__(self) -> str:
        return f"Fraction({self.num}, {self.den})"
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 1. Greatest Common Divisor and Extended GCD
    long long a = 240, b = 46;
    long long x, y;
    long long g = ext_gcd(a, b, x, y);
    std::cout << "gcd(" << a << ", " << b << ") = " << g << "\n";
    std::cout << "Bézout's coefficients: " << a << "*(" << x << ") + " 
              << b << "*(" << y << ") = " << g << "\n\n";

    // 2. Fractional Arithmetic
    Fraction f1(2, 6); // Simplifies to 1/3
    Fraction f2(-5, 4); // Negative fraction

    std::cout << "Fraction 1: " << f1 << "\n";
    std::cout << "Fraction 2: " << f2 << "\n";
    std::cout << f1 << " + (" << f2 << ") = " << (f1 + f2) << "\n";
    std::cout << f1 << " - (" << f2 << ") = " << (f1 - f2) << "\n";
    std::cout << f1 << " * (" << f2 << ") = " << (f1 * f2) << "\n";
    std::cout << f1 << " / (" << f2 << ") = " << (f1 / f2) << "\n";

    // Comparisons
    Fraction f3(4, 12); // Equal to 1/3
    std::cout << f1 << " == " << f3 << "? " << (f1 == f3 ? "True" : "False") << "\n";
    std::cout << f1 << " < " << f2 << "? " << (f1 < f2 ? "True" : "False") << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. GCD and Extended GCD
    a, b = 240, 46
    g, x, y = ext_gcd(a, b)
    print(f"gcd({a}, {b}) = {g}")
    print(f"Bézout's coefficients: {a}*({x}) + {b}*({y}) = {g}\n")

    # 2. Fractional Arithmetic
    f1 = Fraction(2, 6)  # Simplifies to 1/3
    f2 = Fraction(-5, 4)  # Negative fraction

    print(f"Fraction 1: {f1}")
    print(f"Fraction 2: {f2}")
    print(f"{f1} + ({f2}) = {f1 + f2}")
    print(f"{f1} - ({f2}) = {f1 - f2}")
    print(f"{f1} * ({f2}) = {f1 * f2}")
    print(f"{f1} / ({f2}) = {f1 / f2}")

    # Comparisons
    f3 = Fraction(4, 12)
    print(f"{f1} == {f3}? {f1 == f3}")
    print(f"{f1} < {f2}? {f1 < f2}")
```
