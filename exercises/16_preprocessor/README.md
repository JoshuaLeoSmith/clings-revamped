# Preprocessor

Before the compiler proper sees your code, the preprocessor handles every line that starts with `#`: `#include` pastes in a file, and `#define` makes a name stand for some text.
Macro replacement is purely textual, so a stray `;` in a definition lands wherever the macro is used, and a macro's arguments and result need parentheses to survive operator precedence; a macro that expands to several statements is wrapped in `do { ... } while (0)` so it behaves as one.
`#if`, `#ifdef`, `#ifndef` and `#endif` keep or drop lines of code depending on the macros defined, and `#error` stops the build with a message.
In an `#if` condition, any identifier that is not a defined macro is quietly replaced by `0`, so a forgotten `#define` does not stop `#if` from evaluating.

## Further information

- K&R 4.11 The C Preprocessor
- K&R 4.11.1 File Inclusion
- K&R 4.11.2 Macro Substitution
- K&R 4.11.3 Conditional Inclusion
- Beej's Guide to C, chapter 19 (The C Preprocessor)
