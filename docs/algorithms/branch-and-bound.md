# Branch and Bound

## Conceptual Explanation

**Branch and Bound** is a state-space search method used to solve combinatorial optimization problems. It systematically explores a state space by dividing it into smaller subproblems (branching) and using bounds to prune parts of the search space that cannot yield a better solution than the best one found so far (bounding).

It is an extension of backtracking, but specifically optimized for finding the **optimal** (minimum or maximum) solution rather than just any valid or all valid solutions.

### Key Components

1. **Branching**: Splitting a search node into two or more child nodes representing subproblems. For example, in the 0/1 Knapsack problem, at level $i$, we branch by either including or excluding the $i$-th item.
2. **Bounding**: Estimating the best possible objective value (upper bound for maximization, lower bound for minimization) that can be obtained from the current search node and its descendants.
3. **Pruning**: If the bound of a node is worse than the best global solution found so far (often called the * incumbent solution*), we discard (prune) this node and all of its descendants.
4. **Search Strategy**:
   - **Depth-First Search (DFS)**: Low memory footprint; finds feasible solutions quickly to establish initial bounds.
   - **Breadth-First Search (BFS)**: Explores level by level.
   - **Best-First Search (Least Cost Search)**: Uses a priority queue to always expand the node with the most promising bound. This often finds the optimal solution faster but requires more memory.

### Canonical Example: 0/1 Knapsack Problem
To solve the 0/1 Knapsack problem, we sort items by their value-to-weight ratio. For any node, the upper bound is computed using the **Fractional Knapsack** algorithm on the remaining capacity. Because the items are sorted, Fractional Knapsack can be solved greedily in linear time and provides a tight upper bound for the maximum possible value.

---

## Complexity Analysis

- **Time Complexity**:
  - **Worst Case**: $\mathcal{O}(2^N)$ where $N$ is the number of items. In the worst case, we might have to explore the entire state space.
  - **Average Case**: Significantly better than pure backtracking or exhaustive search due to aggressive pruning. In many practical instances, only a small fraction of the $2^N$ nodes are explored.
- **Space Complexity**:
  - **Best-First Search**: $\mathcal{O}(2^N)$ in the worst case, because the priority queue needs to store active frontier nodes.
  - **Depth-First Search**: $\mathcal{O}(N)$ space on the recursion stack.

---

## C++ Implementation

Here is a C++ implementation of the 0/1 Knapsack problem using **Best-First Search Branch and Bound**.

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Item {
    int id;
    int weight;
    int value;
    double ratio; // value / weight
};

struct Node {
    int level;      // Index of next item to consider
    int profit;     // Cumulative profit of path
    int weight;     // Cumulative weight of path
    double bound;   // Upper bound of profit in subtree

    // For priority queue (max-heap based on upper bound profit)
    bool operator<(const Node& other) const {
        return bound < other.bound;
    }
};

class KnapsackBranchAndBound {
private:
    int capacity;
    std::vector<Item> items;
    int num_items;

    // Computes the upper bound of profit using Fractional Knapsack
    double calculate_bound(const Node& u) const {
        if (u.weight >= capacity) {
            return 0.0;
        }

        double profit_bound = u.profit;
        int j = u.level + 1;
        int total_weight = u.weight;

        // Greedily grab whole items
        while (j < num_items && total_weight + items[j].weight <= capacity) {
            total_weight += items[j].weight;
            profit_bound += items[j].value;
            j++;
        }

        // Grab fraction of the next item if capacity remains
        if (j < num_items) {
            profit_bound += (capacity - total_weight) * items[j].ratio;
        }

        return profit_bound;
    }

public:
    KnapsackBranchAndBound(int cap, const std::vector<std::pair<int, int>>& item_pairs) : capacity(cap) {
        num_items = item_pairs.size();
        for (int i = 0; i < num_items; ++i) {
            items.push_back({
                i,
                item_pairs[i].first,
                item_pairs[i].second,
                static_cast<double>(item_pairs[i].second) / item_pairs[i].first
            });
        }

        // Sort items by value/weight ratio in descending order
        std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.ratio > b.ratio;
        });
    }

    int solve() {
        std::priority_queue<Node> pq;
        Node u, v;

        // Dummy starting node
        u.level = -1;
        u.profit = 0;
        u.weight = 0;
        u.bound = calculate_bound(u);
        pq.push(u);

        int max_profit = 0;

        while (!pq.empty()) {
            u = pq.top();
            pq.pop();

            // If the node is not promising, prune it
            if (u.bound <= max_profit) {
                continue;
            }

            // v is the next level child (explore decisions for items[level + 1])
            v.level = u.level + 1;

            // Scenario 1: Include items[v.level]
            v.weight = u.weight + items[v.level].weight;
            v.profit = u.profit + items[v.level].value;

            if (v.weight <= capacity && v.profit > max_profit) {
                max_profit = v.profit;
            }

            v.bound = calculate_bound(v);
            if (v.bound > max_profit) {
                pq.push(v);
            }

            // Scenario 2: Exclude items[v.level]
            v.weight = u.weight;
            v.profit = u.profit;
            v.bound = calculate_bound(v);
            if (v.bound > max_profit) {
                pq.push(v);
            }
        }

        return max_profit;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple
import heapq

class Item:
    def __init__(self, item_id: int, weight: int, value: int):
        self.id = item_id
        self.weight = weight
        self.value = value
        self.ratio = value / weight

class Node:
    def __init__(self, level: int, profit: int, weight: int, bound: float):
        self.level = level
        self.profit = profit
        self.weight = weight
        self.bound = bound

    # Python's heapq is a min-heap. We want a max-heap on the bound,
    # so we define __lt__ with a reversed comparison.
    def __lt__(self, other: 'Node') -> bool:
        return self.bound > other.bound

class KnapsackBranchAndBound:
    def __init__(self, capacity: int, item_pairs: List[Tuple[int, int]]):
        self.capacity = capacity
        self.items = [Item(i, w, v) for i, (w, v) in enumerate(item_pairs)]
        self.num_items = len(self.items)
        # Sort items by ratio in descending order
        self.items.sort(key=lambda x: x.ratio, reverse=True)

    def _calculate_bound(self, u: Node) -> float:
        if u.weight >= self.capacity:
            return 0.0

        profit_bound = u.profit
        j = u.level + 1
        total_weight = u.weight

        while j < self.num_items and total_weight + self.items[j].weight <= self.capacity:
            total_weight += self.items[j].weight
            profit_bound += self.items[j].value
            j += 1

        if j < self.num_items:
            profit_bound += (self.capacity - total_weight) * self.items[j].ratio

        return float(profit_bound)

    def solve(self) -> int:
        pq: List[Node] = []
        max_profit = 0

        # Create root node
        root = Node(level=-1, profit=0, weight=0, bound=0.0)
        root.bound = self._calculate_bound(root)
        heapq.heappush(pq, root)

        while pq:
            u = heapq.heappop(pq)

            if u.bound <= max_profit:
                continue

            v_level = u.level + 1
            if v_level >= self.num_items:
                continue

            # Scenario 1: Include item
            w_inc = u.weight + self.items[v_level].weight
            p_inc = u.profit + self.items[v_level].value

            if w_inc <= self.capacity and p_inc > max_profit:
                max_profit = p_inc

            node_inc = Node(level=v_level, profit=p_inc, weight=w_inc, bound=0.0)
            node_inc.bound = self._calculate_bound(node_inc)
            if node_inc.bound > max_profit:
                heapq.heappush(pq, node_inc)

            # Scenario 2: Exclude item
            node_exc = Node(level=v_level, profit=u.profit, weight=u.weight, bound=0.0)
            node_exc.bound = self._calculate_bound(node_exc)
            if node_exc.bound > max_profit:
                heapq.heappush(pq, node_exc)

        return max_profit
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // items: {weight, value}
    std::vector<std::pair<int, int>> items = {
        {2, 40}, {3, 50}, {1, 100}, {5, 95}, {4, 30}
    };
    int capacity = 10;

    KnapsackBranchAndBound solver(capacity, items);
    std::cout << "Maximum profit (0/1 Knapsack): " << solver.solve() << " (Expected: 245)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # items: (weight, value)
    items = [(2, 40), (3, 50), (1, 100), (5, 95), (4, 30)]
    capacity = 10

    solver = KnapsackBranchAndBound(capacity, items)
    print(f"Maximum profit (0/1 Knapsack): {solver.solve()} (Expected: 245)")
```
