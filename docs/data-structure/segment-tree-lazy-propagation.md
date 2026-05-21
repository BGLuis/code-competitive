# Segment Tree with Lazy Propagation

## Conceptual Explanation

**Lazy Propagation** is an optimization technique applied to Segment Trees that allows performing **range updates** (e.g., adding a value $x$ to all elements in range $[QL, QR]$) and **range queries** in $O(\log N)$ time.

### Core Logic: Lazy Deferral
Without Lazy Propagation, updating an interval $[QL, QR]$ requires modifying every leaf node in that range, which takes $O(N \log N)$ worst-case time.
Lazy Propagation resolves this by deferring (postponing) updates to child nodes until those nodes are actually accessed.

1. **Lazy Array**: Alongside the standard `tree` array of size $4N$, we maintain a `lazy` array of size $4N$, initialized to $0$ (or a neutral update sentinel).
2. **Propagation Step (`push`)**: When visiting a node $i$ representing $[start, end]$:
   - If `lazy[i] != 0`:
     - Apply the deferred update to the current node: `tree[i] += lazy[i] * (end - start + 1)` (for range sums).
     - If $i$ is not a leaf node, pass (propagate) the lazy value to its children: `lazy[2*i] += lazy[i]` and `lazy[2*i+1] += lazy[i]`.
     - Reset `lazy[i] = 0`.
3. **Range Update**: Recursively traverse the tree. If the current node's segment is completely inside the update range, apply the update to the current node, store the pending update in `lazy[i]` for children, and return. Otherwise, `push` the lazy updates down, recurse on both children, and merge the results.
4. **Range Query**: Similar to standard queries, but must execute `push` at each step before recursing to ensure all values are up-to-date.

---

## Complexity Analysis

Let $N$ be the number of elements in the array.

| Operation | Standard Segment Tree | Segment Tree with Lazy Propagation |
| :--- | :--- | :--- |
| **Range Update** | $O(N \log N)$ | $O(\log N)$ |
| **Range Query** | $O(\log N)$ | $O(\log N)$ |
| **Space Complexity** | $O(N)$ (typically $4N$) | $O(N)$ (typically $8N$ for both arrays) |

---

## C++ Implementation

This implementation represents a **Range Sum Query (RSQ)** Segment Tree with **Range Addition Updates**.

```cpp
#include <iostream>
#include <vector>

template <typename T>
class LazySegmentTree {
private:
    int n;
    std::vector<T> tree;
    std::vector<T> lazy;

    // Push pending lazy updates from node to its children
    void push(int node, int start, int end) {
        if (lazy[node] != 0) {
            // Apply lazy update to current node (adjusting for interval length)
            tree[node] += lazy[node] * (end - start + 1);

            // Propagate the lazy value to children (if not a leaf)
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            // Reset the lazy value of the current node
            lazy[node] = 0;
        }
    }

    void build(const std::vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void updateRange(int node, int start, int end, int l, int r, T val) {
        push(node, start, end); // Apply any pending updates

        if (start > end || start > r || end < l) {
            return; // Out of bounds
        }

        if (l <= start && end <= r) {
            // Segment is completely inside update range
            tree[node] += val * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += val;
                lazy[2 * node + 1] += val;
            }
            return;
        }

        int mid = start + (end - start) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    T queryRange(int node, int start, int end, int l, int r) {
        push(node, start, end); // Apply any pending updates

        if (start > end || start > r || end < l) {
            return 0; // Out of bounds sentinel
        }

        if (l <= start && end <= r) {
            return tree[node];
        }

        int mid = start + (end - start) / 2;
        T left_sum = queryRange(2 * node, start, mid, l, r);
        T right_sum = queryRange(2 * node + 1, mid + 1, end, l, r);
        return left_sum + right_sum;
    }

public:
    LazySegmentTree(const std::vector<T>& arr) {
        n = arr.size();
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        build(arr, 1, 0, n - 1);
    }

    // Update range [l, r] by adding 'val' (0-indexed)
    void update(int l, int r, T val) {
        updateRange(1, 0, n - 1, l, r, val);
    }

    // Query sum of range [l, r] inclusive (0-indexed)
    T query(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
};

int main() {
    std::vector<long long> arr = {1, 2, 3, 4, 5};
    std::cout << "Original Array: ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    LazySegmentTree<long long> segTree(arr);

    std::cout << "Query sum [0, 4] (all):      " << segTree.query(0, 4) << " (Expected: 15)\n";
    std::cout << "Query sum [1, 3] (2 + 3 + 4): " << segTree.query(1, 3) << " (Expected: 9)\n";

    std::cout << "\nUpdating range [1, 3] by adding 10...\n";
    segTree.update(1, 3, 10); // Array becomes: 1, 12, 13, 14, 5

    std::cout << "Query sum [1, 3] (12 + 13 + 14): " << segTree.query(1, 3) << " (Expected: 39)\n";
    std::cout << "Query sum [0, 4] (all):          " << segTree.query(0, 4) << " (Expected: 45)\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List

class LazySegmentTree:
    def __init__(self, arr: List[int]):
        self.n = len(arr)
        self.tree = [0] * (4 * self.n)
        self.lazy = [0] * (4 * self.n)
        self._build(arr, 1, 0, self.n - 1)

    def _push(self, node: int, start: int, end: int) -> None:
        """Push pending lazy updates down to children."""
        if self.lazy[node] != 0:
            # Apply lazy value to current node's sum
            self.tree[node] += self.lazy[node] * (end - start + 1)

            # Propagate to children (if not a leaf)
            if start != end:
                self.lazy[2 * node] += self.lazy[node]
                self.lazy[2 * node + 1] += self.lazy[node]

            # Reset current node's lazy value
            self.lazy[node] = 0

    def _build(self, arr: List[int], node: int, start: int, end: int) -> None:
        if start == end:
            self.tree[node] = arr[start]
            return
        mid = (start + end) // 2
        self._build(arr, 2 * node, start, mid)
        self._build(arr, 2 * node + 1, mid + 1, end)
        self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def _update_range(self, node: int, start: int, end: int, l: int, r: int, val: int) -> None:
        self._push(node, start, end)

        if start > end or start > r or end < l:
            return  # Out of bounds

        if l <= start and end <= r:
            # Completely inside update range
            self.tree[node] += val * (end - start + 1)
            if start != end:
                self.lazy[2 * node] += val
                self.lazy[2 * node + 1] += val
            return

        mid = (start + end) // 2
        self._update_range(2 * node, start, mid, l, r, val)
        self._update_range(2 * node + 1, mid + 1, end, l, r, val)
        self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def _query_range(self, node: int, start: int, end: int, l: int, r: int) -> int:
        self._push(node, start, end)

        if start > end or start > r or end < l:
            return 0  # Out of bounds sentinel

        if l <= start and end <= r:
            return self.tree[node]

        mid = (start + end) // 2
        left_sum = self._query_range(2 * node, start, mid, l, r)
        right_sum = self._query_range(2 * node + 1, mid + 1, end, l, r)
        return left_sum + right_sum

    def update(self, l: int, r: int, val: int) -> None:
        """Add val to all elements in range [l, r] inclusive (0-indexed)."""
        self._update_range(1, 0, self.n - 1, l, r, val)

    def query(self, l: int, r: int) -> int:
        """Query sum in range [l, r] inclusive (0-indexed)."""
        return self._query_range(1, 0, self.n - 1, l, r)


if __name__ == '__main__':
    arr = [1, 2, 3, 4, 5]
    print("Original Array:", arr)

    seg_tree = LazySegmentTree(arr)

    print("Query sum [0, 4] (all):      ", seg_tree.query(0, 4), "(Expected: 15)")
    print("Query sum [1, 3] (2 + 3 + 4):", seg_tree.query(1, 3), "(Expected: 9)")

    print("\nUpdating range [1, 3] by adding 10...")
    seg_tree.update(1, 3, 10)  # Array becomes: [1, 12, 13, 14, 5]

    print("Query sum [1, 3] (12 + 13 + 14):", seg_tree.query(1, 3), "(Expected: 39)")
    print("Query sum [0, 4] (all):          ", seg_tree.query(0, 4), "(Expected: 45)")
```
