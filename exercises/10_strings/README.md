# Strings

C has no string type: a string is an array of `char` whose last element is the null character `'\0'`, and every string function finds the end by searching for it.
So `"Hello"` takes six `char`s, not five, and an array that is too small has no room for the terminator; then `strlen`, `printf` and `strcpy` run past its end.
Nothing checks that a copy fits: `strcpy` writes as many characters as the source has, whatever the size of the destination.
Comparing two strings with `==` compares where they are stored, not what they contain; use `strcmp` from `<string.h>`, which returns 0 when they are equal.
These exercises use array notation (`char s[]`) on purpose, to show that a string is just an array; the next topic, pointers, is the other way C code usually writes them.

## Further information

- K&R 1.9 Character Arrays
- K&R 5.5 Character Pointers and Functions
- K&R 7.8.1 String Operations
- Beej's Guide to C, chapters 7 (Strings) and 21 (Characters and Strings II)
