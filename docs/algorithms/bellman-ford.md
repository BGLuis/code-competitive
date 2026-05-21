# Bellman-Ford Algorithm

## Conceptual Explanation

The **Bellman-Ford algorithm** computes the shortest paths from a single source vertex to all other vertices in a weighted digraph. Unlike Dijkstra's algorithm, Bellman-Ford is capable of handling graphs with negative edge weights and can also detect the presence of negative cycles (cycles whose total edge weight sum is negative).

### Core Principle: Relaxation

The algorithm works by repeatedly relaxing edges. To relax an edge $(u, v)$ with weight $w$ means to check if we can improve the shortest path to $v$ by going through $u$:
$$\text{dist}[v] = \min(\text{dist}[v], \text{dist}[u] + w)$$

If the graph contains $V$ vertices, the shortest path without cycles can have at most $V-1$ edges. Therefore, relaxing all edges $V-1$ times guarantees that the shortest distance to all reachable vertices is correctly computed.

### Negative Cycle Detection

If we relax all edges one more time (the $V$-th time) and find any distance that can still be minimized ($\text{dist}[v] > \text{dist}[u] + w$), it implies that a negative cycle exists. Any vertex whose distance is reduced during this $V$-th iteration, or is reachable from such a vertex, is part of or affected by a negative cycle.

### Typical Competitive Programming Use Cases

- Single-source shortest path on graphs containing negative edge weights.
- Finding negative cycles in a directed graph.
- System of Difference Constraints (converting constraints of the form $x_i - x_j \le c_k$ to edges and finding if a feasible solution exists).

---

## Complexity Analysis

- **Time Complexity**:
  - **Overall**: $\mathcal{O}(V \cdot E)$, where $V$ is the number of vertices and $E$ is the number of edges. This is because we relax all $E$ edges $V-1$ times.
  - **With Optimization (SPFA - Shortest Path Faster Algorithm)**: Average complexity is $\mathcal{O}(E)$, but worst-case remains $\mathcal{O}(V \cdot E)$.
- **Space Complexity**:
  - $\mathcal{O}(V)$ auxiliary space to store distance arrays.
  - $\mathcal{O}(E)$ to store the edge list.

---

## C++ Implementation

Here is a robust and clear C++ implementation using an edge list representation. It returns a boolean indicating whether a negative cycle is present, and updates the distances.

```cpp
#include <iostream>
#include <vector>
#include <limits>

struct Edge {
    int u, v;
    long long weight;
};

class BellmanFord {
private:
    int num_vertices;
    std::vector<Edge> edges;
    const long long INF = std::numeric_limits<long long>::max();

public:
    explicit BellmanFord(int vertices) : num_vertices(vertices) {}

    void add_edge(int u, int v, long long weight) {
        edges.push_back({u, v, weight});
    }

    // Returns true if a negative cycle is detected, false otherwise.
    // dist will contain the shortest distances from source.
    bool find_shortest_paths(int source, std::vector<long long>& dist) {
        dist.assign(num_vertices, INF);
        dist[source] = 0;

        // Relax all edges V - 1 times
        for (int i = 0; i < num_vertices - 1; ++i) {
            for (const auto& edge : edges) {
                if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                    dist[edge.v] = dist[edge.u] + edge.weight;
                }
            }
        }

        // V-th relaxation to check for negative cycles
        bool has_negative_cycle = false;
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                has_negative_cycle = true;
                break;
            }
        }

        return has_negative_cycle;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation using complete type hints.

```python
from typing import List, Tuple, Optional
import sys

class BellmanFord:
    INF = sys.maxsize

    def __init__(self, vertices: int):
        self.num_vertices = vertices
        self.edges: List[Tuple[int, int, int]] = []  # List of (u, v, weight)

    def add_edge(self, u: int, v: int, weight: int) -> None:
        self.edges.append((u, v, weight))

    def find_shortest_paths(self, source: int) -> Tuple[bool, List[int]]:
        """
        Computes the shortest paths from source.
        Returns:
            A tuple (has_negative_cycle, distances)
        """
        dist = [self.INF] * self.num_vertices
        dist[source] = 0

        # Relax all edges V - 1 times
        for _ in range(self.num_vertices - 1):
            for u, v, weight in self.edges:
                if dist[u] != self.INF and dist[u] + weight < dist[v]:
                    dist[v] = dist[u] + weight

        # Check for negative cycle (V-th iteration)
        has_negative_cycle = False
        for u, v, weight in self.edges:
            if dist[u] != self.INF and dist[u] + weight < dist[v]:
                has_negative_cycle = True
                break

        return has_negative_cycle, dist
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    int vertices = 5;
    BellmanFord bf(vertices);

    // Add edges
    bf.add_edge(0, 1, -1);
    bf.add_edge(0, 2, 4);
    bf.add_edge(1, 2, 3);
    bf.add_edge(1, 3, 2);
    bf.add_edge(1, 4, 2);
    bf.add_edge(3, 2, 5);
    bf.add_edge(3, 1, 1);
    bf.add_edge(4, 3, -3);

    std::vector<long long> dist;
    bool has_neg_cycle = bf.find_shortest_paths(0, dist);

    if (has_neg_cycle) {
        std::cout << "Negative cycle detected in the graph!\n";
    } else {
        std::cout << "Shortest distances from source vertex 0:\n";
        for (int i = 0; i < vertices; ++i) {
            std::cout << "To vertex " << i << " : ";
            if (dist[i] == std::numeric_limits<long long>::max()) {
                std::cout << "INF\n";
            } else {
                std::cout << dist[i] << "\n";
            }
        }
    }

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    vertices = 5
    bf = BellmanFord(vertices)

    # Add edges
    bf.add_edge(0, 1, -1)
    bf.add_edge(0, 2, 4)
    bf.add_edge(1, 2, 3)
    bf.add_edge(1, 3, 2)
    bf.add_edge(1, 4, 2)
    bf.add_edge(3, 2, 5)
    bf.add_edge(3, 1, 1)
    bf.add_edge(4, 3, -3)

    has_neg_cycle, dist = bf.find_shortest_paths(0)

    if has_neg_cycle:
        print("Negative cycle detected in the graph!")
    else:
        print("Shortest distances from source vertex 0:")
        for i in range(vertices):
            d_str = "INF" if dist[i] == BellmanFord.INF else str(dist[i])
            print(f"To vertex {i} : {d_str}")
```
