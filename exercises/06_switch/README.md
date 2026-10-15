# Switch

A `switch` compares an integer value (a `char` counts) against constant `case` labels and jumps to the one that matches, or to `default` if none does.
Once it has jumped, execution simply runs on through the following cases until a `break` (or `return`) stops it; this "fall through" is a common bug.
It is also useful on purpose: stacking several `case` labels with no statements between them lets them share one body.

## Further information

- K&R 3.4 Switch
- Beej's Guide to C, section 3.3.5 (The switch statement)
