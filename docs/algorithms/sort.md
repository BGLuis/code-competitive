# Sorting Algorithms (Quick Sort and Merge Sort)

## Conceptual Explanation

Sorting is the process of rearranging a collection of elements into a specific order (typically ascending or descending). In competitive programming, standard library sorting functions (like `std::sort` in C++ or `.sort()` in Python) are almost always preferred due to their high optimizations. However, implementing and understanding two fundamental divide-and-conquer sorting algorithms—**Quick Sort** and **Merge Sort**—is critical for solving advanced custom sorting problems and computing tree/range properties.

---

### 1. Quick Sort

Quick Sort is an in-place, divide-and-conquer sorting algorithm.

#### Core Principle: Partitioning
1. **Choose a Pivot**: Select an element from the array as the pivot (e.g., the middle element, a random element, or using the median-of-three). Selecting a random or median pivot is crucial to avoid the worst-case $\mathcal{O}(N^2)$ complexity on already sorted inputs.
2. **Partitioning**: Rearrange the array such that all elements smaller than the pivot are placed to its left, and all elements larger than the pivot are placed to its right.
3. **Recursive Step**: Recursively apply the same steps to the left and right subarrays.

---

### 2. Merge Sort & Inversion Counting

Merge Sort is a stable, divide-and-conquer sorting algorithm.

#### Core Principle: Merging
1. **Divide**: Split the array into two equal halves.
2. **Conquer**: Recursively sort both halves.
3. **Combine (Merge)**: Merge the two sorted halves back into a single sorted array.

#### Inversion Counting
An **inversion** is a pair of indices $(i, j)$ such that $i < j$ and $arr[i] > arr[j]$.
Merge Sort can be easily adapted to count the total number of inversions in an array in $\mathcal{O}(N \log N)$ time.
During the merge step, if we find that the element in the right subarray ($R[j]$) is smaller than the element in the left subarray ($L[i]$), it means $R[j]$ is smaller than *all* remaining elements in the left subarray (since $L$ is sorted). We can simply add the number of remaining elements in $L$ (which is $\text{size}(L) - i$) to our inversion count!

---

## Complexity Analysis

| Algorithm | Average Time | Worst-Case Time | Auxiliary Space | Stable | In-Place |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Quick Sort** | $\mathcal{O}(N \log N)$ | $\mathcal{O}(N^2)$ (rare with random pivot) | $\mathcal{O}(\log N)$ (call stack) | No | Yes |
| **Merge Sort** | $\mathcal{O}(N \log N)$ | $\mathcal{O}(N \log N)$ | $\mathcal{O}(N)$ | Yes | No |

---

## C++ Implementation

Below is a complete C++ class containing:
1. In-place Randomized Quick Sort.
2. Merge Sort that counts and returns the number of inversions.

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Sorter {
private:
    // Helper partition for Quick Sort
    static int partition(std::vector<int>& arr, int low, int high) {
        // Choose a random pivot to avoid O(N^2) worst case
        int pivot_idx = low + std::rand() % (high - low + 1);
        std::swap(arr[pivot_idx], arr[high]);

        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    static void quick_sort_helper(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quick_sort_helper(arr, low, pi - 1);
            quick_sort_helper(arr, pi + 1, high);
        }
    }

    // Helper merge and count for Merge Sort
    static long long merge_and_count(std::vector<int>& arr, std::vector<int>& temp, int left, int mid, int right) {
        int i = left;    // Left subarray pointer
        int j = mid + 1; // Right subarray pointer
        int k = left;    // Merged temp array pointer
        long long inv_count = 0;

        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
                // Core Inversion Counting step:
                // Since left subarray is sorted, all elements from i to mid are > arr[j]
                inv_count += (mid - i + 1);
            }
        }

        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        while (j <= right) {
            temp[k++] = arr[j++];
        }

        // Copy back to original array
        for (i = left; i <= right; i++) {
            arr[i] = temp[i];
        }

        return inv_count;
    }

    static long long merge_sort_helper(std::vector<int>& arr, std::vector<int>& temp, int left, int right) {
        long long inv_count = 0;
        if (left < right) {
            int mid = left + (right - left) / 2;

            inv_count += merge_sort_helper(arr, temp, left, mid);
            inv_count += merge_sort_helper(arr, temp, mid + 1, right);
            inv_count += merge_and_count(arr, temp, left, mid, right);
        }
        return inv_count;
    }

public:
    // 1. Randomized In-Place Quick Sort
    static void quick_sort(std::vector<int>& arr) {
        std::srand(std::time(nullptr));
        if (arr.empty()) return;
        quick_sort_helper(arr, 0, arr.size() - 1);
    }

    // 2. Merge Sort with Inversion Count
    static long long merge_sort_and_count_inversions(std::vector<int>& arr) {
        if (arr.empty()) return 0;
        std::vector<int> temp(arr.size());
        return merge_sort_helper(arr, temp, 0, arr.size() - 1);
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple
import random

class Sorter:
    @staticmethod
    def quick_sort(arr: List[int]) -> None:
        """
        Sorts the list in-place using randomized Quick Sort.
        """
        if not arr:
            return
        Sorter._quick_sort_helper(arr, 0, len(arr) - 1)

    @staticmethod
    def _quick_sort_helper(arr: List[int], low: int, high: int) -> None:
        if low < high:
            pi = Sorter._partition(arr, low, high)
            Sorter._quick_sort_helper(arr, low, pi - 1)
            Sorter._quick_sort_helper(arr, pi + 1, high)

    @staticmethod
    def _partition(arr: List[int], low: int, high: int) -> int:
        pivot_idx = random.randint(low, high)
        arr[pivot_idx], arr[high] = arr[high], arr[pivot_idx]

        pivot = arr[high]
        i = low - 1

        for j in range(low, high):
            if arr[j] < pivot:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]
        
        arr[i + 1], arr[high] = arr[high], arr[i + 1]
        return i + 1

    @staticmethod
    def merge_sort_and_count_inversions(arr: List[int]) -> int:
        """
        Sorts the list and returns the total inversion count using Merge Sort.
        """
        if not arr:
            return 0
        temp = [0] * len(arr)
        return Sorter._merge_sort_helper(arr, temp, 0, len(arr) - 1)

    @staticmethod
    def _merge_sort_helper(arr: List[int], temp: List[int], left: int, right: int) -> int:
        inv_count = 0
        if left < right:
            mid = left + (right - left) // 2
            inv_count += Sorter._merge_sort_helper(arr, temp, left, mid)
            inv_count += Sorter._merge_sort_helper(arr, temp, mid + 1, right)
            inv_count += Sorter._merge_and_count(arr, temp, left, mid, right)
        return inv_count

    @staticmethod
    def _merge_and_count(arr: List[int], temp: List[int], left: int, mid: int, right: int) -> int:
        i = left
        j = mid + 1
        k = left
        inv_count = 0

        while i <= mid and j <= right:
            if arr[i] <= arr[j]:
                temp[k] = arr[i]
                i += 1
            else:
                temp[k] = arr[j]
                j += 1
                inv_count += (mid - i + 1)
            k += 1

        while i <= mid:
            temp[k] = arr[i]
            i += 1
            k += 1

        while j <= right:
            temp[k] = arr[j]
            j += 1
            k += 1

        for idx in range(left, right + 1):
            arr[idx] = temp[idx]

        return inv_count
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    std::vector<int> arr1 = {8, 4, 2, 1, 5, 7, 3, 6};
    std::vector<int> arr2 = {8, 4, 2, 1, 5, 7, 3, 6};

    // Test Quick Sort
    Sorter::quick_sort(arr1);
    std::cout << "Quick Sorted Array: ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "\n";

    // Test Merge Sort and Inversion Counting
    long long invs = Sorter::merge_sort_and_count_inversions(arr2);
    std::cout << "Merge Sorted Array: ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Number of Inversions: " << invs << " (Expected: 17)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    arr1 = [8, 4, 2, 1, 5, 7, 3, 6]
    arr2 = [8, 4, 2, 1, 5, 7, 3, 6]

    # Test Quick Sort
    Sorter.quick_sort(arr1)
    print("Quick Sorted Array: ", arr1)

    # Test Merge Sort and Inversions
    invs = Sorter.merge_sort_and_count_inversions(arr2)
    print("Merge Sorted Array: ", arr2)
    print("Number of Inversions: ", invs, "(Expected: 17)")
```
