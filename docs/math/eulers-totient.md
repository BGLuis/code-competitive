# Euler's Totient Function

## Conceptual Explanation
**Euler's Totient Function**, denoted as $\phi(n)$, counts the number of positive integers up to $n$ that are relatively prime (coprime) to $n$. Two integers $a$ and $b$ are coprime if their greatest common divisor $\gcd(a, b) = 1$.

### Mathematical Properties
1. **Formula**: If the prime factorization of $n$ is $p_1^{a_1} p_2^{a_2} \dots p_k^{a_k}$, then:
   $$\phi(n) = n \prod_{i=1}^k \left(1 - \frac{1}{p_i}\right) = n \prod_{i=1}^k \frac{p_i - 1}{p_i}$$
2. **Multiplicative Property**: If $\gcd(a, b) = 1$, then $\phi(a \cdot b) = \phi(a) \cdot \phi(b)$.
3. **Euler's Theorem**: For any $a$ coprime to $m$:
   $$a^{\phi(m)} \equiv 1 \pmod m$$
   This is the generalization of Fermat's Little Theorem and forms the basis for modular division when the modulus is not prime.

### Algorithmic Approaches
1. **Single Value $\phi(n)$**: Using trial division, we can find all prime factors of $n$ up to $\sqrt{n}$. We then apply the product formula directly.
2. **Range $\phi(i)$ for $i \in [1, N]$ (Divisor Sieve)**: Similar to the Sieve of Eratosthenes. We initialize $\phi(i) = i$, and for each prime $p$, we update its multiples $k \cdot p$ by multiplying them by $(1 - 1/p)$.
3. **Linear Sieve ($O(N)$)**: By computing $\phi$ alongside the linear sieve of Eratosthenes, we can find primes and $\phi$ values in strict $O(N)$ time. We use the multiplicative property and the relation:
   - If $p \mid i$: $\phi(i \cdot p) = \phi(i) \cdot p$
   - If $p \nmid i$: $\phi(i \cdot p) = \phi(i) \cdot (p - 1)$

---

## Complexity Analysis
- **Single value $\phi(n)$**:
  - **Time**: $O(\sqrt{n})$ using trial division.
  - **Space**: $O(1)$ auxiliary space.
- **Range sieve (Sieve of Eratosthenes style)**:
  - **Time**: $O(N \log \log N)$ (or $O(N \log N)$ depending on implementation detail).
  - **Space**: $O(N)$ to store values.
- **Linear Sieve ($O(N)$)**:
  - **Time**: $O(N)$ since every number is visited exactly once by its smallest prime factor.
  - **Space**: $O(N)$ for sieve and prime lists.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <numeric>

// 1. Single value phi(n) in O(sqrt(n))
long long get_phi(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            result -= result / p;
        }
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

// 2. Range phi from 1 to N using traditional sieve in O(N log log N)
std::vector<int> get_phi_sieve(int n) {
    std::vector<int> phi(n + 1);
    std::iota(phi.begin(), phi.end(), 0); // phi[i] = i
    
    for (int i = 2; i <= n; ++i) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
    return phi;
}

// 3. Linear Sieve (O(N)) generating primes and phi values
struct LinearSieveResult {
    std::vector<int> primes;
    std::vector<int> phi;
};

LinearSieveResult get_phi_linear_sieve(int n) {
    std::vector<int> primes;
    std::vector<int> phi(n + 1, 0);
    std::vector<bool> is_prime(n + 1, true);

    if (n >= 1) phi[1] = 1;

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int p : primes) {
            if (i * p > n) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1);
            }
        }
    }
    return {primes, phi};
}
```

---

## Python Implementation

```python
from typing import List, Tuple


def get_phi(n: int) -> int:
    """Calculates single phi(n) in O(sqrt(n))."""
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result


def get_phi_sieve(n: int) -> List[int]:
    """Calculates phi values for 1 to n in O(n log log n)."""
    phi = list(range(n + 1))
    for i in range(2, n + 1):
        if phi[i] == i:  # i is prime
            for j in range(i, n + 1, i):
                phi[j] -= phi[j] // i
    return phi


def get_phi_linear_sieve(n: int) -> Tuple[List[int], List[int]]:
    """Calculates phi values and primes in O(n) time using linear sieve."""
    primes: List[int] = []
    phi = [0] * (n + 1)
    is_prime = [True] * (n + 1)

    if n >= 1:
        phi[1] = 1

    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            phi[i] = i - 1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                phi[i * p] = phi[i] * p
                break
            else:
                phi[i * p] = phi[i] * (p - 1)

    return primes, phi
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    long long n = 36;
    std::cout << "phi(" << n << ") = " << get_phi(n) << " (expected 12)\n";

    int limit = 20;
    std::vector<int> phi_sieve = get_phi_sieve(limit);
    std::cout << "\nTraditional Sieve results (1 to 20):\n";
    for (int i = 1; i <= limit; ++i) {
        std::cout << "phi(" << i << ") = " << phi_sieve[i] << "\n";
    }

    auto linear_sieve = get_phi_linear_sieve(limit);
    std::cout << "\nPrimes found by Linear Sieve:\n";
    for (int p : linear_sieve.primes) {
        std::cout << p << " ";
    }
    std::cout << "\n\nLinear Sieve phi[20] = " << linear_sieve.phi[20] << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    n = 36
    print(f"phi({n}) = {get_phi(n)} (expected 12)")

    limit = 20
    phi_sieve = get_phi_sieve(limit)
    print("\nTraditional Sieve results (1 to 20):")
    for i in range(1, limit + 1):
        print(f"phi({i}) = {phi_sieve[i]}")

    primes, phi_linear = get_phi_linear_sieve(limit)
    print(f"\nPrimes found by Linear Sieve: {primes}")
    print(f"Linear Sieve phi[20] = {phi_linear[20]}")
```
