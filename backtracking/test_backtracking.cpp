#include <iostream>
#include "Permutations.hpp"
#include "Combinations.hpp"
#include "NQueens.hpp"
using namespace std;

int main() {
    MyVector<int> arr;
    arr.push_back(1); arr.push_back(2); arr.push_back(3);

    cout << "permutations of {1,2,3}:\n";
    generatePermutations(arr, [](MyVector<int>& p){
        for (int x : p) cout << x << " ";
        cout << "\n";
    });

    cout << "combinations of {1,2,3} choose 2:\n";
    generateCombinations(arr, 2, [](MyVector<int>& c){
        for (int x : c) cout << x << " ";
        cout << "\n";
    });

    NQueens nq(8);
    cout << "N-Queens(8) solutions = " << nq.countSolutions() << " (expect 92)\n";

    return 0;
}
