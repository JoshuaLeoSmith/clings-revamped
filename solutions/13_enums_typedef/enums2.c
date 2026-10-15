// Fix: an enum value is an int, so it can be compared like one. `MONDAY = 1`
// makes the following enumerators count on from 1, so SUNDAY is 7.
/*
 * enums2
 *
 * Enumerators count up from 0 unless you give them a value, and any
 * enumerator without one is the previous value plus 1. `HttpStatus` gives
 * every value explicitly: implement `is_error`, which returns 1 for the
 * 4xx and 5xx codes and 0 otherwise. Then make `Weekday` count from 1.
 * See K&R 2.3 and Beej 22.
 */

#include "clings.h"

enum HttpStatus {
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERROR = 500
};

enum Weekday { MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };

int is_error(enum HttpStatus s) {
    return s >= 400;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(is_error(HTTP_OK), 0);
    CHECK_EQ_INT(is_error(HTTP_NOT_FOUND), 1);
    CHECK_EQ_INT(HTTP_CREATED, 201);
    CHECK_EQ_INT(SUNDAY, 7);
    return clings_summary();
}
