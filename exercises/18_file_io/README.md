# File I/O

C reads and writes files through a `FILE *` from `<stdio.h>`: `fopen(path, mode)` opens one ("r" to read, "w" to create or overwrite) and returns NULL if it cannot, so every `fopen` needs a NULL check.
The familiar functions have file versions: `fprintf`, `fscanf` and `fgets` take the `FILE *`, and `fscanf` returns how many values it converted, which is how you notice the end of the input.
Close every file you open with `fclose`, on every path out of the function, or its buffered output may never reach the disk.
Relative paths are relative to the program's current directory; clings runs these exercises from this directory, so `fileio1.txt` here is found as just `"fileio1.txt"`.

## Further information

- K&R 7.5 File Access
- K&R 7.6 Error Handling - Stderr and Exit
- K&R 7.7 Line Input and Output
- Beej's Guide to C, chapter 9 (File Input/Output)
