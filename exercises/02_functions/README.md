# Functions

A C function has a fixed return type and a fixed list of typed parameters, and the compiler must have seen a declaration (a prototype) of a function before any call to it.
Arguments are passed by value: the function gets copies, so changing a parameter never changes the caller's variable.
A function with a non-`void` return type must end with an explicit `return`; nothing is returned implicitly.

## Further information

- K&R 1.7 Functions
- K&R 1.8 Arguments: Call by Value
- K&R 4.1-4.2 Basics of Functions, Functions Returning Non-integers
- Beej's Guide to C, chapter 4 (Functions)
