# Dual Graph

## Conceptual Explanation

In graph theory, the **Dual Graph** of a planar graph $G$ is a graph $G^*$ where:
- Each vertex in $G^*$ corresponds to a **face** (a bounded or unbounded region of the plane) of $G$.
- An edge exists between two vertices in $G^*$ if and only if their corresponding faces in $G$ share a common boundary edge.

### Core Logic & Planar Dual Properties
1. **Vertices and Faces**: By Euler's Formula for planar graphs, $V - E + F = 2$, where $V$ is vertices, $E$ is edges, and $F$ is faces (including the single unbounded outer face). Thus, the dual graph has exactly $F$ vertices.
2. **Edges**: Each edge in $G$ separates exactly two faces (possibly the same face on both sides, which creates a self-loop in $G^*$). Therefore, there is a one-to-one correspondence between the edges of $G$ and the edges of $G^*$.
3. **Planar Min-Cut to Dual Shortest Path**: A classic, powerful competitive programming trick is converting a Min-Cut problem on a planar $s$-$t$ graph into a Shortest Path problem on its Dual Graph. By adding an edge from $s$ to $t$ to partition the outer face into two, the min $s$-$t$ cut corresponds to the shortest cycle in the dual graph that separates $s$ and $t$.

### Typical Competitive Programming Use Cases
- **Planar Graph Max-Flow/Min-Cut**: When $N, M \le 10^5$, standard max-flow algorithms (like Dinic's $O(V^2 E)$) are too slow. If the graph is planar, we can find the min-cut in $O(N \log N)$ by finding the shortest path in the Dual Graph using Dijkstra's algorithm.
- **Region Connectivity**: Determining if different regions separated by boundaries or lines are connected.

---

## Complexity Analysis

Let $V$ be the number of vertices and $E$ be the number of edges in the planar graph $G$.

| Operation / Property | Planar Graph $G$ | Dual Graph $G^*$ | Complexity |
| :--- | :--- | :--- | :--- |
| **Number of Vertices** | $V$ | $F = E - V + 2$ | $O(V)$ (since $E \le 3V - 6$ for planar graphs) |
| **Number of Edges** | $E$ | $E^* = E$ | $O(E)$ |
| **Dual Construction** | - | - | $O(E \log E)$ or $O(E)$ |
| **Shortest Path on Dual** | - | - | $O(F \log F)$ via Dijkstra |

---

## C++ Implementation

This implementation demonstrates constructing the **Dual Graph** of a 2D grid-based planar graph (where edges represent boundaries, and faces represent cells). This is the most common form of dual graphs in CP.

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

// Structure to represent an edge in the Dual Graph
struct DualEdge {
    int to;
    int weight;
};

class GridDualGraph {
private:
    int rows, cols;
    // Original grid cells are faces. 
    // We have rows * cols bounded faces, and 1 unbounded outer face (index: rows * cols).
    int num_faces;
    int outer_face;
    std::vector<std::vector<DualEdge>> adj;

    // Helper to get face index of a grid cell (r, c)
    int getFaceIndex(int r, int c) const {
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            return outer_face;
        }
        return r * cols + c;
    }

public:
    GridDualGraph(int r, int c) : rows(r), cols(c) {
        num_faces = rows * cols + 1; // cells + 1 outer face
        outer_face = rows * cols;
        adj.resize(num_faces);
    }

    // Add a boundary edge in the original graph separating cell (r1, c1) and cell (r2, c2)
    // The weight represents the cost to "cross" this boundary (cut weight)
    void addBoundary(int r1, int c1, int r2, int c2, int cost) {
        int u = getFaceIndex(r1, c1);
        int v = getFaceIndex(r2, c2);
        adj[u].push_back({v, cost});
        adj[v].push_back({u, cost});
    }

    // Find the shortest path (min cut) from one region to another (Dijkstra)
    int shortestPath(int start_face, int end_face) {
        std::vector<int> dist(num_faces, 1e9);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

        dist[start_face] = 0;
        pq.push({0, start_face});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : adj[u]) {
                if (dist[u] + edge.weight < dist[edge.to]) {
                    dist[edge.to] = dist[u] + edge.weight;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        return dist[end_face];
    }

    int getOuterFace() const {
        return outer_face;
    }
};

int main() {
    // Create a 2x3 grid planar graph
    // 2 rows, 3 columns -> 6 internal cells (faces 0..5) and 1 outer face (face 6)
    GridDualGraph gdg(2, 3);

    // Let's add boundaries with weights
    // Boundary between (0,0) and (0,1) with cost 5
    gdg.addBoundary(0, 0, 0, 1, 5);
    // Boundary between (0,1) and (0,2) with cost 10
    gdg.addBoundary(0, 1, 0, 2, 10);
    // Boundary between (1,0) and (1,1) with cost 3
    gdg.addBoundary(1, 0, 1, 1, 3);
    // Boundary between (1,1) and (1,2) with cost 2
    gdg.addBoundary(1, 1, 1, 2, 2);

    // Vertical boundaries between row 0 and row 1
    gdg.addBoundary(0, 0, 1, 0, 4);
    gdg.addBoundary(0, 1, 1, 1, 8);
    gdg.addBoundary(0, 2, 1, 2, 6);

    // Boundary from cells to the outside world (outer face)
    gdg.addBoundary(0, 0, -1, 0, 10); // top of (0,0)
    gdg.addBoundary(1, 0, 2, 0, 1);   // bottom of (1,0)
    gdg.addBoundary(1, 2, 2, 2, 7);   // bottom of (1,2)

    // Compute shortest path in dual graph between face 0 (cell 0,0) and the outer face
    int min_cut = gdg.shortestPath(0, gdg.getOuterFace());
    std::cout << "Shortest path (Min Cut) from Face 0 to Outer Face: " << min_cut << "\n";

    return 0;
}
```

---

## Python Implementation

```python
import heapq
from typing import List, Tuple

class GridDualGraph:
    def __init__(self, rows: int, cols: int):
        self.rows = rows
        self.cols = cols
        # Grid cells are faces 0 to (rows * cols - 1). 
        # The outside region is the single unbounded outer face.
        self.outer_face = rows * cols
        self.num_faces = self.outer_face + 1
        self.adj: List[List[Tuple[int, int]]] = [[] for _ in range(self.num_faces)]

    def _get_face_index(self, r: int, c: int) -> int:
        if r < 0 or r >= self.rows or c < 0 or c >= self.cols:
            return self.outer_face
        return r * self.cols + c

    def add_boundary(self, r1: int, c1: int, r2: int, c2: int, cost: int) -> None:
        """Add a boundary separating two cells. Weight represents cost to cross/cut it."""
        u = self._get_face_index(r1, c1)
        v = self._get_face_index(r2, c2)
        self.adj[u].append((v, cost))
        self.adj[v].append((u, cost))

    def shortest_path(self, start_face: int, end_face: int) -> int:
        """Find the shortest path (minimum cost) using Dijkstra's algorithm."""
        dists = [float('inf')] * self.num_faces
        dists[start_face] = 0
        pq = [(0, start_face)]

        while pq:
            d, u = heapq.heappop(pq)

            if d > dists[u]:
                continue

            for v, weight in self.adj[u]:
                if dists[u] + weight < dists[v]:
                    dists[v] = dists[u] + weight
                    heapq.heappush(pq, (dists[v], v))

        return int(dists[end_face])


if __name__ == '__main__':
    # Create a 2x3 grid planar graph
    # Faces: 0..5 (internal cells) and 6 (outer face)
    gdg = GridDualGraph(2, 3)

    # Horizontal boundaries
    gdg.add_boundary(0, 0, 0, 1, 5)
    gdg.add_boundary(0, 1, 0, 2, 10)
    gdg.add_boundary(1, 0, 1, 1, 3)
    gdg.add_boundary(1, 1, 1, 2, 2)

    # Vertical boundaries between row 0 and 1
    gdg.add_boundary(0, 0, 1, 0, 4)
    gdg.add_boundary(0, 1, 1, 1, 8)
    gdg.add_boundary(0, 2, 1, 2, 6)

    # Boundary from cells to the outside (outer face)
    gdg.add_boundary(0, 0, -1, 0, 10)  # top boundary
    gdg.add_boundary(1, 0, 2, 0, 1)    # bottom boundary
    gdg.add_boundary(1, 2, 2, 2, 7)    # bottom boundary

    # Shortest path in dual graph between face 0 and the outer face (face 6)
    min_cut = gdg.shortest_path(0, gdg.outer_face)
    print(f"Shortest path (Min Cut) from Face 0 to Outer Face: {min_cut}")
```
