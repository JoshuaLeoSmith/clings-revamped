// Fix: a call must pass one argument for each parameter in the prototype; C has
// no default arguments, so `ring` always needs its `times`.
/*
 * functions2
 *
 * A function's parameters say what it needs from every caller. `ring`
 * prints a line for each ring, and this program should make it ring 3 times.
 * See K&R 1.7 and Beej 4.
 */

#include <stdio.h>

void ring(int times) {
    for (int i = 1; i <= times; i++) {
        printf("Ring! Call number %d\n", i);
    }
}

int main(void) {
    ring(3);
    return 0;
}
