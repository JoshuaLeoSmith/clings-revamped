/*
 * quiz1
 *
 * This quiz covers variables, functions, if, loops, io, switch, types and
 * operators (topics 01 to 08). Write these three functions above `main`;
 * each takes `int` arguments and returns an `int`:
 *
 * - `price_of_apples(quantity)`: Mary sells apples at 2 each, but whoever
 *   buys more than 40 pays only 1 per apple. Return the total price.
 * - `is_leap_year(year)`: return 1 if `year` is a leap year, 0 if not. A year
 *   is a leap year when it divides by 4, except that years dividing by 100
 *   are not, except that years dividing by 400 are after all.
 * - `gcd(a, b)`: return the greatest common divisor of two non-negative
 *   numbers, using a loop. Euclid: replace (a, b) with (b, a % b) until b is
 *   0; then a is the answer.
 *
 * See K&R 2.5 and 3.5, and Beej 3.2 and 4.
 */

#include "clings.h"

// TODO: write the three functions here.
// int price_of_apples(???) { ??? }
// int is_leap_year(???) { ??? }
// int gcd(???) { ??? }

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(price_of_apples(35), 70);
    CHECK_EQ_INT(price_of_apples(40), 80);
    CHECK_EQ_INT(price_of_apples(41), 41);
    CHECK_EQ_INT(price_of_apples(65), 65);
    CHECK_EQ_INT(is_leap_year(2000), 1);
    CHECK_EQ_INT(is_leap_year(1900), 0);
    CHECK_EQ_INT(is_leap_year(2024), 1);
    CHECK_EQ_INT(is_leap_year(2023), 0);
    CHECK_EQ_INT(gcd(48, 18), 6);
    CHECK_EQ_INT(gcd(17, 5), 1);
    CHECK_EQ_INT(gcd(0, 9), 9);
    return clings_summary();
}
