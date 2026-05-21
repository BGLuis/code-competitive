# Floyd-Warshall Algorithm

## Conceptual Explanation

The **Floyd-Warshall algorithm** is a dynamic programming algorithm that finds the shortest paths between **all pairs** of vertices in a weighted graph. It can handle directed or undirected graphs with positive or negative edge weights, provided there are no negative cycles.

### Core Principle: Dynamic Programming

Let $dp[k][i][j]$ be the shortest distance from vertex $i$ to vertex $j$ using only the vertices in $\{0, 1, 2, \dots, k\}$ as intermediate steps.

#### Transition Relation:
To find the value of $dp[k][i][j]$, we have two options:
1. **Exclude vertex $k$**: The shortest path is just $dp[k-1][i][j]$.
2. **Include vertex $k$**: The path goes from $i$ to $k$, then from $k$ to $j$. The shortest distance is $dp[k-1][i][k] + dp[k-1][k][j]$.

Thus, the recurrence relation is:
$$dp[k][i][j] = \min(dp[k-1][i][j], dp[k-1][i][k] + dp[k-1][k][j])$$

#### Space Optimization:
Since the state $dp[k]$ only depends on states from $dp[k-1]$, we can drop the first dimension and compute everything in-place in a 2D matrix:
$$dist[i][j] = \min(dist[i][j], dist[i][k] + dist[k][j])$$
This optimization works because the values of $dist[i][k]$ and $dist[k][j]$ do not change when updating the $k$-th iteration.

### Negative Cycle Detection

If there is a negative cycle reachable from any vertex $i$, the distance from $i$ to itself ($dist[i][i]$) will become negative after running the algorithm. We can simply check the diagonal entries $dist[i][i] < 0$ to identify negative cycle presence.

### Path Reconstruction

To reconstruct the shortest path between $i$ and $j$, we maintain a `next_node` table where `next_node[i][j]` stores the first step on the shortest path from $i$ to $j$.
When relaxing an edge:
$$\text{if } dist[i][k] + dist[k][j] < dist[i][j] \implies \text{next\_node}[i][j] = \text{next\_node}[i][k]$$

---

## Complexity Analysis

- **Time Complexity**: $\mathcal{O}(V^3)$ because of the three nested loops (for intermediate $k$, source $i$, and destination $j$).
- **Space Complexity**: $\mathcal{O}(V^2)$ to store the 2D distance and path reconstruction tables.

---

## C++ Implementation

Below is a robust C++ implementation of Floyd-Warshall with path reconstruction and negative cycle detection.

```cpp
#include <iostream>
#include <vector>
#include <limits>

class FloydWarshall {
private:
    int num_vertices;
    std::vector<std::vector<long long>> dist;
    std::vector<std::vector<int>> next_node;
    const long long INF = std::numeric_limits<long long>::max() / 2; // Prevent overflow during addition

public:
    explicit FloydWarshall(int vertices) : num_vertices(vertices) {
        dist.assign(num_vertices, std::vector<long long>(num_vertices, INF));
        next_node.assign(num_vertices, std::vector<int>(num_vertices, -1));

        for (int i = 0; i < num_vertices; ++i) {
            dist[i][i] = 0;
            next_node[i][i] = i;
        }
    }

    void add_edge(int u, int v, long long weight, bool directed = true) {
        dist[u][v] = std::min(dist[u][v], weight);
        next_node[u][v] = v;
        if (!directed) {
            dist[v][u] = std::min(dist[v][u], weight);
            next_node[v][u] = u;
        }
    }

    // Computes all pairs shortest paths.
    // Returns false if a negative cycle is detected, true otherwise.
    bool compute() {
        for (int k = 0; k < num_vertices; ++k) {
            for (int i = 0; i < num_vertices; ++i) {
                for (int j = 0; j < num_vertices; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next_node[i][j] = next_node[i][k];
                        }
                    }
                }
            }
        }

        // Check diagonal for negative cycles
        for (int i = 0; i < num_vertices; ++i) {
            if (dist[i][i] < 0) {
                return false; // Negative cycle detected
            }
        }
        return true;
    }

    long long get_distance(int u, int v) const {
        return dist[u][v];
    }

    // Reconstructs the shortest path from u to v
    std::vector<int> reconstruct_path(int u, int v) const {
        if (dist[u][v] >= INF) {
            return {}; // No path exists
        }
        std::vector<int> path;
        int curr = u;
        while (curr != v) {
            if (curr == -1) return {};
            path.push_back(curr);
            curr = next_node[curr][v];
        }
        path.push_back(v);
        return path;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with complete type hints.

```python
from typing import List, Tuple
import sys

class FloydWarshall:
    INF = sys.maxsize // 2  # Safe value to prevent overflow additions

    def __init__(self, vertices: int):
        self.num_vertices = vertices
        self.dist = [[self.INF] * vertices for _ in range(vertices)]
        self.next_node = [[-1] * vertices for _ in range(vertices)]

        for i in range(vertices):
            self.dist[i][i] = 0
            self.next_node[i][i] = i

    def add_edge(self, u: int, v: int, weight: int, directed: bool = True) -> None:
        self.dist[u][v] = min(self.dist[u][v], weight)
        self.next_node[u][v] = v
        if not directed:
            self.dist[v][u] = min(self.dist[v][u], weight)
            self.next_node[v][u] = u

    def compute(self) -> bool:
        """
        Computes all pairs shortest paths.
        Returns:
            False if negative cycle detected, True otherwise.
        """
        for k in range(self.num_vertices):
            for i in range(self.num_vertices):
                for j in range(self.num_vertices):
                    if self.dist[i][k] < self.INF and self.dist[k][j] < self.INF:
                        if self.dist[i][k] + self.dist[k][j] < self.dist[i][j]:
                            self.dist[i][j] = self.dist[i][k] + self.dist[k][j]
                            self.next_node[i][j] = self.next_node[i][k]

        # Check for negative cycle
        for i in range(self.num_vertices):
            if self.dist[i][i] < 0:
                return False
        return True

    def get_distance(self, u: int, v: int) -> int:
        return self.dist[u][v]

    def reconstruct_path(self, u: int, v: int) -> List[int]:
        if self.dist[u][v] >= self.INF:
            return []
        path = []
        curr = u
        while curr != v:
            if curr == -1:
                return []
            path.append(curr)
            curr = self.next_node[curr][v]
        path.append(v)
        return path
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    int vertices = 4;
    FloydWarshall fw(vertices);

    // Graph setup
    fw.add_edge(0, 1, 3);
    fw.add_edge(0, 2, 8);
    fw.add_edge(1, 2, 2);
    fw.add_edge(1, 3, 5);
    fw.add_edge(2, 3, 1);

    if (!fw.compute()) {
        std::cout << "Negative cycle detected!\n";
    } else {
        std::cout << "All-Pairs Shortest Path Distances:\n";
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                long long d = fw.get_distance(i, j);
                if (d >= fw.get_distance(0, 3) && d > 1000000) { // Large distance comparison
                    std::cout << "INF ";
                } else {
                    std::cout << d << " ";
                }
            }
            std::cout << "\n";
        }

        std::vector<int> path = fw.reconstruct_path(0, 3);
        std::cout << "\nShortest path from 0 to 3: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
        }
        std::cout << "\n";
    }

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    vertices = 4
    fw = FloydWarshall(vertices)

    # Graph setup
    fw.add_edge(0, 1, 3)
    fw.add_edge(0, 2, 8)
    fw.add_edge(1, 2, 2)
    fw.add_edge(1, 3, 5)
    fw.add_edge(2, 3, 1)

    if not fw.compute():
        print("Negative cycle detected!")
    else:
        print("All-Pairs Shortest Path Distances:")
        for i in range(vertices):
            row = []
            for j in range(vertices):
                d = fw.get_distance(i, j)
                row.append("INF" if d >= FloydWarshall.INF else str(d))
            print(" ".join(row))

        path = fw.reconstruct_path(0, 3)
        print("\nShortest path from 0 to 3:", " -> ".join(map(str, path)))
```
