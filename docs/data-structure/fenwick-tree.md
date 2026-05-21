# Fenwick Tree (Binary Indexed Tree)

## Conceptual Explanation

A **Fenwick Tree**, also known as a **Binary Indexed Tree (BIT)**, is an extremely lightweight, array-based data structure that supports dynamic prefix sum queries and point updates.

### Core Logic & Operations
The core philosophy of a Fenwick Tree relies on binary representations of indices. Each index $i$ stores the sum of a range of elements. The size of this range is determined by the **Least Significant Bit (LSB)** of $i$.
Mathematically, the LSB can be isolated using the bitwise operation:
$$\text{LSB}(i) = i \& (-i)$$

1. **Range Query (Prefix Sum)**: To find the sum from $1$ to $i$, we add the value at index $i$ to our total, then strip the LSB from $i$ ($i \leftarrow i - \text{LSB}(i)$), and repeat until $i$ becomes $0$.
2. **Point Update**: To add a value to element at index $i$, we must update all ranges containing $i$. We add the value to index $i$, then add the LSB of $i$ ($i \leftarrow i + \text{LSB}(i)$), and repeat until $i$ exceeds the tree size.

*Note: Fenwick Trees traditionally use **1-based indexing** because index $0$ would cause an infinite loop during updates ($0 + \text{LSB}(0) = 0$).*

### Typical Competitive Programming Use Cases
- **Dynamic Prefix Sums**: Computing range sums in $O(\log N)$ time with dynamic point updates.
- **Inversion Counting**: Counting the number of inversions in an array (i.e., pairs $(i, j)$ such that $i < j$ and $A[i] > A[j]$) in $O(N \log N)$ time.
- **Multi-Dimensional Range Queries**: Extending the Fenwick Tree to 2D or 3D for grid sum queries.

---

## Complexity Analysis

Let $N$ be the size of the array.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Point Update** | $O(\log N)$ | $O(1)$ |
| **Prefix Sum Query** | $O(\log N)$ | $O(1)$ |
| **Range Sum Query** | $O(\log N)$ | $O(1)$ |
| **Tree Construction** | $O(N)$ (or $O(N \log N)$ via updates) | $O(N)$ total |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class FenwickTree {
private:
    int size;
    std::vector<T> tree;

    // Isolate the Least Significant Bit
    inline int lsb(int i) const {
        return i & (-i);
    }

public:
    // Initialize Fenwick Tree of size N with all zeros
    FenwickTree(int n) : size(n) {
        tree.assign(n + 1, 0);
    }

    // Initialize Fenwick Tree directly from an array in O(N) time
    FenwickTree(const std::vector<T>& arr) : size(arr.size()) {
        tree.assign(size + 1, 0);
        for (int i = 1; i <= size; ++i) {
            tree[i] += arr[i - 1];
            int parent = i + lsb(i);
            if (parent <= size) {
                tree[parent] += tree[i];
            }
        }
    }

    // Add 'val' to index 'idx' (0-indexed externally)
    void update(int idx, T val) {
        if (idx < 0 || idx >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        for (int i = idx + 1; i <= size; i += lsb(i)) {
            tree[i] += val;
        }
    }

    // Get prefix sum from index 0 to 'idx' (0-indexed externally)
    T query(int idx) const {
        if (idx < 0) return 0;
        if (idx >= size) idx = size - 1;
        T sum = 0;
        for (int i = idx + 1; i > 0; i -= lsb(i)) {
            sum += tree[i];
        }
        return sum;
    }

    // Get range sum between 'left' and 'right' inclusive (0-indexed externally)
    T queryRange(int left, int right) const {
        if (left > right) return 0;
        return query(right) - query(left - 1);
    }
};

int main() {
    std::vector<int> arr = {3, 2, -1, 6, 5, 4, -3, 3, 7, 2};
    std::cout << "Original Array: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    // Build the Fenwick Tree in O(N)
    FenwickTree<int> bit(arr);

    std::cout << "Prefix sum up to index 4 (0 to 4): " << bit.query(4) << " (Expected: 15)\n";
    std::cout << "Range sum [2, 6]:                 " << bit.queryRange(2, 6) << " (Expected: 11)\n";

    std::cout << "\nUpdating index 2 by adding 5 (arr[2] becomes 4)...\n";
    bit.update(2, 5);

    std::cout << "Prefix sum up to index 4 (0 to 4): " << bit.query(4) << " (Expected: 20)\n";
    std::cout << "Range sum [2, 6]:                 " << bit.queryRange(2, 6) << " (Expected: 16)\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List

class FenwickTree:
    def __init__(self, size_or_arr):
        """
        Can be initialized with an integer (size) or an existing list.
        If a list is passed, construction is done in O(N) time.
        """
        if isinstance(size_or_arr, int):
            self.size = size_or_arr
            self.tree = [0] * (self.size + 1)
        else:
            self.size = len(size_or_arr)
            self.tree = [0] * (self.size + 1)
            # O(N) initialization
            for i in range(1, self.size + 1):
                self.tree[i] += size_or_arr[i - 1]
                parent = i + self._lsb(i)
                if parent <= self.size:
                    self.tree[parent] += self.tree[i]

    def _lsb(self, i: int) -> int:
        return i & (-i)

    def update(self, idx: int, val: int) -> None:
        """Add val to index idx (0-indexed externally)."""
        if not (0 <= idx < self.size):
            raise IndexError("Index out of bounds")
        i = idx + 1
        while i <= self.size:
            self.tree[i] += val
            i += self._lsb(i)

    def query(self, idx: int) -> int:
        """Get prefix sum from index 0 to idx inclusive (0-indexed externally)."""
        if idx < 0:
            return 0
        if idx >= self.size:
            idx = self.size - 1
        sum_val = 0
        i = idx + 1
        while i > 0:
            sum_val += self.tree[i]
            i -= self._lsb(i)
        return sum_val

    def query_range(self, left: int, right: int) -> int:
        """Get range sum between left and right inclusive (0-indexed externally)."""
        if left > right:
            return 0
        return self.query(right) - self.query(left - 1)


if __name__ == '__main__':
    arr = [3, 2, -1, 6, 5, 4, -3, 3, 7, 2]
    print("Original Array:", arr)

    # Build tree
    bit = FenwickTree(arr)

    print("Prefix sum up to index 4 (0 to 4):", bit.query(4), "(Expected: 15)")
    print("Range sum [2, 6]:                ", bit.query_range(2, 6), "(Expected: 11)")

    print("\nUpdating index 2 by adding 5...")
    bit.update(2, 5)

    print("Prefix sum up to index 4 (0 to 4):", bit.query(4), "(Expected: 20)")
    print("Range sum [2, 6]:                ", bit.query_range(2, 6), "(Expected: 16)")
```
