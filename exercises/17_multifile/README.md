# Multiple files

Real C programs are split into several `.c` files, each compiled on its own and then joined by the linker.
A header (`.h`) holds the declarations other files need, such as function prototypes and `extern` variable declarations, while exactly one `.c` file holds each definition; an include guard (`#ifndef NAME_H` / `#define NAME_H` / `#endif`) keeps a header from being pasted in twice.
A global variable defined in one file is used from another after `extern int name;`, and `static` in front of a file-scope function or variable makes it private to its file.
Compiler errors name a line in your file; linker errors such as "undefined reference to `helper'" appear only after every file compiled, when a name has no visible definition.

## Further information

- K&R 4.5 Header Files
- Beej's Guide to C, chapter 17 (Multifile Projects)
