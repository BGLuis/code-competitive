# Queue

## Conceptual Explanation

A **Queue** is a linear data structure that follows the **First In First Out (FIFO)** principle. The element that is inserted first is the first one to be removed. It resembles a real-world line of people waiting for a service.

### Core Logic & Operations
A queue supports two main structural operations:
- **Enqueue (Push)**: Add an element to the end (rear/back) of the queue.
- **Dequeue (Pop)**: Remove the element from the beginning (front) of the queue.
- **Front (Peek)**: Inspect the front element without removing it.

#### Implementation Strategies:
1. **Circular Array**: To prevent elements from crawling out of bounds of a standard array after multiple enqueues and dequeues, we use a **circular buffer**. We maintain `front` and `rear` indices and use modulo arithmetic to wrap around the array boundaries:
   $$\text{next\_index} = (\text{index} + 1) \pmod{\text{capacity}}$$
2. **Linked List**: Maintaining references to the head and tail of a linked list permits efficient front deletion and back insertion.

### Typical Competitive Programming Use Cases
- **Breadth-First Search (BFS)**: Traversal of graphs, trees, or state-spaces layer-by-layer.
- **Buffer / Queueing Systems**: Buffering messages, print jobs, or requests in simulation problems.

---

## Complexity Analysis

| Operation | Circular Array | Linked List | Std Queue (`std::queue` / `deque`) |
| :--- | :--- | :--- | :--- |
| **Enqueue (Push)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Dequeue (Pop)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Front (Peek)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Space Complexity** | $O(C)$ (fixed cap) | $O(N)$ (dynamic) | $O(N)$ (dynamic) |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <queue>

// Custom circular array-based Queue
template <typename T>
class CircularQueue {
private:
    std::vector<T> arr;
    int capacity;
    int front_idx;
    int rear_idx;
    int element_count;

public:
    CircularQueue(int cap = 100) : capacity(cap), front_idx(0), rear_idx(0), element_count(0) {
        arr.resize(capacity);
    }

    bool isFull() const {
        return element_count == capacity;
    }

    bool isEmpty() const {
        return element_count == 0;
    }

    void enqueue(const T& val) {
        if (isFull()) {
            throw std::overflow_error("Queue is full");
        }
        arr[rear_idx] = val;
        rear_idx = (rear_idx + 1) % capacity;
        element_count++;
    }

    void dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }
        front_idx = (front_idx + 1) % capacity;
        element_count--;
    }

    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }
        return arr[front_idx];
    }

    int size() const {
        return element_count;
    }
};

int main() {
    // --- SECTION 1: Custom Circular Queue ---
    std::cout << "=== Custom Circular Queue ===\n";
    CircularQueue<int> cq(5);

    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);

    std::cout << "Front: " << cq.front() << " (Expected: 10)\n";
    cq.dequeue();
    std::cout << "Front after 1 dequeue: " << cq.front() << " (Expected: 20)\n";

    cq.enqueue(40);
    cq.enqueue(50);
    cq.enqueue(60); // Queue now has: 20, 30, 40, 50, 60 (Full)

    std::cout << "Queue size: " << cq.size() << " (Expected: 5)\n";

    // --- SECTION 2: Standard C++ queue ---
    std::cout << "\n=== Standard std::queue ===\n";
    std::queue<std::string> q;

    q.push("First");
    q.push("Second");
    q.push("Third");

    std::cout << "Standard Queue elements: ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from collections import deque
from typing import Generic, TypeVar

T = TypeVar('T')

class CircularQueue(Generic[T]):
    def __init__(self, capacity: int = 100):
        self.capacity = capacity
        self.arr = [None] * capacity
        self.front_idx = 0
        self.rear_idx = 0
        self.element_count = 0

    def is_full(self) -> bool:
        return self.element_count == self.capacity

    def is_empty(self) -> bool:
        return self.element_count == 0

    def enqueue(self, val: T) -> None:
        """Add an element to the rear of the queue."""
        if self.is_full():
            raise IndexError("enqueue into a full queue")
        self.arr[self.rear_idx] = val
        self.rear_idx = (self.rear_idx + 1) % self.capacity
        self.element_count += 1

    def dequeue(self) -> T:
        """Remove and return the element from the front of the queue."""
        if self.is_empty():
            raise IndexError("dequeue from an empty queue")
        val = self.arr[self.front_idx]
        self.front_idx = (self.front_idx + 1) % self.capacity
        self.element_count -= 1
        return val

    def front(self) -> T:
        """Get the front element without removing it."""
        if self.is_empty():
            raise IndexError("peek from an empty queue")
        return self.arr[self.front_idx]

    def __len__(self) -> int:
        return self.element_count


if __name__ == '__main__':
    # --- SECTION 1: Custom Circular Queue ---
    print("=== Custom Circular Queue ===")
    cq = CircularQueue[int](5)

    cq.enqueue(10)
    cq.enqueue(20)
    cq.enqueue(30)

    print("Front:", cq.front(), "(Expected: 10)")
    cq.dequeue()
    print("Front after 1 dequeue:", cq.front(), "(Expected: 20)")

    cq.enqueue(40)
    cq.enqueue(50)
    cq.enqueue(60)

    print("Queue size:", len(cq), "(Expected: 5)")

    # --- SECTION 2: Standard Python queue ---
    # In Python, we use collections.deque for high-performance queues
    print("\n=== Standard collections.deque ===")
    q = deque()

    q.append("First")
    q.append("Second")
    q.append("Third")

    print("Standard queue elements: ", end="")
    while q:
        print(q.popleft(), end=" ")
    print()
```
