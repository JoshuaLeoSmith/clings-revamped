# Function pointers

A function's name, used without calling it, gives you a pointer to that function, which you can store in a variable, pass to another function, and call later, much like passing a function or closure in other languages (C has no closures, though: only the function itself).
The declaration syntax is the tricky part: `int (*op)(int, int)` reads from the name outwards as "`op` is a pointer to a function taking two `int`s and returning `int`"; without the parentheses, `int *op(int, int)` declares a function returning `int *` instead.
Both `op = add;` and `op = &add;` work, as do `op(2, 3)` and `(*op)(2, 3)`, and a `typedef` such as `typedef int (*IntFn)(int);` makes the type easier to write.
The standard library uses function pointers for callbacks, most famously `qsort`, which calls your comparison function with `const void *` pointers to the two elements it is comparing.

## Further information

- K&R 5.11 Pointers to Functions
- Beej's Guide to C, section 23.7 (Pointers to Functions)
