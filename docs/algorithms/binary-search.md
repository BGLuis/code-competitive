# Binary Search

## Conceptual Explanation

**Binary Search** is an efficient algorithm for locating a specific value or identifying a boundary of a condition (predicate) within a sorted collection or over a monotonic search space.

### Core Principle

Instead of scanning linearly, binary search repeatedly divides the search space in half.
At each step:
1. Find the middle element `mid`.
2. Check if `mid` satisfies the search criteria or predicate.
3. Eliminate the half of the search space in which the target/boundary cannot lie.

For a classic array search, this means adjusting the left index `L` or right index `R`.

### Monotonicity & Binary Search on the Answer

The most powerful application of binary search in competitive programming is **Binary Search on the Answer**. It is used when we want to find the minimum or maximum value $X$ such that a predicate function $P(X)$ holds, under the condition that $P(X)$ is **monotonic**.

A predicate $P(X)$ is monotonic if:
- If $P(X)$ is true, then $P(Y)$ is true for all $Y > X$ (or all $Y < X$).
- This creates a decision space resembling `[False, False, ..., False, True, True, ..., True]`.

The goal is to find the transition point (first `True` or last `True`).

### Typical Competitive Programming Use Cases

- Searching in a sorted array (e.g., finding exact matches, `lower_bound`, `upper_bound`).
- Optimization problems where checking if a solution is feasible is easier than directly computing the optimal solution (e.g., "minimize the maximum distance", "maximize the minimum weight").
- Finding roots of continuous monotonic mathematical functions.

---

## Complexity Analysis

- **Time Complexity**:
  - **Searching in Array**: $\mathcal{O}(\log N)$, as the search space size is halved at each step.
  - **Binary Search on Answer**: $\mathcal{O}(\log(\text{High} - \text{Low}) \cdot f)$, where $f$ is the time complexity of evaluating the predicate function $P(X)$.
- **Space Complexity**:
  - $\mathcal{O}(1)$ auxiliary space since the search is done iteratively.

---

## C++ Implementation

Below is a modern and clean C++ implementation demonstrating both a standard array boundary search (`lower_bound` equivalent) and binary search on the answer (e.g., the classic "Aggressive Cows" / "Distribute Items" problem).

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class BinarySearch {
public:
    // 1. Classic Binary Search: Find the first index where element >= target
    // Equivalent to std::lower_bound
    static int lower_bound(const std::vector<int>& arr, int target) {
        int low = 0;
        int high = static_cast<int>(arr.size()) - 1;
        int ans = arr.size(); // Default if all elements are less than target

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] >= target) {
                ans = mid;
                high = mid - 1; // Look for a smaller index to the left
            } else {
                low = mid + 1; // Look to the right
            }
        }
        return ans;
    }

    // 2. Binary Search on the Answer:
    // Find the maximum minimum distance to place C items in N sorted slots.
    static int max_min_distance(const std::vector<int>& positions, int num_items) {
        std::vector<int> sorted_pos = positions;
        std::sort(sorted_pos.begin(), sorted_pos.end());

        // Predicate function: Can we place all items with a minimum gap of `dist`?
        auto can_place = [&](int dist) -> bool {
            int count = 1;
            int last_placed = sorted_pos[0];
            for (size_t i = 1; i < sorted_pos.size(); ++i) {
                if (sorted_pos[i] - last_placed >= dist) {
                    count++;
                    last_placed = sorted_pos[i];
                    if (count >= num_items) {
                        return true;
                    }
                }
            }
            return false;
        };

        int low = 1; // Minimum possible distance
        int high = sorted_pos.back() - sorted_pos.front(); // Maximum possible distance
        int ans = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (can_place(mid)) {
                ans = mid;      // Mid is feasible, try to find a larger distance
                low = mid + 1;
            } else {
                high = mid - 1; // Mid is not feasible, try smaller distance
            }
        }
        return ans;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List

class BinarySearch:
    @staticmethod
    def lower_bound(arr: List[int], target: int) -> int:
        """
        Finds the first index where the element is >= target.
        """
        low = 0
        high = len(arr) - 1
        ans = len(arr)

        while low <= high:
            mid = low + (high - low) // 2
            if arr[mid] >= target:
                ans = mid
                high = mid - 1
            else:
                low = mid + 1
        return ans

    @staticmethod
    def max_min_distance(positions: List[int], num_items: int) -> int:
        """
        Finds the maximum minimum distance to place num_items in sorted positions.
        """
        sorted_pos = sorted(positions)

        def can_place(dist: int) -> bool:
            count = 1
            last_placed = sorted_pos[0]
            for i in range(1, len(sorted_pos)):
                if sorted_pos[i] - last_placed >= dist:
                    count += 1
                    last_placed = sorted_pos[i]
                    if count >= num_items:
                        return True
            return False

        low = 1
        high = sorted_pos[-1] - sorted_pos[0]
        ans = 0

        while low <= high:
            mid = low + (high - low) // 2
            if can_place(mid):
                ans = mid
                low = mid + 1
            else:
                high = mid - 1
        return ans
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // Test lower_bound
    std::vector<int> arr = {1, 3, 3, 5, 7, 8, 9};
    std::cout << "First element >= 3 at index: " << BinarySearch::lower_bound(arr, 3) << " (Expected: 1)\n";
    std::cout << "First element >= 6 at index: " << BinarySearch::lower_bound(arr, 6) << " (Expected: 4)\n";

    // Test binary search on the answer
    // E.g., place 3 items at positions {1, 2, 8, 4, 9}
    std::vector<int> positions = {1, 2, 8, 4, 9};
    int num_items = 3;
    int optimal_dist = BinarySearch::max_min_distance(positions, num_items);
    std::cout << "Maximized minimum distance: " << optimal_dist << " (Expected: 3, items at 1, 4, 8 or 1, 4, 9)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # Test lower_bound
    arr = [1, 3, 3, 5, 7, 8, 9]
    print(f"First element >= 3 at index: {BinarySearch.lower_bound(arr, 3)} (Expected: 1)")
    print(f"First element >= 6 at index: {BinarySearch.lower_bound(arr, 6)} (Expected: 4)")

    # Test binary search on the answer
    positions = [1, 2, 8, 4, 9]
    num_items = 3
    optimal_dist = BinarySearch.max_min_distance(positions, num_items)
    print(f"Maximized minimum distance: {optimal_dist} (Expected: 3)")
```
