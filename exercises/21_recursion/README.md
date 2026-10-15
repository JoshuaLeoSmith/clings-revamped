# Recursion

A C function may call itself, and each call gets its own copy of its parameters and local variables in a new stack frame, so a recursive function works just as it does in other languages.
Every recursive function needs a base case, an input it answers without recursing, and every recursive call must move closer to it; otherwise the calls never stop.
C has no recursion limit and no exception for "too deep": each call takes more stack space until the stack runs out, which normally kills the program with a segmentation fault and, under AddressSanitizer, shows up as a `stack-overflow` report.
Recursion is elegant for problems that contain smaller copies of themselves, but a loop uses no extra stack, so prefer one when the iterative version is just as clear.

## Further information

- K&R 4.10 Recursion
