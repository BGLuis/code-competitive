# Sweep-Line Algorithm

## Conceptual Explanation

The **Sweep-Line** algorithm is a powerful geometric paradigm. Instead of analyzing objects in a 2D plane all at once, we sweep an imaginary straight line (usually vertical) across the plane from left to right.

As the sweep-line moves, it intersects geometric objects, and we maintain an active "status" or "frontier" of objects intersecting the line. By doing so, we reduce a 2D problem into a sequence of 1D queries.

### Key Components

1. **Events**: Points on the plane where the status of the sweep-line changes (e.g., segment endpoints, point positions). Events are sorted by their X-coordinate.
2. **Event Queue**: A priority queue or sorted array that determines the order in which we process events.
3. **Active Set**: A data structure (typically a self-balancing BST, like `std::set` in C++) that stores the objects currently intersecting the sweep-line, usually ordered by their Y-coordinate.

---

### Classic Application: Closest Pair of Points

Given $N$ points in a 2D plane, find the minimum Euclidean distance between any two points. A naive search takes $\mathcal{O}(N^2)$ time. A Sweep-Line algorithm solves it in $\mathcal{O}(N \log N)$ time.

#### Sweep-Line Strategy:
1. **Sort**: Sort all points by their X-coordinates. This defines our left-to-right sweep order.
2. **Maintain best distance $d$**: Let $d$ be the minimum distance found so far. Initialize $d = \infty$.
3. **Sweep**: For each point $P(x, y)$ from left to right:
   - **Prune Active Set**: Remove points from the active set whose X-coordinate difference with $P.x$ is greater than or equal to $d$ ($P.x - A.x \ge d$). These points can never be closer than $d$ to $P$.
   - **Query Y-range**: In the active set, search for points whose Y-coordinates lie in the range $[P.y - d, P.y + d]$. We can do this efficiently using binary search (`lower_bound` on the set sorted by Y).
   - **Update $d$**: Calculate distance from $P$ to each of these candidates, and update $d$ if a smaller distance is found.
   - **Insert**: Insert $P$ into the active set.

*Why is this fast?* The number of candidate points inside a $2d \times d$ box is bounded by a constant (at most 6 points due to packing limits). Thus, each query takes $\mathcal{O}(\log N)$ time, yielding an overall time complexity of $\mathcal{O}(N \log N)$.

---

## Complexity Analysis

- **Time Complexity**: $\mathcal{O}(N \log N)$. Sorting the points takes $\mathcal{O}(N \log N)$. For each of the $N$ points, we perform insertions, deletions, and range queries on the active set, each taking $\mathcal{O}(\log N)$ time.
- **Space Complexity**: $\mathcal{O}(N)$ to store the points and the active set.

---

## C++ Implementation

Below is a robust C++ implementation of the Closest Pair of Points problem using the Sweep-Line algorithm.

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <iomanip>

struct Point {
    double x, y;
    int id;

    // Sort by X-coordinate primarily, and Y-coordinate secondarily
    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

// Custom comparator to store active points in a set sorted by Y-coordinate
struct CompareY {
    bool operator()(const Point& a, const Point& b) const {
        if (a.y != b.y) {
            return a.y < b.y;
        }
        return a.x < b.x;
    }
};

class ClosestPair {
private:
    static double euclidean_distance(const Point& a, const Point& b) {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

public:
    // Finds the closest pair of points. Returns the pair and the minimum distance.
    static std::pair<std::pair<Point, Point>, double> find_closest(std::vector<Point>& points) {
        int n = points.size();
        if (n < 2) {
            return {}; // Needs at least two points
        }

        // 1. Sort points by X-coordinate
        std::sort(points.begin(), points.end());

        double min_dist = euclidean_distance(points[0], points[1]);
        std::pair<Point, Point> best_pair = {points[0], points[1]};

        // Active set: ordered by Y-coordinate
        std::set<Point, CompareY> active_set;
        active_set.insert(points[0]);
        active_set.insert(points[1]);

        int left_idx = 0; // Index of the leftmost point in the sliding X-strip

        for (int i = 2; i < n; ++i) {
            Point cur = points[i];

            // 2. Prune active set: Remove points too far left (X-diff >= min_dist)
            while (left_idx < i && cur.x - points[left_idx].x >= min_dist) {
                active_set.erase(points[left_idx]);
                left_idx++;
            }

            // 3. Binary search on Y-coordinates: range [cur.y - min_dist, cur.y + min_dist]
            Point lower_bound_point = {-2e9, cur.y - min_dist, -1};
            auto it = active_set.lower_bound(lower_bound_point);

            while (it != active_set.end() && it->y <= cur.y + min_dist) {
                double d = euclidean_distance(cur, *it);
                if (d < min_dist) {
                    min_dist = d;
                    best_pair = {cur, *it};
                }
                it++;
            }

            // 4. Insert current point into active set
            active_set.insert(cur);
        }

        return {best_pair, min_dist};
    }
};
```

---

## Python Implementation

Here is the Python 3 implementation with type hints.

```python
from typing import List, Tuple
import math

class Point:
    def __init__(self, x: float, y: float, point_id: int):
        self.x = x
        self.y = y
        self.id = point_id

    def __lt__(self, other: 'Point') -> bool:
        if self.x != other.x:
            return self.x < other.x
        return self.y < other.y

def euclidean_distance(a: Point, b: Point) -> float:
    return math.sqrt((a.x - b.x) ** 2 + (a.y - b.y) ** 2)

class ClosestPair:
    @staticmethod
    def find_closest(points_list: List[Tuple[float, float]]) -> Tuple[Tuple[Point, Point], float]:
        """
        Finds the closest pair of points in O(N log N) using Sweep-Line.
        """
        n = len(points_list)
        if n < 2:
            raise ValueError("Need at least two points")

        points = [Point(x, y, i) for i, (x, y) in enumerate(points_list)]
        # 1. Sort points by X-coordinate
        points.sort()

        min_dist = euclidean_distance(points[0], points[1])
        best_pair = (points[0], points[1])

        # Active list sorted by Y-coordinate
        # (In Python, maintaining a sorted list can be done using a list + bisect,
        # or manual insertion, as Python does not have a built-in Balanced BST set).
        active_set: List[Point] = [points[0], points[1]]
        active_set.sort(key=lambda p: (p.y, p.x))

        left_idx = 0

        for i in range(2, n):
            cur = points[i]

            # 2. Prune active set: Remove points with X-diff >= min_dist
            while left_idx < i and cur.x - points[left_idx].x >= min_dist:
                active_set.remove(points[left_idx])
                left_idx += 1

            # 3. Search Y-range [cur.y - min_dist, cur.y + min_dist]
            # Find starting index using binary search on Y
            low_y = cur.y - min_dist
            high_y = cur.y + min_dist

            # Binary search to find first point with point.y >= low_y
            low = 0
            high = len(active_set) - 1
            start_idx = len(active_set)
            while low <= high:
                mid = low + (high - low) // 2
                if active_set[mid].y >= low_y:
                    start_idx = mid
                    high = mid - 1
                else:
                    low = mid + 1

            # Scan candidates
            idx = start_idx
            while idx < len(active_set) and active_set[idx].y <= high_y:
                d = euclidean_distance(cur, active_set[idx])
                if d < min_dist:
                    min_dist = d
                    best_pair = (cur, active_set[idx])
                idx += 1

            # 4. Insert current point sorted by Y
            # Find correct index to insert cur in active_set sorted by Y
            insert_idx = len(active_set)
            low = 0
            high = len(active_set) - 1
            while low <= high:
                mid = low + (high - low) // 2
                if active_set[mid].y >= cur.y:
                    insert_idx = mid
                    high = mid - 1
                else:
                    low = mid + 1
            active_set.insert(insert_idx, cur)

        return best_pair, min_dist
```

---

## Usage Examples

### C++ Usage Example

```cpp
// Compile standard: g++ -O3 -std=c++17 main.cpp
int main() {
    std::vector<Point> points = {
        {2.0, 3.0, 0},
        {12.0, 30.0, 1},
        {40.0, 50.0, 2},
        {5.0, 1.0, 3},
        {12.0, 10.0, 4},
        {3.0, 4.0, 5}
    };

    auto result = ClosestPair::find_closest(points);
    auto pair = result.first;
    double dist = result.second;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Closest Pair: Point #" << pair.first.id 
              << " (" << pair.first.x << ", " << pair.first.y << ") and Point #" 
              << pair.second.id << " (" << pair.second.x << ", " << pair.second.y << ")\n";
    std::cout << "Minimum Distance: " << dist << " (Expected: 1.414214)\n";

    return 0;
}
```

### Python Usage Example

```python
if __name__ == "__main__":
    points = [
        (2.0, 3.0),
        (12.0, 30.0),
        (40.0, 50.0),
        (5.0, 1.0),
        (12.0, 10.0),
        (3.0, 4.0)
    ]

    pair, dist = ClosestPair.find_closest(points)

    print(f"Closest Pair: Point #{pair[0].id} ({pair[0].x}, {pair[0].y}) and Point #{pair[1].id} ({pair[1].x}, {pair[1].y})")
    print(f"Minimum Distance: {dist:.6f} (Expected: 1.414214)")
```
