# 2D Geometry Primitives

## Conceptual Explanation
Computational geometry is a fundamental pillar of competitive programming. Working in 2D Euclidean space requires robust, standard primitives to handle vectors, lines, and polygons without running into floating-point issues or edge-case failures.

### Core Concepts

#### 1. Points and Vectors
A point $P(x, y)$ can double as a vector from the origin. We implement standard addition, subtraction, and scaling.

#### 2. Dot Product
For vectors $\vec{u}$ and $\vec{v}$:
$$\vec{u} \cdot \vec{v} = u_x v_x + u_y v_y = |\vec{u}| |\vec{v}| \cos(\theta)$$
- If $> 0$: angle is acute ($< 90^\circ$).
- If $= 0$: vectors are perpendicular.
- If $< 0$: angle is obtuse ($> 90^\circ$).

#### 3. Cross Product (2D)
For vectors $\vec{u}$ and $\vec{v}$:
$$\vec{u} \times \vec{v} = u_x v_y - u_y v_x = |\vec{u}| |\vec{v}| \sin(\theta)$$
The signed value represents the area of the parallelogram spanned by the vectors.
- If $> 0$: $\vec{v}$ is counter-clockwise (to the left) of $\vec{u}$.
- If $< 0$: $\vec{v}$ is clockwise (to the right) of $\vec{u}$.
- If $= 0$: $\vec{u}$ and $\vec{v}$ are collinear.

#### 4. Orientation / CCW Test
Given three points $A, B, C$, we can find the direction of the turn $A \to B \to C$ by computing the cross product of vectors $\vec{AB}$ and $\vec{AC}$:
$$\text{orient}(A, B, C) = (B_x - A_x)(C_y - A_y) - (B_y - A_y)(C_x - A_x)$$
- $> 0$: Counter-Clockwise (left turn).
- $< 0$: Clockwise (right turn).
- $= 0$: Collinear.

#### 5. Segment-Segment Intersection
Two segments $AB$ and $CD$ intersect if and only if:
- They are collinear and their projections overlap, or
- $C$ and $D$ lie on different sides of line $AB$, AND $A$ and $B$ lie on different sides of line $CD$.
This is verified using the CCW test.

#### 6. Polygon Area (Shoelace Formula)
For a simple polygon with vertices $P_1, P_2, \dots, P_n$ in order:
$$\text{Area} = \frac{1}{2} \left| \sum_{i=1}^n (x_i y_{i+1} - x_{i+1} y_i) \right|, \quad \text{where } P_{n+1} = P_1$$

---

## Complexity Analysis
- **Vector Operations, Dot/Cross Product, CCW Test**: $O(1)$ time and space.
- **Segment Intersection**: $O(1)$ time and space.
- **Point-to-Segment Distance**: $O(1)$ time and space.
- **Polygon Area**: $O(N)$ time, $O(1)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const double EPS = 1e-9;

struct Point {
    double x, y;

    Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y}; }
    Point operator*(double scalar) const { return {x * scalar, y * scalar}; }
    Point operator/(double scalar) const { return {x / scalar, y / scalar}; }

    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
    }
};

// Dot product of vectors A and B
double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

// Cross product of vectors A and B (2D)
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

// Signed area of triangle ABC (multiplied by 2)
double cross(Point a, Point b, Point c) {
    return cross(b - a, c - a);
}

// CCW / Orientation test: +1 for CCW, -1 for CW, 0 for collinear
int orient(Point a, Point b, Point c) {
    double val = cross(a, b, c);
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

// Check if point P lies on segment AB
bool on_segment(Point p, Point a, Point b) {
    return std::abs(cross(a - p, b - p)) < EPS &&
           dot(a - p, b - p) <= EPS;
}

// Check if segment AB and CD intersect
bool segment_intersection(Point a, Point b, Point c, Point d) {
    int o1 = orient(a, b, c);
    int o2 = orient(a, b, d);
    int o3 = orient(c, d, a);
    int o4 = orient(c, d, b);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && on_segment(c, a, b)) return true;
    if (o2 == 0 && on_segment(d, a, b)) return true;
    if (o3 == 0 && on_segment(a, c, d)) return true;
    if (o4 == 0 && on_segment(b, c, d)) return true;

    return false;
}

// Find the intersection point of lines AB and CD (assuming they intersect)
Point line_intersection(Point a, Point b, Point c, Point d) {
    double cp1 = cross(d - c, a - c);
    double cp2 = cross(d - c, b - c);
    return a + (b - a) * (cp1 / (cp1 - cp2));
}

// Point to segment AB distance
double point_to_segment_distance(Point p, Point a, Point b) {
    if (a == b) return std::hypot(p.x - a.x, p.y - a.y);
    double factor = dot(p - a, b - a) / dot(b - a, b - a);
    if (factor < 0.0) return std::hypot(p.x - a.x, p.y - a.y);
    if (factor > 1.0) return std::hypot(p.x - b.x, p.y - b.y);
    Point projection = a + (b - a) * factor;
    return std::hypot(p.x - projection.x, p.y - projection.y);
}

// Calculate area of a simple polygon using Shoelace formula
double polygon_area(const std::vector<Point>& vertices) {
    double area = 0.0;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) {
        Point curr = vertices[i];
        Point next = vertices[(i + 1) % n];
        area += cross(curr, next);
    }
    return std::abs(area) / 2.0;
}
```

---

## Python Implementation

```python
from typing import List
import math

EPS = 1e-9


class Point:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

    def __add__(self, other: "Point") -> "Point":
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other: "Point") -> "Point":
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar: float) -> "Point":
        return Point(self.x * scalar, self.y * scalar)

    def __truediv__(self, scalar: float) -> "Point":
        return Point(self.x / scalar, self.y / scalar)

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Point):
            return False
        return abs(self.x - other.x) < EPS and abs(self.y - other.y) < EPS

    def __repr__(self) -> str:
        return f"Point({self.x}, {self.y})"


def dot(a: Point, b: Point) -> float:
    return a.x * b.x + a.y * b.y


def cross(a: Point, b: Point) -> float:
    return a.x * b.y - a.y * b.x


def cross_three(a: Point, b: Point, c: Point) -> float:
    return cross(b - a, c - a)


def orient(a: Point, b: Point, c: Point) -> int:
    val = cross_three(a, b, c)
    if val > EPS:
        return 1
    if val < -EPS:
        return -1
    return 0


def on_segment(p: Point, a: Point, b: Point) -> bool:
    return abs(cross_three(p, a, b)) < EPS and dot(a - p, b - p) <= EPS


def segment_intersection(a: Point, b: Point, c: Point, d: Point) -> bool:
    o1 = orient(a, b, c)
    o2 = orient(a, b, d)
    o3 = orient(c, d, a)
    o4 = orient(c, d, b)

    if o1 != o2 and o3 != o4:
        return True

    if o1 == 0 and on_segment(c, a, b):
        return True
    if o2 == 0 and on_segment(d, a, b):
        return True
    if o3 == 0 and on_segment(a, c, d):
        return True
    if o4 == 0 and on_segment(b, c, d):
        return True

    return False


def line_intersection(a: Point, b: Point, c: Point, d: Point) -> Point:
    cp1 = cross(d - c, a - c)
    cp2 = cross(d - c, b - c)
    return a + (b - a) * (cp1 / (cp1 - cp2))


def point_to_segment_distance(p: Point, a: Point, b: Point) -> float:
    if a == b:
        return math.hypot(p.x - a.x, p.y - a.y)
    factor = dot(p - a, b - a) / dot(b - a, b - a)
    if factor < 0.0:
        return math.hypot(p.x - a.x, p.y - a.y)
    if factor > 1.0:
        return math.hypot(p.x - b.x, p.y - b.y)
    projection = a + (b - a) * factor
    return math.hypot(p.x - projection.x, p.y - projection.y)


def polygon_area(vertices: List[Point]) -> float:
    area = 0.0
    n = len(vertices)
    for i in range(n):
        curr = vertices[i]
        nxt = vertices[(i + 1) % n]
        area += cross(curr, nxt)
    return abs(area) / 2.0
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    Point a{0.0, 0.0};
    Point b{4.0, 0.0};
    Point c{4.0, 3.0};
    Point d{0.0, 3.0};

    // Calculate Polygon Area (a 4x3 rectangle)
    std::vector<Point> rect = {a, b, c, d};
    std::cout << "Rectangle Area: " << polygon_area(rect) << " (expected 12.0)\n";

    // Test Segment Intersection
    Point p1{2.0, -1.0};
    Point p2{2.0, 4.0};
    Point s1{0.0, 1.0};
    Point s2{4.0, 1.0};

    if (segment_intersection(p1, p2, s1, s2)) {
        Point intersect = line_intersection(p1, p2, s1, s2);
        std::cout << "Segments intersect at: (" << intersect.x << ", " << intersect.y << ")\n";
    } else {
        std::cout << "Segments do not intersect.\n";
    }

    // Distance from Point to Segment
    Point target{2.0, 5.0};
    double dist = point_to_segment_distance(target, a, b);
    std::cout << "Distance from (2, 5) to segment [(0,0), (4,0)]: " << dist << " (expected 5.0)\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    a = Point(0.0, 0.0)
    b = Point(4.0, 0.0)
    c = Point(4.0, 3.0)
    d = Point(0.0, 3.0)

    # Polygon Area (Rectangle 4x3)
    rect = [a, b, c, d]
    print(f"Rectangle Area: {polygon_area(rect)} (expected 12.0)")

    # Segment Intersection
    p1 = Point(2.0, -1.0)
    p2 = Point(2.0, 4.0)
    s1 = Point(0.0, 1.0)
    s2 = Point(4.0, 1.0)

    if segment_intersection(p1, p2, s1, s2):
        intersect = line_intersection(p1, p2, s1, s2)
        print(f"Segments intersect at: {intersect}")
    else:
        print("Segments do not intersect.")

    # Distance from Point to Segment
    target = Point(2.0, 5.0)
    dist = point_to_segment_distance(target, a, b)
    print(
        f"Distance from (2, 5) to segment [(0,0), (4,0)]: {dist} (expected 5.0)"
    )
```
