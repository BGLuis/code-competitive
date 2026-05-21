# Coordinate Compression

## Conceptual Explanation

**Coordinate Compression** is a technique used to map a large, sparse set of values (such as coordinates ranging up to $10^9$ or even $10^{18}$) into a small, dense range of integers (typically $0$ to $N - 1$, where $N$ is the number of unique elements). This mapping preserves the **relative ordering** of the elements:
$$A < B \implies \text{compressed}(A) < \text{compressed}(B)$$

### Core Logic & Operations
1. **Gather Values**: Collect all values that need to be indexed (e.g., points, interval endpoints, query coordinates).
2. **Sort and Deduplicate**: Sort the values in ascending order and remove any duplicate values.
3. **Map Construction**:
   - In **C++**, you can query the compressed rank of a value on the fly using binary search (`std::lower_bound`) on the sorted, unique vector.
   - In **Python**, you can build a hash map (dictionary) where the key is the original value and the value is its rank (its index in the sorted list of unique elements).
4. **Decompress (Query Original)**: The index in the sorted unique array represents the compressed coordinate, while the value at that index is the original coordinate.

### Typical Competitive Programming Use Cases
- **Segment Tree / Fenwick Tree Indexing**: Standard Segment Trees require the array size to be moderate (usually $\le 2 \cdot 10^5$). If the coordinates of updates/queries are in the range of $[0, 10^9]$, we compress them first to index into the tree.
- **Sweep-Line Algorithms**: Compressing $Y$-coordinates or event times to easily update intervals or query statuses in a range query data structure.
- **DP State Reduction**: Compressing sparse transitions or values to fit within memory limitations.

---

## Complexity Analysis

Let $N$ be the number of coordinates to compress.

| Step | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Gather and Sort** | $O(N \log N)$ | $O(N)$ |
| **Deduplicate** | $O(N)$ | $O(1)$ |
| **Query Compressed Value** | $O(\log N)$ (or $O(1)$ with Hash Map) | $O(N)$ |
| **Query Original Value** | $O(1)$ | $O(N)$ |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class CoordinateCompressor {
private:
    std::vector<T> unique_coords;
    bool finalized;

public:
    CoordinateCompressor() : finalized(false) {}

    // Add a coordinate to the compressor pool
    void add(const T& val) {
        unique_coords.push_back(val);
        finalized = false;
    }

    // Finalize the coordinates by sorting and deduplicating
    void compress() {
        std::sort(unique_coords.begin(), unique_coords.end());
        unique_coords.erase(std::unique(unique_coords.begin(), unique_coords.end()), unique_coords.end());
        finalized = true;
    }

    // Get the compressed index of a value (0-indexed rank)
    int getCompressed(const T& val) const {
        // Must be finalized before querying
        auto it = std::lower_bound(unique_coords.begin(), unique_coords.end(), val);
        if (it == unique_coords.end() || *it != val) {
            return -1; // Value not found
        }
        return std::distance(unique_coords.begin(), it);
    }

    // Get the original coordinate from a compressed index
    T getOriginal(int compressed_idx) const {
        return unique_coords[compressed_idx];
    }

    // Get the total number of unique coordinates
    size_t size() const {
        return unique_coords.size();
    }
};

int main() {
    // Original coordinates: notice very large values and duplicates
    std::vector<long long> coords = {1000000000LL, 15LL, 1000000000LL, 42000LL, 15LL, 999999999999LL};

    CoordinateCompressor<long long> cc;
    for (auto c : coords) {
        cc.add(c);
    }

    // Compress
    cc.compress();

    std::cout << "Original coordinate -> Compressed index:\n";
    for (auto c : coords) {
        std::cout << c << " -> " << cc.getCompressed(c) << "\n";
    }

    std::cout << "\nCompressed index -> Original coordinate:\n";
    for (size_t i = 0; i < cc.size(); ++i) {
        std::cout << i << " -> " << cc.getOriginal(i) << "\n";
    }

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar

T = TypeVar('T')

class CoordinateCompressor(Generic[T]):
    def __init__(self, coords: List[T] = None):
        self.unique_coords: List[T] = []
        self._val_to_compressed = {}
        if coords:
            for val in coords:
                self.add(val)
            self.compress()

    def add(self, val: T) -> None:
        """Add a coordinate to the pool."""
        self.unique_coords.append(val)

    def compress(self) -> None:
        """Sort and deduplicate coordinates, then build mapping dict."""
        self.unique_coords = sorted(list(set(self.unique_coords)))
        self._val_to_compressed = {val: idx for idx, val in enumerate(self.unique_coords)}

    def get_compressed(self, val: T) -> int:
        """Get the compressed index of a value (0-indexed rank)."""
        return self._val_to_compressed.get(val, -1)

    def get_original(self, compressed_idx: int) -> T:
        """Get the original coordinate from a compressed index."""
        return self.unique_coords[compressed_idx]

    def __len__(self) -> int:
        return len(self.unique_coords)

if __name__ == '__main__':
    # Original coordinates: large values and duplicates
    coords = [1000000000, 15, 1000000000, 42000, 15, 999999999999]

    # Instantiate compressor
    cc = CoordinateCompressor[int](coords)

    print("Original coordinate -> Compressed index:")
    for c in coords:
        print(f"{c} -> {cc.get_compressed(c)}")

    print("\nCompressed index -> Original coordinate:")
    for i in range(len(cc)):
        print(f"{i} -> {cc.get_original(i)}")
```
