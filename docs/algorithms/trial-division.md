# Trial Division

## Conceptual Explanation

**Trial Division** is the most fundamental, intuitive, and simple algorithm for primality testing, prime factorization, and finding all divisors of an integer $N$.

The core mathematical principle is that if an integer $N$ is composite, it must have a factor $d$ such that:
$$2 \le d \le \sqrt{N}$$

If no such divisor is found in this range, then $N$ is guaranteed to be prime. This is because divisors always come in pairs $(d, N/d)$. If both divisors in a pair were strictly greater than $\sqrt{N}$, their product would exceed $N$.

### Applications covered in this guide:

1. **Primality Testing**: Checking whether a given number $N$ is prime in $\mathcal{O}(\sqrt{N})$ time.
   - *Optimization*: We can check divisibility by 2 and 3 first, and then only iterate through candidate divisors of the form $6k \pm 1$. This reduces the search space by a factor of 3.
2. **Prime Factorization**: Extracting all prime factors of a number $N$ along with their exponents in $\mathcal{O}(\sqrt{N})$ time.
3. **Divisors Extraction**: Finding all positive divisors of $N$ (both prime and composite) in $\mathcal{O}(\sqrt{N})$ time.

---

## Complexity Analysis

Let $N$ be the input integer.
- **Time Complexity**:
  - **Primality Testing / Factorization / Divisor Extraction**: $\mathcal{O}(\sqrt{N})$ in the worst case (when $N$ is prime or a product of two large primes).
  - **Best Case**: $\mathcal{O}(1)$ or $\mathcal{O}(\log N)$ when $N$ has small prime factors (e.g. $N = 2^k$).
- **Space Complexity**:
  - $\mathcal{O}(1)$ auxiliary space for primality testing.
  - $\mathcal{O}(\log N)$ to store prime factors.
  - $\mathcal{O}(\sqrt{N})$ to store all divisors.

---

## C++ Implementation

Below is a robust C++ implementation containing static methods for primality testing, prime factorization, and divisor extraction.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class TrialDivision {
public:
    // 1. Primality Testing in O(sqrt(N))
    // Highly optimized check using 6k +/- 1 rule
    static bool is_prime(long long n) {
        if (n <= 1) return false;
        if (n <= 3) return true; // 2 and 3 are prime
        if (n % 2 == 0 || n % 3 == 0) return false;

        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }

    // 2. Prime Factorization in O(sqrt(N))
    // Returns pairs of {prime_factor, exponent}
    static std::vector<std::pair<long long, int>> factorize(long long n) {
        std::vector<std::pair<long long, int>> factors;
        
        // Check for 2s
        if (n % 2 == 0) {
            int count = 0;
            while (n % 2 == 0) {
                count++;
                n /= 2;
            }
            factors.push_back({2, count});
        }

        // Check odd candidate divisors up to sqrt(n)
        for (long long d = 3; d * d <= n; d += 2) {
            if (n % d == 0) {
                int count = 0;
                while (n % d == 0) {
                    count++;
                    n /= d;
                }
                factors.push_back({d, count});
            }
        }

        // If the remaining n is greater than 1, then it must be prime
        if (n > 1) {
            factors.push_back({n, 1});
        }

        return factors;
    }

    // 3. Extract all positive divisors in O(sqrt(N))
    // Returns a sorted list of all divisors of n
    static std::vector<long long> get_divisors(long long n) {
        std::vector<long long> divisors;
        if (n <= 0) return divisors;

        for (long long d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                divisors.push_back(d);
                if (d * d != n) {
                    divisors.push_back(n / d);
                }
            }
        }

        std::sort(divisors.begin(), divisors.end());
        return divisors;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple
import math

class TrialDivision:
    @staticmethod
    def is_prime(n: int) -> bool:
        """
        Tests if n is prime in O(sqrt(n)) using the 6k +/- 1 optimization.
        """
        if n <= 1:
            return False
        if n <= 3:
            return True
        if n % 2 == 0 or n % 3 == 0:
            return False

        i = 5
        while i * i <= n:
            if n % i == 0 or n % (i + 2) == 0:
                return False
            i += 6
        return True

    @staticmethod
    def factorize(n: int) -> List[Tuple[int, int]]:
        """
        Computes prime factorization of n in O(sqrt(n)).
        Returns: List of tuples (prime_factor, count)
        """
        factors = []
        
        # Check divisibility by 2
        if n % 2 == 0:
            count = 0
            while n % 2 == 0:
                count += 1
                n //= 2
            factors.append((2, count))

        # Check odd candidate divisors starting from 3
        d = 3
        while d * d <= n:
            if n % d == 0:
                count = 0
                while n % d == 0:
                    count += 1
                    n //= d
                factors.append((d, count))
            d += 2

        # If remaining n is prime
        if n > 1:
            factors.append((n, 1))

        return factors

    @staticmethod
    def get_divisors(n: int) -> List[int]:
        """
        Computes all divisors of n in O(sqrt(n)).
        Returns a sorted list of divisors.
        """
        divisors = []
        if n <= 0:
            return divisors

        d = 1
        while d * d <= n:
            if n % d == 0:
                divisors.append(d)
                if d * d != n:
                    divisors.append(n // d)
            d += 1

        divisors.sort()
        return divisors
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    long long n = 84;

    // 1. Primality Check
    std::cout << n << " is prime? " << (TrialDivision::is_prime(n) ? "Yes" : "No") << " (Expected: No)\n";
    std::cout << "97 is prime? " << (TrialDivision::is_prime(97) ? "Yes" : "No") << " (Expected: Yes)\n\n";

    // 2. Factorize
    auto factors = TrialDivision::factorize(n);
    std::cout << "Prime factorization of " << n << ":\n";
    for (const auto& f : factors) {
        std::cout << f.first << "^" << f.second << " ";
    }
    std::cout << "(Expected: 2^2 * 3^1 * 7^1)\n\n";

    // 3. Divisors
    auto divisors = TrialDivision::get_divisors(n);
    std::cout << "All divisors of " << n << " (" << divisors.size() << " total):\n";
    for (long long d : divisors) {
        std::cout << d << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    n = 84

    # 1. Primality Check
    print(f"{n} is prime? {'Yes' if TrialDivision.is_prime(n) else 'No'} (Expected: No)")
    print(f"97 is prime? {'Yes' if TrialDivision.is_prime(97) else 'No'} (Expected: Yes)\n")

    # 2. Factorize
    factors = TrialDivision.factorize(n)
    fact_str = " * ".join(f"{f}^{exp}" for f, exp in factors)
    print(f"Prime factorization of {n}: {fact_str} (Expected: 2^2 * 3^1 * 7^1)\n")

    # 3. Divisors
    divisors = TrialDivision.get_divisors(n)
    print(f"All divisors of {n} ({len(divisors)} total):")
    print(divisors)
```
