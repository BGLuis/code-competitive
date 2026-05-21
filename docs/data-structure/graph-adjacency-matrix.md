# Graph Adjacency Matrix

## Conceptual Explanation

A **Graph Adjacency Matrix** is a square 2D array of size $V \times V$ (where $V$ is the number of vertices) used to represent a graph. The value at `matrix[u][v]` indicates whether there is an edge from vertex $u$ to vertex $v$, along with its weight:
- For an **unweighted graph**, `matrix[u][v] = 1` if an edge exists, and `0` otherwise.
- For a **weighted graph**, `matrix[u][v]` holds the edge weight (typically initialized to positive infinity $\infty$ or a special placeholder value like $-1$ or $0$ to indicate no edge).

### Core Logic & Operations
- **Directed vs. Undirected**:
  - In an **undirected graph**, the adjacency matrix is symmetric along its main diagonal (`matrix[u][v] == matrix[v][u]`).
  - In a **directed graph**, the matrix is generally asymmetric.
- **Add / Remove Edge**: Trivial $O(1)$ updates by writing directly to `matrix[u][v]`.
- **Query Neighbor**: Querying whether $v$ is a neighbor of $u$ takes exactly $O(1)$ time.

### Typical Competitive Programming Use Cases
- **Dense Graphs**: When the number of edges is close to the maximum possible ($E \approx V^2$), the adjacency matrix is space-efficient and has lower constant overhead compared to pointer-heavy adjacency lists.
- **Floyd-Warshall Algorithm**: Finding all-pairs shortest paths in $O(V^3)$ time requires an adjacency matrix.
- **Matrix Exponentiation**: In DP problems involving transitions on graphs, we raise the adjacency matrix to the power of $K$ in $O(V^3 \log K)$ time to find the number of paths of length $K$.

---

## Complexity Analysis

Let $V$ be the number of vertices.

| Operation | Adjacency Matrix | Adjacency List |
| :--- | :--- | :--- |
| **Space Complexity** | $O(V^2)$ | $O(V + E)$ |
| **Add / Remove Edge** | $O(1)$ | $O(1)$ (or $O(\text{degree}(u))$ for deletion) |
| **Check Edge Existence $(u, v)$** | $O(1)$ | $O(\text{degree}(u))$ |
| **Iterate over Neighbors of $u$** | $O(V)$ | $O(\text{degree}(u))$ |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <iomanip>

class GraphMatrix {
private:
    int num_vertices;
    bool is_directed;
    const int INF = 1e9; // Representation of Infinity for no-edge
    std::vector<std::vector<int>> matrix;

public:
    GraphMatrix(int vertices, bool directed = false) : num_vertices(vertices), is_directed(directed) {
        // Initialize the matrix with INF (no edges). Self-loops are initialized to 0.
        matrix.assign(num_vertices, std::vector<int>(num_vertices, INF));
        for (int i = 0; i < num_vertices; ++i) {
            matrix[i][i] = 0;
        }
    }

    // Add an edge with a weight (default: 1)
    void addEdge(int u, int v, int weight = 1) {
        matrix[u][v] = weight;
        if (!is_directed) {
            matrix[v][u] = weight;
        }
    }

    // Remove an edge
    void removeEdge(int u, int v) {
        matrix[u][v] = (u == v) ? 0 : INF;
        if (!is_directed) {
            matrix[v][u] = (u == v) ? 0 : INF;
        }
    }

    // Check if an edge exists
    bool hasEdge(int u, int v) const {
        return u != v && matrix[u][v] != INF;
    }

    // Floyd-Warshall All-Pairs Shortest Path Algorithm
    std::vector<std::vector<int>> floydWarshall() const {
        std::vector<std::vector<int>> dist = matrix;

        for (int k = 0; k < num_vertices; ++k) {
            for (int i = 0; i < num_vertices; ++i) {
                for (int j = 0; j < num_vertices; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF) {
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
        return dist;
    }

    // Print the matrix
    void print() const {
        for (int i = 0; i < num_vertices; ++i) {
            for (int j = 0; j < num_vertices; ++j) {
                if (matrix[i][j] == INF) {
                    std::cout << std::setw(5) << "INF";
                } else {
                    std::cout << std::setw(5) << matrix[i][j];
                }
            }
            std::cout << "\n";
        }
    }
};

int main() {
    // Create an undirected weighted graph with 4 vertices
    GraphMatrix g(4, false);

    g.addEdge(0, 1, 3);
    g.addEdge(0, 3, 7);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 5);

    std::cout << "Initial Adjacency Matrix:\n";
    g.print();
    std::cout << "\n";

    std::cout << "Has edge (0, 2)? " << (g.hasEdge(0, 2) ? "Yes" : "No") << "\n";
    std::cout << "Has edge (1, 3)? " << (g.hasEdge(1, 3) ? "Yes" : "No") << "\n";

    // Run Floyd-Warshall
    std::vector<std::vector<int>> shortest_paths = g.floydWarshall();
    std::cout << "\nAll-Pairs Shortest Paths (Floyd-Warshall):\n";
    for (size_t i = 0; i < shortest_paths.size(); ++i) {
        for (size_t j = 0; j < shortest_paths[i].size(); ++j) {
            std::cout << std::setw(5) << shortest_paths[i][j];
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## Python Implementation

```python
from typing import List

class GraphMatrix:
    def __init__(self, num_vertices: int, is_directed: bool = False):
        self.num_vertices = num_vertices
        self.is_directed = is_directed
        self.INF = float('inf')
        # Initialize a V x V matrix with INF for no-edge, and 0 on the diagonal
        self.matrix: List[List[float]] = [[self.INF] * num_vertices for _ in range(num_vertices)]
        for i in range(num_vertices):
            self.matrix[i][i] = 0.0

    def add_edge(self, u: int, v: int, weight: float = 1.0) -> None:
        """Add an edge from vertex u to v with a weight."""
        self.matrix[u][v] = weight
        if not self.is_directed:
            self.matrix[v][u] = weight

    def remove_edge(self, u: int, v: int) -> None:
        """Remove the edge between vertex u and v."""
        self.matrix[u][v] = 0.0 if u == v else self.INF
        if not self.is_directed:
            self.matrix[v][u] = 0.0 if u == v else self.INF

    def has_edge(self, u: int, v: int) -> bool:
        """Check if an edge exists between vertex u and v."""
        return u != v and self.matrix[u][v] != self.INF

    def floyd_warshall(self) -> List[List[float]]:
        """Run Floyd-Warshall all-pairs shortest paths."""
        dist = [row[:] for row in self.matrix]
        for k in range(self.num_vertices):
            for i in range(self.num_vertices):
                for j in range(self.num_vertices):
                    if dist[i][k] + dist[k][j] < dist[i][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]
        return dist

    def print_matrix(self) -> None:
        """Print the adjacency matrix."""
        for row in self.matrix:
            print(" ".join(f"{x:5}" if x != self.INF else f"{'INF':>5}" for x in row))


if __name__ == '__main__':
    # Create an undirected graph with 4 vertices
    g = GraphMatrix(4, is_directed=False)

    g.add_edge(0, 1, 3)
    g.add_edge(0, 3, 7)
    g.add_edge(1, 2, 1)
    g.add_edge(1, 3, 2)
    g.add_edge(2, 3, 5)

    print("Initial Adjacency Matrix:")
    g.print_matrix()
    print()

    print("Has edge (0, 2)?", "Yes" if g.has_edge(0, 2) else "No")
    print("Has edge (1, 3)?", "Yes" if g.has_edge(1, 3) else "No")

    # Run Floyd-Warshall
    shortest_paths = g.floyd_warshall()
    print("\nAll-Pairs Shortest Paths (Floyd-Warshall):")
    for row in shortest_paths:
        print(" ".join(f"{x:5.0f}" for x in row))
```
