#pragma once
#include "../arrays/MyVector.hpp"

// Classic N-Queens: place N queens on an N x N board so none attacks
// another (no shared row, column, or diagonal).
//
// HOW IT WORKS
// We place exactly one queen per ROW (so rows never conflict by
// construction) and try each COLUMN in that row. isSafe checks the
// column and both diagonals against queens already placed in earlier
// rows. If a column works, place the queen and recurse to the next
// row; if that path leads nowhere, backtrack (remove the queen,
// implicit here since we just overwrite positions[row] on the next
// iteration) and try the next column.
//
// Complexity: worst case O(N!) (heavily pruned in practice by isSafe,
// which is what makes backtracking far faster than brute force here).
class NQueens {
private:
    int n_;
    MyVector<int> positions_; // positions_[row] = column of the queen in that row
    long long solutionCount_;

    bool isSafe(int row, int col) const {
        for (int r = 0; r < row; r++) {
            int c = positions_[r];
            if (c == col) return false;                    // same column
            if (abs(c - col) == abs(r - row)) return false; // same diagonal
        }
        return true;
    }

    void solve(int row) {
        if (row == n_) { solutionCount_++; return; }
        for (int col = 0; col < n_; col++) {
            if (isSafe(row, col)) {
                if ((int)positions_.size() <= row) positions_.push_back(col);
                else positions_[row] = col;
                solve(row + 1);
                // backtrack is implicit: next loop iteration overwrites positions_[row]
            }
        }
    }

public:
    explicit NQueens(int n) : n_(n), solutionCount_(0) {}

    long long countSolutions() {
        solutionCount_ = 0;
        positions_.clear();
        solve(0);
        return solutionCount_;
    }
};
