# Interval Intersection and Merging

## Conceptual Explanation
An **Interval** represents a continuous range of values between a starting point $L$ and an ending point $R$. In competitive programming, we frequently work with sets of intervals (representing time ranges, coordinate spans, or segment segments) and need to perform set-like operations:
- **Merging**: Combining overlapping or adjacent intervals into a minimal list of disjoint intervals.
- **Intersection**: Finding the overlapping regions between two different sets of intervals.

### Core Algorithmic Operations

#### 1. Merging Intervals
To merge a list of arbitrary intervals:
1. Sort the intervals primarily by their start times $L$, and secondarily by their end times $R$.
2. Iterate through the sorted list, maintaining the "active" merged interval.
3. For each interval, if it starts after the active interval ends, the active interval is finalized, and the current interval becomes the new active one. Otherwise, we merge them by extending the active interval's end to the maximum of both ends.

#### 2. Intersection of Two Sorted Disjoint Lists (Two-Pointer Technique)
Given two lists of intervals, $A$ and $B$, where each list is individually sorted and consists of pairwise disjoint intervals, we want to find their intersection in $O(N + M)$ time:
1. Initialize two pointers $i = 0$ and $j = 0$ for lists $A$ and $B$.
2. The intersection of $A[i] = [L_A, R_A]$ and $B[j] = [L_B, R_B]$ is:
   $$\text{Overlap} = [\max(L_A, L_B), \min(R_A, R_B)]$$
   If this overlap is valid (i.e., $\max(L_A, L_B) \le \min(R_A, R_B)$), we add it to our results.
3. We increment the pointer of the interval that ends earlier. That is, if $R_A < R_B$, we increment $i$; otherwise, we increment $j$.

---

## Complexity Analysis
- **Merge Intervals**:
  - **Time Complexity**: $O(N \log N)$ due to sorting. The linear scan takes $O(N)$.
  - **Space Complexity**: $O(N)$ to store the merged output, or $O(1)$ auxiliary space if sorted in place.
- **Sorted Lists Intersection**:
  - **Time Complexity**: $O(N + M)$ since each step advances at least one pointer, visiting each interval at most once.
  - **Space Complexity**: $O(\min(N, M))$ for storing the resulting intersection intervals.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

struct Interval {
    int l, r;

    // Check if two individual intervals intersect
    bool intersects(const Interval& other) const {
        return std::max(l, other.l) <= std::min(r, other.r);
    }

    // Get the intersection of two individual intervals
    Interval get_intersection(const Interval& other) const {
        return Interval{std::max(l, other.l), std::min(r, other.r)};
    }
};

// Merge overlapping/adjacent intervals in O(N log N)
std::vector<Interval> merge_intervals(std::vector<Interval> intervals) {
    if (intervals.empty()) return {};

    // Sort intervals by start point
    std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        if (a.l != b.l) return a.l < b.l;
        return a.r < b.r;
    });

    std::vector<Interval> merged;
    Interval active = intervals[0];

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].l <= active.r) {
            // Overlapping or adjacent, expand active interval
            active.r = std::max(active.r, intervals[i].r);
        } else {
            // Gap detected, push active and start new
            merged.push_back(active);
            active = intervals[i];
        }
    }
    merged.push_back(active);
    return merged;
}

// Find intersection of two sorted, disjoint interval lists in O(N + M)
std::vector<Interval> intersect_interval_lists(const std::vector<Interval>& A, const std::vector<Interval>& B) {
    std::vector<Interval> result;
    size_t i = 0, j = 0;

    while (i < A.size() && j < B.size()) {
        int start = std::max(A[i].l, B[j].l);
        int end = std::min(A[i].r, B[j].r);

        if (start <= end) {
            result.push_back(Interval{start, end});
        }

        // Advance pointer of interval that ends earlier
        if (A[i].r < B[j].r) {
            i++;
        } else {
            j++;
        }
    }
    return result;
}
```

---

## Python Implementation

```python
from typing import List


class Interval:
    def __init__(self, l: int, r: int):
        self.l = l
        self.r = r

    def intersects(self, other: "Interval") -> bool:
        """Returns True if this interval intersects with other."""
        return max(self.l, other.l) <= min(self.r, other.r)

    def get_intersection(self, other: "Interval") -> "Interval":
        """Returns the intersection of two intervals."""
        return Interval(max(self.l, other.l), min(self.r, other.r))

    def __repr__(self) -> str:
        return f"[{self.l}, {self.r}]"


def merge_intervals(intervals: List[Interval]) -> List[Interval]:
    """Merges overlapping intervals in O(N log N)."""
    if not intervals:
        return []

    # Sort primarily by start, secondarily by end
    sorted_intervals = sorted(intervals, key=lambda x: (x.l, x.r))

    merged = []
    active = sorted_intervals[0]

    for curr in sorted_intervals[1:]:
        if curr.l <= active.r:
            # Overlapping or adjacent, expand
            active.r = max(active.r, curr.r)
        else:
            # Disjoint, push active and swap
            merged.append(active)
            active = curr

    merged.append(active)
    return merged


def intersect_interval_lists(
    A: List[Interval], B: List[Interval]
) -> List[Interval]:
    """Finds intersection of two sorted, disjoint interval lists in O(N + M)."""
    result = []
    i, j = 0, 0

    while i < len(A) and j < len(B):
        start = max(A[i].l, B[j].l)
        end = min(A[i].r, B[j].r)

        if start <= end:
            result.append(Interval(start, end))

        if A[i].r < B[j].r:
            i += 1
        else:
            j += 1

    return result
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // 1. Merging Intervals
    std::vector<Interval> raw_intervals = {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}, {9, 12}
    };

    std::vector<Interval> merged = merge_intervals(raw_intervals);
    std::cout << "Merged Intervals:\n";
    for (const auto& val : merged) {
        std::cout << "[" << val.l << ", " << val.r << "] ";
    }
    std::cout << "\n(Expected: [1, 6] [8, 12] [15, 18])\n\n";

    // 2. Intersecting Two Sorted Lists
    std::vector<Interval> A = {{0, 2}, {5, 10}, {13, 23}, {24, 25}};
    std::vector<Interval> B = {{1, 5}, {8, 12}, {15, 24}};

    std::vector<Interval> intersect = intersect_interval_lists(A, B);
    std::cout << "Intersection of lists A and B:\n";
    for (const auto& val : intersect) {
        std::cout << "[" << val.l << ", " << val.r << "] ";
    }
    std::cout << "\n(Expected: [1, 2] [5, 5] [8, 10] [15, 23] [24, 24])\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # 1. Merging Intervals
    raw = [Interval(1, 3), Interval(2, 6), Interval(8, 10), Interval(15, 18), Interval(9, 12)]
    merged = merge_intervals(raw)
    print(f"Merged Intervals: {merged}")
    print("Expected: [[1, 6], [8, 12], [15, 18]]\n")

    # 2. Intersecting Sorted Lists
    A = [Interval(0, 2), Interval(5, 10), Interval(13, 23), Interval(24, 25)]
    B = [Interval(1, 5), Interval(8, 12), Interval(15, 24)]
    intersect = intersect_interval_lists(A, B)
    print(f"Intersection: {intersect}")
    print("Expected: [[1, 2], [5, 5], [8, 10], [15, 23], [24, 24]]")
```
