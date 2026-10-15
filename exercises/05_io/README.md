# Input and output

`getchar()` reads one character from standard input and returns it as an `int`, or the special value `EOF` when the input runs out.
That is why the variable you read into must be an `int`, not a `char`: it has to hold every character and `EOF` too.
`scanf` reads formatted input such as numbers, and it needs the address of each variable to store into, written with `&`.
In these exercises clings feeds a fixed text to your program's standard input.

## Further information

- K&R 1.5 Character Input and Output (1.5.1-1.5.4)
- Beej's Guide to C, section 9.4
