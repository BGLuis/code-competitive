# Greedy Algorithm

## Conceptual Explanation

A **Greedy Algorithm** is an algorithmic paradigm that builds a solution step-by-step, making the locally optimal choice at each stage in the hope of finding a globally optimal solution.

### Core Principle

Greedy algorithms do not backtrack or look at the big picture. They make a choice that looks best *right now* and stick with it. For a greedy algorithm to be correct, the problem must exhibit two main properties:

1. **Greedy Choice Property**: A global optimum can be arrived at by making locally optimal (greedy) choices without ever needing to reconsider past choices.
2. **Optimal Substructure**: An optimal solution to the overall problem contains optimal solutions to its subproblems.

### Proving Correctness

Proving that a greedy strategy is correct can be challenging. Standard techniques include:
- **Greedy Stays Ahead**: Proving that at each step, the greedy choice is at least as good as any other potential choice.
- **Exchange Argument**: Showing that any optimal solution can be iteratively transformed into the greedy solution without worsening its quality.

### Canonical Example: Interval Scheduling (Activity Selection)

Given $N$ activities, each with a start time $s_i$ and end time $f_i$, we want to find the maximum number of non-overlapping activities we can perform.

**The Greedy Choice**: Always pick the next activity that **ends as early as possible** ($f_i$ is minimized) and does not overlap with previously selected activities.
*Why it works*: Choosing the activity that ends first leaves the maximum amount of time for remaining activities.

---

## Complexity Analysis

- **Time Complexity**:
  - **Sorting**: $\mathcal{O}(N \log N)$ to sort the activities by their end times.
  - **Linear Scan**: $\mathcal{O}(N)$ to sweep through the sorted activities and greedily pick non-overlapping ones.
  - **Total**: $\mathcal{O}(N \log N)$.
- **Space Complexity**:
  - $\mathcal{O}(N)$ (or $\mathcal{O}(1)$ auxiliary space if sorted in place) to store intervals.

---

## C++ Implementation

Below is a robust and clear C++ implementation of the Activity Selection Problem.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

struct Interval {
    int start;
    int end;
    int id;
};

class IntervalScheduler {
public:
    // Finds the maximum subset of mutually compatible intervals.
    static std::vector<Interval> schedule(const std::vector<std::pair<int, int>>& intervals) {
        int n = intervals.size();
        std::vector<Interval> sorted_intervals(n);
        for (int i = 0; i < n; ++i) {
            sorted_intervals[i] = {intervals[i].first, intervals[i].second, i};
        }

        // Greedy strategy: Sort by finish (end) time in ascending order
        std::sort(sorted_intervals.begin(), sorted_intervals.end(), [](const Interval& a, const Interval& b) {
            if (a.end != b.end) {
                return a.end < b.end;
            }
            return a.start < b.start;
        });

        std::vector<Interval> selected;
        if (sorted_intervals.empty()) {
            return selected;
        }

        // Always select the first activity (which ends earliest)
        selected.push_back(sorted_intervals[0]);
        int last_end_time = sorted_intervals[0].end;

        for (int i = 1; i < n; ++i) {
            // If the activity starts at or after the end of the last selected activity
            if (sorted_intervals[i].start >= last_end_time) {
                selected.push_back(sorted_intervals[i]);
                last_end_time = sorted_intervals[i].end;
            }
        }

        return selected;
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple

class Interval:
    def __init__(self, start: int, end: int, item_id: int):
        self.start = start
        self.end = end
        self.id = item_id

class IntervalScheduler:
    @staticmethod
    def schedule(intervals: List[Tuple[int, int]]) -> List[Interval]:
        """
        Finds the maximum set of non-overlapping intervals.
        """
        objs = [Interval(start, end, idx) for idx, (start, end) in enumerate(intervals)]

        # Greedy choice: Sort by end time ascending
        objs.sort(key=lambda x: (x.end, x.start))

        selected: List[Interval] = []
        if not objs:
            return selected

        # Select the first interval
        selected.append(objs[0])
        last_end_time = objs[0].end

        for i in range(1, len(objs)):
            if objs[i].start >= last_end_time:
                selected.append(objs[i])
                last_end_time = objs[i].end

        return selected
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    // List of (start_time, end_time) pairs
    std::vector<std::pair<int, int>> intervals = {
        {1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 8}, {5, 9}, {6, 10}, {8, 11}, {8, 12}, {2, 13}, {12, 14}
    };

    std::vector<Interval> selected = IntervalScheduler::schedule(intervals);

    std::cout << "Maximum number of non-overlapping activities: " << selected.size() << "\n\n";
    std::cout << "Selected Activities Details:\n";
    for (const auto& act : selected) {
        std::cout << "Activity ID: " << act.id 
                  << " [Start: " << act.start 
                  << ", End: " << act.end << "]\n";
    }

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    # List of (start_time, end_time) pairs
    intervals = [
        (1, 4), (3, 5), (0, 6), (5, 7), (3, 8), (5, 9), (6, 10), (8, 11), (8, 12), (2, 13), (12, 14)
    ]

    selected = IntervalScheduler.schedule(intervals)

    print(f"Maximum number of non-overlapping activities: {len(selected)}\n")
    print("Selected Activities Details:")
    for act in selected:
        print(f"Activity ID: {act.id} [Start: {act.start}, End: {act.end}]")
```
