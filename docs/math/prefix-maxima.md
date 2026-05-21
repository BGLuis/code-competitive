# Prefix Maxima and Monotonic Structures

## Conceptual Explanation

### 1. Prefix Maxima
Given an array $A$ of size $N$, the **Prefix Maxima** array $P$ is defined as:
$$P[i] = \max_{0 \le j \le i} A[j]$$
An element $A[i]$ is called a **prefix maximum** (or outstanding element) if it is strictly greater than all elements before it:
$$A[i] > \max_{0 \le j < i} A[j]$$
(With $A[0]$ being a trivial prefix maximum). Counting or identifying these outstanding elements is a classic subproblem.

---

### 2. Monotonic Stack (Next Greater Element)
A natural extension of prefix maxima is finding the **Next Greater Element (NGE)** for every element in an array. For each index $i$, we want to find the smallest index $j > i$ such that $A[j] > A[i]$.
A naive search takes $O(N^2)$ time. Using a **Monotonic Stack** (a stack that maintains its elements in a strictly decreasing/increasing order), we can solve this in $O(N)$ time:
1. Traverse the array from right to left (or left to right, adapting indices).
2. Pop elements from the stack that are smaller than or equal to the current element $A[i]$.
3. If the stack is empty, there is no NGE (return $-1$ or $N$). Otherwise, the top of the stack is the NGE.
4. Push the current element's index onto the stack.

---

### 3. Monotonic Queue (Sliding Window Maximum)
For a sliding window of size $K$ moving across an array $A$, we want to find the maximum in each window.
Using a **Monotonic Queue** (implemented via a double-ended queue `std::deque`), we maintain elements in a decreasing order of values. For each element $A[i]$:
1. Remove elements from the back of the deque that are smaller than or equal to $A[i]$ (since they can never be the maximum of any window containing $A[i]$).
2. Push the index $i$ to the back of the deque.
3. Remove the index from the front of the deque if it has fallen outside the window ($< i - K + 1$).
4. The maximum of the current window is always at the front of the deque: $A[\text{deque.front()}]$.

---

## Complexity Analysis
- **Prefix Maxima calculation**: $O(N)$ time and $O(N)$ space (or $O(1)$ auxiliary space).
- **Next Greater Element**: $O(N)$ time since each element is pushed and popped from the stack at most once. $O(N)$ auxiliary space.
- **Sliding Window Maximum**: $O(N)$ time since each element index enters and leaves the deque at most once. $O(K)$ auxiliary space.

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stack>
#include <deque>
#include <algorithm>

// 1. Prefix Maxima Computation
// Returns the prefix maxima array, and populates outstanding_indices
struct PrefixMaximaResult {
    std::vector<int> prefix_max;
    std::vector<int> outstanding_indices; // Indices where A[i] is a new maximum
};

PrefixMaximaResult compute_prefix_maxima(const std::vector<int>& A) {
    if (A.empty()) return {};
    int n = A.size();
    std::vector<int> P(n);
    std::vector<int> indices;

    P[0] = A[0];
    indices.push_back(0);
    int current_max = A[0];

    for (int i = 1; i < n; ++i) {
        P[i] = std::max(P[i - 1], A[i]);
        if (A[i] > current_max) {
            current_max = A[i];
            indices.push_back(i);
        }
    }
    return {P, indices};
}

// 2. Next Greater Element in O(N) using Monotonic Stack
// Returns a vector of indices of the next greater element (or -1 if none exists)
std::vector<int> next_greater_element(const std::vector<int>& A) {
    int n = A.size();
    std::vector<int> nge(n, -1);
    std::stack<int> s; // Stores indices

    for (int i = n - 1; i >= 0; --i) {
        // Pop elements that are smaller or equal to the current element
        while (!s.empty() && A[s.top()] <= A[i]) {
            s.pop();
        }
        if (!s.empty()) {
            nge[i] = s.top(); // Store the index of the NGE
        }
        s.push(i);
    }
    return nge;
}

// 3. Sliding Window Maximum in O(N) using Monotonic Queue (Deque)
std::vector<int> sliding_window_maximum(const std::vector<int>& A, int k) {
    int n = A.size();
    if (n == 0 || k <= 0) return {};
    std::vector<int> result;
    std::deque<int> dq; // Stores indices of elements

    for (int i = 0; i < n; ++i) {
        // Remove elements that are out of the current window
        if (!dq.empty() && dq.front() < i - k + 1) {
            dq.pop_front();
        }
        // Remove elements from the back that are smaller than the current element
        while (!dq.empty() && A[dq.back()] <= A[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        // Store the maximum of the window once we have processed at least k elements
        if (i >= k - 1) {
            result.push_back(A[dq.front()]);
        }
    }
    return result;
}
```

---

## Python Implementation

```python
from collections import deque
from typing import Dict, List, Tuple


def compute_prefix_maxima(A: List[int]) -> Tuple[List[int], List[int]]:
    """Computes prefix maxima and returns (P, indices_of_outstanding)."""
    if not A:
        return [], []
    n = len(A)
    P = [0] * n
    P[0] = A[0]
    indices = [0]
    current_max = A[0]

    for i in range(1, n):
        P[i] = max(P[i - 1], A[i])
        if A[i] > current_max:
            current_max = A[i]
            indices.append(i)

    return P, indices


def next_greater_element(A: List[int]) -> List[int]:
    """Finds index of the next greater element for each index in O(N).

    Returns -1 if none exists.
    """
    n = len(A)
    nge = [-1] * n
    stack = []  # Stores indices

    for i in range(n - 1, -1, -1):
        while stack and A[stack[-1]] <= A[i]:
            stack.pop()
        if stack:
            nge[i] = stack[-1]
        stack.append(i)

    return nge


def sliding_window_maximum(A: List[int], k: int) -> List[int]:
    """Finds maximum of every sliding window of size k in O(N)."""
    if not A or k <= 0:
        return []
    n = len(A)
    result = []
    dq = deque()  # Stores indices

    for i in range(n):
        # Remove indices out of range
        if dq and dq[0] < i - k + 1:
            dq.popleft()
        # Remove indices of smaller values
        while dq and A[dq[-1]] <= A[i]:
            dq.pop()
        dq.append(i)

        if i >= k - 1:
            result.append(A[dq[0]])

    return result
```

---

## Usage Examples

### C++ Example
```cpp
int main() {
    std::vector<int> A = {2, 1, 5, 6, 2, 3, 4, 7};

    // 1. Prefix Maxima
    auto pm = compute_prefix_maxima(A);
    std::cout << "Original Array: ";
    for (int x : A) std::cout << x << " ";
    std::cout << "\nPrefix Maxima:  ";
    for (int x : pm.prefix_max) std::cout << x << " ";
    std::cout << "\nOutstanding Indices (new max): ";
    for (int idx : pm.outstanding_indices) std::cout << idx << " (value " << A[idx] << ") ";
    std::cout << "\n\n";

    // 2. Next Greater Element
    std::vector<int> nge = next_greater_element(A);
    std::cout << "Next Greater Element Indices:\n";
    for (int i = 0; i < A.size(); ++i) {
        std::cout << "Element " << A[i] << " at index " << i << " -> NGE is ";
        if (nge[i] == -1) std::cout << "None\n";
        else std::cout << A[nge[i]] << " at index " << nge[i] << "\n";
    }
    std::cout << "\n";

    // 3. Sliding Window Maximum (K = 3)
    std::vector<int> win_max = sliding_window_maximum(A, 3);
    std::cout << "Sliding Window Max (K=3): ";
    for (int x : win_max) std::cout << x << " ";
    std::cout << "\n(Expected: 5 6 6 6 4 7)\n";

    return 0;
}
```

### Python Example
```python
if __name__ == "__main__":
    A = [2, 1, 5, 6, 2, 3, 4, 7]

    # 1. Prefix Maxima
    P, outstanding = compute_prefix_maxima(A)
    print(f"Original Array: {A}")
    print(f"Prefix Maxima:  {P}")
    print(
        f"Outstanding Indices: {outstanding} (values {[A[i] for i in outstanding]})\n"
    )

    # 2. Next Greater Element
    nge = next_greater_element(A)
    print("Next Greater Elements:")
    for i in range(len(A)):
        nge_val = A[nge[i]] if nge[i] != -1 else "None"
        print(f"Element {A[i]} (idx {i}) -> NGE: {nge_val} (idx {nge[i]})")
    print()

    # 3. Sliding Window Maximum
    win_max = sliding_window_maximum(A, 3)
    print(f"Sliding Window Max (K=3): {win_max}")
    print("Expected: [5, 6, 6, 6, 4, 7]")
```
