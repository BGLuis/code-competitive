# Multiplicative Order

## Conceptual Explanation
In modular arithmetic, the **Multiplicative Order** of an integer $a$ modulo $m$ (denoted as $\text{ord}_m(a)$) is the smallest positive integer $k$ such that:
$$a^k \equiv 1 \pmod m$$

### Existence and Properties
- **Coprimality**: The multiplicative order $\text{ord}_m(a)$ exists if and only if $\gcd(a, m) = 1$. If they are not coprime, powers of $a$ can never reach a value of $1 \bmod m$.
- **Divisibility**: By Euler's Totient Theorem, if $\gcd(a, m) = 1$, then $a^{\phi(m)} \equiv 1 \pmod m$. A key algebraic property is that **$\text{ord}_m(a)$ must divide $\phi(m)$**.
- **Generators / Primitive Roots**: If $\text{ord}_m(a) = \phi(m)$, then $a$ is called a **primitive root** (or generator) modulo $m$.

---

### Algorithm to Find $\text{ord}_m(a)$
A naive search for $k$ by testing $a^1, a^2, \dots$ takes $O(m)$ time, which is too slow for large $m$ (e.g., $10^9$). We can find the order much faster by using the property that $k$ divides $\phi(m)$:

1. Compute $L = \phi(m)$ in $O(\sqrt{m})$ time.
2. Find the prime factorization of $L$:
   $$L = p_1^{e_1} p_2^{e_2} \dots p_r^{e_r}$$
3. Start with the candidate order $k = L$.
4. For each prime factor $p_i$ of $L$:
   While $k$ is divisible by $p_i$ and $a^{k / p_i} \equiv 1 \pmod m$, we can reduce $k$ by dividing it by $p_i$:
   $$k \leftarrow \frac{k}{p_i}$$
5. The final value of $k$ is the exact multiplicative order $\text{ord}_m(a)$.

This factorization-based optimization reduces the search space of $k$ to the prime exponents of $\phi(m)$, allowing us to find the multiplicative order in $O(\sqrt{m} + \log m \cdot \log(\phi(m)))$ time.

---

## Complexity Analysis
- **Time Complexity**:
  - **$\phi(m)$ and Factorization of $\phi(m)$**: $O(\sqrt{m})$ using trial division.
  - **Order Reduction**: $O(\log(\phi(m)))$ steps. In each step, we perform modular exponentiation which takes $O(\log m)$ operations.
  - **Total Time**: $O(\sqrt{m} + \log^2 m)$ which is extremely fast and easily runs within 1-2 milliseconds for $m \le 2 \cdot 10^9$.
- **Space Complexity**: $O(\log m)$ to store the prime factors of $\phi(m)$.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>


// Modular exponentiation helper: (base^exp) % mod
long long power_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

// Compute Euler's Totient phi(m) in O(sqrt(m))
long long get_phi(long long m) {
    long long result = m;
    for (long long p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
            }
            result -= result / p;
        }
    }
    if (m > 1) {
        result -= result / m;
    }
    return result;
}

// Find prime factors of a number
std::vector<long long> get_prime_factors(long long n) {
    std::vector<long long> factors;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            factors.push_back(p);
            while (n % p == 0) {
                n /= p;
            }
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}

// Find multiplicative order of a mod m in O(sqrt(m) + log^2 m)
// Returns -1 if gcd(a, m) != 1
long long multiplicative_order(long long a, long long m) {
    if (std::numeric_limits<long long>::max() == m) return -1;
    
    // Check coprimality using std::gcd (C++17) or manual gcd
    auto gcd = [](auto self, long long x, long long y) -> long long {
        return y == 0 ? x : self(self, y, x % y);
    };
    if (gcd(gcd, a, m) != 1) return -1;

    long long phi = get_phi(m);
    std::vector<long long> factors = get_prime_factors(phi);

    long long order = phi;
    for (long long p : factors) {
        while (order % p == 0 && power_mod(a, order / p, m) == 1) {
            order /= p;
        }
    }
    return order;
}
```

---

## Python Implementation

```python
import math
from typing import List


def power_mod(base: int, exp: int, mod: int) -> int:
    """Modular exponentiation: (base^exp) % mod."""
    return pow(base, exp, mod)


def get_phi(m: int) -> int:
    """Calculates Euler's Totient phi(m) in O(sqrt(m))."""
    result = m
    p = 2
    temp = m
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result


def get_prime_factors(n: int) -> List[int]:
    """Retrieves unique prime factors of n."""
    factors = []
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            factors.append(p)
            while temp % p == 0:
                temp //= p
        p += 1
    if temp > 1:
        factors.append(temp)
    return factors


def multiplicative_order(a: int, m: int) -> int:
    """Finds the multiplicative order of a mod m in O(sqrt(m) + log^2 m).

    Returns -1 if gcd(a, m) != 1.
    """
    if math.gcd(a, m) != 1:
        return -1

    phi = get_phi(m)
    factors = get_prime_factors(phi)

    order = phi
    for p in factors:
        while order % p == 0 and power_mod(a, order // p, m) == 1:
            order //= p

    return order
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    long long a = 3;
    long long m = 7;
    
    // Powers of 3 mod 7:
    // 3^1 = 3
    // 3^2 = 9 = 2
    // 3^3 = 6
    // 3^4 = 18 = 4
    // 3^5 = 12 = 5
    // 3^6 = 15 = 1.
    // Order should be 6.
    long long order = multiplicative_order(a, m);
    std::cout << "order of " << a << " mod " << m << " = " << order << " (expected 6)\n";

    // Non-coprime case
    long long non_coprime_order = multiplicative_order(2, 6);
    std::cout << "order of 2 mod 6 = " << non_coprime_order << " (expected -1)\n";

    // Big modulo test
    long long a_big = 2;
    long long m_big = 1000000007; // Prime. phi(m_big) = 1000000006
    std::cout << "order of " << a_big << " mod " << m_big << " = " 
              << multiplicative_order(a_big, m_big) << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    a = 3
    m = 7

    order = multiplicative_order(a, m)
    print(f"order of {a} mod {m} = {order} (expected 6)")

    # Non-coprime case
    non_coprime_order = multiplicative_order(2, 6)
    print(f"order of 2 mod 6 = {non_coprime_order} (expected -1)")

    # Big modulo test
    a_big = 2
    m_big = 1000000007
    print(
        f"order of {a_big} mod {m_big} = {multiplicative_order(a_big, m_big)}"
    )
```
