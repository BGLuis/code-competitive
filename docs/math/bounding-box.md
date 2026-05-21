# Bounding Box

## Conceptual Explanation
A **Bounding Box** is the smallest geometric container (typically a rectangle in 2D) that encloses a given set of points or shapes. In competitive programming, we primarily use two variants:
1. **Axis-Aligned Bounding Box (AABB)**: The box's edges are parallel to the coordinate axes. It is extremely simple to compute and fast to query.
2. **Oriented Bounding Box (OBB)**: The box can be rotated arbitrarily to fit the enclosed points more tightly. OBB calculations typically require algorithms like Convex Hull followed by **Rotating Calipers** to find the minimum area/perimeter enclosing rectangle.

### Operations on AABB
- **Construction**: Finding the minimum and maximum $x$ and $y$ coordinates among a set of points:
  $$x_{\min} = \min(x_i), \quad x_{\max} = \max(x_i), \quad y_{\min} = \min(y_i), \quad y_{\max} = \max(y_i)$$
- **Containment Check**: A point $(p_x, p_y)$ is inside the AABB if:
  $$x_{\min} \le p_x \le x_{\max} \quad \text{and} \quad y_{\min} \le p_y \le y_{\max}$$
- **Intersection check**: Two boxes $A$ and $B$ intersect if:
  $$A.x_{\min} \le B.x_{\max} \quad \text{and} \quad A.x_{\max} \ge B.x_{\min} \quad \text{and} \quad A.y_{\min} \le B.y_{\max} \quad \text{and} \quad A.y_{\max} \ge B.y_{\min}$$
- **Merge/Union**: Creating a single AABB that encloses two boxes $A$ and $B$:
  $$\text{Merged}.x_{\min} = \min(A.x_{\min}, B.x_{\min}), \quad \text{Merged}.x_{\max} = \max(A.x_{\max}, B.x_{\max})$$
  $$\text{Merged}.y_{\min} = \min(A.y_{\min}, B.y_{\min}), \quad \text{Merged}.y_{\max} = \max(A.y_{\max}, B.y_{\max})$$

---

## Complexity Analysis
- **Construction from $N$ points**: $O(N)$ time complexity, $O(1)$ auxiliary space.
- **Intersection and Containment Checks**: $O(1)$ time complexity, $O(1)$ space.
- **Merge/Union**: $O(1)$ time complexity, $O(1)$ space.
- **Intersection Area**: $O(1)$ time complexity, $O(1)$ space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iomanip>

struct Point {
    double x, y;
};

class BoundingBox {
public:
    double x_min, x_max;
    double y_min, y_max;

    // Construct an empty/invalid bounding box
    BoundingBox() {
        x_min = std::numeric_limits<double>::infinity();
        x_max = -std::numeric_limits<double>::infinity();
        y_min = std::numeric_limits<double>::infinity();
        y_max = -std::numeric_limits<double>::infinity();
    }

    // Construct a bounding box from a single point
    BoundingBox(const Point& p) : x_min(p.x), x_max(p.x), y_min(p.y), y_max(p.y) {}

    // Construct a bounding box from coordinates
    BoundingBox(double xmin, double xmax, double ymin, double ymax)
        : x_min(xmin), x_max(xmax), y_min(ymin), y_max(ymax) {}

    // Add a point to the bounding box, expanding it if necessary
    void expand_to_include(const Point& p) {
        x_min = std::min(x_min, p.x);
        x_max = std::max(x_max, p.x);
        y_min = std::min(y_min, p.y);
        y_max = std::max(y_max, p.y);
    }

    // Merge this bounding box with another
    BoundingBox get_union(const BoundingBox& other) const {
        return BoundingBox(
            std::min(x_min, other.x_min),
            std::max(x_max, other.x_max),
            std::min(y_min, other.y_min),
            std::max(y_max, other.y_max)
        );
    }

    // Check if this bounding box intersects with another
    bool intersects(const BoundingBox& other) const {
        return x_min <= other.x_max && x_max >= other.x_min &&
               y_min <= other.y_max && y_max >= other.y_min;
    }

    // Compute the intersection bounding box
    BoundingBox get_intersection(const BoundingBox& other) const {
        if (!intersects(other)) {
            return BoundingBox(); // Return empty box
        }
        return BoundingBox(
            std::max(x_min, other.x_min),
            std::min(x_max, other.x_max),
            std::max(y_min, other.y_min),
            std::min(y_max, other.y_max)
        );
    }

    // Check if a point is inside the bounding box
    bool contains(const Point& p) const {
        return p.x >= x_min && p.x <= x_max && p.y >= y_min && p.y <= y_max;
    }

    // Calculate area of the bounding box
    double area() const {
        if (x_min >= x_max || y_min >= y_max) return 0.0;
        return (x_max - x_min) * (y_max - y_min);
    }

    // Calculate perimeter
    double perimeter() const {
        if (x_min >= x_max || y_min >= y_max) return 0.0;
        return 2.0 * ((x_max - x_min) + (y_max - y_min));
    }
};
```

---

## Python Implementation

```python
from typing import List, Optional
import math


class Point:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

    def __repr__(self) -> str:
        return f"Point({self.x}, {self.y})"


class BoundingBox:
    def __init__(
        self,
        x_min: float = float("inf"),
        x_max: float = float("-inf"),
        y_min: float = float("inf"),
        y_max: float = float("-inf"),
    ):
        self.x_min = x_min
        self.x_max = x_max
        self.y_min = y_min
        self.y_max = y_max

    @classmethod
    def from_points(cls, points: List[Point]) -> "BoundingBox":
        """Creates a bounding box enclosing a list of points."""
        box = cls()
        for p in points:
            box.expand_to_include(p)
        return box

    def expand_to_include(self, p: Point) -> None:
        """Expands the bounding box to include point p."""
        self.x_min = min(self.x_min, p.x)
        self.x_max = max(self.x_max, p.x)
        self.y_min = min(self.y_min, p.y)
        self.y_max = max(self.y_max, p.y)

    def get_union(self, other: "BoundingBox") -> "BoundingBox":
        """Returns the union of this box and another box."""
        return BoundingBox(
            min(self.x_min, other.x_min),
            max(self.x_max, other.x_max),
            min(self.y_min, other.y_min),
            max(self.y_max, other.y_max),
        )

    def intersects(self, other: "BoundingBox") -> bool:
        """Returns True if this box intersects with another box."""
        return (
            self.x_min <= other.x_max
            and self.x_max >= other.x_min
            and self.y_min <= other.y_max
            and self.y_max >= other.y_min
        )

    def get_intersection(self, other: "BoundingBox") -> "BoundingBox":
        """Returns the overlapping bounding box of this box and another."""
        if not self.intersects(other):
            return BoundingBox()
        return BoundingBox(
            max(self.x_min, other.x_min),
            min(self.x_max, other.x_max),
            max(self.y_min, other.y_min),
            min(self.y_max, other.y_max),
        )

    def contains(self, p: Point) -> bool:
        """Returns True if point p is inside the bounding box."""
        return self.x_min <= p.x <= self.x_max and self.y_min <= p.y <= self.y_max

    def area(self) -> float:
        """Returns the area of the bounding box."""
        if self.x_min >= self.x_max or self.y_min >= self.y_max:
            return 0.0
        return (self.x_max - self.x_min) * (self.y_max - self.y_min)

    def perimeter(self) -> float:
        """Returns the perimeter of the bounding box."""
        if self.x_min >= self.x_max or self.y_min >= self.y_max:
            return 0.0
        return 2.0 * ((self.x_max - self.x_min) + (self.y_max - self.y_min))

    def __repr__(self) -> str:
        return f"BoundingBox(x:[{self.x_min}, {self.x_max}], y:[{self.y_min}, {self.y_max}])"
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    // Construct some points
    std::vector<Point> points = {
        {1.0, 2.0},
        {5.0, -1.0},
        {-2.0, 4.0},
        {3.0, 3.0}
    };

    // Initialize bounding box
    BoundingBox box;
    for (const auto& p : points) {
        box.expand_to_include(p);
    }

    std::cout << "Bounding Box: [" << box.x_min << ", " << box.x_max 
              << "] x [" << box.y_min << ", " << box.y_max << "]\n";
    std::cout << "Area: " << box.area() << "\n";
    std::cout << "Perimeter: " << box.perimeter() << "\n";

    // Create a second box
    BoundingBox other(0.0, 6.0, 0.0, 5.0);

    // Test intersection
    if (box.intersects(other)) {
        BoundingBox intersect = box.get_intersection(other);
        std::cout << "Boxes intersect!\n";
        std::cout << "Overlap Area: " << intersect.area() << "\n";
    } else {
        std::cout << "Boxes do not intersect.\n";
    }

    // Containment check
    Point test_point{2.0, 2.0};
    std::cout << "Box contains (2, 2)? " << (box.contains(test_point) ? "Yes" : "No") << "\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    # Construct some points
    points = [
        Point(1.0, 2.0),
        Point(5.0, -1.0),
        Point(-2.0, 4.0),
        Point(3.0, 3.0),
    ]

    # Create bounding box from points
    box1 = BoundingBox.from_points(points)
    print(f"Bounding Box 1: {box1}")
    print(f"Area: {box1.area()}")
    print(f"Perimeter: {box1.perimeter()}")

    # Create a second bounding box
    box2 = BoundingBox(0.0, 6.0, 0.0, 5.0)
    print(f"Bounding Box 2: {box2}")

    # Check intersection and union
    if box1.intersects(box2):
        overlap = box1.get_intersection(box2)
        print(f"Overlap Bounding Box: {overlap}")
        print(f"Overlap Area: {overlap.area()}")

    merged = box1.get_union(box2)
    print(f"Merged Bounding Box: {merged}")
```
