# Permutation Algorithms (Rank, Unrank, and Cycles)

## Conceptual Explanation
A **Permutation** of a set is an arrangement of its elements into a sequence or linear order. In competitive programming, permutations have rich mathematical properties and algorithmic applications.

### Key Algorithmic Operations

#### 1. Next Lexicographical Permutation
Given a sequence, we want to transform it into the next lexicographically larger permutation. The algorithm (which standard C++ implements as `std::next_permutation`):
1. Find the largest index $i$ such that $A[i] < A[i + 1]$. If no such index exists, the permutation is the last one (descending order).
2. Find the largest index $j > i$ such that $A[j] > A[i]$.
3. Swap $A[i]$ and $A[j]$.
4. Reverse the suffix starting at $A[i + 1]$.

#### 2. Permutation Ranking (Lexicographical Index)
Given a permutation of size $N$ of elements $\{1, 2, \dots, N\}$, we want to find its 0-based lexicographical index among all $N!$ permutations.
Using **Lehmer Code**, the rank is calculated as:
$$\text{Rank}(P) = \sum_{i=0}^{N-1} d_i \times (N - 1 - i)!$$
where $d_i$ is the number of elements strictly smaller than $P[i]$ that appear to the right of $P[i]$. A naive calculation of all $d_i$ takes $O(N^2)$. By using a **Binary Indexed Tree (Fenwick Tree)**, we can count remaining elements in $O(\log N)$ per element, leading to $O(N \log N)$ total time.

#### 3. Permutation Unranking
Given a rank $R$ and size $N$, we want to reconstruct the unique permutation.
1. We iterate from $i = 0$ to $N - 1$.
2. At each step, $d_i = \lfloor R / (N - 1 - i)! \rfloor$, and we update $R \leftarrow R \bmod (N - 1 - i)!$.
3. We select the $(d_i + 1)$-th available unused element as $P[i]$.
Using a simple list, this step takes $O(N^2)$.

#### 4. Cycle Decomposition and Parity
Every permutation can be uniquely decomposed into disjoint **Cycles**. For example, the permutation $P = [3, 4, 1, 5, 2]$ (1-indexed: $1 \to 3 \to 1$ and $2 \to 4 \to 5 \to 2$) has two cycles: $(1, 3)$ and $(2, 4, 5)$.
- **Parity/Sign**: A permutation is **even** if it can be expressed as an even number of transpositions (swaps), and **odd** otherwise.
- The number of swaps needed to sort a permutation of size $N$ with $C$ disjoint cycles is $N - C$. Thus, the permutation is even if $N - C$ is even, and odd if $N - C$ is odd.

---

## Complexity Analysis
- **Next Permutation**: $O(N)$ time and $O(1)$ space.
- **Lexicographical Ranking**: $O(N \log N)$ using a Fenwick Tree, $O(N)$ space.
- **Lexicographical Unranking**: $O(N^2)$ using array deletion, or $O(N \log N)$ using a Fenwick tree + binary lifting. The $O(N^2)$ algorithm is standard and preferred in CP for $N \le 5000$.
- **Cycle Decomposition**: $O(N)$ time and space (since every vertex is visited exactly once).

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Fenwick Tree (Binary Indexed Tree) helper for O(N log N) ranking
class FenwickTree {
    int n;
    std::vector<int> tree;
public:
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

// Compute factorials up to N modulo a large number or using __int128 for no overflow up to N=20
std::vector<unsigned __int128> get_factorials(int n) {
    std::vector<unsigned __int128> fact(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    return fact;
}

// 1. Next Permutation (Manual Implementation)
bool custom_next_permutation(std::vector<int>& a) {
    if (a.empty()) return false;
    int i = static_cast<int>(a.size()) - 2;
    while (i >= 0 && a[i] >= a[i + 1]) {
        i--;
    }
    if (i < 0) {
        std::reverse(a.begin(), a.end());
        return false; // Last permutation
    }
    int j = static_cast<int>(a.size()) - 1;
    while (a[j] <= a[i]) {
        j--;
    }
    std::swap(a[i], a[j]);
    std::reverse(a.begin() + i + 1, a.end());
    return true;
}

// 2. Rank of a permutation of size N (values 1 to N) in O(N log N)
unsigned __int128 get_permutation_rank(const std::vector<int>& perm) {
    int n = perm.size();
    auto fact = get_factorials(n);
    FenwickTree bit(n);
    for (int i = 1; i <= n; ++i) {
        bit.add(i, 1); // Mark all numbers as available
    }

    unsigned __int128 rank = 0;
    for (int i = 0; i < n; ++i) {
        // Count how many available numbers are strictly smaller than perm[i]
        int smaller_available = bit.query(perm[i] - 1);
        rank += smaller_available * fact[n - 1 - i];
        bit.add(perm[i], -1); // Remove perm[i] from available
    }
    return rank;
}

// 3. Unrank a permutation of size N (rank is 0-indexed) in O(N^2)
std::vector<int> get_permutation_unrank(int n, unsigned __int128 rank) {
    auto fact = get_factorials(n);
    std::vector<int> available(n);
    std::iota(available.begin(), available.end(), 1); // 1, 2, ..., n
    std::vector<int> perm;

    for (int i = 0; i < n; ++i) {
        unsigned __int128 fact_val = fact[n - 1 - i];
        int idx = static_cast<int>(rank / fact_val);
        rank %= fact_val;
        perm.push_back(available[idx]);
        available.erase(available.begin() + idx);
    }
    return perm;
}

// 4. Cycle Decomposition: Returns the list of cycles (1-indexed representation)
struct CycleDecomposition {
    std::vector<std::vector<int>> cycles;
    int parity; // 0 for even, 1 for odd
};

CycleDecomposition decompose_cycles(const std::vector<int>& perm) {
    int n = perm.size();
    std::vector<bool> visited(n + 1, false);
    std::vector<std::vector<int>> cycles;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            std::vector<int> cycle;
            int curr = i;
            while (!visited[curr]) {
                visited[curr] = true;
                cycle.push_back(curr);
                curr = perm[curr - 1]; // perm is 0-indexed, values are 1-indexed
            }
            cycles.push_back(cycle);
        }
    }

    // Parity calculation: (N - Number of cycles) % 2
    int swaps = n - cycles.size();
    int parity = swaps % 2;
    return {cycles, parity};
}
```

---

## Python Implementation

```python
import math
from typing import List, Tuple


def custom_next_permutation(a: List[int]) -> bool:
    """Transforms list to next lexicographical permutation in-place.

    Returns False if already at maximum.
    """
    i = len(a) - 2
    while i >= 0 and a[i] >= a[i + 1]:
        i -= 1
    if i < 0:
        a.reverse()
        return False
    j = len(a) - 1
    while a[j] <= a[i]:
        j -= 1
    a[i], a[j] = a[j], a[i]
    a[i + 1 :] = reversed(a[i + 1 :])
    return True


class FenwickTree:
    def __init__(self, n: int):
        self.n = n
        self.tree = [0] * (n + 1)

    def add(self, i: int, delta: int) -> None:
        while i <= self.n:
            self.tree[i] += delta
            i += i & -i

    def query(self, i: int) -> int:
        s = 0
        while i > 0:
            s += self.tree[i]
            i -= i & -i
        return s


def get_permutation_rank(perm: List[int]) -> int:
    """Computes 0-based lexicographical rank of permutation in O(N log N)."""
    n = len(perm)
    bit = FenwickTree(n)
    for i in range(1, n + 1):
        bit.add(i, 1)

    rank = 0
    for i in range(n):
        smaller_available = bit.query(perm[i] - 1)
        rank += smaller_available * math.factorial(n - 1 - i)
        bit.add(perm[i], -1)
    return rank


def get_permutation_unrank(n: int, rank: int) -> List[int]:
    """Reconstructs permutation from rank in O(N^2)."""
    available = list(range(1, n + 1))
    perm = []
    curr_rank = rank

    for i in range(n):
        fact = math.factorial(n - 1 - i)
        idx = curr_rank // fact
        curr_rank %= fact
        perm.append(available[idx])
        available.pop(idx)
    return perm


def decompose_cycles(perm: List[int]) -> Tuple[List[List[int]], int]:
    """Decomposes 1-indexed permutation values into disjoint cycles."""
    n = len(perm)
    visited = [False] * (n + 1)
    cycles = []

    for i in range(1, n + 1):
        if not visited[i]:
            cycle = []
            curr = i
            while not visited[curr]:
                visited[curr] = True
                cycle.append(curr)
                curr = perm[curr - 1]
            cycles.append(cycle)

    swaps = n - len(cycles)
    parity = swaps % 2
    return cycles, parity
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 1. Next Permutation demonstration
    std::vector<int> a = {1, 2, 3};
    std::cout << "Generating all permutations of {1, 2, 3}:\n";
    do {
        for (int x : a) std::cout << x << " ";
        std::cout << "\n";
    } while (custom_next_permutation(a));
    std::cout << "\n";

    // 2 & 3. Rank and Unrank
    std::vector<int> perm = {3, 1, 4, 2}; // size N = 4
    unsigned __int128 rank = get_permutation_rank(perm);
    
    // Outputting 128-bit unsigned rank
    std::cout << "Rank of {3, 1, 4, 2}: " << static_cast<long long>(rank) << " (expected 13)\n";

    std::vector<int> reconstructed = get_permutation_unrank(4, 13);
    std::cout << "Reconstructed Permutation from rank 13: ";
    for (int x : reconstructed) std::cout << x << " ";
    std::cout << "\n\n";

    // 4. Cycle Decomposition of {3, 4, 1, 5, 2}
    std::vector<int> perm_cycle = {3, 4, 1, 5, 2};
    auto decomp = decompose_cycles(perm_cycle);
    std::cout << "Cycle Decomposition:\n";
    for (const auto& cycle : decomp.cycles) {
        std::cout << "( ";
        for (int node : cycle) std::cout << node << " ";
        std::cout << ") ";
    }
    std::cout << "\nPermutation parity: " << (decomp.parity == 0 ? "EVEN" : "ODD") << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. Next Permutation
    a = [1, 2, 3]
    print("Generating all permutations of [1, 2, 3]:")
    print(a)
    while custom_next_permutation(a):
        print(a)
    print()

    # 2 & 3. Rank and Unrank
    perm = [3, 1, 4, 2]
    rank = get_permutation_rank(perm)
    print(f"Rank of {perm}: {rank} (expected 13)")

    recon = get_permutation_unrank(4, 13)
    print(f"Reconstructed Permutation from rank 13: {recon}\n")

    # 4. Cycle Decomposition of [3, 4, 1, 5, 2]
    perm_cycle = [3, 4, 1, 5, 2]
    cycles, parity = decompose_cycles(perm_cycle)
    print(f"Cycles: {cycles}")
    print(f"Parity: {'EVEN' if parity == 0 else 'ODD'}")
```
