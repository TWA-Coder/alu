# Algorithm Analysis and Critical Evaluation

## Algorithm Analysis

### Derivation of Running Time Function

The running time of Quick Sort depends heavily on the choice of the pivot and the arrangement of the input elements.

Let $T(n)$ be the time complexity for sorting $n$ records.
The recurrence relation is generally:
$$T(n) = T(k) + T(n-k-1) + \Theta(n)$$
where $\Theta(n)$ is the time for the partition process.

#### 1. Best Case
**Scenario:** The pivot always divides the array into two equal halves.
**Recurrence:** $T(n) = 2T(n/2) + \Theta(n)$
**Derivation:**
Using the Master Theorem or expansion:
$$T(n) = 2T(n/2) + cn$$
$$T(n) = 2(2T(n/4) + c(n/2)) + cn = 4T(n/4) + 2cn$$
...
$$T(n) = 2^k T(n/2^k) + kcn$$
Let $n = 2^k \implies k = \log_2 n$:
$$T(n) = n T(1) + cn \log_2 n$$
**Complexity:** $O(n \log n)$

#### 2. Average Case
**Scenario:** The pivot divides the array in a reasonably random ratio.
**Analysis:** Mathematically, the average case depth of the recursion tree is still logarithmic.
**Complexity:** $O(n \log n)$

#### 3. Worst Case
**Scenario:** The pivot is always the smallest or largest element (e.g., array is already sorted or reverse sorted), and we pick the last element as pivot.
**Recurrence:** $T(n) = T(n-1) + \Theta(n)$
**Derivation:**
$$T(n) = T(n-1) + cn$$
$$T(n) = T(n-2) + c(n-1) + cn$$
...
$$T(n) = \sum_{i=1}^{n} ci = c \frac{n(n+1)}{2}$$
**Complexity:** $O(n^2)$

### Big-O Notation Resolution
Based on the derivation above, the running time expressed in Big-O notation is:
- **Best Case:** $O(n \log n)$
- **Average Case:** $O(n \log n)$
- **Worst Case:** $O(n^2)$

---

## Critical Evaluation

### Why Quick Sort is more appropriate than Insertion Sort

#### 1. Dataset Scale and Scalability
**Insertion Sort** has a time complexity of $O(n^2)$. While efficient for very small datasets (n < 50), its performance degrades drastically as $n$ increases.
**Quick Sort** generally operates at $O(n \log n)$.
*For a dataset of 1,000,000 records:*
- Insertion Sort: $\approx (10^6)^2 = 10^{12}$ operations.
- Quick Sort: $\approx 10^6 \times \log_2(10^6) \approx 2 \times 10^7$ operations.
Quick Sort is exponentially faster for large datasets.

#### 2. File-Based Input & Memory
For file-based processing where data is loaded into memory (as required), memory efficiency is key.
- Merge Sort requires $O(n)$ auxiliary space.
- **Quick Sort** is an in-place sort (requires only $O(\log n)$ stack space for recursion).
This makes it memory-efficient for sorting large arrays loaded from files compared to non-in-place algorithms.

#### 3. Performance Characteristics
Although Quick Sort has a worst-case of $O(n^2)$, this is rare with randomized or median-of-three pivot selection (though simple implementation uses the last element). On average, constant factors in Quick Sort are small, making it often faster than Merge Sort or Heap Sort in practice for in-memory arrays.
