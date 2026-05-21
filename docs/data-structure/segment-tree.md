# Segment Tree

## Conceptual Explanation

A **Segment Tree** is a highly versatile, tree-based data structure used for storing information about intervals or segments. It allows processing both **range queries** (e.g., range sum, minimum, maximum, GCD) and **point updates** efficiently in logarithmic time.

### Core Logic & Structure
A Segment Tree is a full binary tree. If the original array has size $N$:
1. **Leaves**: The leaf nodes of the tree represent the individual elements of the original array (intervals of size $1$, i.e., $[i, i]$).
2. **Internal Nodes**: Each internal node represents the merged result of its two children. If a node represents the interval $[L, R]$, its left child represents $[L, M]$ and its right child represents $[M+1, R]$ (where $M = \lfloor(L+R)/2\rfloor$).
3. **Representation**: Typically implemented as a 1-indexed flat array `tree` of size $4N$. For a node at index $i$:
   - **Left Child**: `2 * i`
   - **Right Child**: `2 * i + 1`

Operations:
- **Build**: Recursively build the tree from bottom up.
- **Point Update**: Traverses down the tree to the leaf corresponding to the index, updates its value, and then updates all ancestors on the path back to the root by merging the child values.
- **Range Query**: Recursively checks if the current node's interval $[L, R]$ is completely inside, completely outside, or partially overlapping with the query interval $[QL, QR]$. It aggregates results from overlapping intervals.

### Typical Competitive Programming Use Cases
- **Range Query Problems**: Finding the minimum, maximum, sum, or GCD in any arbitrary subarray $[L, R]$ under dynamic updates.
- **Inversion Counting / Coordinate Queries**: Solving sweep-line geometry problems.

---

## Complexity Analysis

Let $N$ be the number of elements in the array.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Build Tree** | $O(N)$ | $O(N)$ (typically allocated as $4N$) |
| **Point Update** | $O(\log N)$ | $O(\log N)$ recursive stack |
| **Range Query** | $O(\log N)$ | $O(\log N)$ recursive stack |
| **Space Complexity** | - | $O(N)$ |

---

## C++ Implementation

This implementation represents a **Range Sum Query (RSQ)** Segment Tree with **Point Updates**.

```cpp
#include <iostream>
#include <vector>
#include <functional>

template <typename T>
class SegmentTree {
private:
    int n;
    std::vector<T> tree;

    // Helper to build the tree
    void build(const std::vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1]; // Merge operation: Sum
    }

    // Helper for point update
    void update(int node, int start, int end, int idx, T val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = tree[2 * node] + tree[2 * node + 1]; // Merge
    }

    // Helper for range query
    T query(int node, int start, int end, int l, int r) const {
        if (r < start || end < l) {
            return 0; // Out of range sentinel (identity element for addition)
        }
        if (l <= start && end <= r) {
            return tree[node]; // Completely in range
        }
        int mid = start + (end - start) / 2;
        T left_sum = query(2 * node, start, mid, l, r);
        T right_sum = query(2 * node + 1, mid + 1, end, l, r);
        return left_sum + right_sum; // Merge
    }

public:
    SegmentTree(const std::vector<T>& arr) {
        n = arr.size();
        tree.assign(4 * n, 0);
        build(arr, 1, 0, n - 1);
    }

    // Update element at 'idx' to 'val' (0-indexed)
    void update(int idx, T val) {
        update(1, 0, n - 1, idx, val);
    }

    // Query range sum in [l, r] inclusive (0-indexed)
    T query(int l, int r) const {
        return query(1, 0, n - 1, l, r);
    }
};

int main() {
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};
    std::cout << "Original Array: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    SegmentTree<int> segTree(arr);

    std::cout << "Query sum in range [1, 3] (3 + 5 + 7): " << segTree.query(1, 3) << " (Expected: 15)\n";
    std::cout << "Query sum in range [0, 5] (all):       " << segTree.query(0, 5) << " (Expected: 36)\n";

    std::cout << "\nUpdating element at index 2 to 10 (was 5)...\n";
    segTree.update(2, 10);

    std::cout << "Query sum in range [1, 3] (3 + 10 + 7): " << segTree.query(1, 3) << " (Expected: 20)\n";
    std::cout << "Query sum in range [0, 5] (all):        " << segTree.query(0, 5) << " (Expected: 41)\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar

T = TypeVar('T')

class SegmentTree(Generic[T]):
    def __init__(self, arr: List[T]):
        self.n = len(arr)
        self.tree = [0] * (4 * self.n)
        self._build(arr, 1, 0, self.n - 1)

    def _build(self, arr: List[T], node: int, start: int, end: int) -> None:
        if start == end:
            self.tree[node] = arr[start]
            return
        mid = (start + end) // 2
        self._build(arr, 2 * node, start, mid)
        self._build(arr, 2 * node + 1, mid + 1, end)
        self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def _update(self, node: int, start: int, end: int, idx: int, val: T) -> None:
        if start == end:
            self.tree[node] = val
            return
        mid = (start + end) // 2
        if idx <= mid:
            self._update(2 * node, start, mid, idx, val)
        else:
            self._update(2 * node + 1, mid + 1, end, idx, val)
        self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def _query(self, node: int, start: int, end: int, l: int, r: int) -> T:
        if r < start or end < l:
            return 0  # Sentinel identity element for sum
        if l <= start and end <= r:
            return self.tree[node]
        mid = (start + end) // 2
        left_sum = self._query(2 * node, start, mid, l, r)
        right_sum = self._query(2 * node + 1, mid + 1, end, l, r)
        return left_sum + right_sum

    def update(self, idx: int, val: T) -> None:
        """Update element at index idx (0-indexed) to val."""
        self._update(1, 0, self.n - 1, idx, val)

    def query(self, l: int, r: int) -> T:
        """Query range sum in [l, r] inclusive (0-indexed)."""
        return self._query(1, 0, self.n - 1, l, r)


if __name__ == '__main__':
    arr = [1, 3, 5, 7, 9, 11]
    print("Original Array:", arr)

    seg_tree = SegmentTree(arr)

    print("Query sum in range [1, 3] (3 + 5 + 7):", seg_tree.query(1, 3), "(Expected: 15)")
    print("Query sum in range [0, 5] (all):      ", seg_tree.query(0, 5), "(Expected: 36)")

    print("\nUpdating element at index 2 to 10...")
    seg_tree.update(2, 10)

    print("Query sum in range [1, 3] (3 + 10 + 7):", seg_tree.query(1, 3), "(Expected: 20)")
    print("Query sum in range [0, 5] (all):       ", seg_tree.query(0, 5), "(Expected: 41)")
```
