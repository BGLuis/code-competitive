# Insert Sorted

## Conceptual Explanation

Maintaining a collection of elements in sorted order on the fly is a common requirement in competitive programming. The process of inserting a new element into an already sorted collection while maintaining its sorted property is known as **Insert Sorted**.

Depending on the underlying data structures, this operation behaves differently in terms of complexity and implementation.

### Approaches

#### 1. Sorted Dynamic Arrays (C++ `std::vector`, Python `list`)
If we maintain a sorted dynamic array:
- We first use **binary search** to find the correct index where the element should be placed (e.g., using `std::lower_bound` in C++ or `bisect.insort` in Python).
- We then insert the element at that index.
- *Pros*: $\mathcal{O}(1)$ random access, extremely fast lookup, cache-friendly.
- *Cons*: Shifting elements to make space for the new item takes $\mathcal{O}(N)$ time.

#### 2. Balanced Binary Search Trees (C++ `std::set` or `std::multiset`)
If we use a self-balancing binary search tree:
- Inserting a new node automatically finds the correct position and rebalances the tree.
- *Pros*: Both search and insertion take $\mathcal{O}(\log N)$ time.
- *Cons*: High constant factor overhead, no $\mathcal{O}(1)$ random access to index.

---

## Complexity Analysis

| Data Structure | Search / Find Position | Shift / Rebalance | Total Insertion Complexity | Random Access |
| :--- | :--- | :--- | :--- | :--- |
| **Sorted Array (Vector/List)** | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ |
| **Balanced BST (Set)** | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ |

---

## C++ Implementation

Below is a complete C++ implementation illustrating:
1. Inserting into a sorted `std::vector` (using `std::lower_bound`).
2. Inserting into a sorted `std::multiset` (allows duplicates) to achieve $\mathcal{O}(\log N)$ insert.

```cpp
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class SortedCollection {
private:
    std::vector<int> arr;
    std::multiset<int> bst; // Allows duplicates

public:
    SortedCollection() = default;

    // 1. Insert into Vector maintaining sorted order
    // Time complexity: O(N) due to element shifting
    void insert_vector(int val) {
        // Binary search to find the first element >= val
        auto it = std::lower_bound(arr.begin(), arr.end(), val);
        arr.insert(it, val);
    }

    // 2. Insert into Multiset maintaining sorted order
    // Time complexity: O(log N)
    void insert_set(int val) {
        bst.insert(val);
    }

    const std::vector<int>& get_vector() const {
        return arr;
    }

    std::vector<int> get_set_elements() const {
        return std::vector<int>(bst.begin(), bst.end());
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints using the built-in `bisect` library to insert into a sorted list.

```python
from typing import List
import bisect

class SortedCollection:
    def __init__(self):
        self.arr: List[int] = []

    def insert(self, val: int) -> None:
        """
        Inserts val into self.arr while maintaining sorted order using binary search.
        Time Complexity: O(N) due to list shifting.
        """
        # bisect.insort performs binary search followed by list insertion
        bisect.insort(self.arr, val)

    def insert_manual(self, val: int) -> None:
        """
        Alternative manual implementation demonstrating the underlying steps.
        """
        # Find index of the first element >= val
        idx = bisect.bisect_left(self.arr, val)
        self.arr.insert(idx, val)

    def get_elements(self) -> List[int]:
        return self.arr
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    SortedCollection collector;

    // Insert into vector
    collector.insert_vector(5);
    collector.insert_vector(1);
    collector.insert_vector(8);
    collector.insert_vector(3);
    collector.insert_vector(3); // Duplicate

    std::cout << "Vector Elements (Sorted): ";
    for (int x : collector.get_vector()) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Insert into multiset
    collector.insert_set(10);
    collector.insert_set(4);
    collector.insert_set(9);
    collector.insert_set(4); // Duplicate

    std::cout << "Multiset Elements (Sorted): ";
    for (int x : collector.get_set_elements()) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    collector = SortedCollection()

    # Insert items
    collector.insert(5)
    collector.insert(1)
    collector.insert(8)
    collector.insert(3)
    collector.insert_manual(3)  # Manual insert of duplicate

    print("Sorted List Elements:", collector.get_elements())
```
