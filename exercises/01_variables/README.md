# Variables

In C every variable is declared with a type before it is used, and the type is fixed for its whole life.
The type decides what values fit: assigning a `double` to an `int` silently drops the fraction, and a `char` holds one character, not a string.
Local variables are not initialized for you, so reading one before assigning it gives you garbage.
`const` asks the compiler to reject any later assignment to a variable.

## Further information

- K&R 1.2 Variables and Arithmetic Expressions
- K&R 2.1-2.4 Variable Names, Data Types and Sizes, Constants, Declarations
- Beej's Guide to C, section 3.1 (Variables)
