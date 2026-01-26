# Algorithm Analysis - Binary Tree

## Time Complexity of Inserting a New Node

### Insertion Strategy
The program uses a **Binary Search Tree (BST)** insertion strategy.
This rule dictates that:
1.  Starting from the root, we compare the new value with the current node's value.
2.  If the new value is **smaller**, we traverse to the **left** child.
3.  If the new value is **larger**, we traverse to the **right** child.
4.  This process repeats until a `NULL` pointer (empty spot) is found, where the new node is attached.

### Complexity Analysis
The time complexity of inserting a new node into a binary tree with $n$ nodes depends on the **height of the tree**, denoted as $h$.
In every step of the insertion, we move one level deeper into the tree. Therefore, the number of comparisons is proportional to the depth of the insertion point, which is at most the height of the tree.

**Complexity: $O(h)$**

#### Relating $h$ to $n$ (Number of Nodes)

1.  **Best/Average Case (Balanced Tree)**:
    -   If the input array is random or structured such that the tree remains relatively balanced (the left and right subtrees have roughly equal height), the height $h$ is logarithmic relative to $n$.
    -   $h \approx \log_2 n$.
    -   **Time Complexity: $O(\log n)$**

2.  **Worst Case (Skewed Tree)**:
    -   If the input array is sorted (ascending or descending), the tree becomes a linked list (skewed to the right or left).
    -   In this scenario, the height $h$ becomes equal to $n$.
    -   **Time Complexity: $O(n)$**

### Conclusion
Since the task involves an array of 68 "distinct" integers (likely random or unsorted), the average case is the most relevant expectation.
However, strictly speaking in Big-O notation, the time complexity of insertion is **$O(h)$**, where $h$ is the height of the tree.
In terms of $n$:
-   **Average:** $O(\log n)$
-   **Worst:** $O(n)$
