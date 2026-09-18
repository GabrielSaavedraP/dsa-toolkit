cmake_minimum_required(VERSION 4.2)
project(DSA_2026_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(arrays)
include_directories(backtracking)
include_directories(balanced_trees)
include_directories(graphs)
include_directories(hashing)
include_directories(heaps)
include_directories(linked_lists)
include_directories(queues)
include_directories(stacks)
include_directories(strings)
include_directories(trees)

add_executable(DSA_2026_2
        arrays/MyArray.hpp
        arrays/MyVector.hpp
        arrays/test_arrays.cpp
        backtracking/Combinations.hpp
        backtracking/NQueens.hpp
        backtracking/Permutations.hpp
        backtracking/test_backtracking.cpp
        balanced_trees/AVLTree.hpp
        balanced_trees/BPlusTree.hpp
        balanced_trees/BTree.hpp
        balanced_trees/test_avl.cpp
        balanced_trees/test_bplustree.cpp
        balanced_trees/test_btree.cpp
        graphs/BFS.hpp
        graphs/DFS.hpp
        graphs/Dijkstra.hpp
        graphs/DSU.hpp
        graphs/GraphList.hpp
        graphs/GraphMatrix.hpp
        graphs/Kruskal.hpp
        graphs/Prim.hpp
        graphs/test_graphs.cpp
        hashing/HashTable.hpp
        hashing/test_hashing.cpp
        heaps/MaxHeap.hpp
        heaps/MinHeap.hpp
        heaps/test_heaps.cpp
        linked_lists/CircularLinkedList.hpp
        linked_lists/DoublyLinkedList.hpp
        linked_lists/SinglyLinkedList.hpp
        linked_lists/test_linked_lists.cpp
        queues/CircularQueue.hpp
        queues/Queue.hpp
        queues/test_queues.cpp
        stacks/Stack.hpp
        stacks/test_stack.cpp
        strings/MyString.hpp
        strings/test_strings.cpp
        trees/BST.hpp
        trees/test_trees.cpp
        trees/TreeTraversals.hpp
        linked_lists/IndexedReversibleList.hpp
        "hashing/ejercicios de clase/codeforces3 NC.cpp"
        "hashing/ejercicios de clase/d.cpp"
        "hashing/ejercicios de clase/f NC.cpp"
        "hashing/ejercicios de clase/g NC.cpp"
        "hashing/ejercicios de clase/h NC.cpp"
        "hashing/ejercicios de clase/leetcode1.cpp"
        "hashing/ejercicios de clase/leetcode2.cpp"
        "hashing/ejercicios de clase/sec1A.cpp"
        "hashing/ejercicios de clase/sec1B.cpp")
