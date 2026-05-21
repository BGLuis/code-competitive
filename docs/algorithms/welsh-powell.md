# Welsh-Powell Algorithm (Greedy Graph Coloring)

## Conceptual Explanation

Graph Coloring (Vertex Coloring) is the assignment of a label (or "color") to each vertex in a graph such that no two adjacent vertices share the same color. The goal is to minimize the total number of colors used, which is known as the **Chromatic Number** $\chi(G)$ of the graph.

Finding the exact chromatic number of a general graph is an NP-complete problem. The **Welsh-Powell algorithm** is a highly popular and intuitive greedy heuristic that provides a good upper bound on the chromatic number in polynomial time.

### Core Principle

The Welsh-Powell algorithm sorts the vertices by their degrees in descending order. The intuition is that vertices with higher degrees (more connections) are harder to color without conflict, so coloring them first provides more flexibility for the remaining vertices.

### Algorithm Steps

1. **Calculate Degrees**: Find the degree (number of neighbors) of each vertex in the graph.
2. **Sort**: Sort all vertices in descending order of their degrees.
3. **Coloring Loop**:
   - Initialize a color counter $C = 1$.
   - While there are still uncolored vertices:
     - Find the first uncolored vertex $u$ in the sorted list, and assign it color $C$.
     - Iterate through the remaining uncolored vertices in the sorted list. For each vertex $v$, if $v$ is **not adjacent** to any other vertex that was already colored with color $C$ in this step, assign $v$ the color $C$.
     - Increment the color counter: $C \leftarrow C + 1$.

---

## Complexity Analysis

Let $V$ be the number of vertices and $E$ be the number of edges.
- **Time Complexity**:
  - **Degree Calculation**: $\mathcal{O}(V + E)$ using an adjacency list.
  - **Sorting**: $\mathcal{O}(V \log V)$ to sort vertices by degree.
  - **Coloring**: $\mathcal{O}(V^2)$ in the worst case. In each coloring pass, we scan through the uncolored vertices, and for each candidate, we check if it is adjacent to any already-colored vertex in the current pass. With an adjacency matrix or fast lookup set, the check takes $\mathcal{O}(V)$ time.
  - **Overall Complexity**: $\mathcal{O}(V^2)$ time.
- **Space Complexity**:
  - $\mathcal{O}(V + E)$ to represent the graph and keep track of degrees and assigned colors.

---

## C++ Implementation

Below is a robust and complete C++ class implementing the Welsh-Powell graph coloring algorithm.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

struct Vertex {
    int id;
    int degree;

    // For sorting in descending order of degrees
    bool operator<(const Vertex& other) const {
        if (degree != other.degree) {
            return degree > other.degree; // Larger degree first
        }
        return id < other.id; // Stable backup order
    }
};

class WelshPowellColoring {
private:
    int num_vertices;
    std::vector<std::vector<int>> adj;
    std::vector<std::set<int>> adj_set; // To perform O(1) adjacency checks

public:
    explicit WelshPowellColoring(int vertices) 
        : num_vertices(vertices), adj(vertices), adj_set(vertices) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        adj_set[u].insert(v);
        adj_set[v].insert(u);
    }

    // Colors the graph and returns the color assignment array.
    // Colors are 1-indexed. Returns a vector where result[i] is the color of vertex i.
    std::vector<int> color_graph(int& max_color_used) {
        std::vector<Vertex> vertices(num_vertices);
        for (int i = 0; i < num_vertices; ++i) {
            vertices[i] = {i, static_cast<int>(adj[i].size())};
        }

        // 1. Sort vertices by degree in descending order
        std::sort(vertices.begin(), vertices.end());

        std::vector<int> color_assignment(num_vertices, 0); // 0 means uncolored
        int current_color = 0;

        for (size_t i = 0; i < vertices.size(); ++i) {
            int u = vertices[i].id;
            
            // If already colored, skip
            if (color_assignment[u] != 0) {
                continue;
            }

            // Assign new color
            current_color++;
            color_assignment[u] = current_color;

            // Track all vertices colored with the current color in this pass
            std::vector<int> colored_in_this_pass;
            colored_in_this_pass.push_back(u);

            // 2. Go down the list to find other non-adjacent vertices to color
            for (size_t j = i + 1; j < vertices.size(); ++j) {
                int v = vertices[j].id;

                if (color_assignment[v] == 0) {
                    bool can_color = true;

                    // Check if v is adjacent to any vertex already colored in this pass
                    for (int already_colored : colored_in_this_pass) {
                        if (adj_set[v].count(already_colored)) {
                            can_color = false;
                            break;
                        }
                    }

                    if (can_color) {
                        color_assignment[v] = current_color;
                        colored_in_this_pass.push_back(v);
                    }
                }
            }
        }

        max_color_used = current_color;
        return color_assignment;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with complete type hints.

```python
from typing import List, Tuple, Set

class Vertex:
    def __init__(self, vertex_id: int, degree: int):
        self.id = vertex_id
        self.degree = degree

    def __lt__(self, other: 'Vertex') -> bool:
        if self.degree != other.degree:
            return self.degree > other.degree  # Descending order of degree
        return self.id < other.id

class WelshPowellColoring:
    def __init__(self, vertices: int):
        self.num_vertices = vertices
        self.adj: List[List[int]] = [[] for _ in range(vertices)]
        self.adj_set: List[Set[int]] = [set() for _ in range(vertices)]

    def add_edge(self, u: int, v: int) -> None:
        self.adj[u].append(v)
        self.adj[v].append(u)
        self.adj_set[u].add(v)
        self.adj_set[v].add(u)

    def color_graph(self) -> Tuple[List[int], int]:
        """
        Colors the graph using Welsh-Powell algorithm.
        Returns:
            A tuple (color_assignments, chromatic_number_estimate)
        """
        vertices = [Vertex(i, len(self.adj[i])) for i in range(self.num_vertices)]
        # 1. Sort vertices by degree in descending order
        vertices.sort()

        color_assignment = [0] * self.num_vertices
        current_color = 0

        for i in range(self.num_vertices):
            u = vertices[i].id
            if color_assignment[u] != 0:
                continue

            current_color += 1
            color_assignment[u] = current_color
            colored_in_this_pass = [u]

            # 2. Go down the list to color non-adjacent vertices
            for j in range(i + 1, self.num_vertices):
                v = vertices[j].id
                if color_assignment[v] == 0:
                    # Check if v is adjacent to any vertex already colored with current_color
                    can_color = True
                    for already_colored in colored_in_this_pass:
                        if already_colored in self.adj_set[v]:
                            can_color = False
                            break
                    
                    if can_color:
                        color_assignment[v] = current_color
                        colored_in_this_pass.append(v)

        return color_assignment, current_color
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Constructing a sample graph
    // Vertices: 0, 1, 2, 3, 4, 5
    WelshPowellColoring graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);
    graph.add_edge(3, 5);
    graph.add_edge(4, 5);

    int total_colors = 0;
    std::vector<int> colors = graph.color_graph(total_colors);

    std::cout << "Welsh-Powell Graph Coloring Results:\n";
    std::cout << "Estimated chromatic number: " << total_colors << "\n";
    for (int i = 0; i < 6; ++i) {
        std::cout << "Vertex " << i << " -> Color " << colors[i] << "\n";
    }

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # Constructing a sample graph
    graph = WelshPowellColoring(6)
    graph.add_edge(0, 1)
    graph.add_edge(0, 2)
    graph.add_edge(1, 2)
    graph.add_edge(1, 3)
    graph.add_edge(2, 4)
    graph.add_edge(3, 4)
    graph.add_edge(3, 5)
    graph.add_edge(4, 5)

    colors, total_colors = graph.color_graph()

    print("Welsh-Powell Graph Coloring Results:")
    print(f"Estimated chromatic number: {total_colors}")
    for idx, color in enumerate(colors):
        print(f"Vertex {idx} -> Color {color}")
```
