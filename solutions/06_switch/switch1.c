// Fix: a `case` label is only a jump target; without `break` execution falls
// through into the next case. `break` leaves the switch.
/*
 * switch1
 *
 * `points_for` turns a letter grade into grade points, but every grade gets
 * the points for the last case. In C, once a `case` matches, execution runs
 * on into the cases below it. The compiler warns about this, and warnings
 * count as errors here.
 * See K&R 3.4 and Beej 3.3.5.
 */

#include "clings.h"

int points_for(char grade) {
    int points = 0;
    switch (grade) {
    case 'A':
        points = 4;
        break;
    case 'B':
        points = 3;
        break;
    case 'C':
        points = 2;
        break;
    default:
        points = 0;
        break;
    }
    return points;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(points_for('A'), 4);
    CHECK_EQ_INT(points_for('B'), 3);
    CHECK_EQ_INT(points_for('C'), 2);
    CHECK_EQ_INT(points_for('F'), 0);
    return clings_summary();
}
