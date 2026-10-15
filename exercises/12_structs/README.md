# Structs

A `struct` groups named fields, possibly of different types, into one new type: `struct Point { int x; int y; };`.
You can initialize one by position (`{3, 4}`) or by field name (`{.x = 3, .y = 4}`), and any field you leave out is set to zero.
Structs are copied when you assign them, pass them to a function, or return them, so a function that must change the caller's struct takes a pointer to it; through a pointer, fields are reached with `p->x`, which is shorthand for `(*p).x`.
Structs can contain other structs and arrays, and arrays can hold structs.

## Further information

- K&R 6.1 Basics of Structures
- K&R 6.2 Structures and Functions
- K&R 6.3 Arrays of Structures
- K&R 6.4 Pointers to Structures
- Beej's Guide to C, chapters 8 (Structs) and 20 (Structs II: More Fun with Structs)
