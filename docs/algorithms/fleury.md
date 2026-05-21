# Fleury's Algorithm

## Conceptual Explanation

**Fleury's Algorithm** is a classical method to find an Eulerian Path or Eulerian Circuit in an undirected graph. While Hierholzer's algorithm is generally preferred in competitive programming due to its linear time complexity, Fleury's algorithm is highly intuitive and historically significant.

### Core Principle

The fundamental rule of Fleury's algorithm is simple: **Do not burn your bridges!**

When constructing the Eulerian path, we start from an appropriate vertex and greedily traverse any available edge. However, we must follow a strict constraint: **Never choose a bridge (cut-edge) unless there is no other option.**

- A **bridge** is an edge whose removal increases the number of connected components in the graph.
- If we traverse a bridge, we partition the remaining graph. If there are still edges left on the side we left behind, we will never be able to return to visit them (since each edge is visited exactly once), making an Eulerian path impossible.

### Algorithm Steps

1. **Verify Existence**: Ensure the undirected graph satisfies the Eulerian path or circuit existence criteria:
   - For a circuit: All vertices have even degrees.
   - For a path: Exactly zero or two vertices have odd degrees.
2. **Choose Start Node**:
   - Start at any vertex if all vertices have even degrees.
   - Start at one of the odd-degree vertices if there are exactly two odd-degree vertices.
3. **Iterative Traversal**:
   - From the current vertex $u$, pick the next edge $(u, v)$ such that:
     - Edge $(u, v)$ is NOT a bridge in the *remaining* graph.
     - OR, if $(u, v)$ is a bridge, it is the only remaining outgoing edge from $u$.
   - Traverse the edge $(u, v)$, print/store it, remove $(u, v)$ from the graph, and set the current vertex to $v$.
   - Repeat until all edges are removed.

---

## Complexity Analysis

- **Time Complexity**: $\mathcal{O}(E^2)$, where $E$ is the number of edges. At each step, we have to determine if an edge is a bridge. A standard bridge test (DFS) takes $\mathcal{O}(V + E)$. Since we repeat this for all $E$ edges, the total time is $\mathcal{O}(E \cdot (V + E)) = \mathcal{O}(E^2)$ on connected graphs where $E \ge V$.
- **Space Complexity**: $\mathcal{O}(V + E)$ to represent the adjacency structure and run the DFS for bridge checking.

---

## C++ Implementation

Below is a robust and complete C++ class implementing Fleury's algorithm. It uses a simple DFS-based reachability check to determine if an edge is a bridge.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class FleuryEulerian {
private:
    int num_vertices;
    std::vector<std::vector<int>> adj;

    // Helper DFS to count reachable nodes
    int count_reachable(int u, std::vector<bool>& visited) const {
        visited[u] = true;
        int count = 1;
        for (int v : adj[u]) {
            if (v != -1 && !visited[v]) { // v != -1 denotes an active edge
                count += count_reachable(v, visited);
            }
        }
        return count;
    }

    // Checks if the edge (u, v) is a bridge in the current graph state
    bool is_valid_next_edge(int u, int v) {
        // Count active edges from u
        int active_edges = 0;
        for (int neighbor : adj[u]) {
            if (neighbor != -1) active_edges++;
        }

        // If it's the only edge left, we must take it
        if (active_edges == 1) {
            return true;
        }

        // Otherwise, compare reachability before and after removing the edge (u, v)
        std::vector<bool> visited(num_vertices, false);
        int count1 = count_reachable(u, visited);

        // Temporarily remove edge (u, v)
        remove_edge(u, v);

        std::fill(visited.begin(), visited.end(), false);
        int count2 = count_reachable(u, visited);

        // Restore edge (u, v)
        add_edge(u, v);

        // If reachability decreased, (u, v) is a bridge
        return (count1 <= count2);
    }

    void remove_edge(int u, int v) {
        auto it1 = std::find(adj[u].begin(), adj[u].end(), v);
        if (it1 != adj[u].end()) *it1 = -1; // Mark as deleted

        auto it2 = std::find(adj[v].begin(), adj[v].end(), u);
        if (it2 != adj[v].end()) *it2 = -1; // Mark as deleted
    }

    void dfs_euler(int u, std::vector<int>& path) {
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v != -1 && is_valid_next_edge(u, v)) {
                path.push_back(v);
                remove_edge(u, v);
                dfs_euler(v, path);
                break;
            }
        }
    }

public:
    explicit FleuryEulerian(int vertices) : num_vertices(vertices), adj(vertices) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> find_eulerian_path() {
        // Find starting vertex
        int odd_count = 0;
        int start = 0;
        for (int i = 0; i < num_vertices; ++i) {
            int degree = 0;
            for (int neighbor : adj[i]) {
                if (neighbor != -1) degree++;
            }
            if (degree % 2 != 0) {
                odd_count++;
                start = i; // Start at odd degree vertex if available
            }
        }

        if (odd_count != 0 && odd_count != 2) {
            return {}; // No Eulerian Path exists (degrees are invalid)
        }

        std::vector<int> path;
        path.push_back(start);
        dfs_euler(start, path);

        return path;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List

class FleuryEulerian:
    def __init__(self, vertices: int):
        self.num_vertices = vertices
        # Adjacency list storing neighbors. Deleted edges are set to -1.
        self.adj: List[List[int]] = [[] for _ in range(vertices)]

    def add_edge(self, u: int, v: int) -> None:
        self.adj[u].append(v)
        self.adj[v].append(u)

    def _remove_edge(self, u: int, v: int) -> None:
        if v in self.adj[u]:
            idx = self.adj[u].index(v)
            self.adj[u][idx] = -1
        if u in self.adj[v]:
            idx = self.adj[v].index(u)
            self.adj[v][idx] = -1

    def _add_edge_back(self, u: int, v: int) -> None:
        if -1 in self.adj[u]:
            idx = self.adj[u].index(-1)
            self.adj[u][idx] = v
        else:
            self.adj[u].append(v)
            
        if -1 in self.adj[v]:
            idx = self.adj[v].index(-1)
            self.adj[v][idx] = u
        else:
            self.adj[v].append(u)

    def _count_reachable(self, u: int, visited: List[bool]) -> int:
        visited[u] = True
        count = 1
        for v in self.adj[u]:
            if v != -1 and not visited[v]:
                count += self._count_reachable(v, visited)
        return count

    def _is_valid_next_edge(self, u: int, v: int) -> bool:
        active_edges = sum(1 for neighbor in self.adj[u] if neighbor != -1)
        if active_edges == 1:
            return True

        visited = [False] * self.num_vertices
        count1 = self._count_reachable(u, visited)

        self._remove_edge(u, v)

        visited = [False] * self.num_vertices
        count2 = self._count_reachable(u, visited)

        self._add_edge_back(u, v)

        return count1 <= count2

    def find_eulerian_path(self) -> List[int]:
        odd_count = 0
        start = 0

        for i in range(self.num_vertices):
            degree = sum(1 for neighbor in self.adj[i] if neighbor != -1)
            if degree % 2 != 0:
                odd_count += 1
                start = i

        if odd_count not in (0, 2):
            return []

        path = [start]
        self._dfs_euler(start, path)
        return path

    def _dfs_euler(self, u: int, path: List[int]) -> None:
        for i in range(len(self.adj[u])):
            v = self.adj[u][i]
            if v != -1 and self._is_valid_next_edge(u, v):
                path.append(v)
                self._remove_edge(u, v)
                self._dfs_euler(v, path)
                break
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Construct undirected graph with an Eulerian Path
    // Triangle (0-1-2) with a tail (2-3)
    FleuryEulerian graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(2, 3);

    std::vector<int> path = graph.find_eulerian_path();

    if (path.empty()) {
        std::cout << "No Eulerian Path or Circuit exists!\n";
    } else {
        std::cout << "Eulerian Path (Fleury's): ";
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
    # Construct undirected graph with an Eulerian Path
    # Triangle (0-1-2) with a tail (2-3)
    graph = FleuryEulerian(4)
    graph.add_edge(0, 1)
    graph.add_edge(1, 2)
    graph.add_edge(2, 0)
    graph.add_edge(2, 3)

    path = graph.find_eulerian_path()

    if not path:
        print("No Eulerian Path or Circuit exists!")
    else:
        print("Eulerian Path (Fleury's):", " -> ".join(map(str, path)))
```
