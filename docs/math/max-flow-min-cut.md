# Maximum Flow and Minimum Cut (Dinic's Algorithm)

## Conceptual Explanation
The **Maximum Flow** problem asks for the maximum amount of flow that can be routed from a source vertex $s$ to a sink vertex $t$ through a directed graph with capacity-constrained edges. The **Minimum $s$-$t$ Cut** problem seeks a partition of the vertices into two sets $S$ and $T$ ($s \in S$, $t \in T$) such that the sum of the capacities of edges crossing from $S$ to $T$ is minimized.
According to the **Max-Flow Min-Cut Theorem**, the value of the maximum flow is exactly equal to the capacity of the minimum cut.

### Dinic's Algorithm
Dinic's algorithm is one of the most powerful and widely used algorithms for network flow in competitive programming. It computes the maximum flow in $O(V^2 E)$ time (and even faster in practice, e.g., $O(E \sqrt{V})$ for unit networks or bipartite matching).

The algorithm runs in phases:
1. **Level Graph Construction (BFS)**: We perform a Breadth-First Search from $s$ to assign a level (shortest distance in terms of edge count) to each vertex. An edge $(u, v)$ is valid for flow augmentation in the current phase only if it has positive residual capacity and satisfies $\text{level}[v] = \text{level}[u] + 1$. If the sink $t$ cannot be reached, the algorithm terminates.
2. **Augmentation Path Search (DFS)**: We perform a Depth-First Search from $s$ to find augmenting paths in the level graph. To achieve $O(V^2 E)$ complexity, we implement two crucial optimizations:
   - **Blocking Flow**: We push as much flow as possible along the level graph in a single phase.
   - **Pointer Optimization (`ptr` array)**: We maintain a pointer to the next active edge for each vertex during DFS. This prevents re-examining saturated or dead-end edges, ensuring that each edge is visited only a constant number of times per phase.

### Reconstructing the Minimum Cut
After computing the maximum flow, the residual graph contains no paths from $s$ to $t$.
1. Run a BFS starting from the source $s$ visiting only edges with positive residual capacity ($U_{cap} - \text{flow} > 0$).
2. Let $S$ be the set of reachable vertices, and $T$ be the remaining vertices.
3. An original edge $(u, v)$ belongs to the **Minimum Cut** if $u \in S$ and $v \notin S$.

---

## Complexity Analysis
- **Time Complexity**:
  - General Networks: $O(V^2 E)$ in the worst case, but practically much faster.
  - Unit Networks (capacities are 0 or 1): $O(E \sqrt{V})$.
  - Bipartite Matching: $O(E \sqrt{V})$ (equivalent to Hopcroft-Karp).
- **Space Complexity**: $O(V + E)$ to store the adjacency lists, levels, and pointer arrays.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const long long INF = 1e18;

class Dinic {
public:
    struct Edge {
        int to;
        int rev; // Index of the reverse edge in adj[to]
        long long cap;
        long long flow;
    };

    int n;
    std::vector<std::vector<Edge>> adj;
    std::vector<int> level;
    std::vector<int> ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    // Add a directed edge with a given capacity
    void add_edge(int from, int to, long long cap) {
        adj[from].push_back({to, static_cast<int>(adj[to].size()), cap, 0});
        adj[to].push_back({from, static_cast<int>(adj[from].size()) - 1, 0, 0}); // Reverse edge has 0 initial capacity
    }

    // Step 1: BFS to find the levels of vertices
    bool bfs(int s, int t) {
        std::fill(level.begin(), level.end(), -1);
        level[s] = 0;
        std::queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& edge : adj[u]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    // Step 2: DFS to find blocking flows
    long long dfs(int u, int t, long long pushed) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;

        for (int& cid = ptr[u]; cid < adj[u].size(); ++cid) {
            auto& edge = adj[u][cid];
            int trg = edge.to;

            if (level[u] + 1 != level[trg] || edge.cap - edge.flow == 0) continue;

            long long push = dfs(trg, t, std::min(pushed, edge.cap - edge.flow));
            if (push == 0) continue;

            edge.flow += push;
            adj[trg][edge.rev].flow -= push;
            return push;
        }
        return 0;
    }

    // Compute the Maximum Flow
    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            std::fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }

    // Identifies the min-cut edges as pairs of {from, to}
    std::vector<std::pair<int, int>> min_cut(int s) {
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(s);
        visited[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& edge : adj[u]) {
                if (edge.cap - edge.flow > 0 && !visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        std::vector<std::pair<int, int>> cut_edges;
        for (int u = 0; u < n; ++u) {
            if (visited[u]) {
                for (const auto& edge : adj[u]) {
                    if (!visited[edge.to] && edge.cap > 0) {
                        cut_edges.push_back({u, edge.to});
                    }
                }
            }
        }
        return cut_edges;
    }
};
```

---

## Python Implementation

```python
from collections import deque
from typing import List, Tuple

INF = 10**18


class Dinic:
    class Edge:
        def __init__(self, to: int, rev: int, cap: int):
            self.to = to
            self.rev = rev
            self.cap = cap
            self.flow = 0

    def __init__(self, n: int):
        self.n = n
        self.adj: List[List[Dinic.Edge]] = [[] for _ in range(n)]
        self.level = [-1] * n
        self.ptr = [0] * n

    def add_edge(self, u: int, v: int, cap: int) -> None:
        """Adds directed edge with capacity."""
        self.adj[u].append(Dinic.Edge(v, len(self.adj[v]), cap))
        self.adj[v].append(Dinic.Edge(u, len(self.adj[u]) - 1, 0))

    def bfs(self, s: int, t: int) -> bool:
        """Constructs level graph using BFS."""
        self.level = [-1] * self.n
        self.level[s] = 0
        q = deque([s])

        while q:
            u = q.popleft()
            for edge in self.adj[u]:
                if edge.cap - edge.flow > 0 and self.level[edge.to] == -1:
                    self.level[edge.to] = self.level[u] + 1
                    q.append(edge.to)

        return self.level[t] != -1

    def dfs(self, u: int, t: int, pushed: int) -> int:
        """Finds blocking flow using DFS and pointer optimization."""
        if pushed == 0:
            return 0
        if u == t:
            return pushed

        for i in range(self.ptr[u], len(self.adj[u])):
            self.ptr[u] = i
            edge = self.adj[u][i]
            trg = edge.to

            if self.level[u] + 1 != self.level[trg] or edge.cap - edge.flow == 0:
                continue

            push = self.dfs(trg, t, min(pushed, edge.cap - edge.flow))
            if push == 0:
                continue

            edge.flow += push
            self.adj[trg][edge.rev].flow -= push
            return push

        self.ptr[u] = len(self.adj[u])
        return 0

    def max_flow(self, s: int, t: int) -> int:
        """Computes maximum flow from s to t."""
        flow = 0
        while self.bfs(s, t):
            self.ptr = [0] * self.n
            while True:
                pushed = self.dfs(s, t, INF)
                if pushed == 0:
                    break
                flow += pushed
        return flow

    def min_cut(self, s: int) -> List[Tuple[int, int]]:
        """Finds minimum cut edges crossing from S to T."""
        visited = [False] * self.n
        visited[s] = True
        q = deque([s])

        while q:
            u = q.popleft()
            for edge in self.adj[u]:
                if edge.cap - edge.flow > 0 and not visited[edge.to]:
                    visited[edge.to] = True
                    q.append(edge.to)

        cut_edges = []
        for u in range(self.n):
            if visited[u]:
                for edge in self.adj[u]:
                    if not visited[edge.to] and edge.cap > 0:
                        cut_edges.append((u, edge.to))
        return cut_edges
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 4 Vertices: 0 (Source), 1, 2, 3 (Sink)
    Dinic solver(4);
    
    // Add capacities
    solver.add_edge(0, 1, 10);
    solver.add_edge(0, 2, 5);
    solver.add_edge(1, 2, 15);
    solver.add_edge(1, 3, 10);
    solver.add_edge(2, 3, 10);

    long long flow = solver.max_flow(0, 3);
    std::cout << "Maximum Flow: " << flow << " (expected 15)\n";

    // Retrieve minimum cut edges
    auto cut = solver.min_cut(0);
    std::cout << "Minimum Cut Edges:\n";
    for (auto edge : cut) {
        std::cout << edge.first << " -> " << edge.second << "\n";
    }

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 4 Vertices: 0 (Source), 1, 2, 3 (Sink)
    solver = Dinic(4)

    solver.add_edge(0, 1, 10)
    solver.add_edge(0, 2, 5)
    solver.add_edge(1, 2, 15)
    solver.add_edge(1, 3, 10)
    solver.add_edge(2, 3, 10)

    flow = solver.max_flow(0, 3)
    print(f"Maximum Flow: {flow} (expected 15)")

    cut = solver.min_cut(0)
    print("Minimum Cut Edges:")
    for u, v in cut:
        print(f"{u} -> {v}")
```
