#pragma once
#include "../arrays/MyVector.hpp"

// Generates all combinations of size k from arr (order doesn't matter,
// no repeats). Decision tree: at each step, decide "include arr[i] or
// skip it", but ALWAYS move forward through the array (never revisit
// an earlier index) — that's what prevents duplicate combinations
// like {1,2} and {2,1} from both appearing.
//
// Complexity: O(C(n,k) * k) — C(n,k) combinations, O(k) to report each.
template <typename T, typename F>
void combinationsHelper(MyVector<T>& arr, int start, int k, MyVector<T>& current, F onCombination) {
    if (current.size() == k) { onCombination(current); return; }
    if (start == arr.size()) return; // ran out of elements before reaching size k

    for (int i = start; i < arr.size(); i++) {
        current.push_back(arr[i]);
        combinationsHelper(arr, i + 1, k, current, onCombination); // i+1: never reuse or go backward
        current.pop_back(); // backtrack
    }
}

template <typename T, typename F>
void generateCombinations(MyVector<T> arr, int k, F onCombination) {
    MyVector<T> current;
    combinationsHelper(arr, 0, k, current, onCombination);
}
