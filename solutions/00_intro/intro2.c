// Fix: `#include <stdio.h>` declares `printf`, so the program now compiles.
/*
 * intro2
 *
 * This program should print `Hello, world!`.
 * But something that `printf` needs is missing, so it does not compile.
 * Read the compiler message: it tells you exactly what to add.
 */

#include <stdio.h>

int main(void) {
    printf("Hello, world!\n");
    return 0;
}
