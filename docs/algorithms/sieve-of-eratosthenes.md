# Sieve of Eratosthenes

## Conceptual Explanation

The **Sieve of Eratosthenes** is a highly efficient ancient algorithm for finding all prime numbers up to a specified limit $N$.

---

### 1. Standard Sieve of Eratosthenes

The core principle is to iteratively mark the multiples of each prime number as composite (not prime), starting from $2$.

#### Algorithm Steps:
1. Create a boolean array `is_prime` of size $N+1$, initialized to `true`. Set `is_prime[0] = is_prime[1] = false`.
2. For each number $p$ from 2 up to $\sqrt{N}$:
   - If `is_prime[p]` is true, then $p$ is prime. We then mark all multiples of $p$ starting from $p^2$ (i.e., $p^2, p^2 + p, p^2 + 2p, \dots$) as `false`.
   - *Optimization*: We start marking multiples at $p^2$ because any smaller multiple of $p$ (e.g., $k \cdot p$ where $k < p$) will have already been marked by a prime factor smaller than $p$.
3. When the loop terminates, all indices remaining `true` are prime numbers.

---

### 2. Linear Sieve (Euler's Sieve) & Smallest Prime Factor (SPF)

While the standard sieve runs in $\mathcal{O}(N \log \log N)$ time, the **Linear Sieve** runs in exact $\mathcal{O}(N)$ time. It achieves this by ensuring that **every composite number is marked exactly once**, by its **smallest prime factor (SPF)**.

#### Algorithm Steps:
1. Maintain a list of `primes` and an array `spf` where `spf[i]` stores the smallest prime factor of $i$. Initialize `spf[i] = i`.
2. For each number $i$ from 2 to $N$:
   - If `spf[i] == i`, then $i$ is prime, so we add $i$ to our `primes` list.
   - For each prime $p$ in our list:
     - If $p > spf[i]$ or $i \cdot p > N$, we break the loop.
     - Otherwise, set `spf[i * p] = p`.
3. If $p == spf[i]$, we break the inner loop. This guarantees that any number $i \cdot p$ is marked only by its smallest prime factor.

#### Fast Prime Factorization using SPF
Once the `spf` array is computed up to $N$, we can factorize any query number $X \le N$ in $\mathcal{O}(\log X)$ time by repeatedly dividing $X$ by its smallest prime factor:
$$X \leftarrow X / spf[X]$$

---

## Complexity Analysis

| Algorithm | Time Complexity | Space Complexity | Use Cases |
| :--- | :--- | :--- | :--- |
| **Standard Sieve** | $\mathcal{O}(N \log \log N)$ | $\mathcal{O}(N)$ (bitset/bool array) | Finding primes up to $10^7$ or $10^8$. |
| **Linear Sieve** | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ (integer array) | Precomputing SPF for fast $\mathcal{O}(\log X)$ query factorizations up to $10^7$. |

---

## C++ Implementation

Here is a comprehensive C++ class containing both the Standard Sieve and the Linear Sieve with an $\mathcal{O}(\log X)$ prime factorization query function.

```cpp
#include <iostream>
#include <vector>

class PrimeSieve {
private:
    int limit;
    std::vector<bool> is_prime_table;
    std::vector<int> spf; // Smallest Prime Factor
    std::vector<int> primes;

public:
    explicit PrimeSieve(int n) : limit(n) {}

    // 1. Standard Sieve of Eratosthenes
    // Highly memory-efficient using std::vector<bool>
    void compute_standard_sieve() {
        is_prime_table.assign(limit + 1, true);
        is_prime_table[0] = is_prime_table[1] = false;

        for (int p = 2; p * p <= limit; ++p) {
            if (is_prime_table[p]) {
                for (int i = p * p; i <= limit; i += p) {
                    is_prime_table[i] = false;
                }
            }
        }

        primes.clear();
        for (int p = 2; p <= limit; ++p) {
            if (is_prime_table[p]) {
                primes.push_back(p);
            }
        }
    }

    // 2. Linear Sieve (Euler's Sieve)
    // Computes SPF for O(log X) factorization queries
    void compute_linear_sieve() {
        spf.assign(limit + 1, 0);
        primes.clear();

        for (int i = 2; i <= limit; ++i) {
            if (spf[i] == 0) { // i is prime
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || i * p <= 0 || i * p > limit) {
                    break;
                }
                spf[i * p] = p;
            }
        }
    }

    // Returns prime factorization of X in O(log X) time
    // Precondition: compute_linear_sieve() must have been called
    std::vector<int> get_factorization(int x) const {
        std::vector<int> factors;
        while (x > 1) {
            factors.push_back(spf[x]);
            x /= spf[x];
        }
        return factors;
    }

    const std::vector<int>& get_primes() const {
        return primes;
    }

    bool is_prime(int x) const {
        if (x < 0 || x > limit) return false;
        if (!is_prime_table.empty()) {
            return is_prime_table[x];
        }
        return spf[x] == x;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with complete type hints.

```python
from typing import List

class PrimeSieve:
    def __init__(self, limit: int):
        self.limit = limit
        self.is_prime_table: List[bool] = []
        self.spf: List[int] = []
        self.primes: List[int] = []

    def compute_standard_sieve(self) -> None:
        """
        Runs the standard Sieve of Eratosthenes in O(N log log N).
        """
        self.is_prime_table = [True] * (self.limit + 1)
        self.is_prime_table[0] = self.is_prime_table[1] = False

        p = 2
        while p * p <= self.limit:
            if self.is_prime_table[p]:
                for i in range(p * p, self.limit + 1, p):
                    self.is_prime_table[i] = False
            p += 1

        self.primes = [i for i, prime in enumerate(self.is_prime_table) if prime]

    def compute_linear_sieve(self) -> None:
        """
        Runs the linear Euler's Sieve in O(N) time and computes SPF.
        """
        self.spf = [0] * (self.limit + 1)
        self.primes = []

        for i in range(2, self.limit + 1):
            if self.spf[i] == 0:
                self.spf[i] = i
                self.primes.append(i)
            for p in self.primes:
                if p > self.spf[i] or i * p > self.limit:
                    break
                self.spf[i * p] = p

    def get_factorization(self, x: int) -> List[int]:
        """
        Computes prime factorization of x in O(log x) time using SPF table.
        """
        factors = []
        while x > 1:
            factors.append(self.spf[x])
            x //= self.spf[x]
        return factors

    def is_prime(self, x: int) -> bool:
        if not (0 <= x <= self.limit):
            return False
        if self.is_prime_table:
            return self.is_prime_table[x]
        return self.spf[x] == x
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    int limit = 100;
    PrimeSieve sieve(limit);

    // Compute standard sieve
    sieve.compute_standard_sieve();
    std::cout << "Primes up to " << limit << " (Standard Sieve):\n";
    for (int p : sieve.get_primes()) {
        std::cout << p << " ";
    }
    std::cout << "\n\n";

    // Compute linear sieve
    sieve.compute_linear_sieve();
    int val = 84;
    std::vector<int> factors = sieve.get_factorization(val);
    std::cout << "Prime factorization of " << val << " using SPF (Linear Sieve):\n";
    for (size_t i = 0; i < factors.size(); ++i) {
        std::cout << factors[i] << (i + 1 == factors.size() ? "" : " * ");
    }
    std::cout << "\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    limit = 100
    sieve = PrimeSieve(limit)

    # Compute standard sieve
    sieve.compute_standard_sieve()
    print(f"Primes up to {limit} (Standard Sieve):")
    print(sieve.primes)
    print()

    # Compute linear sieve
    sieve.compute_linear_sieve()
    val = 84
    factors = sieve.get_factorization(val)
    print(f"Prime factorization of {val} using SPF (Linear Sieve):")
    print(" * ".join(map(str, factors)))
```
