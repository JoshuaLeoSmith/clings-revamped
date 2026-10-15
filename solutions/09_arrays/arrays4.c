// Fix: rows are stored one after another, so grid[r][c] lives at r * 4 + c;
// the compiler needs the row length 4 for that, but not the number of rows.
/*
 * arrays4
 *
 * `int grid[3][4]` is 3 rows of 4 ints, and `grid[r][c]` is row r, column c.
 * A parameter may leave out the number of rows but not the row length:
 * `int grid[][4]`. Implement `sum_row`, which adds up one row, and
 * `sum_all`, which adds up the whole grid.
 * See K&R 5.7 and Beej 6.
 */

#include "clings.h"

int sum_row(int grid[][4], int row) {
    int total = 0;
    for (int c = 0; c < 4; c++) {
        total += grid[row][c];
    }
    return total;
}

int sum_all(int grid[3][4]) {
    int total = 0;
    for (int r = 0; r < 3; r++) {
        total += sum_row(grid, r);
    }
    return total;
}

int main(void) {
    int grid[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
    };

    // Don't change the tests!
    CHECK_EQ_INT(sum_row(grid, 0), 10);
    CHECK_EQ_INT(sum_row(grid, 2), 42);
    CHECK_EQ_INT(sum_all(grid), 78);
    return clings_summary();
}
