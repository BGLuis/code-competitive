# Priority Queue

## Conceptual Explanation

A **Priority Queue** is an abstract data type similar to a standard queue, but where each element has an associated "priority". Elements are served such that the element with the highest priority is always at the front (for a max-priority queue) or the lowest priority is at the front (for a min-priority queue).

### Core Logic & Operations
Under the hood, a priority queue is almost always implemented using a **Binary Heap** (like a Max-Heap or Min-Heap), which guarantees logarithmic time complexity for insertions and deletions.

Main operations:
- **push (enqueue)**: Add a new element with a priority.
- **pop (dequeue)**: Remove the element with the highest/lowest priority.
- **top (peek)**: Get the element with the highest/lowest priority without removing it.

### Typical Competitive Programming Use Cases
- **Dijkstra’s Algorithm**: Finding the shortest path in a weighted graph in $O(E \log V)$ time by always visiting the node with the minimum tentative distance.
- **Huffman Coding / Greedy Merging**: Repeatedly merging the two smallest elements to minimize costs.
- **Event-Driven Simulation (Sweep-Line)**: Scheduling and processing events in order of their occurrence coordinates or times.

---

## Complexity Analysis

Let $N$ be the number of elements in the priority queue.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Push** | $O(\log N)$ | $O(1)$ |
| **Pop** | $O(\log N)$ | $O(1)$ |
| **Top (Peek)** | $O(1)$ | $O(1)$ |
| **Space Complexity** | $O(N)$ | $O(N)$ |

---

## C++ Implementation

In C++, we use `std::priority_queue` from the `<queue>` header. By default, it is a max-heap. We can customize it using `std::greater` for a min-heap, or pass a custom struct comparator.

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <string>

// A custom structure representing a job task
struct Task {
    std::string name;
    int priority;

    // Custom comparator: We want the Task with the HIGHER priority number to come first.
    // In std::priority_queue, the element that compares "less" is placed lower down, 
    // so the largest element is at the top. Hence, we define operator< accordingly.
    bool operator<(const Task& other) const {
        return priority < other.priority; 
    }
};

int main() {
    // --- SECTION 1: Standard Max-Priority Queue ---
    std::cout << "=== Max-Priority Queue (Default) ===\n";
    std::priority_queue<int> max_pq;

    max_pq.push(10);
    max_pq.push(30);
    max_pq.push(20);
    max_pq.push(5);

    std::cout << "Max-PQ Elements (in extraction order): ";
    while (!max_pq.empty()) {
        std::cout << max_pq.top() << " ";
        max_pq.pop();
    }
    std::cout << "\n\n";

    // --- SECTION 2: Standard Min-Priority Queue ---
    std::cout << "=== Min-Priority Queue ===\n";
    // We pass std::vector<int> as container and std::greater<int> as comparator
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

    min_pq.push(10);
    min_pq.push(30);
    min_pq.push(20);
    min_pq.push(5);

    std::cout << "Min-PQ Elements (in extraction order): ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << "\n\n";

    // --- SECTION 3: Custom Struct Priority Queue ---
    std::cout << "=== Custom Struct Priority Queue ===\n";
    std::priority_queue<Task> task_pq;

    task_pq.push({"Clean room", 2});
    task_pq.push({"Code competitive programming", 10});
    task_pq.push({"Fix bug", 8});
    task_pq.push({"Sleep", 1});

    std::cout << "Task Queue order:\n";
    while (!task_pq.empty()) {
        Task t = task_pq.top();
        std::cout << "Task: " << t.name << " (Priority: " << t.priority << ")\n";
        task_pq.pop();
    }

    return 0;
}
```

---

## Python Implementation

In Python, the `heapq` module provides binary heap operations on a standard list. It is a **min-heap** by default. To implement a max-heap, we can negate the numerical values, or store custom objects with custom comparison methods.

```python
import heapq
from typing import List, Tuple

class Task:
    def __init__(self, name: str, priority: int):
        self.name = name
        self.priority = priority

    def __lt__(self, other: 'Task') -> bool:
        # We want the HIGHEST priority Task to come first.
        # heapq is a min-heap, so the smallest value gets popped first.
        # Therefore, we define self < other as self.priority > other.priority.
        return self.priority > other.priority


if __name__ == '__main__':
    # --- SECTION 1: Standard Min-Priority Queue (heapq) ---
    print("=== Min-Priority Queue ===")
    min_pq: List[int] = []

    heapq.heappush(min_pq, 10)
    heapq.heappush(min_pq, 30)
    heapq.heappush(min_pq, 20)
    heapq.heappush(min_pq, 5)

    print("Min-PQ Elements (in extraction order): ", end="")
    while min_pq:
        print(heapq.heappop(min_pq), end=" ")
    print("\n")

    # --- SECTION 2: Standard Max-Priority Queue ---
    print("=== Max-Priority Queue (Negated values) ===")
    max_pq: List[int] = []

    # To simulate max-priority, we push negated values
    heapq.heappush(max_pq, -10)
    heapq.heappush(max_pq, -30)
    heapq.heappush(max_pq, -20)
    heapq.heappush(max_pq, -5)

    print("Max-PQ Elements (in extraction order): ", end="")
    while max_pq:
        val = heapq.heappop(max_pq)
        print(-val, end=" ")
    print("\n")

    # --- SECTION 3: Custom Struct Priority Queue ---
    print("=== Custom Struct Priority Queue ===")
    task_pq: List[Task] = []

    heapq.heappush(task_pq, Task("Clean room", 2))
    heapq.heappush(task_pq, Task("Code competitive programming", 10))
    heapq.heappush(task_pq, Task("Fix bug", 8))
    heapq.heappush(task_pq, Task("Sleep", 1))

    print("Task Queue order:")
    while task_pq:
        t = heapq.heappop(task_pq)
        print(f"Task: {t.name} (Priority: {t.priority})")
```
