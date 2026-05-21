# Double-Ended Queue (Deque)

## Conceptual Explanation

A **Double-Ended Queue (Deque)** is a sequence-like container that supports fast, $O(1)$ insertions and deletions at both its **beginning** (front) and its **end** (back). It combines the properties of both stacks and queues.

### Core Logic & Operations
- **push_front / appendleft**: Insert an element at the beginning.
- **push_back / append**: Insert an element at the end.
- **pop_front / popleft**: Remove the element at the beginning.
- **pop_back / pop**: Remove the element at the end.
- **front / peek_front**: View the first element.
- **back / peek_back**: View the last element.

In a custom array-based implementation, a deque is usually built using a **circular buffer**. We maintain a pointer/index for the `front` and the `back`, and when they hit the boundaries of the array, they wrap around using modulo arithmetic.

### Typical Competitive Programming Use Cases
- **0-1 BFS (Shortest Path)**: If graph edges have weight $0$ or $1$, we can find the shortest path in $O(V + E)$ time using a deque. We push vertices with $0$-weight edges to the **front** and $1$-weight edges to the **back**.
- **Sliding Window Minimum/Maximum**: Using a **Monotonic Deque** to find the minimum/maximum in all contiguous subarrays of size $K$ in linear $O(N)$ time.
- **Undo / Redo Buffer**: Standard applications of keeping track of elements added and removed from both directions.

---

## Complexity Analysis

| Operation | Custom/Std Deque (Circular/Chunked) | Singly Linked List (Standard) |
| :--- | :--- | :--- |
| **Push Front** | $O(1)$ | $O(1)$ |
| **Push Back** | $O(1)$ | $O(1)$ |
| **Pop Front** | $O(1)$ | $O(1)$ |
| **Pop Back** | $O(1)$ | $O(N)$ (without tail/double link) |
| **Access Front/Back** | $O(1)$ | $O(1)$ |
| **Space Complexity** | $O(N)$ | $O(N)$ |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <deque>

// Custom circular array-based Deque implementation
template <typename T>
class CustomDeque {
private:
    std::vector<T> arr;
    int capacity;
    int front_idx;
    int back_idx;
    int element_count;

public:
    CustomDeque(int cap = 100) : capacity(cap), front_idx(0), back_idx(cap - 1), element_count(0) {
        arr.resize(capacity);
    }

    bool isFull() const {
        return element_count == capacity;
    }

    bool isEmpty() const {
        return element_count == 0;
    }

    void pushFront(const T& val) {
        if (isFull()) {
            throw std::overflow_error("Deque is full");
        }
        front_idx = (front_idx - 1 + capacity) % capacity;
        arr[front_idx] = val;
        element_count++;
    }

    void pushBack(const T& val) {
        if (isFull()) {
            throw std::overflow_error("Deque is full");
        }
        back_idx = (back_idx + 1) % capacity;
        arr[back_idx] = val;
        element_count++;
    }

    void popFront() {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        front_idx = (front_idx + 1) % capacity;
        element_count--;
    }

    void popBack() {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        back_idx = (back_idx - 1 + capacity) % capacity;
        element_count--;
    }

    T getFront() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return arr[front_idx];
    }

    T getBack() const {
        if (isEmpty()) {
            throw std::underflow_error("Deque is empty");
        }
        return arr[back_idx];
    }

    int size() const {
        return element_count;
    }
};

int main() {
    // --- SECTION 1: Custom circular-buffer Deque ---
    std::cout << "=== Custom Deque Demo ===\n";
    CustomDeque<int> my_deque(10);

    my_deque.pushBack(10);
    my_deque.pushBack(20);
    my_deque.pushFront(5);
    my_deque.pushFront(1);

    std::cout << "Front: " << my_deque.getFront() << " (Expected: 1)\n";
    std::cout << "Back:  " << my_deque.getBack() << " (Expected: 20)\n";

    my_deque.popFront();
    std::cout << "After popFront, Front: " << my_deque.getFront() << " (Expected: 5)\n";

    my_deque.popBack();
    std::cout << "After popBack, Back:   " << my_deque.getBack() << " (Expected: 10)\n";

    // --- SECTION 2: Standard std::deque (used in CP) ---
    std::cout << "\n=== Standard std::deque Demo ===\n";
    std::deque<int> std_dq;

    std_dq.push_back(100);
    std_dq.push_front(50);
    std_dq.push_back(200);

    std::cout << "Elements in std::deque: ";
    for (int x : std_dq) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "Front: " << std_dq.front() << ", Back: " << std_dq.back() << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from collections import deque
from typing import Generic, TypeVar

T = TypeVar('T')

class CustomDeque(Generic[T]):
    def __init__(self, capacity: int = 100):
        self.capacity: int = capacity
        self.arr: list = [None] * capacity
        self.front_idx: int = 0
        self.back_idx: int = capacity - 1
        self.element_count: int = 0

    def is_full(self) -> bool:
        return self.element_count == self.capacity

    def is_empty(self) -> bool:
        return self.element_count == 0

    def push_front(self, val: T) -> None:
        if self.is_full():
            raise IndexError("push to a full Deque")
        self.front_idx = (self.front_idx - 1 + self.capacity) % self.capacity
        self.arr[self.front_idx] = val
        self.element_count += 1

    def push_back(self, val: T) -> None:
        if self.is_full():
            raise IndexError("push to a full Deque")
        self.back_idx = (self.back_idx + 1) % self.capacity
        self.arr[self.back_idx] = val
        self.element_count += 1

    def pop_front(self) -> T:
        if self.is_empty():
            raise IndexError("pop from an empty Deque")
        val = self.arr[self.front_idx]
        self.front_idx = (self.front_idx + 1) % self.capacity
        self.element_count -= 1
        return val

    def pop_back(self) -> T:
        if self.is_empty():
            raise IndexError("pop from an empty Deque")
        val = self.arr[self.back_idx]
        self.back_idx = (self.back_idx - 1 + self.capacity) % self.capacity
        self.element_count -= 1
        return val

    def get_front(self) -> T:
        if self.is_empty():
            raise IndexError("peek from an empty Deque")
        return self.arr[self.front_idx]

    def get_back(self) -> T:
        if self.is_empty():
            raise IndexError("peek from an empty Deque")
        return self.arr[self.back_idx]

    def __len__(self) -> int:
        return self.element_count


if __name__ == '__main__':
    # --- SECTION 1: Custom circular-buffer Deque ---
    print("=== Custom Deque Demo ===")
    cdq = CustomDeque[int](10)
    cdq.push_back(10)
    cdq.push_back(20)
    cdq.push_front(5)
    cdq.push_front(1)

    print("Front:", cdq.get_front(), "(Expected: 1)")
    print("Back: ", cdq.get_back(), "(Expected: 20)")

    cdq.pop_front()
    print("After pop_front, Front:", cdq.get_front(), "(Expected: 5)")

    cdq.pop_back()
    print("After pop_back, Back:  ", cdq.get_back(), "(Expected: 10)")

    # --- SECTION 2: Standard collections.deque (highly optimized C implementation) ---
    print("\n=== Standard collections.deque Demo ===")
    std_dq = deque([100])
    std_dq.appendleft(50)
    std_dq.append(200)

    print("Elements in deque:", list(std_dq))
    print(f"Front: {std_dq[0]}, Back: {std_dq[-1]}")
    std_dq.popleft()
    print("After popleft:", list(std_dq))
```
