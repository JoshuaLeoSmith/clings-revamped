// Fix: the header gives the compiler the prototypes of functions defined in
// another .c file; the linker then connects the calls to mathutil.c.
/*
 * multifile1
 *
 * `add` and `multiply` are defined in another file, mathutil.c, which
 * clings compiles alongside this one. The compiler works on one .c file at
 * a time, though, so it has never heard of them here. Include the header
 * that declares them.
 * See K&R 4.5 and Beej 17.
 */

#include "clings.h"
#include "mathutil.h"

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(add(2, 3), 5);
    CHECK_EQ_INT(multiply(4, 5), 20);
    return clings_summary();
}
