# Eulerian Path and Circuit (Hierholzer's Algorithm)

## Conceptual Explanation

An **Eulerian Path** is a trail in a finite graph that visits every edge exactly once. An **Eulerian Circuit** (or Eulerian Cycle) is an Eulerian Path that starts and ends at the same vertex.

### Existence Conditions

Whether an Eulerian Path or Circuit exists depends on the connectivity of the graph and the degrees of its vertices. We assume all edges belong to a single connected component (vertices of degree 0 are ignored).

#### For Undirected Graphs:
- **Eulerian Circuit**: Every vertex has an **even** degree.
- **Eulerian Path**: Exactly **zero** or **two** vertices have an **odd** degree. If there are two odd-degree vertices, the path must start at one of them and end at the other.

#### For Directed Graphs:
- **Eulerian Circuit**: For every vertex, its in-degree equals its out-degree ($\text{in}[u] = \text{out}[u]$).
- **Eulerian Path**: At most one vertex has $\text{out}[u] - \text{in}[u] = 1$ (start vertex), at most one vertex has $\text{in}[u] - \text{out}[u] = 1$ (end vertex), and all other vertices have $\text{in}[u] = \text{out}[u]$.

---

### Hierholzer's Algorithm ($\mathcal{O}(E)$)

Hierholzer's algorithm is the standard, highly efficient algorithm to find an Eulerian Path or Circuit. It works by greedily drawing a cycle, and then splicing other cycles into it.

#### Algorithm Steps (using a stack/DFS):
1. **Verify Existence**: Check the degree constraints described above.
2. **Select Start Node**:
   - If an Eulerian Circuit is sought, start at any node with degree $> 0$.
   - If an Eulerian Path is sought, start at the unique node with $\text{out}[u] - \text{in}[u] = 1$ (directed) or odd degree (undirected).
3. **Traversal (DFS-like)**:
   - From the current vertex, traverse any unused outgoing edge to a neighbor, mark the edge as used, and push the neighbor onto the call stack.
   - If the current vertex has no unused outgoing edges, add this vertex to the result list and backtrack (pop from stack).
4. **Reverse Result**: The order in which nodes are added to the result list is the reverse of the Eulerian path. Reverse the list to get the final path.

---

## Complexity Analysis

- **Time Complexity**: $\mathcal{O}(V + E)$, since we visit each vertex and traverse each edge exactly once. To maintain this, we must efficiently delete or skip visited edges in $\mathcal{O}(1)$ time. In C++, this can be achieved by keeping an iterator or current-edge index pointer for each adjacency list.
- **Space Complexity**: $\mathcal{O}(V + E)$ to store the graph, recursion stack, and the resulting path.

---

## C++ Implementation

Below is a robust and complete C++ class to find an Eulerian Path in a **directed** graph using Hierholzer's algorithm.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

class EulerianDirected {
private:
    int num_vertices;
    std::vector<std::vector<int>> adj;
    std::vector<int> in_degree;
    std::vector<int> out_degree;

    // Track the next unvisited edge pointer for O(1) edge deletion
    std::vector<int> edge_ptr;

public:
    explicit EulerianDirected(int vertices) 
        : num_vertices(vertices), adj(vertices), in_degree(vertices, 0), out_degree(vertices, 0), edge_ptr(vertices, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        out_degree[u]++;
        in_degree[v]++;
    }

    // Checks the degree conditions and returns the starting vertex.
    // Returns -1 if no Eulerian Path or Circuit exists.
    int get_start_vertex() const {
        int start_nodes = 0;
        int end_nodes = 0;
        int potential_start = -1;
        int first_active_node = -1;

        for (int i = 0; i < num_vertices; ++i) {
            if (out_degree[i] > 0 && first_active_node == -1) {
                first_active_node = i;
            }

            int diff = out_degree[i] - in_degree[i];
            if (std::abs(diff) > 1) {
                return -1;
            } else if (diff == 1) {
                start_nodes++;
                potential_start = i;
            } else if (diff == -1) {
                end_nodes++;
            }
        }

        // Case 1: Eulerian Circuit (all degrees balanced)
        if (start_nodes == 0 && end_nodes == 0) {
            return first_active_node;
        }

        // Case 2: Eulerian Path (exactly one start and one end)
        if (start_nodes == 1 && end_nodes == 1) {
            return potential_start;
        }

        return -1;
    }

    // Computes the Eulerian Path/Circuit using Hierholzer's Algorithm
    std::vector<int> find_eulerian_path() {
        int start = get_start_vertex();
        if (start == -1) {
            return {}; // No Eulerian Path exists
        }

        std::vector<int> path;
        std::stack<int> s;
        s.push(start);

        // Reset edge pointers
        std::fill(edge_ptr.begin(), edge_ptr.end(), 0);

        while (!s.empty()) {
            int u = s.top();
            if (edge_ptr[u] < static_cast<int>(adj[u].size())) {
                // Move to next neighbor and advance pointer to "delete" the edge
                int next_v = adj[u][edge_ptr[u]];
                edge_ptr[u]++;
                s.push(next_v);
            } else {
                // No more outgoing edges from u, add to path and pop
                path.push_back(u);
                s.pop();
            }
        }

        std::reverse(path.begin(), path.end());

        // Verify if all edges in the graph were visited
        int total_edges = 0;
        for (int i = 0; i < num_vertices; ++i) {
            total_edges += adj[i].size();
        }

        if (static_cast<int>(path.size()) != total_edges + 1) {
            return {}; // Graph is disconnected (except for isolated components with no edges)
        }

        return path;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List
import collections

class EulerianDirected:
    def __init__(self, vertices: int):
        self.num_vertices = vertices
        self.adj: List[List[int]] = [[] for _ in range(vertices)]
        self.in_degree = [0] * vertices
        self.out_degree = [0] * vertices
        self.edge_ptr = [0] * vertices

    def add_edge(self, u: int, v: int) -> None:
        self.adj[u].append(v)
        self.out_degree[u] += 1
        self.in_degree[v] += 1

    def get_start_vertex(self) -> int:
        start_nodes = 0
        end_nodes = 0
        potential_start = -1
        first_active_node = -1

        for i in range(self.num_vertices):
            if self.out_degree[i] > 0 and first_active_node == -1:
                first_active_node = i

            diff = self.out_degree[i] - self.in_degree[i]
            if abs(diff) > 1:
                return -1
            elif diff == 1:
                start_nodes += 1
                potential_start = i
            elif diff == -1:
                end_nodes += 1

        # Case 1: Eulerian Circuit (all degrees balanced)
        if start_nodes == 0 and end_nodes == 0:
            return first_active_node

        # Case 2: Eulerian Path (exactly one start and one end)
        if start_nodes == 1 and end_nodes == 1:
            return potential_start

        return -1

    def find_eulerian_path(self) -> List[int]:
        start = self.get_start_vertex()
        if start == -1:
            return []

        path = []
        stack = [start]
        self.edge_ptr = [0] * self.num_vertices

        while stack:
            u = stack[-1]
            if self.edge_ptr[u] < len(self.adj[u]):
                # Traverse next neighbor and advance edge pointer
                next_v = self.adj[u][self.edge_ptr[u]]
                self.edge_ptr[u] += 1
                stack.append(next_v)
            else:
                path.append(stack.pop())

        path.reverse()

        # Check if we visited all edges
        total_edges = sum(len(self.adj[i]) for i in range(self.num_vertices))
        if len(path) != total_edges + 1:
            return []

        return path
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Construct a directed graph with an Eulerian Path
    // Edges: 0 -> 1, 1 -> 2, 2 -> 0, 0 -> 3, 3 -> 4
    EulerianDirected graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(0, 3);
    graph.add_edge(3, 4);

    std::vector<int> path = graph.find_eulerian_path();

    if (path.empty()) {
        std::cout << "No Eulerian Path or Circuit exists!\n";
    } else {
        std::cout << "Eulerian Path: ";
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
    # Construct a directed graph with an Eulerian Path
    # Edges: 0 -> 1, 1 -> 2, 2 -> 0, 0 -> 3, 3 -> 4
    graph = EulerianDirected(5)
    graph.add_edge(0, 1)
    graph.add_edge(1, 2)
    graph.add_edge(2, 0)
    graph.add_edge(0, 3)
    graph.add_edge(3, 4)

    path = graph.find_eulerian_path()

    if not path:
        print("No Eulerian Path or Circuit exists!")
    else:
        print("Eulerian Path:", " -> ".join(map(str, path)))
```
