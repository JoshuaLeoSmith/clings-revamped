# Enums and typedef

An `enum` defines a set of named integer constants: `enum Color { RED, GREEN, BLUE };` makes `RED` 0, `GREEN` 1 and `BLUE` 2, and you can give any enumerator an explicit value, after which counting continues from there.
gcc's `-Wswitch` warning (part of `-Wall`) notices when a `switch` on an enum without a `default:` has no `case` for one of the enumerators, which helps when you add a new one later.
`typedef` gives an existing type another name, such as `typedef unsigned char byte;`; it is often used to name a struct so you can write `Vec2` instead of `struct Vec2`, and a struct defined without a tag inside a typedef has only the typedef name.

## Further information

- K&R 2.3 Constants (enumeration constants)
- K&R 6.7 Typedef
- Beej's Guide to C, chapters 10 (typedef: Making New Types) and 22 (Enumerated Types: enum)
