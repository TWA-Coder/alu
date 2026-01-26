# Algorithm Analysis - Bus Route

## Time Complexity of Adding a New Bus Stop

### Problem Statement
Determine the time complexity of adding a new bus stop at the end of the doubly linked list when the number of bus stops is $n$.

### Analysis
To add a node to the end of a detailed linked list (bus route), the following steps are generally taken in a standard implementation where we only hold a reference to the `head` (start) of the list:

1.  **Creation**: Allocate memory for the new node and set its data. This takes Constant Time, $O(1)$.
2.  **Traversal**: We must find the last node in the list to link it to the new node.
    -   We start at the `head`.
    -   We iterate through each node until we reach the node where `next` is `NULL`.
    -   If there are $n$ bus stops, we traverse $n$ nodes.
    -   This traversal process takes Linear Time, $O(n)$.
3.  **Linking**: Once the last node is found:
    -   Update the last node's `next` pointer to the new node.
    -   Update the new node's `prev` pointer to the last node.
    -   This takes Constant Time, $O(1)$.

### Conclusion
Combining these steps, the dominant operation is the traversal of the list.
Therefore, the time complexity is **$O(n)$**, where $n$ is the number of bus stops.

> **Note**: If we had maintained a separate pointer to the `tail` (end) of the list throughout the program's lifecycle, we could access the end immediately, bringing the complexity down to $O(1)$. However, in the standard traversal implementation derived from a single `head` entry point (as is common in basic list models), it remains $O(n)$.
