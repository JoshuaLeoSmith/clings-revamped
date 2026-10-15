# Bits

C's bitwise operators work on each bit of an integer separately: `&` (AND) gives 1 where both bits are 1, `|` (OR) where either is, `^` (XOR) where exactly one is, and `~` (NOT) flips every bit; `<<` and `>>` shift the bits left or right, so `1u << n` is a number with only bit `n` set.

| a | b | a & b | a \| b | a ^ b | ~a |
|---|---|-------|--------|-------|----|
| 0 | 0 | 0     | 0      | 0     | 1  |
| 0 | 1 | 0     | 1      | 1     | 1  |
| 1 | 0 | 0     | 1      | 1     | 0  |
| 1 | 1 | 1     | 1      | 0     | 0  |

Combined with a mask such as `1u << n`, `x & mask` tests a bit, `x | mask` sets it, `x & ~mask` clears it and `x ^ mask` toggles it.
Do bit work on `unsigned` types: shifting a 1 into or past the sign bit of a signed `int` is undefined behaviour, and the `u` suffix (`1u`) makes a constant unsigned.
A set of on/off options (flags) fits in one integer when each option is a distinct power of two, so each one owns a single bit and `|` combines them without collisions.

## Further information

- K&R 2.9 Bitwise Operators
- K&R 6.9 Bit-fields
- Beej's Guide to C, chapter 24 (Bitwise Operations)
