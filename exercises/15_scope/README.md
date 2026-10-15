# Scope

Scope is where in the source a name can be used; lifetime (storage duration) is how long the object behind it exists.
A variable declared inside a block `{ ... }` is visible only from its declaration to the closing brace, and an automatic local is created each time the block is entered and destroyed when it is left.
A variable declared outside every function has file scope and lives for the whole program; a local with the same name hides ("shadows") it, which is legal but almost always a bug, so `-Wshadow` warns about it.
`static` on a local changes its lifetime but not its scope: the variable is created and initialized once, keeps its value between calls, and can still be named only inside its function.

## Further information

- K&R 4.3 External Variables
- K&R 4.4 Scope Rules
- K&R 4.5 Header Files
- K&R 4.6 Static Variables
- K&R 4.7 Register Variables
- K&R 4.8 Block Structure
- Beej's Guide to C, chapters 13 (Scope) and 16 (Types IV: Qualifiers and Specifiers)
