# Stack

## Conceptual Explanation

A **Stack** is a linear data structure that follows the **Last In First Out (LIFO)** principle. The element that is inserted last is the first one to be removed. It is analogous to a stack of physical plates; you add new plates to the top and remove them from the top.

### Core Logic & Operations
A stack supports three primary operations, all executing in strict $O(1)$ time:
- **Push**: Add an element to the top of the stack.
- **Pop**: Remove the element from the top of the stack.
- **Top / Peek**: Retrieve the top element without removing it.

#### Implementation Strategies:
- **Dynamic Array**: Elements are stored in a standard vector/dynamic array. Pushing means appending to the back, and popping means shrinking the array's size by one. Extremely fast due to cache locality.
- **Linked List**: Pushing and popping are performed at the head of a singly linked list.

### Typical Competitive Programming Use Cases
- **Depth-First Search (DFS)**: Explicit iterative traversal using a stack rather than recursive call stack frames (avoiding stack overflow).
- **Monotonic Stack**: A powerful technique where elements are kept in a strictly sorted order (either increasing or decreasing) inside the stack. Used to solve "next greater element" or "largest rectangle in histogram" problems in linear $O(N)$ time.
- **Expression Evaluation / Parsing**: Checking for balanced parentheses, converting Infix to Postfix/Prefix, and calculating mathematical expressions.

---

## Complexity Analysis

| Operation | Dynamic Array / Vector | Linked List | Std Stack (`std::stack`) |
| :--- | :--- | :--- | :--- |
| **Push** | $O(1)$ amortized | $O(1)$ | $O(1)$ |
| **Pop** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Top (Peek)** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Space Complexity** | $O(N)$ total | $O(N)$ total | $O(N)$ total |

---

## C++ Implementation

This implementation showcases a custom template-based dynamic-array **Stack** along with standard library stack operations.

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <stack>

// Custom dynamic array-based Stack
template <typename T>
class CustomStack {
private:
    std::vector<T> elements;

public:
    CustomStack() = default;

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }

    void push(const T& val) {
        elements.push_back(val);
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        elements.pop_back();
    }

    T top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return elements.back();
    }
};

int main() {
    // --- SECTION 1: Custom Stack ---
    std::cout << "=== Custom Stack ===\n";
    CustomStack<int> s;

    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "Stack top:  " << s.top() << " (Expected: 30)\n";
    s.pop();
    std::cout << "Stack top after 1 pop: " << s.top() << " (Expected: 20)\n";

    // --- SECTION 2: Standard std::stack (used in CP) ---
    std::cout << "\n=== Standard std::stack ===\n";
    std::stack<std::string> std_stack;

    std_stack.push("Bottom");
    std_stack.push("Middle");
    std_stack.push("Top");

    std::cout << "Standard stack elements (extracted LIFO): ";
    while (!std_stack.empty()) {
        std::cout << std_stack.top() << " ";
        std_stack.pop();
    }
    std::cout << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar

T = TypeVar('T')

class CustomStack(Generic[T]):
    def __init__(self):
        # We use a standard Python list as our dynamic array
        self.elements: List[T] = []

    def is_empty(self) -> bool:
        return len(self.elements) == 0

    def size(self) -> int:
        return len(self.elements)

    def push(self, val: T) -> None:
        """Push an element onto the stack."""
        self.elements.append(val)

    def pop(self) -> T:
        """Remove and return the top element of the stack."""
        if self.is_empty():
            raise IndexError("pop from empty stack")
        return self.elements.pop()

    def top(self) -> T:
        """Get the top element without removing it."""
        if self.is_empty():
            raise IndexError("peek from empty stack")
        return self.elements[-1]

    def __len__(self) -> int:
        return len(self.elements)


if __name__ == '__main__':
    # --- SECTION 1: Custom Stack ---
    print("=== Custom Stack ===")
    s = CustomStack[int]()

    s.push(10)
    s.push(20)
    s.push(30)

    print("Stack top:", s.top(), "(Expected: 30)")
    s.pop()
    print("Stack top after 1 pop:", s.top(), "(Expected: 20)")

    # --- SECTION 2: Idiomatic Python Stack ---
    # In Python, lists are highly optimized for LIFO stack operations 
    # using append() and pop()
    print("\n=== Idiomatic Python List Stack ===")
    list_stack = []

    list_stack.append("Bottom")
    list_stack.append("Middle")
    list_stack.append("Top")

    print("List stack elements (extracted LIFO): ", end="")
    while list_stack:
        print(list_stack.pop(), end=" ")
    print()
```
