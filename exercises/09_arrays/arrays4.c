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
    // TODO: add the 4 values in grid[row].
    (void)grid;
    (void)row;
    return 0;
}

int sum_all(int grid[3][4]) {
    // TODO: add every value in the grid.
    (void)grid;
    return 0;
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
