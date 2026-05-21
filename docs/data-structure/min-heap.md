# Min Heap

## Conceptual Explanation

A **Min Heap** is a complete binary tree that satisfies the **heap property**: the value of each node is greater than or equal to the value of its parent. Consequently, the root of the tree always contains the minimum element in the heap.

### Core Logic & Operations
A Min Heap is efficiently represented as an array (0-indexed) where for any element at index $i$:
- **Parent**: `(i - 1) / 2`
- **Left Child**: `2 * i + 1`
- **Right Child**: `2 * i + 2`

Main operations:
1. **Insert (Push)**: Append the new element to the end of the array. Restores the min-heap property by executing **heapifyUp** (bubble up/sift up): compare the element with its parent and swap if the element is smaller. Repeat until the parent is smaller or the element becomes the root.
2. **Extract Min (Pop)**: Replace the root (minimum element) with the last element in the array, then remove the last element. Next, restore the heap property by running **heapifyDown** (bubble down/sift down): compare the new root with its children, swap with the smaller child if any child is smaller, and repeat recursively down the tree.
3. **Get Min (Top)**: Return the root element in $O(1)$ time.

### Typical Competitive Programming Use Cases
- **Dijkstra's Algorithm**: Extracting the vertex with the minimum distance in $O(\log V)$ time.
- **Prim's Algorithm**: Finding the minimum weight edge to expand the Minimum Spanning Tree (MST).
- **Top-K Smallest Elements / K-way Merge**: Efficiently merging multiple sorted lists.

---

## Complexity Analysis

Let $N$ be the number of elements in the heap.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Insert (Push)** | $O(\log N)$ | $O(1)$ |
| **Extract Min (Pop)** | $O(\log N)$ | $O(1)$ |
| **Get Min (Top)** | $O(1)$ | $O(1)$ |
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
class MinHeap {
private:
    std::vector<T> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int min_idx = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left] < heap[min_idx]) {
            min_idx = left;
        }
        if (right < heap.size() && heap[right] < heap[min_idx]) {
            min_idx = right;
        }

        if (i != min_idx) {
            std::swap(heap[i], heap[min_idx]);
            heapifyDown(min_idx);
        }
    }

public:
    MinHeap() = default;

    // Build Heap in O(N)
    MinHeap(const std::vector<T>& arr) : heap(arr) {
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
    MinHeap<int> heap(arr);

    std::cout << "Top element after O(N) heap construction: " << heap.top() << " (Expected: 4)\n";

    // Insert new elements
    heap.push(2);
    heap.push(25);

    std::cout << "Top element after pushing 2 and 25:      " << heap.top() << " (Expected: 2)\n";

    // Extract elements in sorted ascending order
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

class MinHeap(Generic[T]):
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
        while i > 0 and self.heap[self._parent(i)] > self.heap[i]:
            p = self._parent(i)
            self.heap[p], self.heap[i] = self.heap[i], self.heap[p]
            i = p

    def _heapify_down(self, i: int) -> None:
        min_idx = i
        left = self._left_child(i)
        right = self._right_child(i)

        if left < len(self.heap) and self.heap[left] < self.heap[min_idx]:
            min_idx = left
        if right < len(self.heap) and self.heap[right] < self.heap[min_idx]:
            min_idx = right

        if i != min_idx:
            self.heap[i], self.heap[min_idx] = self.heap[min_idx], self.heap[i]
            self._heapify_down(min_idx)

    def is_empty(self) -> bool:
        return len(self.heap) == 0

    def size(self) -> int:
        return len(self.heap)

    def push(self, val: T) -> None:
        """Insert element into the heap."""
        self.heap.append(val)
        self._heapify_up(len(self.heap) - 1)

    def top(self) -> T:
        """Get the minimum element (root)."""
        if self.is_empty():
            raise IndexError("top from an empty heap")
        return self.heap[0]

    def pop(self) -> T:
        """Remove and return the minimum element."""
        if self.is_empty():
            raise IndexError("pop from an empty heap")
        
        min_val = self.heap[0]
        # Replace root with last element
        self.heap[0] = self.heap[-1]
        self.heap.pop()
        
        if not self.is_empty():
            self._heapify_down(0)
            
        return min_val


if __name__ == '__main__':
    # Build heap in O(N)
    arr = [10, 4, 15, 20, 8, 30]
    heap = MinHeap[int](arr)

    print("Top element after O(N) construction:", heap.top(), "(Expected: 4)")

    heap.push(2)
    heap.push(25)

    print("Top element after pushing 2 and 25:     ", heap.top(), "(Expected: 2)")

    # Extract elements in sorted ascending order
    print("Extracting elements: ", end="")
    while not heap.is_empty():
        print(heap.pop(), end=" ")
    print()
```
