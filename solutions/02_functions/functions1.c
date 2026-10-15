// Fix: a prototype declares a function's name, return type and parameters, so
// calls can be checked before the definition appears later in the file.
/*
 * functions1
 *
 * The C compiler reads a file from top to bottom, and it must know about a
 * function before you call it. Here `main` calls `greet`, which is only
 * defined further down. Make the program compile without changing `greet`.
 * See K&R 1.7 and 4.2, and Beej 4.
 */

#include <stdio.h>

void greet(void);

int main(void) {
    greet();
    return 0;
}

void greet(void) {
    printf("Hello from a function!\n");
}
