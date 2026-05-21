# Complete Graphs and Prufer Sequences

## Conceptual Explanation
A **Complete Graph** (denoted as $K_n$) is a simple undirected graph in which every pair of distinct vertices is connected by a unique edge. 

### Key Theoretical Properties of $K_n$
1. **Edges**: $K_n$ has exactly $\frac{n(n-1)}{2}$ edges.
2. **Degrees**: Every vertex in $K_n$ has degree $n-1$.
3. **Hamiltonian Cycles**: Since the degree of every vertex is $n-1 \ge \frac{n}{2}$ (for $n \ge 3$), Dirac's theorem guarantees $K_n$ is Hamiltonian. In fact, $K_n$ contains $\frac{(n-1)!}{2}$ distinct Hamiltonian cycles.
4. **Eulerian Paths/Cycles**: $K_n$ has an Eulerian cycle if and only if $n$ is odd (so that every vertex has an even degree, $n-1$). It has an Eulerian path (but not cycle) if $n = 2$.
5. **Cayley's Formula**: The number of spanning trees in a complete graph $K_n$ is:
   $$T_n = n^{n-2}$$

---

### Prufer Sequences (Prüfer Code)
A **Prüfer sequence** is a unique sequence of $n - 2$ integers associated with a labeled tree of $n$ vertices. The sequence provides an elegant bijective mapping between the set of labeled trees on $n$ vertices and the set of sequences of length $n - 2$ on the alphabet $\{1, 2, \dots, n\}$. This bijection proves Cayley's Formula ($n^{n-2}$ possible sequences of length $n-2$ with elements from $1$ to $n$).

#### Prufer Sequence Construction (Tree to Code)
1. Find the leaf (vertex of degree 1) with the smallest label.
2. Remove this leaf from the tree and write down its unique neighbor's label into the sequence.
3. Repeat the process until only two vertices remain.
Using a min-heap or pointer-based scanning, this can be implemented in $O(n \log n)$ or $O(n)$ time.

#### Prufer Sequence Reconstruction (Code to Tree)
1. Initialize the degrees of all vertices to $1 + (\text{number of occurrences in the sequence})$.
2. Find the smallest vertex with degree 1.
3. Connect this vertex with the first element of the sequence, decrement the degrees of both, and remove the first element of the sequence.
4. Repeat until the sequence is empty.
5. Finally, connect the two remaining vertices with degree 1.
This can also be implemented in $O(n \log n)$ or $O(n)$ time.

---

## Complexity Analysis
- **Tree to Prufer Sequence**: $O(N \log N)$ using a priority queue, or $O(N)$ with a single-pass pointer (linear scan) optimization. We present the standard $O(N \log N)$ algorithm using a priority queue as it is highly intuitive and extremely robust.
- **Prufer Sequence to Tree**: $O(N \log N)$ using a priority queue, or $O(N)$ with linear scan.
- **Space Complexity**: $O(N)$ auxiliary space for degree tracking and graph representation.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

// Encodes a tree (represented as adjacency list) into a Prufer sequence in O(N log N)
std::vector<int> tree_to_prufer(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<int> degree(n + 1, 0);
    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;
    std::vector<bool> killed(n + 1, false);

    for (int i = 1; i <= n; ++i) {
        degree[i] = adj[i].size();
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }

    std::vector<int> code;
    for (int iter = 0; iter < n - 2; ++iter) {
        int leaf = leaves.top();
        leaves.pop();
        killed[leaf] = true;

        int neighbor = -1;
        for (int v : adj[leaf]) {
            if (!killed[v]) {
                neighbor = v;
                break;
            }
        }

        code.push_back(neighbor);
        degree[neighbor]--;
        if (degree[neighbor] == 1) {
            leaves.push(neighbor);
        }
    }
    return code;
}

// Decodes a Prufer sequence into a tree (returns list of edges) in O(N log N)
std::vector<std::pair<int, int>> prufer_to_tree(int n, const std::vector<int>& code) {
    std::vector<int> degree(n + 1, 1);
    for (int x : code) {
        degree[x]++;
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }

    std::vector<std::pair<int, int>> edges;
    for (int x : code) {
        int leaf = leaves.top();
        leaves.pop();
        
        edges.push_back({leaf, x});
        
        degree[leaf]--;
        degree[x]--;
        if (degree[x] == 1) {
            leaves.push(x);
        }
    }

    // Connect the final two remaining nodes
    int u = -1, v = -1;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] == 1) {
            if (u == -1) u = i;
            else v = i;
        }
    }
    edges.push_back({u, v});
    return edges;
}
```

---

## Python Implementation

```python
import heapq
from typing import List, Tuple


def tree_to_prufer(n: int, adj: List[List[int]]) -> List[int]:
    """Encodes a labeled tree (1-indexed) into its Prufer sequence in O(N log N)."""
    degree = [0] * (n + 1)
    killed = [False] * (n + 1)
    leaves = []

    for i in range(1, n + 1):
        degree[i] = len(adj[i])
        if degree[i] == 1:
            heapq.heappush(leaves, i)

    code = []
    for _ in range(n - 2):
        leaf = heapq.heappop(leaves)
        killed[leaf] = True

        neighbor = -1
        for v in adj[leaf]:
            if not killed[v]:
                neighbor = v
                break

        code.append(neighbor)
        degree[neighbor] -= 1
        if degree[neighbor] == 1:
            heapq.heappush(leaves, neighbor)

    return code


def prufer_to_tree(n: int, code: List[int]) -> List[Tuple[int, int]]:
    """Decodes a Prufer sequence into a set of edges (1-indexed) in O(N log N)."""
    degree = [1] * (n + 1)
    for x in code:
        degree[x] += 1

    leaves = []
    for i in range(1, n + 1):
        if degree[i] == 1:
            heapq.heappush(leaves, i)

    edges = []
    for x in code:
        leaf = heapq.heappop(leaves)
        edges.append((leaf, x))

        degree[leaf] -= 1
        degree[x] -= 1
        if degree[x] == 1:
            heapq.heappush(leaves, x)

    # Find the last two remaining vertices with degree 1
    rem = [i for i in range(1, n + 1) if degree[i] == 1]
    edges.append((rem[0], rem[1]))
    return edges
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // Let's create a tree of size 5:
    // 1 - 2 - 3
    //     |
    //     4 - 5
    // Neighbors:
    // 1: {2}
    // 2: {1, 3, 4}
    // 3: {2}
    // 4: {2, 5}
    // 5: {4}
    int n = 5;
    std::vector<std::vector<int>> adj(n + 1);
    adj[1] = {2};
    adj[2] = {1, 3, 4};
    adj[3] = {2};
    adj[4] = {2, 5};
    adj[5] = {4};

    // Tree to Prufer sequence
    std::vector<int> code = tree_to_prufer(n, adj);
    std::cout << "Prufer Sequence for the tree: ";
    for (int x : code) {
        std::cout << x << " ";
    }
    std::cout << "\n(Expected sequence: 2 4 2)\n\n";

    // Prufer sequence to tree
    std::vector<std::pair<int, int>> edges = prufer_to_tree(n, code);
    std::cout << "Reconstructed Tree Edges:\n";
    for (auto edge : edges) {
        std::cout << edge.first << " - " << edge.second << "\n";
    }

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # Create tree of size 5:
    # 1 - 2 - 3
    #     |
    #     4 - 5
    n = 5
    adj = [[], [2], [1, 3, 4], [2], [2, 5], [4]]

    code = tree_to_prufer(n, adj)
    print(f"Prufer Sequence for the tree: {code}")
    print("Expected: [2, 4, 2]\n")

    edges = prufer_to_tree(n, code)
    print("Reconstructed Tree Edges:")
    for u, v in edges:
        print(f"{u} - {v}")
```
