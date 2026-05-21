# Max Heap

## Conceptual Explanation

A **Max Heap** is a complete binary tree that satisfies the **heap property**: the value of each node is greater than or equal to the values of its children. The root of the tree always contains the maximum element in the heap.

### Core Logic & Operations
A Max Heap is efficiently represented as an array (0-indexed) where for any element at index $i$:
- **Parent**: `(i - 1) / 2`
- **Left Child**: `2 * i + 1`
- **Right Child**: `2 * i + 2`

Main operations:
1. **Insert (Push)**: Append the new element to the end of the array. Then, restore the heap property by running **heapifyUp** (bubble up/sift up): compare the element with its parent and swap if the element is larger. Repeat until the parent is larger or the element becomes the root.
2. **Extract Max (Pop)**: Replace the root (maximum element) with the last element in the array, then remove the last element. Next, restore the heap property by running **heapifyDown** (bubble down/sift down): compare the new root with its children, swap with the larger child if any child is larger, and repeat recursively down the tree.
3. **Get Max (Top)**: Return the root element in $O(1)$ time.

### Typical Competitive Programming Use Cases
- **Priority Queue Applications**: Scheduling tasks, Dijkstra's shortest path algorithm (where weights are inverted or min-heap is simulated), Prim's Minimum Spanning Tree.
- **Top-K Elements**: Finding the $K$ largest elements in a stream.
- **Heap Sort**: Sorting an array in $O(N \log N)$ time and $O(1)$ auxiliary space.

---

## Complexity Analysis

Let $N$ be the number of elements in the heap.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Insert (Push)** | $O(\log N)$ | $O(1)$ |
| **Extract Max (Pop)** | $O(\log N)$ | $O(1)$ |
| **Get Max (Top)** | $O(1)$ | $O(1)$ |
| **Heapify (Down / Up)** | $O(\log N)$ | $O(1)$ |
| **Build Heap from Array** | $O(N)$ | $O(1)$ |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

template <typename T>
class MaxHeap {
private:
    std::vector<T> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int max_idx = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left] > heap[max_idx]) {
            max_idx = left;
        }
        if (right < heap.size() && heap[right] > heap[max_idx]) {
            max_idx = right;
        }

        if (i != max_idx) {
            std::swap(heap[i], heap[max_idx]);
            heapifyDown(max_idx);
        }
    }

public:
    MaxHeap() = default;

    // Build Heap in O(N)
    MaxHeap(const std::vector<T>& arr) : heap(arr) {
        for (int i = parent(heap.size() - 1); i >= 0; --i) {
            heapifyDown(i);
        }
    }

    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }

    void push(const T& val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    T top() const {
        if (empty()) {
            throw std::underflow_error("Heap is empty");
        }
        return heap[0];
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Heap is empty");
        }
        heap[0] = heap.back();
        heap.pop_back();
        if (!empty()) {
            heapifyDown(0);
        }
    }
};

int main() {
    // Construct heap from raw vector in O(N)
    std::vector<int> arr = {10, 4, 15, 20, 8, 30};
    MaxHeap<int> heap(arr);

    std::cout << "Top element after O(N) heap construction: " << heap.top() << " (Expected: 30)\n";

    // Insert new elements
    heap.push(40);
    heap.push(5);

    std::cout << "Top element after pushing 40 and 5:      " << heap.top() << " (Expected: 40)\n";

    // Extract elements in sorted descending order
    std::cout << "Extracting elements: ";
    while (!heap.empty()) {
        std::cout << heap.top() << " ";
        heap.pop();
    }
    std::cout << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar

T = TypeVar('T')

class MaxHeap(Generic[T]):
    def __init__(self, arr: List[T] = None):
        self.heap: List[T] = []
        if arr:
            self.heap = list(arr)
            # O(N) heap construction
            for i in range(self._parent(len(self.heap) - 1), -1, -1):
                self._heapify_down(i)

    def _parent(self, i: int) -> int:
        return (i - 1) // 2

    def _left_child(self, i: int) -> int:
        return 2 * i + 1

    def _right_child(self, i: int) -> int:
        return 2 * i + 2

    def _heapify_up(self, i: int) -> None:
        while i > 0 and self.heap[self._parent(i)] < self.heap[i]:
            p = self._parent(i)
            self.heap[p], self.heap[i] = self.heap[i], self.heap[p]
            i = p

    def _heapify_down(self, i: int) -> None:
        max_idx = i
        left = self._left_child(i)
        right = self._right_child(i)

        if left < len(self.heap) and self.heap[left] > self.heap[max_idx]:
            max_idx = left
        if right < len(self.heap) and self.heap[right] > self.heap[max_idx]:
            max_idx = right

        if i != max_idx:
            self.heap[i], self.heap[max_idx] = self.heap[max_idx], self.heap[i]
            self._heapify_down(max_idx)

    def is_empty(self) -> bool:
        return len(self.heap) == 0

    def size(self) -> int:
        return len(self.heap)

    def push(self, val: T) -> None:
        """Insert element into the heap."""
        self.heap.append(val)
        self._heapify_up(len(self.heap) - 1)

    def top(self) -> T:
        """Get the maximum element (root)."""
        if self.is_empty():
            raise IndexError("top from an empty heap")
        return self.heap[0]

    def pop(self) -> T:
        """Remove and return the maximum element."""
        if self.is_empty():
            raise IndexError("pop from an empty heap")
        
        max_val = self.heap[0]
        # Replace root with last element
        self.heap[0] = self.heap[-1]
        self.heap.pop()
        
        if not self.is_empty():
            self._heapify_down(0)
            
        return max_val


if __name__ == '__main__':
    # Build heap in O(N)
    arr = [10, 4, 15, 20, 8, 30]
    heap = MaxHeap[int](arr)

    print("Top element after O(N) construction:", heap.top(), "(Expected: 30)")

    heap.push(40)
    heap.push(5)

    print("Top element after pushing 40 and 5:     ", heap.top(), "(Expected: 40)")

    # Extract elements in sorted descending order
    print("Extracting elements: ", end="")
    while not heap.is_empty():
        print(heap.pop(), end=" ")
    print()
```
