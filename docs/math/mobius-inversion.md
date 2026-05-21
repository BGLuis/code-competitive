# Möbius Inversion

## Conceptual Explanation
**Möbius Inversion** is a fundamental technique in number theory and competitive programming used to simplify arithmetic sums involving the Greatest Common Divisor ($\gcd$).

### The Möbius Function $\mu(n)$
For any positive integer $n$, the Möbius function $\mu(n)$ is defined as:
- $\mu(1) = 1$.
- $\mu(n) = 0$ if $n$ is not square-free (i.e., it is divisible by $p^2$ for some prime $p$).
- $\mu(n) = (-1)^k$ if $n$ is square-free and has $k$ distinct prime factors.

#### Fundamental Property
The most important property of $\mu(n)$ is:
$$\sum_{d \mid n} \mu(d) = \begin{cases} 1 & \text{if } n = 1 \\ 0 & \text{if } n > 1 \end{cases}$$
We can write this using Iverson bracket notation as $\sum_{d \mid n} \mu(d) = [n = 1]$.

### The Möbius Inversion Formula
If $f(n)$ and $g(n)$ are arithmetic functions such that:
$$g(n) = \sum_{d \mid n} f(d)$$
Then $f(n)$ can be recovered via the inversion formula:
$$f(n) = \sum_{d \mid n} \mu(d) g\left(\frac{n}{d}\right) = \sum_{d \mid n} \mu\left(\frac{n}{d}\right) g(d)$$

### Core Competitive Programming Application
Suppose we want to find the number of coprime pairs $(i, j)$ in a range:
$$S(N, M) = \sum_{i=1}^N \sum_{j=1}^M [\gcd(i, j) = 1]$$

By replacing $[\gcd(i, j) = 1]$ with $\sum_{d \mid \gcd(i, j)} \mu(d)$, we get:
$$S(N, M) = \sum_{i=1}^N \sum_{j=1}^M \sum_{d \mid \gcd(i, j)} \mu(d)$$
Rearranging the order of summation by considering the multiplier $d$:
$$S(N, M) = \sum_{d=1}^{\min(N, M)} \mu(d) \sum_{i=1}^{\lfloor N/d \rfloor} \sum_{j=1}^{\lfloor M/d \rfloor} 1 = \sum_{d=1}^{\min(N, M)} \mu(d) \lfloor \frac{N}{d} \rfloor \lfloor \frac{M}{d} \rfloor$$

Using **Linear Sieve** to precompute $\mu(d)$ in $O(N)$ and **Block Division** (square root decomposition on quotients), we can compute $S(N, M)$ in $O(\sqrt{N} + \sqrt{M})$ time per query!

---

## Complexity Analysis
- **Precomputation (Linear Sieve)**:
  - **Time Complexity**: $O(N)$ to find primes, $\mu(n)$, and prefix sums of $\mu$.
  - **Space Complexity**: $O(N)$ to store arrays.
- **Query Processing (Block Division)**:
  - **Time Complexity**: $O(\sqrt{N} + \sqrt{M})$ because the values of $\lfloor N/d \rfloor$ and $\lfloor M/d \rfloor$ only change at most $2\sqrt{N} + 2\sqrt{M}$ times.
  - **Space Complexity**: $O(1)$ auxiliary.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Mobius {
public:
    int limit;
    std::vector<int> primes;
    std::vector<int> mu;
    std::vector<long long> mu_sum; // Prefix sums of mu for range queries

    Mobius(int limit) : limit(limit), mu(limit + 1, 0), mu_sum(limit + 1, 0) {
        std::vector<bool> is_prime(limit + 1, true);
        if (limit >= 1) {
            mu[1] = 1;
        }

        for (int i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                mu[i] = -1;
            }
            for (int p : primes) {
                if (i * p > limit) break;
                is_prime[i * p] = false;
                if (i % p == 0) {
                    mu[i * p] = 0; // Contains squared factor
                    break;
                } else {
                    mu[i * p] = -mu[i];
                }
            }
        }

        // Compute prefix sums
        for (int i = 1; i <= limit; ++i) {
            mu_sum[i] = mu_sum[i - 1] + mu[i];
        }
    }

    // Solves sum_{i=1}^N sum_{j=1}^M [gcd(i, j) == 1] in O(sqrt(N) + sqrt(M))
    long long count_coprime_pairs(long long N, long long M) {
        long long ans = 0;
        long long min_limit = std::min(N, M);
        if (min_limit > limit) {
            throw std::runtime_error("Query parameters exceed sieve limits.");
        }

        for (long long l = 1, r; l <= min_limit; l = r + 1) {
            // Find the maximum r where the quotients floor(N / l) and floor(M / l) remain constant
            r = std::min(N / (N / l), M / (M / l));
            r = std::min(r, min_limit);

            long long sum_mu_segment = mu_sum[r] - mu_sum[l - 1];
            ans += sum_mu_segment * (N / l) * (M / l);
        }
        return ans;
    }
};
```

---

## Python Implementation

```python
from typing import List


class Mobius:
    def __init__(self, limit: int):
        self.limit = limit
        self.primes: List[int] = []
        self.mu = [0] * (limit + 1)
        self.mu_sum = [0] * (limit + 1)

        is_prime = [True] * (limit + 1)
        if limit >= 1:
            self.mu[1] = 1

        for i in range(2, limit + 1):
            if is_prime[i]:
                self.primes.append(i)
                self.mu[i] = -1
            for p in self.primes:
                if i * p > limit:
                    break
                is_prime[i * p] = False
                if i % p == 0:
                    self.mu[i * p] = 0
                    break
                else:
                    self.mu[i * p] = -self.mu[i]

        # Compute prefix sums
        for i in range(1, limit + 1):
            self.mu_sum[i] = self.mu_sum[i - 1] + self.mu[i]

    def count_coprime_pairs(self, N: int, M: int) -> int:
        """Solves sum_{i=1}^N sum_{j=1}^M [gcd(i, j) == 1] in O(sqrt(N) + sqrt(M))."""
        ans = 0
        min_limit = min(N, M)
        if min_limit > self.limit:
            raise ValueError("Query parameters exceed sieve limits.")

        l = 1
        while l <= min_limit:
            # Find quotient block right boundary
            r = min(N // (N // l), M // (M // l))
            r = min(r, min_limit)

            sum_mu_segment = self.mu_sum[r] - self.mu_sum[l - 1]
            ans += sum_mu_segment * (N // l) * (M // l)
            l = r + 1

        return ans
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    int max_val = 100000;
    Mobius solver(max_val);

    // Let's compute coprime pairs for N = 10, M = 10
    // Total pairs = 10 * 10 = 100.
    // Non-coprime pairs:
    // divisible by 2: floor(10/2)*floor(10/2) = 25 pairs
    // divisible by 3: floor(10/3)*floor(10/3) = 9 pairs
    // divisible by 5: floor(10/5)*floor(10/5) = 4 pairs
    // divisible by 7: floor(10/7)*floor(10/7) = 1 pair
    // Including inclusion-exclusion (Möbius):
    // Count = 100*mu(1) + 25*mu(2) + 9*mu(3) + 4*mu(5) + 1*mu(7) + 4*mu(6) + 1*mu(10)
    //       = 100 - 25 - 9 - 4 - 1 + 4 + 1 = 66 pairs.
    long long N = 10, M = 10;
    long long ans = solver.count_coprime_pairs(N, M);

    std::cout << "Coprime pairs count for N = " << N << ", M = " << M 
              << ": " << ans << " (expected 66)\n";

    // Big query test
    long long N_big = 100000, M_big = 100000;
    std::cout << "Coprime pairs count for N = " << N_big << ", M = " << M_big 
              << ": " << solver.count_coprime_pairs(N_big, M_big) << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    max_val = 100000
    solver = Mobius(max_val)

    # Let's compute coprime pairs for N = 10, M = 10 (expected 66)
    N, M = 10, 10
    ans = solver.count_coprime_pairs(N, M)
    print(f"Coprime pairs count for N = {N}, M = {M}: {ans} (expected 66)")

    # Big query test
    N_big, M_big = 100000, 100000
    print(
        f"Coprime pairs count for N = {N_big}, M = {M_big}: {solver.count_coprime_pairs(N_big, M_big)}"
    )
```
