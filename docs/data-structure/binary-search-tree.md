# Binary Search Tree

## Conceptual Explanation

A **Binary Search Tree (BST)** is a hierarchical node-based data structure where
each node has at most two children, referred to as the left child and the right
child. The tree maintains the **BST property**:

- The left subtree of a node contains only nodes with keys **less than** the
  node's key.
- The right subtree of a node contains only nodes with keys **greater than** the
  node's key.
- Both the left and right subtrees must also be binary search trees.

### Core Logic & Operations

1. **Search**: Compare the target value with the root. If they are equal, the
   target is found. If the target is smaller, recurse on the left child; if
   larger, recurse on the right child. If a null pointer is reached, the value
   does not exist.
2. **Insertion**: Traverse the tree similarly to searching until finding a null
   spot where the node can be inserted while maintaining the BST property.
3. **Deletion**: Deletion is slightly more complex and involves three scenarios:
   - **Node to delete is a leaf**: Simply remove the node.
   - **Node to delete has one child**: Replace the node with its child.
   - **Node to delete has two children**: Find the in-order successor (the
     smallest node in the right subtree) or in-order predecessor (the largest
     node in the left subtree), copy its value to the target node, and then
     recursively delete that successor/predecessor node.

### Typical Competitive Programming Use Cases

While standard unbalanced BSTs are rarely used directly in competitive
programming due to their potential to degrade into $O(N)$ skew trees, they form
the foundational understanding for balanced BSTs (like AVL, Red-Black Trees,
Treaps, and Splay Trees). They are also used for implementing custom sorting or
tracking hierarchies when the input is guaranteed to be random or ordered in a
way that prevents degeneration.

---

## Complexity Analysis

| Operation            | Average Case | Worst Case |
| :------------------- | :----------- | :--------- |
| **Search**           | $O(\log N)$  | $O(N)$     |
| **Insertion**        | $O(\log N)$  | $O(N)$     |
| **Deletion**         | $O(\log N)$  | $O(N)$     |
| **Space Complexity** | $O(N)$       | $O(N)$     |

_Note: The worst-case complexities occur when the tree becomes skewed (e.g.,
inserting sorted keys)._

---

## C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <optional>
#include <functional>

template <typename T>
class BinarySearchTree {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* insert(Node* node, const T& val) {
        if (node == nullptr) {
            return new Node(val);
        }
        if (val < node->value) {
            node->left = insert(node->left, val);
        } else if (val > node->value) {
            node->right = insert(node->right, val);
        }
        // Duplicate values are ignored in this implementation
        return node;
    }

    bool search(Node* node, const T& val) const {
        if (node == nullptr) {
            return false;
        }
        if (val == node->value) {
            return true;
        }
        if (val < node->value) {
            return search(node->left, val);
        }
        return search(node->right, val);
    }

    Node* getMin(Node* node) const {
        while (node != nullptr && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const T& val) {
        if (node == nullptr) {
            return nullptr;
        }

        if (val < node->value) {
            node->left = remove(node->left, val);
        } else if (val > node->value) {
            node->right = remove(node->right, val);
        } else {
            // Node to be deleted found

            // Case 1: No child or 1 child
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Case 2: Two children
            // Find the in-order successor (smallest in the right subtree)
            Node* temp = getMin(node->right);
            node->value = temp->value;
            node->right = remove(node->right, temp->value);
        }
        return node;
    }

    void inorder(Node* node, std::vector<T>& res) const {
        if (node != nullptr) {
            inorder(node->left, res);
            res.push_back(node->value);
            inorder(node->right, res);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void insert(const T& val) {
        root = insert(root, val);
    }

    bool search(const T& val) const {
        return search(root, val);
    }

    void remove(const T& val) {
        root = remove(root, val);
    }

    std::vector<T> inorderTraversal() const {
        std::vector<T> res;
        inorder(root, res);
        return res;
    }

    std::optional<T> getMin() const {
        Node* temp = getMin(root);
        if (temp == nullptr) {
            return std::nullopt;
        }
        return temp->value;
    }
};

int main() {
    BinarySearchTree<int> bst;

    // Test insertions
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);
    bst.insert(70);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Inorder traversal after insertions: ";
    for (int val : bst.inorderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // Test Search
    std::cout << "Search 40: " << (bst.search(40) ? "Found" : "Not Found") << "\n";
    std::cout << "Search 90: " << (bst.search(90) ? "Found" : "Not Found") << "\n";

    // Test Min value
    auto minVal = bst.getMin();
    if (minVal) {
        std::cout << "Minimum value in BST: " << *minVal << "\n";
    }

    // Test Deletions
    std::cout << "\nDeleting 20 (leaf node)...\n";
    bst.remove(20);
    std::cout << "Inorder traversal: ";
    for (int val : bst.inorderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "Deleting 30 (node with one child)...\n";
    bst.remove(30);
    std::cout << "Inorder traversal: ";
    for (int val : bst.inorderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "Deleting 50 (node with two children)...\n";
    bst.remove(50);
    std::cout << "Inorder traversal: ";
    for (int val : bst.inorderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Optional, TypeVar, Generic

T = TypeVar('T')

class Node(Generic[T]):
    def __init__(self, value: T):
        self.value: T = value
        self.left: Optional[Node[T]] = None
        self.right: Optional[Node[T]] = None

class BinarySearchTree(Generic[T]):
    def __init__(self):
        self.root: Optional[Node[T]] = None

    def insert(self, val: T) -> None:
        def _insert(node: Optional[Node[T]], val: T) -> Node[T]:
            if node is None:
                return Node(val)
            if val < node.value:
                node.left = _insert(node.left, val)
            elif val > node.value:
                node.right = _insert(node.right, val)
            return node

        self.root = _insert(self.root, val)

    def search(self, val: T) -> bool:
        def _search(node: Optional[Node[T]], val: T) -> bool:
            if node is None:
                return False
            if node.value == val:
                return True
            if val < node.value:
                return _search(node.left, val)
            return _search(node.right, val)

        return _search(self.root, val)

    def remove(self, val: T) -> None:
        def _get_min(node: Node[T]) -> Node[T]:
            current = node
            while current.left is not None:
                current = current.left
            return current

        def _remove(node: Optional[Node[T]], val: T) -> Optional[Node[T]]:
            if node is None:
                return None

            if val < node.value:
                node.left = _remove(node.left, val)
            elif val > node.value:
                node.right = _remove(node.right, val)
            else:
                # Node with only one child or no child
                if node.left is None:
                    return node.right
                elif node.right is None:
                    return node.left

                # Node with two children: Get the inorder successor
                temp = _get_min(node.right)
                node.value = temp.value
                node.right = _remove(node.right, temp.value)

            return node

        self.root = _remove(self.root, val)

    def get_min(self) -> Optional[T]:
        if self.root is None:
            return None
        current = self.root
        while current.left is not None:
            current = current.left
        return current.value

    def inorder_traversal(self) -> List[T]:
        res: List[T] = []
        def _inorder(node: Optional[Node[T]]) -> None:
            if node:
                _inorder(node.left)
                res.append(node.value)
                _inorder(node.right)
        _inorder(self.root)
        return res

if __name__ == '__main__':
    bst = BinarySearchTree[int]()

    # Test insertions
    bst.insert(50)
    bst.insert(30)
    bst.insert(20)
    bst.insert(40)
    bst.insert(70)
    bst.insert(60)
    bst.insert(80)

    print("Inorder traversal after insertions:", bst.inorder_traversal())

    # Test Search
    print("Search 40:", "Found" if bst.search(40) else "Not Found")
    print("Search 90:", "Found" if bst.search(90) else "Not Found")

    # Test Min
    print("Minimum value in BST:", bst.get_min())

    # Test Deletions
    print("\nDeleting 20 (leaf node)...")
    bst.remove(20)
    print("Inorder traversal:", bst.inorder_traversal())

    print("Deleting 30 (node with one child)...")
    bst.remove(30)
    print("Inorder traversal:", bst.inorder_traversal())

    print("Deleting 50 (node with two children)...")
    bst.remove(50)
    print("Inorder traversal:", bst.inorder_traversal())
```
