# Depth-First Search (DFS)

## Conceptual Explanation

**Depth-First Search (DFS)** is a fundamental graph traversal algorithm that visits all vertices in a graph by going as deep as possible along each branch before backtracking.

### Core Principle

Starting from a root node (or an arbitrary node in the case of a general graph), DFS explores the first neighbor of the current node, then explores the first neighbor of that neighbor, and so on. If a node has no unvisited neighbors left, the search backtracks to the most recently visited node that still has unvisited neighbors, and resumes from there.

DFS uses a **stack** to keep track of nodes along the current path. This stack can be managed:
1. **Implicitly** using the call stack of recursive function calls.
2. **Explicitly** using a user-defined stack data structure.

To prevent infinite loops in graphs with cycles, DFS keeps track of visited nodes using a boolean array or a set.

### Typical Competitive Programming Use Cases

- **Connected Components**: Finding the number of connected components in an undirected graph.
- **Pathfinding**: Finding a path between two vertices (not necessarily the shortest).
- **Cycle Detection**: Identifying cycles in directed or undirected graphs.
- **Topological Sorting**: Ordering vertices in a Directed Acyclic Graph (DAG).
- **Bridges and Cut Vertices (Articulation Points)**: Finding critical edges/vertices in a graph.
- **Solving Maze/Grid Problems**: Standard grid traversal.

---

## Complexity Analysis

- **Time Complexity**:
  - **Adjacency List**: $\mathcal{O}(V + E)$, where $V$ is the number of vertices and $E$ is the number of edges. We visit every vertex once and examine every edge once.
  - **Adjacency Matrix**: $\mathcal{O}(V^2)$, since we must scan an entire row of size $V$ for each vertex to discover its neighbors.
- **Space Complexity**:
  - **Call Stack**: $\mathcal{O}(V)$ in the worst case (e.g., a path graph where the stack depth is $V$).
  - **Visited Array / Adj List**: $\mathcal{O}(V + E)$ to store the graph structure and status.

---

## C++ Implementation

Below is a robust and complete C++ implementation showcasing both **recursive** and **iterative** DFS, as well as finding connected components and cycle detection in a directed graph.

```cpp
#include <iostream>
#include <vector>
#include <stack>
#include <stdexcept>

class DFSGraph {
private:
    int num_vertices;
    std::vector<std::vector<int>> adj;

    // Recursive helper for basic DFS traversal
    void dfs_recursive_helper(int u, std::vector<bool>& visited, std::vector<int>& order) {
        visited[u] = true;
        order.push_back(u);

        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs_recursive_helper(v, visited, order);
            }
        }
    }

    // Recursive helper for cycle detection (3-state coloring)
    // 0 = unvisited, 1 = visiting, 2 = fully processed
    bool has_cycle_helper(int u, std::vector<int>& state) const {
        state[u] = 1; // Mark as visiting

        for (int v : adj[u]) {
            if (state[v] == 1) {
                return true; // Found a back edge to a node in the current recursion stack
            }
            if (state[v] == 0) {
                if (has_cycle_helper(v, state)) {
                    return true;
                }
            }
        }

        state[u] = 2; // Mark as fully processed
        return false;
    }

public:
    explicit DFSGraph(int vertices) : num_vertices(vertices), adj(vertices) {}

    void add_edge(int u, int v) {
        if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
            throw std::out_of_range("Vertex index out of range");
        }
        adj[u].push_back(v);
    }

    // 1. Recursive DFS starting from a node
    std::vector<int> traverse_recursive(int start_node) {
        std::vector<bool> visited(num_vertices, false);
        std::vector<int> order;
        dfs_recursive_helper(start_node, visited, order);
        return order;
    }

    // 2. Iterative DFS starting from a node
    std::vector<int> traverse_iterative(int start_node) {
        std::vector<bool> visited(num_vertices, false);
        std::vector<int> order;
        std::stack<int> s;

        s.push(start_node);

        while (!s.empty()) {
            int u = s.top();
            s.pop();

            if (!visited[u]) {
                visited[u] = true;
                order.push_back(u);

                // Push neighbors in reverse order so they are processed in ascending order
                for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it) {
                    if (!visited[*it]) {
                        s.push(*it);
                    }
                }
            }
        }
        return order;
    }

    // 3. Cycle detection in a directed graph
    bool has_cycle() const {
        std::vector<int> state(num_vertices, 0);
        for (int i = 0; i < num_vertices; ++i) {
            if (state[i] == 0) {
                if (has_cycle_helper(i, state)) {
                    return true;
                }
            }
        }
        return false;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with complete type hints.

```python
from typing import List, Set

class DFSGraph:
    def __init__(self, vertices: int):
        self.num_vertices = vertices
        self.adj: List[List[int]] = [[] for _ in range(vertices)]

    def add_edge(self, u: int, v: int) -> None:
        if not (0 <= u < self.num_vertices and 0 <= v < self.num_vertices):
            raise IndexError("Vertex index out of range")
        self.adj[u].append(v)

    def traverse_recursive(self, start_node: int) -> List[int]:
        visited = [False] * self.num_vertices
        order: List[int] = []

        def dfs(u: int) -> None:
            visited[u] = True
            order.append(u)
            for v in self.adj[u]:
                if not visited[v]:
                    dfs(v)

        dfs(start_node)
        return order

    def traverse_iterative(self, start_node: int) -> List[int]:
        visited = [False] * self.num_vertices
        order: List[int] = []
        stack = [start_node]

        while stack:
            u = stack.pop()
            if not visited[u]:
                visited[u] = True
                order.append(u)
                # Push neighbors in reverse order to match recursion order
                for v in reversed(self.adj[u]):
                    if not visited[v]:
                        stack.append(v)
        return order

    def has_cycle(self) -> bool:
        # state: 0 = unvisited, 1 = visiting, 2 = visited
        state = [0] * self.num_vertices

        def dfs_cycle(u: int) -> bool:
            state[u] = 1
            for v in self.adj[u]:
                if state[v] == 1:
                    return True
                if state[v] == 0:
                    if dfs_cycle(v):
                        return True
            state[u] = 2
            return False

        for i in range(self.num_vertices):
            if state[i] == 0:
                if dfs_cycle(i):
                    return True
        return False
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    DFSGraph graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 5);

    std::vector<int> rec_order = graph.traverse_recursive(0);
    std::cout << "Recursive DFS Order starting from 0: ";
    for (int node : rec_order) std::cout << node << " ";
    std::cout << "\n";

    std::vector<int> iter_order = graph.traverse_iterative(0);
    std::cout << "Iterative DFS Order starting from 0: ";
    for (int node : iter_order) std::cout << node << " ";
    std::cout << "\n";

    std::cout << "Has cycle: " << (graph.has_cycle() ? "Yes" : "No") << " (Expected: No)\n";

    // Add a cycle: 4 -> 0
    graph.add_edge(4, 0);
    std::cout << "Has cycle (after adding edge 4->0): " << (graph.has_cycle() ? "Yes" : "No") << " (Expected: Yes)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    graph = DFSGraph(6)
    graph.add_edge(0, 1)
    graph.add_edge(0, 2)
    graph.add_edge(1, 3)
    graph.add_edge(1, 4)
    graph.add_edge(2, 5)

    rec_order = graph.traverse_recursive(0)
    print("Recursive DFS Order starting from 0:", rec_order)

    iter_order = graph.traverse_iterative(0)
    print("Iterative DFS Order starting from 0:", iter_order)

    print("Has cycle:", "Yes" if graph.has_cycle() else "No", "(Expected: No)")

    # Add cycle
    graph.add_edge(4, 0)
    print("Has cycle (after adding edge 4->0):", "Yes" if graph.has_cycle() else "No", "(Expected: Yes)")
```
