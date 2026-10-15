# Text

C has no string type, so text processing is loops over `char`s plus a toolbox of library functions.
`<ctype.h>` classifies and converts single characters (`isdigit`, `isalpha`, `isspace`, `toupper`, `tolower`); pass them an `unsigned char` value, since a plain `char` may be negative.
Turning digits into a number is arithmetic on character codes: `'7' - '0'` is 7, and `atoi`, `strtol` and friends in `<stdlib.h>` do this for you.
`sprintf` formats into a buffer without knowing how big it is, so prefer `snprintf(buf, sizeof buf, ...)`, which never writes past the size you give; `strtok` splits a string into tokens by writing `'\0'` into it and remembering its place between calls.

## Further information

- K&R 2.7 Type Conversions
- K&R 5.5 Character Pointers and Functions
- K&R 7.8.2 Character Class Testing and Conversion
- Beej's Guide to C, section 15.1 (String Conversions) and chapter 21 (Characters and Strings II)
