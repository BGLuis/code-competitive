# Bitset and Bitmask

## Conceptual Explanation

In competitive programming, **Bitmasking** is a technique where integers are treated as binary representations of subsets. A **Bitset** is a fixed-size sequence of $N$ bits that provides space-efficient and fast bitwise operations.

### Core Logic & Operations
1. **Representing Subsets**: A subset of a set with $N$ elements can be represented by an integer mask of length $N$. If the $i$-th element is present, the $i$-th bit is $1$; otherwise, it is $0$.
2. **Basic Bitwise Operations**:
   - **Check if $i$-th bit is set**: `(mask & (1 << i)) != 0`
   - **Set the $i$-th bit to 1**: `mask |= (1 << i)`
   - **Clear the $i$-th bit to 0**: `mask &= ~(1 << i)`
   - **Toggle the $i$-th bit**: `mask ^= (1 << i)`
   - **Union of subsets**: `maskA | maskB`
   - **Intersection of subsets**: `maskA & maskB`
   - **Iterate over all subsets**: `for (int s = mask; s > 0; s = (s - 1) & mask)`
3. **Std::bitset (C++)**: An optimization over boolean arrays. A `std::bitset<N>` takes only $1$ bit per element (compared to 8 bits for `bool` or `std::vector<bool>`) and allows $O(N / W)$ parallel bitwise operations (where $W$ is the word size of the CPU, typically 64).

### Typical Competitive Programming Use Cases
- **Bitmask Dynamic Programming**: Solving problems like the Traveling Salesperson Problem (TSP) in $O(2^N \cdot N^2)$.
- **Set Operations Optimization**: Speeding up reachability queries in graphs (e.g., transitive closure in $O(N^3 / 64)$ via Floyd-Warshall with bitsets).
- **Knapsack Speedup**: Fast subset sum checking using bitsets.

---

## Complexity Analysis

| Operation | Bitmask (Integer) | Std::bitset / Custom Bitset |
| :--- | :--- | :--- |
| **Get / Set / Flip 1 bit** | $O(1)$ | $O(1)$ |
| **Union / Intersection / XOR** | $O(1)$ | $O(N / 64)$ |
| **Count Active Bits (popcount)** | $O(1)$ (using HW instruction) | $O(N / 64)$ |
| **Space Complexity** | $O(1)$ (constant integer size) | $O(N)$ bits |

---

## C++ Implementation

```cpp
#include <iostream>
#include <bitset>
#include <vector>
#include <string>

// Helper to print binary representation of an integer mask
void printMask(int mask, int len = 5) {
    for (int i = len - 1; i >= 0; --i) {
        std::cout << ((mask >> i) & 1);
    }
    std::cout << " (value: " << mask << ")\n";
}

int main() {
    // --- SECTION 1: Standard Integer Bitmasks ---
    std::cout << "=== Integer Bitmask Operations ===\n";
    int mask = 0; // Empty set

    // 1. Add element 1 and 3 to the set
    mask |= (1 << 1);
    mask |= (1 << 3);
    std::cout << "Set elements 1 and 3: ";
    printMask(mask);

    // 2. Check if element 3 is in set
    bool hasThree = (mask & (1 << 3)) != 0;
    std::cout << "Has element 3? " << (hasThree ? "Yes" : "No") << "\n";

    // 3. Clear element 1
    mask &= ~(1 << 1);
    std::cout << "Cleared element 1:    ";
    printMask(mask);

    // 4. Toggle element 2 (was 0, becomes 1)
    mask ^= (1 << 2);
    std::cout << "Toggled element 2:    ";
    printMask(mask);

    // 5. Built-in population count (count set bits)
    std::cout << "Number of elements:   " << __builtin_popcount(mask) << "\n";

    // 6. Submask iteration (useful for DP)
    std::cout << "\nIterating over all submasks of base mask (101 in binary):\n";
    int base_mask = 5; // elements 0 and 2 are set
    for (int s = base_mask; s > 0; s = (s - 1) & base_mask) {
        printMask(s, 3);
    }
    printMask(0, 3); // Include the empty submask

    // --- SECTION 2: C++ std::bitset ---
    std::cout << "\n=== std::bitset Operations ===\n";
    constexpr int BITSET_SIZE = 10;
    std::bitset<BITSET_SIZE> bs1("0000001101"); // from string
    std::bitset<BITSET_SIZE> bs2;

    // Set individual bits
    bs2.set(1); // Set index 1 to true
    bs2.set(3); // Set index 3 to true
    bs2.set(5); // Set index 5 to true

    std::cout << "bs1: " << bs1 << "\n";
    std::cout << "bs2: " << bs2 << "\n";

    // Bitwise ops (highly optimized, 64 bits at a time)
    auto bs_and = bs1 & bs2;
    auto bs_or = bs1 | bs2;
    auto bs_xor = bs1 ^ bs2;

    std::cout << "AND: " << bs_and << "\n";
    std::cout << "OR:  " << bs_or << "\n";
    std::cout << "XOR: " << bs_xor << "\n";

    // Useful member functions
    std::cout << "bs1 set count: " << bs1.count() << "\n";
    std::cout << "bs1 has any bit set? " << (bs1.any() ? "Yes" : "No") << "\n";
    std::cout << "bs1 has all bits set? " << (bs1.all() ? "Yes" : "No") << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List

def print_mask(mask: int, length: int = 5) -> None:
    """Helper to print binary representation of an integer mask."""
    binary_str = bin(mask)[2:].zfill(length)
    # Python displays MSB on left. We match standard output.
    print(f"{binary_str[-length:]} (value: {mask})")

if __name__ == '__main__':
    # --- SECTION 1: Standard Integer Bitmasks ---
    print("=== Integer Bitmask Operations ===")
    mask = 0  # Empty set

    # 1. Add element 1 and 3 to the set
    mask |= (1 << 1)
    mask |= (1 << 3)
    print("Set elements 1 and 3: ", end="")
    print_mask(mask)

    # 2. Check if element 3 is in set
    has_three = (mask & (1 << 3)) != 0
    print(f"Has element 3? {'Yes' if has_three else 'No'}")

    # 3. Clear element 1
    mask &= ~(1 << 1)
    print("Cleared element 1:    ", end="")
    print_mask(mask)

    # 4. Toggle element 2 (was 0, becomes 1)
    mask ^= (1 << 2)
    print("Toggled element 2:    ", end="")
    print_mask(mask)

    # 5. Population count (count set bits)
    pop_count = bin(mask).count('1')
    print(f"Number of elements:   {pop_count}")

    # 6. Submask iteration (useful for DP)
    print("\nIterating over all submasks of base mask (101 in binary):")
    base_mask = 5  # elements 0 and 2 are set
    s = base_mask
    while s > 0:
        print_mask(s, 3)
        s = (s - 1) & base_mask
    print_mask(0, 3)  # Include empty submask

    # --- SECTION 2: Large Bitsets using Python BigInt ---
    # Python integers have arbitrary precision (unlimited bits),
    # meaning they can act as an infinite bitset out of the box!
    print("\n=== Large Python Bitset Operations ===")
    
    # Let's simulate a large bitset of 100 elements
    bs_a = (1 << 10) | (1 << 50) | (1 << 90)
    bs_b = (1 << 50) | (1 << 95)

    # Fast bitwise operations
    intersection = bs_a & bs_b
    union = bs_a | bs_b
    xor_diff = bs_a ^ bs_b

    print(f"Intersection bits set at: {[i for i in range(100) if (intersection & (1 << i))]}")
    print(f"Union bits set count:     {bin(union).count('1')}")
    print(f"XOR unique bits count:    {bin(xor_diff).count('1')}")
```
