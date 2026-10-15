# Types

C's integer types have fixed sizes that depend on the machine, and `sizeof` tells you how many bytes a type or variable takes; its result has the unsigned type `size_t`, printed with `%zu`.
A `char` is just a small integer, so you can do arithmetic on characters.
Going past the largest value of a signed type such as `int` is undefined behaviour, and unsigned types can never be negative, so a test like `i >= 0` on one is always true.
Arithmetic on two integers stays integer arithmetic, dropping any fraction, and floating-point numbers are binary approximations, so compare them with a tolerance instead of `==`.

## Further information

- K&R 2.2 Data Types and Sizes
- K&R 2.3 Constants
- K&R 2.7 Type Conversions
- Beej's Guide to C, chapters 14 (Types II: Way More Types!) and 15 (Types III: Conversions)
