# Algorithm Analysis - Student BST Search

## Time Complexity of Searching (BST)

Searching for a student by last name in a BST with $n$ nodes depends on the height ($h$) of the tree. The search operation follows a path from the root down to the leaves.

### 1. Best Case
**Scenario:** The target node is the **root** of the tree.
**Complexity:** $O(1)$ (Constant time).

### 2. Average Case (Balanced Tree)
**Scenario:** The tree is balanced (random insertion order).
**Height:** $h \approx \log_2 n$.
We eliminate half the remaining nodes at each step.
**Complexity:** $O(\log n)$.

### 3. Worst Case (Skewed Tree)
**Scenario:** The tree is skewed (e.g., records were inserted in alphabetical order of Last Name). The tree effectively becomes a linked list.
**Height:** $h \approx n$.
We visit every node in the chain.
**Complexity:** $O(n)$.

---

## Conceptual Understanding

### Binary Search Tree (BST) vs. General Binary Tree
| Feature | Binary Search Tree (BST) | General Binary Tree |
| :--- | :--- | :--- |
| **Ordering Rule** | **Strict**: LeftChild < Parent < RightChild. | **None**: No specific ordering of data required. |
| **Search Efficiency** | Efficient ($O(\log n)$ average). We know exactly which branch to take. | Inefficient ($O(n)$). Must potentially visit every node. |
| **Structure** | Optimized for searching, inserting, and sorting. | Used for hierarchical representations (expressions, file systems). |

### Comparison: Linear Search (File) vs. BST Search

#### Linear Search (Directly from File)
-   **Method**: Read file line-by-line comparing each record.
-   **Performance**: **$O(n)$** always. Must read through the file (disk I/O is slow).
-   **Suitability**: Only suitable for very small datasets or one-off scripts.

#### BST Search (In-Memory)
-   **Method**: Load data into memory once, organize into BST, then traverse pointers.
-   **Performance**: **$O(\log n)$** on average.
-   **Suitability**: Much faster for **repeated queries**. Once loaded, thousands of searches can be performed instantly without disk access.

### Justification
**The BST approach is more suitable** for this system because:
1.  **Efficiency**: As "the number of students grows" (per requirements), $O(\log n)$ significantly outperforms $O(n)$. For 1,000,000 students, BST takes ~20 comparisons vs 1,000,000 for linear search.
2.  **Responsiveness**: In-memory searching avoids repeated slow disk I/O operations for every query.
