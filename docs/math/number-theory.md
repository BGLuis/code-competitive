# Comprehensive Number Theory Suite

## Conceptual Explanation
Number Theory is a primary component of competitive programming. Having a highly optimized, robust suite of number-theoretic algorithms is essential for solving problems involving divisibility, primality, and modular arithmetic.

### Key Algorithms Included

#### 1. Modular Arithmetic
- **Exponentiation**: Calculating $(B^P) \bmod M$ in $O(\log P)$ time using binary exponentiation.
- **Inverse**: Finding $A^{-1} \pmod M$ such that $A \cdot A^{-1} \equiv 1 \pmod M$. Using the **Extended Euclidean Algorithm**, we can find the modular inverse even when the modulus $M$ is not prime (as long as $\gcd(A, M) = 1$).

#### 2. Miller-Rabin Primality Test
Standard trial division checks primes up to $\sqrt{N}$ in $O(\sqrt{N})$ time, which is too slow for $N > 10^9$.
The **Miller-Rabin test** is a highly efficient randomized primality test. By testing a set of specific bases, it becomes **100% deterministic** for all integers within the 64-bit unsigned range ($N < 2^{64}$).
Bases used for deterministic 64-bit testing:
$$\mathcal{B} = \{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37\}$$

#### 3. Prime Factorization
- **Trial Division**: Extracts all prime factors of $N$ by checking divisibility up to $\sqrt{N}$ in $O(\sqrt{N})$ time. This is extremely robust and fast for numbers up to $10^{12}$.

---

## Complexity Analysis
- **Modular Exponentiation**: $O(\log P)$ time, $O(1)$ space.
- **Modular Inverse**: $O(\log M)$ time, $O(1)$ space.
- **Miller-Rabin Primality Test**: $O(K \log^3 N)$ where $K$ is the number of bases tested (at most 12 for 64-bit numbers). This takes less than a microsecond.
- **Prime Factorization (Trial Division)**: $O(\sqrt{N})$ worst-case time (when $N$ is prime or has very large prime factors), $O(\log N)$ space to store factors.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// 1. Modular Arithmetic Helpers (utilizing __int128 to prevent overflow with 64-bit moduli)
unsigned __int128 mul_mod(unsigned __int128 a, unsigned __int128 b, unsigned __int128 m) {
    return (a * b) % m;
}

long long power_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = (base % mod + mod) % mod;
    while (exp > 0) {
        if (exp & 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

// Extended Euclidean Algorithm
long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

// Modular inverse: returns x such that a * x = 1 mod m. Returns -1 if gcd(a, m) != 1
long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long g = ext_gcd(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

// 2. Miller-Rabin Primality Test (Deterministic for 64-bit integers)
bool is_prime_64(long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Write n - 1 as 2^s * d
    long long d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    // Deterministic bases for n < 2^64
    static const std::vector<long long> bases = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };

    for (long long a : bases) {
        if (n <= a) break;
        long long x = power_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// 3. Prime Factorization using Trial Division
// Returns a list of prime factors in sorted order (including duplicates)
std::vector<long long> factorize(long long n) {
    std::vector<long long> factors;
    // Check 2 and 3 separately
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }
    while (n % 3 == 0) {
        factors.push_back(3);
        n /= 3;
    }
    for (long long i = 5; i * i <= n; i += 6) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
        while (n % (i + 2) == 0) {
            factors.push_back(i + 2);
            n /= (i + 2);
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}
```

---

## Python Implementation

```python
from typing import List, Tuple


def power_mod(base: int, exp: int, mod: int) -> int:
    """Modular exponentiation: (base^exp) % mod."""
    return pow(base, exp, mod)


def ext_gcd(a: int, b: int) -> Tuple[int, int, int]:
    """Extended GCD: Returns (gcd, x, y) such that a*x + b*y = gcd."""
    if b == 0:
        return a, 1, 0
    g, x1, y1 = ext_gcd(b, a % b)
    x = y1
    y = x1 - y1 * (a // b)
    return g, x, y


def mod_inverse(a: int, m: int) -> int:
    """Finds modular multiplicative inverse of a mod m.

    Returns -1 if not coprime.
    """
    g, x, y = ext_gcd(a, m)
    if g != 1:
        return -1
    return (x % m + m) % m


def is_prime_64(n: int) -> bool:
    """Deterministic Miller-Rabin primality test for 64-bit integers."""
    if n < 2:
        return False
    if n in (2, 3):
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False

    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1

    # Deterministic bases for 64-bit limit
    bases = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]

    for a in bases:
        if n <= a:
            break
        x = power_mod(a, d, n)
        if x in (1, n - 1):
            continue

        composite = True
        for _ in range(1, s):
            x = (x * x) % n
            if x == n - 1:
                composite = False
                break
        if composite:
            return False

    return True


def factorize(n: int) -> List[int]:
    """Trial division factorization in O(sqrt(N))."""
    factors = []
    temp = n
    while temp % 2 == 0:
        factors.append(2)
        temp //= 2
    while temp % 3 == 0:
        factors.append(3)
        temp //= 3
    i = 5
    while i * i <= temp:
        while temp % i == 0:
            factors.append(i)
            temp //= i
        while temp % (i + 2) == 0:
            factors.append(i + 2)
            temp //= i + 2
        i += 6
    if temp > 1:
        factors.append(temp)
    return factors
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 1. Modular inverse
    long long a = 3, m = 11;
    std::cout << "3^(-1) mod 11 = " << mod_inverse(a, m) << " (expected 4 since 3*4 = 12 = 1 mod 11)\n\n";

    // 2. Primality testing with Miller-Rabin
    long long prime_candidate = 999999999999999989LL; // Large Prime
    long long comp_candidate =  999999999999999991LL; // Composite (343 * ...)
    
    std::cout << prime_candidate << " is prime? " 
              << (is_prime_64(prime_candidate) ? "Yes" : "No") << "\n";
    std::cout << comp_candidate << " is prime? " 
              << (is_prime_64(comp_candidate) ? "Yes" : "No") << "\n\n";

    // 3. Prime factorization
    long long num = 1234567890LL;
    std::vector<long long> factors = factorize(num);
    std::cout << "Prime factors of " << num << ": ";
    for (long long f : factors) {
        std::cout << f << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. Modular Inverse
    a, m = 3, 11
    print(f"3^(-1) mod 11 = {mod_inverse(a, m)} (expected 4)")

    # 2. Miller-Rabin Primality Test
    prime = 999999999999999989
    composite = 999999999999999991
    print(f"{prime} is prime? {is_prime_64(prime)}")
    print(f"{composite} is prime? {is_prime_64(composite)}\n")

    # 3. Factorization
    num = 1234567890
    print(f"Prime factors of {num}: {factorize(num)}")
```
