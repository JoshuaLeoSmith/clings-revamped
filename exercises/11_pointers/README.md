# Pointers

A pointer is a variable that holds the address of another object: `&x` gives the address of `x`, and `*p` is the object `p` points to, which you can read or assign through.
Because C passes every argument by value, pointers are how a function changes its caller's variables, as in `swap(&a, &b)`.
A pointer into an array can be moved: `p + n` points `n` elements further on, whatever the element size, so `*(a + i)` and `a[i]` are the same thing; and a pair of pointers `begin`, `end` (one past the last element) describes a range.
`NULL` points nowhere, and following it is undefined behaviour, so check for it before you dereference a pointer that may be `NULL`.
A string literal such as `"left"` must not be modified, so the right type to point at one is `const char *`, a pointer to `const char`; the pointer itself can still be changed to point elsewhere, and to change it from inside a function you pass its address, a `const char **`.

## Further information

- K&R 5.1 Pointers and Addresses
- K&R 5.2 Pointers and Function Arguments
- K&R 5.3 Pointers and Arrays
- K&R 5.4 Address Arithmetic
- Beej's Guide to C, chapters 5 (Pointers), 11 (Pointers II: Arithmetic) and 23 (Pointers III: Pointers to Pointers and More)
