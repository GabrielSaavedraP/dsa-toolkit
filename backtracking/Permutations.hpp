#pragma once
#include "../arrays/MyVector.hpp"

// Generates all permutations of `arr` by building a decision tree:
// at each recursion depth, try placing each UNUSED element in the
// current slot, recurse for the next slot, then UNDO the choice
// ("backtrack") and try the next candidate. This explores exactly
// n! leaves of the decision tree.
//
// Complexity: O(n * n!) — n! permutations, O(n) work to copy/report each.
template <typename T, typename F>
void permutationsHelper(MyVector<T>& arr, MyVector<bool>& used, MyVector<T>& current, F onPermutation) {
    if (current.size() == arr.size()) { onPermutation(current); return; }

    for (int i = 0; i < arr.size(); i++) {
        if (used[i]) continue;

        used[i] = true;
        current.push_back(arr[i]);

        permutationsHelper(arr, used, current, onPermutation);

        // backtrack: undo the choice so the next candidate at this
        // depth starts from a clean state
        current.pop_back();
        used[i] = false;
    }
}

template <typename T, typename F>
void generatePermutations(MyVector<T> arr, F onPermutation) {
    MyVector<bool> used;
    for (int i = 0; i < arr.size(); i++) used.push_back(false);
    MyVector<T> current;
    permutationsHelper(arr, used, current, onPermutation);
}
