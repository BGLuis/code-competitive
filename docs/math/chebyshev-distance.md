# Chebyshev Distance

## Conceptual Explanation
The **Chebyshev Distance** (also known as the $L_\infty$ metric or chessboard distance) between two points $A(x_1, y_1)$ and $B(x_2, y_2)$ is defined as the maximum of the absolute differences of their coordinates:
$$D_{\text{Chebyshev}}(A, B) = \max(|x_1 - x_2|, |y_1 - y_2|)$$
It represents the minimum number of moves a king requires to travel between two squares on a chessboard, since a king can move horizontally, vertically, or diagonally.

### The Manhattan-Chebyshev Duality
In competitive programming, the most powerful aspect of Chebyshev distance is its relationship with the **Manhattan Distance** ($L_1$ metric):
$$D_{\text{Manhattan}}(A, B) = |x_1 - x_2| + |y_1 - y_2|$$

We can transform coordinates to convert Manhattan distance queries into Chebyshev distance queries (and vice versa). This is highly beneficial because the Chebyshev formula $\max(|x'_1 - x'_2|, |y'_1 - y'_2|)$ allows the coordinates to be analyzed **independently**.

#### 1. Manhattan to Chebyshev Transformation
If we transform every point $(x, y)$ to $(x + y, x - y)$:
$$(x, y) \to (x', y') = (x + y, x - y)$$
Then the Manhattan distance between two points in the original coordinates is exactly equal to the Chebyshev distance between the transformed points:
$$D_{\text{Manhattan}}(A, B) = D_{\text{Chebyshev}}(A', B') = \max(|x'_1 - x'_2|, |y'_1 - y'_2|)$$

#### 2. Chebyshev to Manhattan Transformation
Conversely, if we want to convert a Chebyshev distance to Manhattan distance, we can apply:
$$(x, y) \to \left(\frac{x + y}{2}, \frac{x - y}{2}\right)$$
To avoid dealing with floating-point divisions in integer coordinates, we often scale by 2:
$$2 \cdot D_{\text{Chebyshev}}(A, B) = D_{\text{Manhattan}}((x_1 + y_1, x_1 - y_1), (x_2 + y_2, x_2 - y_2))$$

#### Typical Use Case: Maximum Pairwise Distance
To find the maximum Manhattan distance between any pair of points in a set of $N$ points, a naive check takes $O(N^2)$. By transforming all points to Chebyshev coordinates $(x', y')$, the maximum distance is:
$$\max_{i, j} D_{\text{Manhattan}}(P_i, P_j) = \max\left( \max_i x'_i - \min_i x'_i, \max_i y'_i - \min_i y'_i \right)$$
This can be calculated in $O(N)$ time!

---

## Complexity Analysis
- **Distance calculation (between 2 points)**: $O(1)$ time and space.
- **Coordinate transformation**: $O(1)$ time and space per point.
- **Maximum pairwise Manhattan distance**: $O(N)$ time and $O(1)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    long long x, y;
};

// Calculate standard Chebyshev distance
long long chebyshev_distance(const Point& a, const Point& b) {
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

// Calculate standard Manhattan distance
long long manhattan_distance(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Transform point from Manhattan to Chebyshev coordinates: (x, y) -> (x + y, x - y)
Point transform_manhattan_to_chebyshev(const Point& p) {
    return Point{p.x + p.y, p.x - p.y};
}

// Transform point from Chebyshev to Manhattan coordinates with scaling by 2: (x, y) -> (x + y, x - y)
Point transform_chebyshev_to_scaled_manhattan(const Point& p) {
    return Point{p.x + p.y, p.x - p.y};
}

// Find the maximum Manhattan distance between any pair of points in O(N)
long long max_pairwise_manhattan(const std::vector<Point>& points) {
    if (points.size() < 2) return 0;
    
    long long min_u = points[0].x + points[0].y;
    long long max_u = min_u;
    long long min_v = points[0].x - points[0].y;
    long long max_v = min_v;

    for (const auto& p : points) {
        long long u = p.x + p.y;
        long long v = p.x - p.y;
        min_u = std::min(min_u, u);
        max_u = std::max(max_u, u);
        min_v = std::min(min_v, v);
        max_v = std::max(max_v, v);
    }
    return std::max(max_u - min_u, max_v - min_v);
}

// Find the maximum Chebyshev distance between any pair of points in O(N)
long long max_pairwise_chebyshev(const std::vector<Point>& points) {
    if (points.size() < 2) return 0;

    long long min_x = points[0].x, max_x = points[0].x;
    long long min_y = points[0].y, max_y = points[0].y;

    for (const auto& p : points) {
        min_x = std::min(min_x, p.x);
        max_x = std::max(max_x, p.x);
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
    }
    return std::max(max_x - min_x, max_y - min_y);
}
```

---

## Python Implementation

```python
from typing import List


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __repr__(self) -> str:
        return f"Point({self.x}, {self.y})"


def chebyshev_distance(a: Point, b: Point) -> int:
    """Calculates Chebyshev distance between two points."""
    return max(abs(a.x - b.x), abs(a.y - b.y))


def manhattan_distance(a: Point, b: Point) -> int:
    """Calculates Manhattan distance between two points."""
    return abs(a.x - b.x) + abs(a.y - b.y)


def transform_manhattan_to_chebyshev(p: Point) -> Point:
    """Transforms point for Manhattan -> Chebyshev equivalence."""
    return Point(p.x + p.y, p.x - p.y)


def max_pairwise_manhattan(points: List[Point]) -> int:
    """Finds maximum Manhattan distance between any pair in O(N)."""
    if len(points) < 2:
        return 0

    u = [p.x + p.y for p in points]
    v = [p.x - p.y for p in points]
    return max(max(u) - min(u), max(v) - min(v))


def max_pairwise_chebyshev(points: List[Point]) -> int:
    """Finds maximum Chebyshev distance between any pair in O(N)."""
    if len(points) < 2:
        return 0

    xs = [p.x for p in points]
    ys = [p.y for p in points]
    return max(max(xs) - min(xs), max(ys) - min(ys))
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    Point a{2, 3};
    Point b{5, 7};

    std::cout << "Point A: (2, 3), Point B: (5, 7)\n";
    std::cout << "Chebyshev Distance: " << chebyshev_distance(a, b) << " (expected 4)\n";
    std::cout << "Manhattan Distance: " << manhattan_distance(a, b) << " (expected 7)\n";

    // Transform points to Chebyshev coordinates
    Point a_prime = transform_manhattan_to_chebyshev(a);
    Point b_prime = transform_manhattan_to_chebyshev(b);
    std::cout << "Transformed A: (" << a_prime.x << ", " << a_prime.y << ")\n";
    std::cout << "Transformed B: (" << b_prime.x << ", " << b_prime.y << ")\n";
    std::cout << "Chebyshev distance of transformed points: " 
              << chebyshev_distance(a_prime, b_prime) << " (expected 7, equal to Manhattan!)\n";

    // O(N) max pairwise distance check
    std::vector<Point> points = { {1, 5}, {2, 2}, {5, 9}, {8, 1} };
    std::cout << "Max Pairwise Manhattan Distance: " << max_pairwise_manhattan(points) << "\n";
    std::cout << "Max Pairwise Chebyshev Distance: " << max_pairwise_chebyshev(points) << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    a = Point(2, 3)
    b = Point(5, 7)

    print("Point A: (2, 3), Point B: (5, 7)")
    print(f"Chebyshev Distance: {chebyshev_distance(a, b)} (expected 4)")
    print(f"Manhattan Distance: {manhattan_distance(a, b)} (expected 7)")

    a_prime = transform_manhattan_to_chebyshev(a)
    b_prime = transform_manhattan_to_chebyshev(b)
    print(f"Transformed A: {a_prime}")
    print(f"Transformed B: {b_prime}")
    print(
        f"Chebyshev distance of transformed: {chebyshev_distance(a_prime, b_prime)} (expected 7)"
    )

    points = [Point(1, 5), Point(2, 2), Point(5, 9), Point(8, 1)]
    print(f"Max Pairwise Manhattan: {max_pairwise_manhattan(points)}")
    print(f"Max Pairwise Chebyshev: {max_pairwise_chebyshev(points)}")
```
