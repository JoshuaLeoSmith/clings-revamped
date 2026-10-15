# Command-line arguments

A C program receives its command line through `int main(int argc, char **argv)`: `argc` counts the strings and `argv` points to them, with the program's own name in `argv[0]` and a NULL pointer after the last one.
Every argument is a string, so a number has to be converted with `strtol`, which reports problems only through its `endptr` argument (no digits, or junk after them) and through `errno`, set to `ERANGE` when the value does not fit in a `long`.
The `int` that `main` returns is the program's exit status, which the shell and build tools read: `EXIT_SUCCESS` (0) means it worked and `EXIT_FAILURE` means it did not, both from `<stdlib.h>`.
clings runs these exercises without arguments, so the tests build a fake `argv` array; in your IDE you can set "Program arguments" to try real ones.

## Further information

- K&R 5.10 Command-line Arguments
- Beej's Guide to C, chapter 18 (The Outside Environment)
