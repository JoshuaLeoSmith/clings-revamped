# Arrays

An array is a fixed number of elements of one type laid out next to each other in memory, and its size is part of its type: `int a[5]` holds exactly five `int`s, indexed from 0 to 4.
C does not check indexes, so writing `a[5]` silently overwrites whatever lies after the array; here the sanitizers catch it and stop the program with a report.
`sizeof(a) / sizeof(a[0])` gives the number of elements, but only where the array itself is in scope: an array passed to a function arrives as a pointer to its first element, so the function must be given the length separately.
A two-dimensional array such as `int grid[3][4]` is an array of 3 rows of 4 `int`s; a function parameter may leave out the number of rows (`int grid[][4]`) but needs the row length, because the compiler uses it to find where each row starts.

## Further information

- K&R 1.6 Arrays
- K&R 5.7 Multi-dimensional Arrays
- Beej's Guide to C, chapter 6 (Arrays)
