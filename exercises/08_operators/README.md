# Operators

Most of C's operators look like those in other languages, but a few behave in their own way.
`i++` gives the old value of `i` and then increments it, while `++i` increments first and gives the new value.
Precedence decides how an expression without parentheses is grouped: `/` binds tighter than `+`, and `==` binds tighter than the bitwise `&`, which surprises almost everyone.
The conditional operator `cond ? a : b` picks one of two values, and compound assignments such as `+=` update a variable in place.

## Further information

- K&R 2.5-2.12 Arithmetic, Relational and Logical, Increment and Decrement, Bitwise, Assignment and Conditional operators, Precedence and Order of Evaluation
- Beej's Guide to C, section 3.2 (Operators and Expressions)
