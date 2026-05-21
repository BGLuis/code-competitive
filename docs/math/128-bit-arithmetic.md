# 128-bit Arithmetic

## Conceptual Explanation
In competitive programming, we frequently encounter intermediate values that exceed the capacity of standard 64-bit integer types (`long long` in C++ or `int` in 64-bit platforms). Common scenarios include:
1. **Modular Multiplication**: Calculating $(a \times b) \pmod m$ where $a, b, m \approx 10^{18}$. Standard multiplication will overflow 64-bit integers.
2. **Geometry**: Evaluating cross products and dot products of coordinates up to $10^9$. For three points $A, B, C$, the cross product $(B_x - A_x)(C_y - A_y) - (B_y - A_y)(C_x - A_x)$ can involve products of terms up to $2 \times 10^9$, requiring more than 64 bits to avoid overflow.
3. **Hashing**: Polynomial rolling hash with 64-bit primes to avoid hash collisions.

### C++ `__int128`
Modern C++ compilers like GCC and Clang provide the non-standard extension `__int128` (signed) and `unsigned __int128` (unsigned). These types support standard arithmetic operators (`+`, `-`, `*`, `/`, `%`, `<<`, `>>`, bitwise, and comparisons) with hardware acceleration. However, standard input/output streams (`std::cin`, `std::cout`) and string conversions (`std::to_string`) do not support them out-of-the-box. We must implement custom input and output routines.

### Python 3 Arbitrary Precision
In Python 3, the standard `int` type automatically handles arbitrary precision (bignum arithmetic). Python transitions seamlessly to larger bit widths as needed, making 128-bit arithmetic native. However, when simulating low-level 128-bit overflow (e.g., to mimic C++ behavior or implement specific hashing schemes), bitwise masking (`& ((1 << 128) - 1)`) is required.

---

## Complexity Analysis
- **Arithmetic Operations (`+`, `-`, bitwise, comparison)**: $O(1)$ time complexity. On 64-bit CPUs, these are translated into a few assembly instructions (e.g., using carry flags).
- **Multiplication/Division**: $O(1)$ time complexity, but slightly slower than 64-bit operations due to multi-word arithmetic simulation.
- **Input/Output Parsing**: $O(\log_{10} N)$ time complexity, proportional to the number of digits in the decimal representation (at most 39 digits).
- **Space Complexity**: $O(1)$ auxiliary space.

---

## C++ Implementation
Below is a complete implementation providing custom stream operators `std::ostream& operator<<(std::ostream&, __int128)` and `std::istream& operator>>(std::istream&, __int128&)` along with robust math helpers.

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

// Typedefs for convenience
using int128 = __int128;
using uint128 = unsigned __int128;

// Overload input stream reader for __int128
std::istream& operator>>(std::istream& is, int128& val) {
    std::string s;
    if (is >> s) {
        val = 0;
        bool negative = false;
        size_t i = 0;
        if (s[0] == '-') {
            negative = true;
            i = 1;
        } else if (s[0] == '+') {
            i = 1;
        }
        for (; i < s.size(); ++i) {
            if (s[i] >= '0' && s[i] <= '9') {
                val = val * 10 + (s[i] - '0');
            } else {
                is.setstate(std::ios::failbit);
                break;
            }
        }
        if (negative) {
            val = -val;
        }
    }
    return is;
}

// Overload output stream writer for __int128
std::ostream& operator<<(std::ostream& os, int128 val) {
    if (val == 0) return os << "0";
    std::string s;
    bool negative = val < 0;
    if (negative) val = -val;
    while (val > 0) {
        s += static_cast<char>('0' + (val % 10));
        val /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    return os << s;
}

// Overload output stream writer for unsigned __int128
std::ostream& operator<<(std::ostream& os, uint128 val) {
    if (val == 0) return os << "0";
    std::string s;
    while (val > 0) {
        s += static_cast<char>('0' + (val % 10));
        val /= 10;
    }
    std::reverse(s.begin(), s.end());
    return os << s;
}

// Modular multiplication for large numbers to prevent overflow
int128 mul_mod(int128 a, int128 b, int128 m) {
    a = (a % m + m) % m;
    b = (b % m + m) % m;
    return static_cast<int128>((static_cast<uint128>(a) * static_cast<uint128>(b)) % m);
}

// Modular exponentiation using __int128
int128 power_mod(int128 base, int128 exp, int128 m) {
    int128 res = 1;
    base = (base % m + m) % m;
    while (exp > 0) {
        if (exp & 1) res = mul_mod(res, base, m);
        base = mul_mod(base, base, m);
        exp >>= 1;
    }
    return res;
}
```

---

## Python Implementation
Python handles large integers natively. Below we define a `Int128` wrapper class to demonstrate how to enforce signed/unsigned 128-bit overflow constraints similar to C++.

```python
class Int128:
    """Simulates signed 128-bit integer arithmetic and overflow behavior."""
    MASK = (1 << 128) - 1
    SIGN_BIT = 1 << 127

    def __init__(self, value: int = 0):
        self.value = self._clamp(value)

    def _clamp(self, val: int) -> int:
        # Wrap value to fits in signed 128-bit integer (-2^127 to 2^127 - 1)
        val = val & self.MASK
        if val & self.SIGN_BIT:
            val -= (1 << 128)
        return val

    def __add__(self, other: "Int128") -> "Int128":
        return Int128(self.value + other.value)

    def __sub__(self, other: "Int128") -> "Int128":
        return Int128(self.value - other.value)

    def __mul__(self, other: "Int128") -> "Int128":
        return Int128(self.value * other.value)

    def __floordiv__(self, other: "Int128") -> "Int128":
        if other.value == 0:
            raise ZeroDivisionError("division by zero")
        # Symmetric division (like C++ / integer division)
        res = abs(self.value) // abs(other.value)
        if (self.value < 0) ^ (other.value < 0):
            res = -res
        return Int128(res)

    def __mod__(self, other: "Int128") -> "Int128":
        if other.value == 0:
            raise ZeroDivisionError("modulo by zero")
        # Symmetric modulo (like C++ % operator)
        res = abs(self.value) % abs(other.value)
        if self.value < 0:
            res = -res
        return Int128(res)

    def __str__(self) -> str:
        return str(self.value)

    def __repr__(self) -> str:
        return f"Int128({self.value})"


def mul_mod(a: int, b: int, m: int) -> int:
    """Returns (a * b) % m without low-level overflow constraints."""
    return (a * b) % m


def power_mod(base: int, exp: int, m: int) -> int:
    """Modular exponentiation: (base^exp) % m."""
    return pow(base, exp, m)
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // Demonstration of reading and printing __int128
    int128 a = 1234567890123456789LL;
    int128 b = 9876543210987654321LL;
    int128 product = a * b;

    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a * b = " << product << "\n";

    // Modular arithmetic with 10^18 sized moduli
    int128 mod = 1000000000000000003LL; // Large Prime
    int128 base = 2;
    int128 exp = 1000000000000000000LL;
    int128 p_mod = power_mod(base, exp, mod);
    std::cout << "2^10^18 mod " << mod << " = " << p_mod << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # Python 3 handles bignums seamlessly
    a = 1234567890123456789
    b = 9876543210987654321
    product = a * b
    print(f"a = {a}")
    print(f"b = {b}")
    print(f"a * b = {product}")

    # Modular arithmetic with large numbers
    mod = 1000000000000000003
    p_mod = power_mod(2, 1000000000000000000, mod)
    print(f"2^10^18 mod {mod} = {p_mod}")

    # Simulating low-level C++ __int128 overflow
    val1 = Int128(1 << 127)
    val2 = Int128(1)
    overflowed = val1 + val2
    print(f"Signed 128-bit overflow: (2^127) + 1 = {overflowed}")
```
