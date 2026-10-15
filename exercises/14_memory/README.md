# Memory

Besides variables on the stack, which vanish when their function returns, a C program can ask for memory on the heap with `malloc(n)`, which returns a pointer to `n` bytes (or `NULL` if there are none to give).
That memory stays allocated until you pass the pointer to `free`; nothing frees it for you, so forgetting is a leak, and using it after `free` is undefined behaviour.
`malloc` does not clear the bytes it gives you, `calloc(count, size)` does, and `realloc(p, n)` resizes a block, possibly moving it to a new address.
Always size an allocation in bytes with `sizeof`, and remember that a string needs one byte more than `strlen` reports, for its `'\0'`.

## Further information

- K&R 7.8.5 Storage Management
- Beej's Guide to C, chapter 12 (Manual Memory Allocation)
