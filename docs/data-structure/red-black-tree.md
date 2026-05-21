# Red-Black Tree

## Conceptual Explanation

A **Red-Black Tree** is a self-balancing Binary Search Tree (BST) where each node contains an extra attribute: **color** (either **Red** or **Black**). Self-balancing guarantees that basic operations (search, insert, delete) run in $O(\log N)$ worst-case time by keeping the height of the tree bounded.

### Red-Black Tree Properties
To maintain balance, the tree must satisfy the following five properties at all times:
1. **Node Color**: Every node is either Red or Black.
2. **Root Property**: The root is always Black.
3. **Leaf Property**: Every leaf node (null/NIL) is Black.
4. **Red Property**: If a node is Red, then both of its children must be Black (no two adjacent Red nodes on any path).
5. **Depth Property (Black Height)**: For each node, all simple paths from the node to descendant leaves contain the same number of Black nodes.

### Balancing Operations: Rotations & Recoloring
When inserting or deleting nodes, the Red-Black properties may be violated. Balance is restored using two main operations:
- **Recoloring**: Changing node colors (Red to Black, or vice versa).
- **Rotations**: Changing the tree structure to adjust height without violating the BST ordering property. Rotations can be **Left Rotations** or **Right Rotations**.

---

## Complexity Analysis

| Operation | Average Case | Worst Case |
| :--- | :--- | :--- |
| **Search** | $O(\log N)$ | $O(\log N)$ |
| **Insertion** | $O(\log N)$ | $O(\log N)$ |
| **Deletion** | $O(\log N)$ | $O(\log N)$ |
| **Space Complexity** | $O(N)$ | $O(N)$ |

*Note: In C++, standard associative containers like `std::set` and `std::map` are internally implemented as Red-Black Trees.*

---

## C++ Implementation

This implementation includes node insertions, balancing (recoloring and rotations), and inorder traversal.

```cpp
#include <iostream>
#include <vector>

enum Color { RED, BLACK };

template <typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const T& val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    Node* TILL; // Null node sentinel

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TILL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != TILL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* k) {
        Node* u;
        while (k->parent != nullptr && k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // uncle
                if (u != nullptr && u->color == RED) {
                    // Case 1: Uncle is Red -> Recolor
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    // Case 2: Uncle is Black (triangle shape)
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    // Case 3: Uncle is Black (line shape)
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right; // uncle
                if (u != nullptr && u->color == RED) {
                    // Case 1 (Mirror)
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    // Case 2 (Mirror)
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    // Case 3 (Mirror)
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void inorder(Node* node, std::vector<T>& res) const {
        if (node != TILL && node != nullptr) {
            inorder(node->left, res);
            res.push_back(node->data);
            inorder(node->right, res);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr && node != TILL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    RedBlackTree() {
        TILL = new Node(T());
        TILL->color = BLACK;
        TILL->left = nullptr;
        TILL->right = nullptr;
        root = TILL;
    }

    ~RedBlackTree() {
        destroyTree(root);
        delete TILL;
    }

    void insert(const T& key) {
        Node* node = new Node(key);
        node->left = TILL;
        node->right = TILL;
        node->color = RED;

        Node* y = nullptr;
        Node* x = root;

        while (x != TILL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->data < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        fixInsert(node);
    }

    std::vector<T> getInorder() const {
        std::vector<T> res;
        inorder(root, res);
        return res;
    }

    bool search(const T& key) const {
        Node* x = root;
        while (x != TILL && x != nullptr) {
            if (key == x->data) return true;
            if (key < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return false;
    }
};

int main() {
    RedBlackTree<int> rbt;

    // Perform insertions
    rbt.insert(8);
    rbt.insert(18);
    rbt.insert(5);
    rbt.insert(15);
    rbt.insert(17);
    rbt.insert(25);
    rbt.insert(40);
    rbt.insert(80);

    std::cout << "Inorder Traversal of Red-Black Tree:\n";
    for (int x : rbt.getInorder()) {
        std::cout << x << " ";
    }
    std::cout << "\n\n";

    // Test search
    std::cout << "Search for 17: " << (rbt.search(17) ? "Found" : "Not Found") << "\n";
    std::cout << "Search for 99: " << (rbt.search(99) ? "Found" : "Not Found") << "\n";

    return 0;
}
```

---

## Python Implementation

```python
from typing import List, Generic, TypeVar, Optional

T = TypeVar('T')

class Color:
    RED = 0
    BLACK = 1

class Node(Generic[T]):
    def __init__(self, data: T):
        self.data: T = data
        self.color: int = Color.RED
        self.left: Optional[Node[T]] = None
        self.right: Optional[Node[T]] = None
        self.parent: Optional[Node[T]] = None

class RedBlackTree(Generic[T]):
    def __init__(self):
        # Create sentinel NIL node
        self.TILL = Node(None)
        self.TILL.color = Color.BLACK
        self.TILL.left = None
        self.TILL.right = None
        self.root = self.TILL

    def left_rotate(self, x: Node[T]) -> None:
        y = x.right
        x.right = y.left
        if y.left != self.TILL:
            y.left.parent = x
        y.parent = x.parent
        if x.parent is None:
            self.root = y
        elif x == x.parent.left:
            x.parent.left = y
        else:
            x.parent.right = y
        y.left = x
        x.parent = y

    def right_rotate(self, y: Node[T]) -> None:
        x = y.left
        y.left = x.right
        if x.right != self.TILL:
            x.right.parent = y
        x.parent = y.parent
        if y.parent is None:
            self.root = x
        elif y == y.parent.left:
            y.parent.left = x
        else:
            y.parent.right = x
        x.right = y
        y.parent = x

    def fix_insert(self, k: Node[T]) -> None:
        while k.parent and k.parent.color == Color.RED:
            if k.parent == k.parent.parent.right:
                u = k.parent.parent.left  # uncle
                if u and u.color == Color.RED:
                    # Case 1: Uncle is Red -> Recolor
                    u.color = Color.BLACK
                    k.parent.color = Color.BLACK
                    k.parent.parent.color = Color.RED
                    k = k.parent.parent
                else:
                    # Case 2: Uncle is Black (triangle shape)
                    if k == k.parent.left:
                        k = k.parent
                        self.right_rotate(k)
                    # Case 3: Uncle is Black (line shape)
                    k.parent.color = Color.BLACK
                    k.parent.parent.color = Color.RED
                    self.left_rotate(k.parent.parent)
            else:
                u = k.parent.parent.right  # uncle
                if u and u.color == Color.RED:
                    # Case 1 Mirror
                    u.color = Color.BLACK
                    k.parent.color = Color.BLACK
                    k.parent.parent.color = Color.RED
                    k = k.parent.parent
                else:
                    # Case 2 Mirror
                    if k == k.parent.right:
                        k = k.parent
                        self.left_rotate(k)
                    # Case 3 Mirror
                    k.parent.color = Color.BLACK
                    k.parent.parent.color = Color.RED
                    self.right_rotate(k.parent.parent)
            if k == self.root:
                break
        self.root.color = Color.BLACK

    def insert(self, key: T) -> None:
        node = Node(key)
        node.parent = None
        node.data = key
        node.left = self.TILL
        node.right = self.TILL
        node.color = Color.RED

        y = None
        x = self.root

        while x != self.TILL:
            y = x
            if node.data < x.data:
                x = x.left
            else:
                x = x.right

        node.parent = y
        if y is None:
            self.root = node
        elif node.data < y.data:
            y.left = node
        else:
            y.right = node

        if node.parent is None:
            node.color = Color.BLACK
            return

        if node.parent.parent is None:
            return

        self.fix_insert(node)

    def search(self, key: T) -> bool:
        x = self.root
        while x != self.TILL and x is not None:
            if key == x.data:
                return True
            if key < x.data:
                x = x.left
            else:
                x = x.right
        return False

    def inorder_traversal(self) -> List[T]:
        res: List[T] = []
        def _inorder(node: Node[T]):
            if node != self.TILL and node is not None:
                _inorder(node.left)
                res.append(node.data)
                _inorder(node.right)
        _inorder(self.root)
        return res


if __name__ == '__main__':
    rbt = RedBlackTree[int]()

    # Perform insertions
    rbt.insert(8)
    rbt.insert(18)
    rbt.insert(5)
    rbt.insert(15)
    rbt.insert(17)
    rbt.insert(25)
    rbt.insert(40)
    rbt.insert(80)

    print("Inorder Traversal of Red-Black Tree:")
    print(rbt.inorder_traversal())
    print()

    # Test search
    print("Search for 17:", "Found" if rbt.search(17) else "Not Found")
    print("Search for 99:", "Found" if rbt.search(99) else "Not Found")
```
