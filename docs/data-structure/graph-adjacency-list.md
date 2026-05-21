# Graph Adjacency List

## Conceptual Explanation

A **Graph Adjacency List** is a collection of unordered lists used to represent a graph. Each vertex in the graph maintains a list of its neighboring vertices (and optionally, edge weights).

### Core Logic & Operations
- **Representation**: An array or vector of lists/vectors of size $V$ (the number of vertices). For vertex $u$, `adj[u]` stores all adjacent vertices $v$ such that there is an edge $(u, v)$.
- **Directed vs. Undirected**:
  - For **undirected** edges, adding an edge $(u, v)$ means adding $v$ to `adj[u]` and $u$ to `adj[v]`.
  - For **directed** edges, adding an edge $(u, v)$ means only adding $v$ to `adj[u]`.
- **Weighted Edges**: Instead of storing plain integer indices of neighbors, we store pairs of `(neighbor, weight)`.

### Typical Competitive Programming Use Cases
The adjacency list is the **gold standard** for representing graphs in competitive programming because:
- Most CP graphs are **sparse** ($E \ll V^2$). Adjacency lists use $O(V + E)$ space, avoiding the $O(V^2)$ memory bottleneck of adjacency matrices.
- Algorithms like **Dijkstra**, **Prim**, **Kruskal**, **Bellman-Ford**, **DFS**, and **BFS** all require quick traversal of a vertex's outgoing edges, which takes exactly $O(\text{degree}(u))$ time in an adjacency list.

---

## Complexity Analysis

Let $V$ be the number of vertices and $E$ be the number of edges.

| Operation | Adjacency List | Adjacency Matrix |
| :--- | :--- | :--- |
| **Space Complexity** | $O(V + E)$ | $O(V^2)$ |
| **Add Edge** | $O(1)$ | $O(1)$ |
| **Check Edge Existence $(u, v)$** | $O(\text{degree}(u))$ | $O(1)$ |
| **Iterate over Neighbors of $u$** | $O(\text{degree}(u))$ | $O(V)$ |
| **Remove Edge $(u, v)$** | $O(\text{degree}(u))$ | $O(1)$ |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <queue>

// Structure to represent a weighted edge
struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    int num_vertices;
    bool is_directed;
    std::vector<std::vector<Edge>> adj;

public:
    Graph(int vertices, bool directed = false) : num_vertices(vertices), is_directed(directed) {
        adj.resize(num_vertices);
    }

    // Add edge with optional weight (default: 1)
    void addEdge(int u, int v, int weight = 1) {
        adj[u].push_back({v, weight});
        if (!is_directed) {
            adj[v].push_back({u, weight});
        }
    }

    // Return the adjacency list for vertex u
    const std::vector<Edge>& getNeighbors(int u) const {
        return adj[u];
    }

    // Perform Breadth-First Search (BFS) starting from 'start'
    void bfs(int start) const {
        std::vector<bool> visited(num_vertices, false);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);

        std::cout << "BFS Traversal: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            std::cout << u << " ";

            for (const auto& edge : adj[u]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
        std::cout << "\n";
    }

    // Helper for DFS traversal
    void dfsUtil(int u, std::vector<bool>& visited) const {
        visited[u] = true;
        std::cout << u << " ";

        for (const auto& edge : adj[u]) {
            if (!visited[edge.to]) {
                dfsUtil(edge.to, visited);
            }
        }
    }

    // Perform Depth-First Search (DFS) starting from 'start'
    void dfs(int start) const {
        std::vector<bool> visited(num_vertices, false);
        std::cout << "DFS Traversal: ";
        dfsUtil(start, visited);
        std::cout << "\n";
    }
};

int main() {
    // Create an undirected, weighted graph with 5 vertices (0 to 4)
    Graph g(5, false);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(3, 4, 3);

    // Print Adjacency List
    std::cout << "Adjacency List Representation:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "Vertex " << i << ":";
        for (const auto& edge : g.getNeighbors(i)) {
            std::cout << " -> (" << edge.to << ", W:" << edge.weight << ")";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    g.bfs(0);
    g.dfs(0);

    return 0;
}
```

---

## Python Implementation

```python
from collections import deque
from typing import List, Tuple, Dict

class Graph:
    def __init__(self, num_vertices: int, is_directed: bool = False):
        self.num_vertices = num_vertices
        self.is_directed = is_directed
        # Adjacency list storing Tuple[neighbor, weight]
        self.adj: List[List[Tuple[int, int]]] = [[] for _ in range(num_vertices)]

    def add_edge(self, u: int, v: int, weight: int = 1) -> None:
        """Add an edge from vertex u to v with an optional weight."""
        self.adj[u].append((v, weight))
        if not self.is_directed:
            self.adj[v].append((u, weight))

    def get_neighbors(self, u: int) -> List[Tuple[int, int]]:
        """Return the adjacent neighbors of vertex u."""
        return self.adj[u]

    def bfs(self, start: int) -> None:
        """Perform Breadth-First Search (BFS) from start vertex."""
        visited = [False] * self.num_vertices
        visited[start] = True
        queue = deque([start])

        bfs_order = []
        while queue:
            u = queue.popleft()
            bfs_order.append(u)

            for neighbor, _ in self.adj[u]:
                if not visited[neighbor]:
                    visited[neighbor] = True
                    queue.append(neighbor)

        print("BFS Traversal:", " ".join(map(str, bfs_order)))

    def dfs(self, start: int) -> None:
        """Perform Depth-First Search (DFS) from start vertex."""
        visited = [False] * self.num_vertices
        dfs_order = []

        def _dfs_util(u: int):
            visited[u] = True
            dfs_order.append(u)
            for neighbor, _ in self.adj[u]:
                if not visited[neighbor]:
                    _dfs_util(neighbor)

        _dfs_util(start)
        print("DFS Traversal:", " ".join(map(str, dfs_order)))


if __name__ == '__main__':
    # Create an undirected graph with 5 vertices (0 to 4)
    g = Graph(5, is_directed=False)

    g.add_edge(0, 1, 4)
    g.add_edge(0, 2, 2)
    g.add_edge(1, 2, 1)
    g.add_edge(1, 3, 5)
    g.add_edge(2, 3, 8)
    g.add_edge(3, 4, 3)

    # Print representation
    print("Adjacency List Representation:")
    for i in range(5):
        neighbors_str = "".join([f" -> ({v}, W:{w})" for v, w in g.get_neighbors(i)])
        print(f"Vertex {i}:{neighbors_str}")
    print()

    g.bfs(0)
    g.dfs(0)
```
