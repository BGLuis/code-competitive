# Planar Graph Duality (Min-Cut to Shortest Path)

## Conceptual Explanation
A graph is **planar** if it can be drawn on a 2D plane such that no two edges cross. For any planar graph $G = (V, E)$, there exists a **dual graph** $G^* = (V^*, E^*)$ defined as:
1. Every face (including the outer unbounded region) in $G$ corresponds to a vertex in $G^*$.
2. For every edge $e \in E$ separating two faces $f_1$ and $f_2$, there is an edge $e^* \in E^*$ connecting the vertices in $G^*$ corresponding to $f_1$ and $f_2$.

### The Planar Min-Cut/Shortest-Path Equivalence
A classic, highly powerful application of graph duality is computing the **Minimum $s$-$t$ Cut** in a planar graph.
In general graphs, Min-Cut is solved using Max-Flow algorithms (like Dinic's in $O(V^2 E)$). However, in a **planar graph**, the minimum cut that separates two specified vertices $s$ and $t$ corresponds exactly to a **shortest path** in the dual graph!

#### Grid Graph Example
Consider an $(N-1) \times (M-1)$ grid of squares. This planar graph has:
- Vertices at grid intersections $(x, y)$ for $x \in [0, N-1], y \in [0, M-1]$.
- Horizontal and vertical edges with positive capacity weights.
- Source $s$ at the top-left corner $(0, 0)$ and Sink $t$ at the bottom-right corner $(N-1, M-1)$.

To find the minimum capacity of edges to cut to separate $s$ and $t$:
1. We construct a dual graph where each cell/square in the grid is a dual vertex.
2. The outer unbounded region is split into two dual vertices representing the regions "above/left" and "below/right" of the grid. Let's call these $S^*$ and $T^*$.
3. An edge of capacity $C$ in the original grid translates to a dual edge of weight $C$ crossing it.
4. The Min $s$-$t$ Cut is exactly the **Shortest Path** from $S^*$ to $T^*$ in the dual graph, which we can solve extremely efficiently using **Dijkstra's Algorithm**.

---

## Complexity Analysis
- **Dual Graph Construction**: $O(V_{grid})$ where $V_{grid} = N \times M$.
- **Shortest Path Computation**: Using Dijkstra with a binary heap, the time complexity is $O(E^* \log V^*) = O((N \cdot M) \log(N \cdot M))$. This is significantly faster than standard max-flow algorithms which would take $O(V^2 E) = O((N \cdot M)^3)$.
- **Space Complexity**: $O(N \cdot M)$ to store the dual graph.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

// Solve shortest path using Dijkstra
long long dijkstra(int start, int end, const std::vector<std::vector<Edge>>& adj) {
    int n = adj.size();
    std::vector<long long> dist(n, INF);
    using pii = std::pair<long long, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == end) return d;

        for (const auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }
    return dist[end];
}

// Computes minimum s-t cut in a grid graph of size N x M (0-indexed nodes)
// s = (0,0), t = (N-1, M-1).
// horizontal_cap[i][j] is capacity of edge between (i, j) and (i, j+1). Size: N x (M-1)
// vertical_cap[i][j] is capacity of edge between (i, j) and (i+1, j). Size: (N-1) x M
long long planar_grid_min_cut(
    int N, int M,
    const std::vector<std::vector<long long>>& horizontal_cap,
    const std::vector<std::vector<long long>>& vertical_cap
) {
    // Number of cells (faces) inside the grid = (N-1) * (M-1)
    // We also need two extra nodes: S* (representing top-right boundary) and T* (representing bottom-left boundary)
    int num_cells = (N - 1) * (M - 1);
    int S_star = num_cells;
    int T_star = num_cells + 1;

    std::vector<std::vector<Edge>> adj(num_cells + 2);

    // Helper to get dual vertex ID for cell (r, c) where r in [0, N-2], c in [0, M-2]
    auto get_cell_id = [&](int r, int c) {
        return r * (M - 1) + c;
    };

    // 1. Add horizontal original edges -> vertical dual edges
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M - 1; ++j) {
            long long cap = horizontal_cap[i][j];
            int u, v;
            if (i == 0) {
                // Top border: connects S* to cell (0, j)
                u = S_star;
                v = get_cell_id(0, j);
            } else if (i == N - 1) {
                // Bottom border: connects cell (N-2, j) to T*
                u = get_cell_id(N - 2, j);
                v = T_star;
            } else {
                // Interior border: connects cell (i-1, j) to cell (i, j)
                u = get_cell_id(i - 1, j);
                v = get_cell_id(i, j);
            }
            adj[u].push_back({v, cap});
            adj[v].push_back({u, cap});
        }
    }

    // 2. Add vertical original edges -> horizontal dual edges
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < M; ++j) {
            long long cap = vertical_cap[i][j];
            int u, v;
            if (j == 0) {
                // Left border: connects cell (i, 0) to T*
                u = get_cell_id(i, 0);
                v = T_star;
            } else if (j == M - 1) {
                // Right border: connects S* to cell (i, M-2)
                u = S_star;
                v = get_cell_id(i, M - 2);
            } else {
                // Interior border: connects cell (i, j-1) to cell (i, j)
                u = get_cell_id(i, j - 1);
                v = get_cell_id(i, j);
            }
            adj[u].push_back({v, cap});
            adj[v].push_back({u, cap});
        }
    }

    return dijkstra(S_star, T_star, adj);
}
```

---

## Python Implementation

```python
import heapq
from typing import List, Tuple

INF = 10**18


def dijkstra(
    start: int, end: int, adj: List[List[Tuple[int, int]]]
) -> int:
    """Standard Dijkstra's shortest path algorithm."""
    n = len(adj)
    dist = [INF] * n
    dist[start] = 0
    pq = [(0, start)]  # (distance, node)

    while pq:
        d, u = heapq.heappop(pq)

        if d > dist[u]:
            continue
        if u == end:
            return d

        for v, weight in adj[u]:
            if dist[u] + weight < dist[v]:
                dist[v] = dist[u] + weight
                heapq.heappush(pq, (dist[v], v))

    return dist[end]


def planar_grid_min_cut(
    N: int,
    M: int,
    horizontal_cap: List[List[int]],
    vertical_cap: List[List[int]],
) -> int:
    """Finds minimum s-t cut on a planar grid using graph duality."""
    num_cells = (N - 1) * (M - 1)
    S_star = num_cells
    T_star = num_cells + 1

    adj: List[List[Tuple[int, int]]] = [[] for _ in range(num_cells + 2)]

    def get_cell_id(r: int, c: int) -> int:
        return r * (M - 1) + c

    # 1. Map horizontal grid edges to vertical dual edges
    for i in range(N):
        for j in range(M - 1):
            cap = horizontal_cap[i][j]
            if i == 0:
                u, v = S_star, get_cell_id(0, j)
            elif i == N - 1:
                u, v = get_cell_id(N - 2, j), T_star
            else:
                u, v = get_cell_id(i - 1, j), get_cell_id(i, j)
            adj[u].append((v, cap))
            adj[v].append((u, cap))

    # 2. Map vertical grid edges to horizontal dual edges
    for i in range(N - 1):
        for j in range(M):
            cap = vertical_cap[i][j]
            if j == 0:
                u, v = get_cell_id(i, 0), T_star
            elif j == M - 1:
                u, v = S_star, get_cell_id(i, M - 2)
            else:
                u, v = get_cell_id(i, j - 1), get_cell_id(i, j)
            adj[u].append((v, cap))
            adj[v].append((u, cap))

    return dijkstra(S_star, T_star, adj)
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 3 x 3 Grid: Node(0,0) is S, Node(2,2) is T
    // Vertices:
    // (0,0) - (0,1) - (0,2)
    //   |       |       |
    // (1,0) - (1,1) - (1,2)
    //   |       |       |
    // (2,0) - (2,1) - (2,2)

    int N = 3, M = 3;

    // Horizontal edges capacities: N rows, each has M-1 horizontal links
    std::vector<std::vector<long long>> horizontal = {
        {10, 5},  // Row 0
        {2,  3},  // Row 1
        {4,  8}   // Row 2
    };

    // Vertical edges capacities: N-1 row transitions, each has M links
    std::vector<std::vector<long long>> vertical = {
        {15, 3, 2}, // Row 0 -> Row 1
        {1,  8, 9}  // Row 1 -> Row 2
    };

    long long min_cut = planar_grid_min_cut(N, M, horizontal, vertical);
    std::cout << "Minimum Planar Grid Cut: " << min_cut << " (expected 9)\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 3 x 3 Grid
    N, M = 3, 3

    horizontal = [
        [10, 5],  # Row 0
        [2, 3],  # Row 1
        [4, 8],  # Row 2
    ]

    vertical = [
        [15, 3, 2],  # Row 0 -> Row 1
        [1, 8, 9],  # Row 1 -> Row 2
    ]

    min_cut = planar_grid_min_cut(N, M, horizontal, vertical)
    print(f"Minimum Planar Grid Cut: {min_cut} (expected 9)")
```
