# Binary Lifting

## Conceptual Explanation

**Binary lifting** is a powerful technique used on trees to answer path queries (like the Lowest Common Ancestor - LCA, path maximum, or path sum) and to jump $K$ steps up a tree structure in $\mathcal{O}(\log K)$ time.

### Core Principle

Instead of traversing a tree step-by-step (which would take $\mathcal{O}(N)$ in the worst case), we precompute jumps of powers of two ($2^0, 2^1, 2^2, \dots, 2^j$).
Any integer $K$ can be represented uniquely as a sum of powers of two (its binary representation). Using this representation, we can jump $K$ steps up in at most $\log_2 K$ jumps.

We define a table:
$$\text{up}[u][j]$$
which stores the $2^j$-th ancestor of vertex $u$.

The recurrence relation to compute this table is:
$$\text{up}[u][j] = \text{up}[\text{up}[u][j-1]][j-1]$$
This means that to jump $2^j$ steps up from $u$, we first jump $2^{j-1}$ steps up to get to $v = \text{up}[u][j-1]$, and then jump another $2^{j-1}$ steps up from $v$.

### Lowest Common Ancestor (LCA) using Binary Lifting

To find the LCA of two nodes $u$ and $v$:
1. **Depth Alignment**: Align $u$ and $v$ to the same depth. If $depth[u] < depth[v]$, swap them. Then, jump $u$ up until $depth[u] == depth[v]$ using the binary representation of the difference in depths.
2. **Check Equality**: If $u == v$, then their LCA is $u$ (or $v$).
3. **Simultaneous Jumps**: Starting from the largest possible jump power $j$, if $\text{up}[u][j] \neq \text{up}[v][j]$, we make the jump: $u = \text{up}[u][j]$ and $v = \text{up}[v][j]$. We decrement $j$ and repeat.
4. **Final Step**: After the loop, $u$ and $v$ are exactly one step below their LCA. The LCA is then $\text{up}[u][0]$.

### Typical Competitive Programming Use Cases

- Finding the Lowest Common Ancestor (LCA) of two nodes in a tree.
- Computing path properties on trees (e.g., path sum, maximum/minimum weight edge on the path) in $\mathcal{O}(\log N)$.
- Answering functional graph jumping queries (graphs where every vertex has out-degree 1).

---

## Complexity Analysis

- **Time Complexity**:
  - **Preprocessing**: $\mathcal{O}(N \log N)$ to run DFS and compute the $\text{up}$ table of size $N \times \log N$.
  - **Query (LCA / Jump)**: $\mathcal{O}(\log N)$ per query since we traverse at most $\log N$ binary choices.
- **Space Complexity**:
  - $\mathcal{O}(N \log N)$ to store the $\text{up}$ table.

---

## C++ Implementation

Below is a robust and complete C++ class to find LCA and tree depth using binary lifting.

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class BinaryLiftingLCA {
private:
    int n;
    int log_n;
    std::vector<std::vector<int>> adj;
    std::vector<int> depth;
    std::vector<std::vector<int>> up;

    void dfs(int u, int p, int d) {
        depth[u] = d;
        up[u][0] = p;

        // Compute 2^j ancestors
        for (int j = 1; j <= log_n; ++j) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }

        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1);
            }
        }
    }

public:
    BinaryLiftingLCA(int num_nodes, const std::vector<std::pair<int, int>>& edges, int root = 0) {
        n = num_nodes;
        log_n = std::ceil(std::log2(n)) + 1;
        adj.resize(n);
        depth.resize(n, 0);
        up.assign(n, std::vector<int>(log_n + 1, root));

        for (const auto& edge : edges) {
            adj[edge.first].push_back(edge.second);
            adj[edge.second].push_back(edge.first);
        }

        // Initialize DFS from the root. The root's parent is itself.
        dfs(root, root, 0);
    }

    int get_lca(int u, int v) const {
        if (depth[u] < depth[v]) {
            std::swap(u, v);
        }

        // 1. Align depths
        int diff = depth[u] - depth[v];
        for (int j = log_n; j >= 0; --j) {
            if ((diff >> j) & 1) {
                u = up[u][j];
            }
        }

        if (u == v) {
            return u;
        }

        // 2. Lift simultaneously
        for (int j = log_n; j >= 0; --j) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

    int get_distance(int u, int v) const {
        int lca = get_lca(u, v);
        return depth[u] + depth[v] - 2 * depth[lca];
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple
import math

class BinaryLiftingLCA:
    def __init__(self, num_nodes: int, edges: List[Tuple[int, int]], root: int = 0):
        self.n = num_nodes
        self.log_n = math.ceil(math.log2(self.n)) + 1
        self.adj: List[List[int]] = [[] for _ in range(self.n)]
        self.depth = [0] * self.n
        self.up = [[root] * (self.log_n + 1) for _ in range(self.n)]

        for u, v in edges:
            self.adj[u].append(v)
            self.adj[v].append(u)

        # Precompute ancestors and depths starting from root
        self._dfs(root, root, 0)

    def _dfs(self, u: int, p: int, d: int) -> None:
        self.depth[u] = d
        self.up[u][0] = p

        for j in range(1, self.log_n + 1):
            self.up[u][j] = self.up[self.up[u][j - 1]][j - 1]

        for v in self.adj[u]:
            if v != p:
                self._dfs(v, u, d + 1)

    def get_lca(self, u: int, v: int) -> int:
        if self.depth[u] < self.depth[v]:
            u, v = v, u

        # 1. Align depths
        diff = self.depth[u] - self.depth[v]
        for j in range(self.log_n, -1, -1):
            if (diff >> j) & 1:
                u = self.up[u][j]

        if u == v:
            return u

        # 2. Lift simultaneously
        for j in range(self.log_n, -1, -1):
            if self.up[u][j] != self.up[v][j]:
                u = self.up[u][j]
                v = self.up[v][j]

        return self.up[u][0]

    def get_distance(self, u: int, v: int) -> int:
        lca = self.get_lca(u, v)
        return self.depth[u] + self.depth[v] - 2 * self.depth[lca]
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Constructing a simple tree
    //        0
    //       / \
    //      1   2
    //     / \
    //    3   4
    int num_nodes = 5;
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}
    };

    BinaryLiftingLCA lca_solver(num_nodes, edges, 0);

    std::cout << "LCA of 3 and 4: " << lca_solver.get_lca(3, 4) << " (Expected: 1)\n";
    std::cout << "LCA of 3 and 2: " << lca_solver.get_lca(3, 2) << " (Expected: 0)\n";
    std::cout << "Distance between 3 and 4: " << lca_solver.get_distance(3, 4) << " (Expected: 2)\n";
    std::cout << "Distance between 3 and 2: " << lca_solver.get_distance(3, 2) << " (Expected: 3)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # Constructing a simple tree
    #        0
    #       / \
    #      1   2
    #     / \
    #    3   4
    num_nodes = 5
    edges = [(0, 1), (0, 2), (1, 3), (1, 4)]

    lca_solver = BinaryLiftingLCA(num_nodes, edges, root=0)

    print(f"LCA of 3 and 4: {lca_solver.get_lca(3, 4)} (Expected: 1)")
    print(f"LCA of 3 and 2: {lca_solver.get_lca(3, 2)} (Expected: 0)")
    print(f"Distance between 3 and 4: {lca_solver.get_distance(3, 4)} (Expected: 2)")
    print(f"Distance between 3 and 2: {lca_solver.get_distance(3, 2)} (Expected: 3)")
```
