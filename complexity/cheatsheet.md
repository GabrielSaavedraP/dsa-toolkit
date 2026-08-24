# Complexity Cheat Sheet

## Growth order, smallest to largest

```
O(1) < O(log n) < O(√n) < O(n) < O(n log n) < O(n^1.5) < O(n²) < O(n³) < O(2ⁿ) < O(n!)
```

| Notation | Name | Typical source |
|---|---|---|
| O(1) | constant | array access, hash table average case, DSU find/union amortized |
| O(log n) | logarithmic | binary search, balanced tree height, heap insert/extract |
| O(n) | linear | single pass over input, linked list traversal |
| O(n log n) | linearithmic | efficient sorting (merge/heap sort), n operations each costing O(log n) |
| O(n²) | quadratic | naive nested loops over the same input, bubble/insertion sort |
| O(n³) | cubic | triple nested loops (e.g. Floyd-Warshall) |
| O(2ⁿ) | exponential | trying all subsets, unpruned backtracking |
| O(n!) | factorial | trying all permutations, brute-force TSP |

## How to derive complexity from n and the time limit (~10⁸ ops/sec rule of thumb)

| n max | Complexity you likely need |
|---|---|
| n ≤ 10-12 | O(n!) or O(2ⁿ · n) |
| n ≤ 20-22 | O(2ⁿ) |
| n ≤ 500 | O(n³) |
| n ≤ 5,000 | O(n²) |
| n ≤ 10⁵-10⁶ | O(n log n) |
| n ≤ 10⁸ | O(n) |
| n very large (10¹⁸) | O(log n) or O(1) |

## Master Theorem (for recursive complexity, T(n) = a·T(n/b) + f(n))

Compare `f(n)` against `n^(log_b a)`:
- If `f(n) = O(n^(log_b a - ε))` → **T(n) = Θ(n^(log_b a))** (recursion dominates)
- If `f(n) = Θ(n^(log_b a))` → **T(n) = Θ(n^(log_b a) · log n)** (balanced)
- If `f(n) = Ω(n^(log_b a + ε))` (and regularity holds) → **T(n) = Θ(f(n))** (the "outside work" dominates)

Example: merge sort is `T(n) = 2T(n/2) + O(n)`. Here a=2, b=2, so `n^(log_b a) = n^1 = n`. Since `f(n)=Θ(n)` matches, case 2 applies → **T(n) = Θ(n log n)**.

## Per-structure operation complexity (this toolkit)

| Structure | Key operations | Complexity |
|---|---|---|
| MyArray | access | O(1) |
| MyVector | access / push_back (amortized) / insert / erase | O(1) / O(1) / O(n) / O(n) |
| MyString | access / concat / substr / find | O(1) / O(n) / O(n) / O(n·m) |
| SinglyLinkedList | push_front / push_back / insert_at / search | O(1) / O(1) / O(n) / O(n) |
| DoublyLinkedList | push/pop front & back / insert_at | O(1) / O(1) / O(n) |
| Stack | push / pop / top | O(1) amortized / O(1) / O(1) |
| Queue | enqueue / dequeue / front | O(1) / O(1) / O(1) |
| CircularQueue | enqueue / dequeue | O(1) / O(1) |
| HashTable | insert / get / remove | O(1) avg, O(n) worst |
| BST | insert / search / remove | O(h), h=O(log n) balanced, O(n) worst |
| AVLTree | insert / search / remove | O(log n) guaranteed |
| BTree / BPlusTree | search / insert | O(log n) (flatter than BST for large t) |
| MinHeap / MaxHeap | insert / extract / peek | O(log n) / O(log n) / O(1) |
| BFS / DFS | full traversal | O(V + E) |
| DSU | find / union | O(α(n)) ≈ O(1) amortized |
| Kruskal | full MST | O(E log E) |
| Prim | full MST | O(E log E) |
| Dijkstra | shortest paths from source | O(E log V) |
