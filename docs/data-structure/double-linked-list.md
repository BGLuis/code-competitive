# Doubly Linked List

## Conceptual Explanation

A **Doubly Linked List** is a linear data structure in which each element is a separate object, called a **Node**. Unlike a Singly Linked List, where each node only has a pointer to the next node, each node in a doubly linked list contains three fields:
1. **Value / Data**: The actual value stored in the node.
2. **Next Pointer**: A reference pointing to the next node in the sequence.
3. **Prev Pointer**: A reference pointing to the previous node in the sequence.

### Core Logic & Operations
- **Insertion at Front**: Create a node, set its `next` to the current head, set its `prev` to `nullptr`, update the old head's `prev` to point to the new node, and update the head pointer.
- **Insertion at End**: Traverse to the end (or update the tail pointer directly if maintained), set the old tail's `next` to the new node, set the new node's `prev` to the old tail, and update the tail pointer.
- **Deletion**: Since we have the `prev` pointer, we can delete any node in $O(1)$ time if we already have a direct pointer/reference to that node, by updating the `next` pointer of the previous node and the `prev` pointer of the next node.

### Typical Competitive Programming Use Cases
- **LRU Cache Implementation**: The Least Recently Used (LRU) Cache eviction policy is typically implemented using a Doubly Linked List combined with a Hash Map to achieve $O(1)$ access and eviction.
- **Adjacent Node Merging/Deletion**: In games or dynamic puzzle-solving algorithms, where elements in a row are eliminated (like Candy Crush or card matching), a doubly linked list lets us remove a node and instantly merge/link its neighbors.

---

## Complexity Analysis

| Operation | Time Complexity (Average/Worst) | Space Complexity |
| :--- | :--- | :--- |
| **Insert at Front / Back** | $O(1)$ | $O(1)$ |
| **Delete Node (given reference)** | $O(1)$ | $O(1)$ |
| **Search / Access by Index** | $O(N)$ | $O(1)$ |
| **Space Complexity** | $O(N)$ total | $O(N)$ total |

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;

        Node(const T& val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int element_count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), element_count(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head = nullptr;
        tail = nullptr;
        element_count = 0;
    }

    int size() const {
        return element_count;
    }

    bool empty() const {
        return element_count == 0;
    }

    void pushFront(const T& val) {
        Node* new_node = new Node(val);
        if (empty()) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        element_count++;
    }

    void pushBack(const T& val) {
        Node* new_node = new Node(val);
        if (empty()) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        element_count++;
    }

    void popFront() {
        if (empty()) {
            throw std::underflow_error("List is empty");
        }
        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr; // List became empty
        }
        delete temp;
        element_count--;
    }

    void popBack() {
        if (empty()) {
            throw std::underflow_error("List is empty");
        }
        Node* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr; // List became empty
        }
        delete temp;
        element_count--;
    }

    std::vector<T> toVectorForward() const {
        std::vector<T> res;
        Node* current = head;
        while (current != nullptr) {
            res.push_back(current->value);
            current = current->next;
        }
        return res;
    }

    std::vector<T> toVectorBackward() const {
        std::vector<T> res;
        Node* current = tail;
        while (current != nullptr) {
            res.push_back(current->value);
            current = current->prev;
        }
        return res;
    }
};

int main() {
    DoublyLinkedList<int> dll;

    dll.pushBack(10);
    dll.pushBack(20);
    dll.pushFront(5);
    dll.pushFront(1);

    // List: 1 <-> 5 <-> 10 <-> 20

    std::cout << "Forward Traversal:  ";
    for (int val : dll.toVectorForward()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "Backward Traversal: ";
    for (int val : dll.toVectorBackward()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    dll.popFront(); // removes 1
    dll.popBack();  // removes 20

    std::cout << "After popping front and back, Forward: ";
    for (int val : dll.toVectorForward()) {
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
        self.prev: Optional[Node[T]] = None
        self.next: Optional[Node[T]] = None

class DoublyLinkedList(Generic[T]):
    def __init__(self):
        self.head: Optional[Node[T]] = None
        self.tail: Optional[Node[T]] = None
        self.element_count: int = 0

    def is_empty(self) -> bool:
        return self.element_count == 0

    def push_front(self, val: T) -> None:
        new_node = Node(val)
        if self.is_empty():
            self.head = self.tail = new_node
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
        self.element_count += 1

    def push_back(self, val: T) -> None:
        new_node = Node(val)
        if self.is_empty():
            self.head = self.tail = new_node
        else:
            new_node.prev = self.tail
            self.tail.next = new_node
            self.tail = new_node
        self.element_count += 1

    def pop_front(self) -> T:
        if self.is_empty():
            raise IndexError("pop from empty doubly linked list")
        temp = self.head
        self.head = self.head.next
        if self.head:
            self.head.prev = None
        else:
            self.tail = None  # List became empty
        self.element_count -= 1
        return temp.value

    def pop_back(self) -> T:
        if self.is_empty():
            raise IndexError("pop from empty doubly linked list")
        temp = self.tail
        self.tail = self.tail.prev
        if self.tail:
            self.tail.next = None
        else:
            self.head = None  # List became empty
        self.element_count -= 1
        return temp.value

    def to_list_forward(self) -> List[T]:
        res = []
        current = self.head
        while current:
            res.append(current.value)
            current = current.next
        return res

    def to_list_backward(self) -> List[T]:
        res = []
        current = self.tail
        while current:
            res.append(current.value)
            current = current.prev
        return res

    def __len__(self) -> int:
        return self.element_count


if __name__ == '__main__':
    dll = DoublyLinkedList[int]()

    dll.push_back(10)
    dll.push_back(20)
    dll.push_front(5)
    dll.push_front(1)

    # List: 1 <-> 5 <-> 10 <-> 20

    print("Forward Traversal: ", dll.to_list_forward())
    print("Backward Traversal:", dll.to_list_backward())

    dll.pop_front()  # Removes 1
    dll.pop_back()   # Removes 20

    print("After popping front and back, Forward:", dll.to_list_forward())
```
