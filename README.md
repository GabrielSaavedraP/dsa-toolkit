# dsa-toolkit

Personal C++ implementations of the data structures and algorithms covered in CS2023 (Algoritmos y Estructuras de Datos), built to be dropped into an exam solution without relying on `std::vector`, `std::string`, `std::list`, `std::stack`, `std::queue`, `std::map`, `std::set`, `std::unordered_map`, or `std::priority_queue`.

Every `.hpp` file is self-contained (only includes other files from this repo, plus `<stdexcept>`/`<algorithm>`/`<utility>`/`<climits>` where genuinely just a utility, never a container). Every folder has a `test_*.cpp` that compiles independently and demonstrates the structure working, so you can copy just the folder you need.

```
g++ -std=c++17 -Wall -o test test_whatever.cpp && ./test
```

---

## Quick reference

| Structure / Algorithm | Main operations | Typical complexity | When to use it | File |
|---|---|---|---|---|
| MyArray | access | O(1) | fixed capacity given up front, no growth needed | `arrays/MyArray.hpp` |
| MyVector | push_back, insert, erase, `[]` | O(1) amortized / O(n) / O(n) / O(1) | general-purpose growable array | `arrays/MyVector.hpp` |
| MyString | concat, substr, find | O(n) / O(n) / O(n·m) | string manipulation without `std::string` | `strings/MyString.hpp` |
| SinglyLinkedList | push_front/back, insert_at, find | O(1) / O(1) / O(n) / O(n) | frequent insert/delete at the front, no random access needed | `linked_lists/SinglyLinkedList.hpp` |
| DoublyLinkedList | push/pop both ends | O(1) | need O(1) removal from the back too, or backward traversal | `linked_lists/DoublyLinkedList.hpp` |
| CircularLinkedList | push, forEach | O(1) / O(n) | round-robin scheduling, Josephus-style problems | `linked_lists/CircularLinkedList.hpp` |
| Stack | push, pop, top | O(1) | LIFO: expression evaluation, backtracking undo, matching brackets | `stacks/Stack.hpp` |
| Queue | enqueue, dequeue | O(1) | FIFO: BFS, task scheduling | `queues/Queue.hpp` |
| CircularQueue | enqueue, dequeue | O(1) | FIFO with a known fixed capacity, no dynamic growth | `queues/CircularQueue.hpp` |
| HashTable | insert, get, remove | O(1) avg | fast key lookup, counting/frequency problems | `hashing/HashTable.hpp` |
| BST | insert, search, remove | O(h) | ordered data, simple case, don't need worst-case guarantees | `trees/BST.hpp` |
| TreeTraversals | inorder/preorder/postorder/levelorder | O(n) | visiting every node in a specific order | `trees/TreeTraversals.hpp` |
| MinHeap / MaxHeap | insert, extractMin/Max, peek | O(log n) / O(log n) / O(1) | repeated min/max extraction, priority queues, heap sort | `heaps/MinHeap.hpp`, `heaps/MaxHeap.hpp` |
| AVLTree | insert, search, remove | O(log n) guaranteed | ordered data where you NEED guaranteed log n (adversarial input) | `balanced_trees/AVLTree.hpp` |
| BTree | insert, search | O(log n) | very large datasets, minimize tree height/disk reads | `balanced_trees/BTree.hpp` |
| BPlusTree | insert, search, rangeQuery | O(log n) / O(log n) / O(log n + k) | same as BTree, plus efficient range queries | `balanced_trees/BPlusTree.hpp` |
| Permutations | generatePermutations | O(n · n!) | generate all orderings | `backtracking/Permutations.hpp` |
| Combinations | generateCombinations | O(C(n,k) · k) | generate all subsets of fixed size | `backtracking/Combinations.hpp` |
| NQueens | countSolutions | O(N!) worst case, pruned in practice | constraint-satisfaction backtracking template | `backtracking/NQueens.hpp` |
| GraphList | addEdge, neighbors | O(1) / O(degree) | sparse graphs (few edges) | `graphs/GraphList.hpp` |
| GraphMatrix | addEdge, hasEdge | O(1) / O(1) | dense graphs, need O(1) edge existence check | `graphs/GraphMatrix.hpp` |
| BFS | bfs | O(V+E) | shortest path unweighted, connected components | `graphs/BFS.hpp` |
| DFS | dfsRecursive, dfsIterative | O(V+E) | cycle detection, topological sort, path exploration | `graphs/DFS.hpp` |
| DSU | find, unite | O(α(n)) ≈ O(1) | connectivity queries, Kruskal cycle detection | `graphs/DSU.hpp` |
| Kruskal | kruskalMST | O(E log E) | minimum spanning tree, edge-list input | `graphs/Kruskal.hpp` |
| Prim | primMST | O(E log E) | minimum spanning tree, growing from one vertex | `graphs/Prim.hpp` |
| Dijkstra | dijkstra | O(E log V) | shortest path, non-negative weights | `graphs/Dijkstra.hpp` |

---

## Exam quick reference — "I need to..."

| Pattern in the problem | Reach for |
|---|---|
| Fast lookup by key | HashTable |
| LIFO (undo, matching brackets, DFS iterative) | Stack |
| FIFO (BFS, task order) | Queue / CircularQueue |
| Hierarchical / nested data | BST or general tree |
| Repeated min/max extraction | MinHeap / MaxHeap |
| Shortest path, non-negative weights | Dijkstra |
| Minimum spanning tree | Kruskal (edge list) or Prim (adjacency list, dense-ish) |
| Connected components / "are these linked?" | DSU, or BFS/DFS |
| Generate all possibilities | Backtracking (Permutations/Combinations/NQueens as templates) |
| Ordered search with WORST-CASE guarantee | AVLTree |
| Ordered search, don't care about worst case | BST |
| Range queries ("all keys between X and Y") | BPlusTree |
| Graph traversal, no weights | BFS or DFS |
| Fixed-capacity array given up front, INS/DEL with shifting | MyArray, done by hand (see "How it works" below) |
| Need to grow an array dynamically | MyVector |
| String manipulation without `std::string` | MyString |

---

## How it works — plain-language explanations

### MyVector (dynamic array)
It's a raw pointer to a heap block plus a `size` (used slots) and `capacity` (allocated slots). When you `push_back` past capacity, it allocates a NEW block **double** the size, copies everything over, and frees the old block. Doubling — not adding a fixed amount — is what makes `push_back` **O(1) amortized**: if you only added 1 slot each resize, you'd copy the whole array almost every push, giving O(n²) total over n pushes. Doubling makes resizes exponentially rarer, so the total copying work over n pushes is only O(n).

### MyString
Same trick as MyVector, specialized to `char`, with one extra rule: the buffer is always allocated as `length + 1` so there's room for a trailing `'\0'`, which lets you still treat it as a C-string when needed (`c_str()`).

### Linked lists (singly / doubly / circular)
Instead of contiguous memory like an array, each element is its own heap allocation ("Node") holding a value and a pointer to the next node (and `prev` too, for doubly linked). This is why `push_front` is O(1) — you just relink a couple of pointers, no shifting — but random access by index is O(n), because you have to walk node by node from the head. Doubly linked adds a `prev` pointer so you can also do O(1) work at the tail and walk backward. Circular just makes the last node point back to the first instead of to `nullptr`.

### Stack vs Queue — why different underlying structures
Stack (LIFO) does all its work at ONE end, so a dynamic array is perfect — `push_back`/`pop_back` are both O(1) there. Queue (FIFO) needs O(1) work at BOTH ends (enqueue at the back, dequeue at the front) — a plain array would need to shift everything left on every dequeue, so a linked list with a tail pointer is used instead, since relinking a pointer at either end costs the same O(1).

### HashTable
An array of "buckets", where each bucket is a small linked list. To find a key: hash it to figure out which bucket it would live in, then walk that bucket's short chain comparing keys. Two keys landing in the same bucket ("collision") just makes that one bucket's chain longer — this is called **separate chaining**. When the average chain length (the "load factor") gets too high, the table doubles in size and everything gets rehashed into the new, bigger set of buckets, keeping chains short and operations close to O(1) on average.

### BST (Binary Search Tree)
Every node keeps the invariant: everything in its left subtree is smaller, everything in its right subtree is larger. Searching means comparing against the current node and eliminating one whole subtree at each step — that's what makes it O(height) instead of O(n). The catch: if you insert already-sorted data, the tree degenerates into a straight line (height = n), which is exactly the problem AVL trees solve.

### AVLTree
A BST that ALSO tracks the height of every subtree and enforces that left and right heights never differ by more than 1 ("balance factor" in {-1,0,1}). After every insert/remove, it walks back up and fixes any node that's become unbalanced using a **rotation** — a local re-arrangement of a few pointers that shortens one subtree and lengthens another WITHOUT breaking the BST ordering. This guarantees height is always O(log n), no matter what order you insert in.

### BTree / BPlusTree
Generalizes a BST so each node holds up to `2t-1` keys and `2t` children instead of just 1 key / 2 children — this makes the tree much shallower for the same number of elements (originally designed to minimize disk reads, where each node = one disk page). Insert always has room to spare because we **split full nodes on the way down**, before descending into them, instead of fixing things up on the way back afterward. BPlusTree additionally keeps ALL data in the leaves (internal nodes are pure "routing" structure) and links the leaves together in a chain, so once you've found where a range starts, you can walk forward through sorted order without ever climbing back up the tree — that's why databases use B+ trees for range queries.

### MinHeap / MaxHeap
A complete binary tree stored flat in an array (no pointers): for index `i`, children live at `2i+1` and `2i+2`, parent at `(i-1)/2`. Insert appends at the end then "sifts up" (swaps with parent while it violates the heap property). Extracting the min/max removes the root, moves the LAST element there, then "sifts down" (swaps with the smaller/larger child while it violates the property). This gives O(log n) insert/extract and O(1) peek, with none of the pointer overhead of a tree-based implementation.

### Backtracking (Permutations, Combinations, N-Queens)
All three explore a decision tree: at each step, try a candidate choice, recurse into the consequences of that choice, then **undo it** ("backtrack") and try the next candidate. Permutations try every unused element in each slot. Combinations only ever move forward through the array (never revisit an earlier index), which is what prevents `{1,2}` and `{2,1}` from both appearing. N-Queens places one queen per row and prunes any column/diagonal that's already attacked — the pruning (via `isSafe`) is what makes it vastly faster than blind brute force in practice, even though the worst-case bound is still O(n!).

### Graphs — adjacency list vs matrix
List: for each vertex, keep a small list of its edges. Good for SPARSE graphs — checking neighbors costs O(degree), space is O(V+E). Matrix: an `n x n` array where `matrix[u][v]` is the edge weight. Good for DENSE graphs — checking "does edge u-v exist" is O(1), but space is always O(V²) no matter how few edges actually exist.

### BFS vs DFS
BFS explores level by level using a Queue: mark a vertex visited the moment you enqueue it (not when you dequeue it) so it's never added twice. This guarantees the first time you reach a vertex, it's via a shortest path (in an unweighted graph). DFS dives as deep as possible before backtracking — the recursive version uses the call stack for free; the iterative version uses an explicit Stack to do the same thing.

### DSU (Union-Find)
Tracks a partition into disjoint groups. `find(x)` walks a `parent[]` array up to the group's root, and **path compression** re-points every visited node straight to the root along the way, so future finds are faster. **Union by rank** always attaches the shallower tree under the deeper one's root when merging, preventing trees from growing tall in the first place. Together these make find/union amortized almost O(1) (technically O(α(n)), the inverse Ackermann function, which is ≤4 for any realistic n).

### Kruskal vs Prim (Minimum Spanning Tree)
Kruskal: sort ALL edges by weight, then greedily add each one unless it would create a cycle (checked with DSU) — global, edge-focused. Prim: grow a SINGLE tree outward from one starting vertex, always adding the cheapest edge that connects the current tree to a new outside vertex (found via a MinHeap of candidate edges) — local, vertex-focused. Both produce a minimum spanning tree; which is faster depends on whether the graph is sparse (Kruskal, since sorting E edges is cheap) or dense (Prim tends to do better).

### Dijkstra
Like Prim, but tracking shortest distance from a single source instead of "cheapest edge to grow the tree". Pop the vertex with the smallest known distance from a MinHeap — once popped, that distance is FINAL (only valid because weights are non-negative: nothing later could ever produce a shorter path to it). For each neighbor, "relax" the edge: if going through the current vertex beats the neighbor's current best distance, update it and push the improvement.

---

## Concepts glossary (brief, only where needed to read this repo)

- **Pointer**: a variable holding a memory address instead of a value directly — `Node* next` means "next points to where another Node lives in memory", not "next contains a Node". `nullptr` means "points to nothing".
- **Template (`template <typename T>`)**: lets a class/function work with ANY type `T` (int, string, custom struct...) without rewriting it — the compiler generates a specialized version for each type you actually use.
- **`new` / `delete`**: manual memory allocation. `new T[n]` reserves a block on the heap that survives until you explicitly `delete[]` it — forget the `delete[]` and you leak memory; forget the `new` and you write into memory you don't own.
- **Rule of Three**: if a class manages its own memory (uses `new`/`delete` directly), it needs a copy constructor, a copy assignment operator, and a destructor — all three, or copies will share/corrupt the same underlying memory.
- **Recursion**: a function that calls itself on a smaller version of the problem, with a base case that stops the recursion. Every recursive call here (BST insert, DFS, backtracking) follows this shape.
- **Amortized complexity**: the AVERAGE cost per operation over a long sequence, even if occasional individual operations are expensive (like a `push_back` that triggers a resize) — the expensive ones are rare enough that they don't affect the average.

See `complexity/cheatsheet.md` for the full asymptotic notation reference and the Master Theorem.
