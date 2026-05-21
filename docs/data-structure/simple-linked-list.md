# Singly Linked List

## Conceptual Explanation

A **Singly Linked List** is a linear data structure consisting of a sequence of elements called **Nodes**. Unlike an array, elements are not stored in contiguous memory locations. Instead, each node contains two components:
1. **Data**: The value stored in the node.
2. **Next Pointer**: A reference pointing to the next node in the sequence. The last node has a pointer to `nullptr` (or `None`), indicating the end of the list.

### Core Logic & Operations
- **Insertion at Front**: Create a node, set its `next` pointer to the current `head`, and make this new node the new `head` of the list. This is an $O(1)$ operation.
- **Insertion at End**: Traverse the list from the `head` to find the last node (tail), and update its `next` pointer to the new node. If we maintain a separate `tail` pointer, this can be optimized to $O(1)$.
- **Deletion at Front**: Store the current `head` node, update the `head` pointer to `head->next`, and free/delete the stored node. This is an $O(1)$ operation.
- **Traversal**: Start at the `head` node, and repeatedly visit the `next` node until a null pointer is encountered.

### Typical Competitive Programming Use Cases
- **Adjacency Lists**: Used under the hood to store graph structures when memory is extremely constrained.
- **Chaining in Hash Tables**: Resolving collisions in custom hash map implementations.
- **Underlying stack/queue representations**: Simple linked lists serve as standard, pure implementations of LIFO/FIFO containers.

---

## Complexity Analysis

| Operation | Time Complexity (Average/Worst) | Space Complexity |
| :--- | :--- | :--- |
| **Insert at Front** | $O(1)$ | $O(1)$ |
| **Insert at End (with tail pointer)** | $O(1)$ | $O(1)$ |
| **Insert at End (without tail pointer)** | $O(N)$ | $O(1)$ |
| **Pop Front** | $O(1)$ | $O(1)$ |
| **Search / Random Access** | $O(N)$ | $O(1)$ |
| **Space Complexity** | $O(N)$ total | $O(N)$ total |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T value;
        Node* next;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int element_count;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), element_count(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
        head = nullptr;
        tail = nullptr;
        element_count = 0;
    }

    bool empty() const {
        return element_count == 0;
    }

    int size() const {
        return element_count;
    }

    // Insert element at the beginning O(1)
    void pushFront(const T& val) {
        Node* new_node = new Node(val);
        new_node->next = head;
        head = new_node;
        if (tail == nullptr) {
            tail = head; // List was empty
        }
        element_count++;
    }

    // Insert element at the end O(1) using tail pointer
    void pushBack(const T& val) {
        Node* new_node = new Node(val);
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        element_count++;
    }

    // Remove element from the beginning O(1)
    void popFront() {
        if (empty()) {
            throw std::underflow_error("List is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        element_count--;
        if (head == nullptr) {
            tail = nullptr; // List became empty
        }
    }

    // Reverse the linked list in O(N) time and O(1) auxiliary space
    void reverse() {
        Node* prev = nullptr;
        Node* current = head;
        Node* next_node = nullptr;
        tail = head; // Current head will become the new tail

        while (current != nullptr) {
            next_node = current->next; // Save next node
            current->next = prev;     // Reverse pointer
            prev = current;           // Move prev one step
            current = next_node;       // Move current one step
        }
        head = prev; // Update head
    }

    // Helper to print/retrieve list contents as a vector
    std::vector<T> toVector() const {
        std::vector<T> res;
        Node* current = head;
        while (current != nullptr) {
            res.push_back(current->value);
            current = current->next;
        }
        return res;
    }
};

int main() {
    SinglyLinkedList<int> sll;

    sll.pushBack(10);
    sll.pushBack(20);
    sll.pushFront(5);
    sll.pushFront(1);

    // List: 1 -> 5 -> 10 -> 20

    std::cout << "Original List: ";
    for (int val : sll.toVector()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    sll.popFront(); // Removes 1
    std::cout << "After popping front: ";
    for (int val : sll.toVector()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    sll.reverse(); // List becomes: 20 -> 10 -> 5
    std::cout << "After reversing:     ";
    for (int val : sll.toVector()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar, Optional

T = TypeVar('T')

class Node(Generic[T]):
    def __init__(self, value: T):
        self.value: T = value
        self.next: Optional[Node[T]] = None

class SinglyLinkedList(Generic[T]):
    def __init__(self):
        self.head: Optional[Node[T]] = None
        self.tail: Optional[Node[T]] = None
        self.element_count: int = 0

    def is_empty(self) -> bool:
        return self.element_count == 0

    def push_front(self, val: T) -> None:
        """Add node to the front of the list in O(1)."""
        new_node = Node(val)
        new_node.next = self.head
        self.head = new_node
        if self.tail is None:
            self.tail = self.head
        self.element_count += 1

    def push_back(self, val: T) -> None:
        """Add node to the back of the list in O(1) using tail pointer."""
        new_node = Node(val)
        if self.is_empty():
            self.head = self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node
        self.element_count += 1

    def pop_front(self) -> T:
        """Remove and return the front node in O(1)."""
        if self.is_empty():
            raise IndexError("pop from empty singly linked list")
        temp = self.head
        self.head = self.head.next
        self.element_count -= 1
        if self.head is None:
            self.tail = None
        return temp.value

    def reverse(self) -> None:
        """Reverse the list in-place in O(N) time and O(1) space."""
        prev = None
        current = self.head
        self.tail = self.head  # The head will become the tail

        while current:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node

        self.head = prev

    def to_list(self) -> List[T]:
        """Convert list elements to a standard python list."""
        res = []
        current = self.head
        while current:
            res.append(current.value)
            current = current.next
        return res

    def __len__(self) -> int:
        return self.element_count


if __name__ == '__main__':
    sll = SinglyLinkedList[int]()

    sll.push_back(10)
    sll.push_back(20)
    sll.push_front(5)
    sll.push_front(1)

    # List: 1 -> 5 -> 10 -> 20

    print("Original List:", sll.to_list())

    sll.pop_front()  # Removes 1
    print("After popping front:", sll.to_list())

    sll.reverse()  # List becomes: 20 -> 10 -> 5
    print("After reversing:    ", sll.to_list())
```
