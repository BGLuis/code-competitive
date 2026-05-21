# Disjoint Set Union (DSU) / Union-Find

## Conceptual Explanation

**Disjoint Set Union (DSU)**, also known as **Union-Find**, is a tree-based data structure that tracks elements partitioned into a number of disjoint (non-overlapping) subsets. It is extremely powerful for checking connectivity and grouping elements efficiently.

### Core Logic & Operations
DSU supports two primary operations:
1. **Find**: Determine which subset a particular element belongs to. This is done by climbing up the parent pointers of the element's tree until reaching the root.
2. **Union**: Join two subsets into a single subset. This is done by finding the roots of both elements and making the root of one root point to the other.

To make these operations highly efficient, DSU utilizes two key optimization techniques:
- **Path Compression**: During a `find` operation, make every visited node point directly to the root of the tree. This flattens the tree structure and keeps subsequent traversals extremely fast.
- **Union by Size/Rank**: Always attach the smaller tree to the root of the larger tree. This ensures that the tree depth remains logarithmic.

### Typical Competitive Programming Use Cases
- **Kruskal's Algorithm**: Finding the Minimum Spanning Tree (MST) of a graph in $O(M \log M)$ or $O(M \log N)$.
- **Dynamic Connectivity / Cycle Detection**: Checking if adding an edge between two vertices creates a cycle in an undirected graph.
- **Offline Query Processing**: Answering connectivity queries after dynamic additions or sorting.

---

## Complexity Analysis

Let $N$ be the number of elements.

| Operation | Without Optimizations | With Path Compression and Union by Size/Rank |
| :--- | :--- | :--- |
| **Find** | $O(N)$ | $O(\alpha(N))$ |
| **Union** | $O(N)$ | $O(\alpha(N))$ |
| **Space Complexity** | $O(N)$ | $O(N)$ |

*Note: $\alpha(N)$ is the inverse Ackermann function, which grows extremely slowly and is less than $5$ for any practical value of $N$ in our universe (effectively $O(1)$).*

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <numeric>

class DisjointSetUnion {
private:
    std::vector<int> parent;
    std::vector<int> size;
    int num_sets;

public:
    // Initialize DSU with N elements (0 to N-1)
    DisjointSetUnion(int n) : num_sets(n) {
        parent.resize(n);
        // Initially, each element is its own parent
        std::iota(parent.begin(), parent.end(), 0);
        // Initially, each set contains exactly 1 element
        size.assign(n, 1);
    }

    // Find the representative root of element v with Path Compression
    int find(int v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = find(parent[v]); // Path compression
    }

    // Union two subsets by Size optimization
    bool unionSets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            // Ensure set 'a' is the larger set
            if (size[a] < size[b]) {
                std::swap(a, b);
            }
            parent[b] = a;
            size[a] += size[b];
            num_sets--;
            return true; // Successfully merged two disjoint sets
        }
        return false; // Already in the same set
    }

    // Check if two elements belong to the same set
    bool connected(int a, int b) {
        return find(a) == find(b);
    }

    // Get the size of the set containing element v
    int getSetSize(int v) {
        return size[find(v)];
    }

    // Get the total number of disjoint sets
    int getNumSets() const {
        return num_sets;
    }
};

int main() {
    int n = 6;
    DisjointSetUnion dsu(n);

    // Initial state: 6 separate sets
    std::cout << "Initial number of sets: " << dsu.getNumSets() << "\n";

    // Merge some elements
    dsu.unionSets(0, 1);
    dsu.unionSets(1, 2);
    dsu.unionSets(3, 4);

    std::cout << "\nAfter unions (0-1, 1-2, 3-4):\n";
    std::cout << "Is 0 connected to 2? " << (dsu.connected(0, 2) ? "Yes" : "No") << "\n";
    std::cout << "Is 0 connected to 3? " << (dsu.connected(0, 3) ? "Yes" : "No") << "\n";
    std::cout << "Size of set containing 0: " << dsu.getSetSize(0) << "\n";
    std::cout << "Size of set containing 3: " << dsu.getSetSize(3) << "\n";
    std::cout << "Total number of sets: " << dsu.getNumSets() << "\n";

    // Merge the two larger groups together
    dsu.unionSets(2, 3);
    std::cout << "\nAfter merging 2 and 3:\n";
    std::cout << "Is 0 connected to 4? " << (dsu.connected(0, 4) ? "Yes" : "No") << "\n";
    std::cout << "Size of set containing 0: " << dsu.getSetSize(0) << "\n";
    std::cout << "Total number of sets: " << dsu.getNumSets() << "\n";

    return 0;
}
```

---

## Python Implementation

```python
class DisjointSetUnion:
    def __init__(self, n: int):
        # Each element is initially its own parent
        self.parent = list(range(n))
        # Initial size of each set is 1
        self.size = [1] * n
        self.num_sets = n

    def find(self, v: int) -> int:
        """Find the root representative of v with Path Compression."""
        if v == self.parent[v]:
            return v
        self.parent[v] = self.find(self.parent[v])  # Path compression
        return self.parent[v]

    def union(self, a: int, b: int) -> bool:
        """Union two sets by Size optimization. Returns True if merged."""
        root_a = self.find(a)
        root_b = self.find(b)
        if root_a != root_b:
            # Union by Size: attach smaller tree to larger tree
            if self.size[root_a] < self.size[root_b]:
                root_a, root_b = root_b, root_a
            self.parent[root_b] = root_a
            self.size[root_a] += self.size[root_b]
            self.num_sets -= 1
            return True
        return False

    def connected(self, a: int, b: int) -> bool:
        """Check if elements a and b are in the same set."""
        return self.find(a) == self.find(b)

    def get_set_size(self, v: int) -> int:
        """Get the size of the set containing element v."""
        return self.size[self.find(v)]

    def get_num_sets(self) -> int:
        """Get the total count of disjoint sets."""
        return self.num_sets

if __name__ == '__main__':
    n = 6
    dsu = DisjointSetUnion(n)

    # Initial state
    print(f"Initial number of sets: {dsu.get_num_sets()}")

    # Merge elements
    dsu.union(0, 1)
    dsu.union(1, 2)
    dsu.union(3, 4)

    print("\nAfter unions (0-1, 1-2, 3-4):")
    print(f"Is 0 connected to 2? {'Yes' if dsu.connected(0, 2) else 'No'}")
    print(f"Is 0 connected to 3? {'Yes' if dsu.connected(0, 3) else 'No'}")
    print(f"Size of set containing 0: {dsu.get_set_size(0)}")
    print(f"Size of set containing 3: {dsu.get_set_size(3)}")
    print(f"Total number of sets: {dsu.get_num_sets()}")

    # Merge groups
    dsu.union(2, 3)
    print("\nAfter merging 2 and 3:")
    print(f"Is 0 connected to 4? {'Yes' if dsu.connected(0, 4) else 'No'}")
    print(f"Size of set containing 0: {dsu.get_set_size(0)}")
    print(f"Total number of sets: {dsu.get_num_sets()}")
```
