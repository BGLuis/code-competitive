# Dijkstra's Algorithm

## Conceptual Explanation

**Dijkstra's Algorithm** is a greedy algorithm that finds the shortest path from a single source vertex to all other vertices in a weighted graph with **non-negative edge weights**.

### Core Principle

Dijkstra's algorithm maintains a set of visited vertices and an array `dist` where `dist[u]` represents the currently known shortest distance from the source to vertex `u`.

1. **Initialization**: Set `dist[source] = 0` and `dist[u] = INF` for all other vertices $u$. Push the pair `(0, source)` into a priority queue (min-heap).
2. **Greedy Selection**: At each step, pop the vertex `u` with the smallest `dist[u]` from the priority queue. If `u` has already been visited (or if the popped distance is greater than the recorded `dist[u]`), skip it.
3. **Edge Relaxation**: For each neighbor `v` of `u` connected by an edge of weight $w$, check if a shorter path to `v` exists through `u`:
   $$\text{if } \text{dist}[u] + w < \text{dist}[v] \implies \text{dist}[v] = \text{dist}[u] + w$$
   If the condition is met, update `dist[v]` and push `(dist[v], v)` into the priority queue.
4. Repeat until the priority queue is empty.

### Why Non-Negative Weights Only?

If negative edge weights exist, the greedy choice of popping the minimum distance element and marking it "visited/finalized" no longer holds. A path with a negative edge could later reduce the distance of an already finalized node, leading to incorrect results. For negative weights, the Bellman-Ford algorithm must be used.

### Typical Competitive Programming Use Cases

- Finding the shortest path in a directed or undirected graph with positive weights.
- State-space shortest path (e.g., finding the shortest path on a 2D grid with certain conditions, where the state space consists of `(row, col, state_vars)`).
- Path optimization problems that can be modeled as graphs.

---

## Complexity Analysis

- **Time Complexity**:
  - **Using a Binary Heap / Priority Queue**: $\mathcal{O}((V + E) \log V)$, where $V$ is the number of vertices and $E$ is the number of edges. We perform $V$ extract-min operations and at most $E$ decrease-key operations (pushes to priority queue), each taking $\mathcal{O}(\log V)$ time.
  - **Using Fibonacci Heap (theoretical)**: $\mathcal{O}(E + V \log V)$, but rarely used due to large constant factors.
- **Space Complexity**:
  - $\mathcal{O}(V + E)$ to store the graph (adjacency list).
  - $\mathcal{O}(V)$ for the distance array and priority queue.

---

## C++ Implementation

Below is a highly optimized C++ implementation using `std::priority_queue` with standard min-heap syntax.

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <limits>

struct Edge {
    int to;
    long long weight;
};

class Dijkstra {
private:
    int num_vertices;
    std::vector<std::vector<Edge>> adj;
    const long long INF = std::numeric_limits<long long>::max();

public:
    explicit Dijkstra(int vertices) : num_vertices(vertices), adj(vertices) {}

    void add_edge(int u, int v, long long weight, bool directed = true) {
        adj[u].push_back({v, weight});
        if (!directed) {
            adj[v].push_back({u, weight});
        }
    }

    // Computes shortest paths from source. Returns the distance array.
    std::vector<long long> find_shortest_paths(int source) {
        std::vector<long long> dist(num_vertices, INF);
        // Min-heap storing pairs of (distance, vertex)
        std::priority_queue<std::pair<long long, int>, 
                            std::vector<std::pair<long long, int>>, 
                            std::greater<std::pair<long long, int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            // If we found a shorter path to u already, skip processing
            if (d > dist[u]) {
                continue;
            }

            // Relax all outgoing edges from u
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                long long weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation using the standard `heapq` module.

```python
from typing import List, Tuple
import heapq
import sys

class Dijkstra:
    INF = sys.maxsize

    def __init__(self, vertices: int):
        self.num_vertices = vertices
        # Adjacency list storing tuples of (to_vertex, weight)
        self.adj: List[List[Tuple[int, int]]] = [[] for _ in range(vertices)]

    def add_edge(self, u: int, v: int, weight: int, directed: bool = True) -> None:
        self.adj[u].append((v, weight))
        if not directed:
            self.adj[v].append((u, weight))

    def find_shortest_paths(self, source: int) -> List[int]:
        """
        Computes the shortest paths from source using Dijkstra's algorithm.
        """
        dist = [self.INF] * self.num_vertices
        dist[source] = 0

        # Min-heap storing (distance, vertex)
        pq: List[Tuple[int, int]] = [(0, source)]

        while pq:
            d, u = heapq.heappop(pq)

            # Skip outdated entries
            if d > dist[u]:
                continue

            # Relax neighbors
            for v, weight in self.adj[u]:
                if dist[u] + weight < dist[v]:
                    dist[v] = dist[u] + weight
                    heapq.heappush(pq, (dist[v], v))

        return dist
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    int vertices = 6;
    Dijkstra solver(vertices);

    // Undirected graph setup
    solver.add_edge(0, 1, 4, false);
    solver.add_edge(0, 2, 2, false);
    solver.add_edge(1, 2, 1, false);
    solver.add_edge(1, 3, 5, false);
    solver.add_edge(2, 3, 8, false);
    solver.add_edge(2, 4, 10, false);
    solver.add_edge(3, 4, 2, false);
    solver.add_edge(3, 5, 6, false);
    solver.add_edge(4, 5, 3, false);

    std::vector<long long> dist = solver.find_shortest_paths(0);

    std::cout << "Shortest distances from source vertex 0:\n";
    for (int i = 0; i < vertices; ++i) {
        std::cout << "To vertex " << i << " : " << dist[i] << "\n";
    }

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    vertices = 6
    solver = Dijkstra(vertices)

    # Undirected graph setup
    solver.add_edge(0, 1, 4, directed=False)
    solver.add_edge(0, 2, 2, directed=False)
    solver.add_edge(1, 2, 1, directed=False)
    solver.add_edge(1, 3, 5, directed=False)
    solver.add_edge(2, 3, 8, directed=False)
    solver.add_edge(2, 4, 10, directed=False)
    solver.add_edge(3, 4, 2, directed=False)
    solver.add_edge(3, 5, 6, directed=False)
    solver.add_edge(4, 5, 3, directed=False)

    dist = solver.find_shortest_paths(0)

    print("Shortest distances from source vertex 0:")
    for i in range(vertices):
        print(f"To vertex {i} : {dist[i]}")
```
