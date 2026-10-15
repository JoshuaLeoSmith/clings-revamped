# clings: rustlings-style exercises for C

Date: 2026-10-15
Status: approved in conversation; written spec for implementation planning

## 1. Purpose

`clings` is a set of small, self-checking C exercises plus a terminal runner,
modelled directly on rustlings. The learner keeps a terminal open running the
watcher, edits exercise files in CLion, and each save recompiles and reruns the
current exercise until it passes.

Audience: one learner who already knows another language (they have used
rustlings) and is new to C. Exercises move at rustlings' pace and concentrate on
what is different about C: types and conversions, pointers, manual memory,
undefined behaviour, the preprocessor, and the standard library.

Source material: Kernighan & Ritchie, *The C Programming Language*, 2nd ed.
(K&R) for the core order and classic exercises, and Beej's Guide to C
Programming (Beej) for modern coverage. Each topic README cites both.

## 2. Goals and non-goals

Goals

- Same learning loop as rustlings: watch mode, `n`/`h`/`l`/`x`/`q`, progress
  bar, hints, solutions, state file, `run`/`hint`/`reset`/`list`/`check-all`.
- 88 exercises across 25 topics plus 3 quizzes (section 12).
- Every exercise fails unsolved and every solution passes with warnings as
  errors, enforced by `clings dev check`.
- First-class CLion use: the folder opens as a CMake project with one target per
  exercise, so code insight, inline warnings, and Run/Debug work per exercise.
- The runner is written in C11 with POSIX only. Dependencies: gcc (or clang),
  make, optionally cmake for CLion.

Non-goals (this version)

- Windows outside WSL.
- rustlings' scrolling TUI list with search. List mode is a printed list plus a
  jump prompt.
- `dev new` scaffolding, parallel check-all, plugin exercises, i18n.
- Threads, atomics, setjmp, locale, wide characters, VLAs, `goto`.

## 3. Repository layout

```
clings/
  README.md                    getting started (terminal + CLion), key map, book mapping
  Makefile                     `make` builds ./clings; `make test`; `make check`; `make clean`
  CMakeLists.txt               runner + generated per-exercise targets (for CLion)
  info.toml                    ordered exercise list, hints, welcome/final messages
  include/clings.h             header-only CHECK macros used by exercise tests
  src/                         the runner (section 9)
  exercises/NN_topic/name.c    working copies the learner edits
  exercises/NN_topic/README.md topic summary + K&R and Beej chapter references
  exercises/NN_topic/*.c|*.h|*.txt  fixed helper sources and data files (never edited)
  exercises/quizzes/quizN.c
  solutions/NN_topic/name.c    one solution per exercise, same path under solutions/
  tests/unit/*.c               runner unit tests
  tests/integration/test_cli.sh  end-to-end test of the built binary
  docs/superpowers/specs/      this spec; plans go in docs/superpowers/plans/
  .clings-state.txt            generated progress file (gitignored)
  .clings-build/               compiled exercise binaries (gitignored)
```

`.gitignore` covers `*.pdf` (the reference books stay on disk but are never
committed), the state file, build dirs (`.clings-build/`, `build/`,
`cmake-build-*/`), `.idea/`, the `clings` binary, `*.o`, and `*_out.txt`
files written by the file I/O exercises.

## 4. Learner workflow

Terminal:

```
cd clings
make            # builds ./clings
./clings        # watch mode
```

CLion: File > Open the `clings` folder. CLion reads `CMakeLists.txt` and
indexes every exercise. Edit in CLion; CLion autosaves, and the watcher in the
terminal reruns the current exercise within about a quarter of a second. The
Run/Debug buttons work on any exercise target for stepping through pointer and
memory exercises. Optional: `./clings --edit-cmd clion` makes the runner open
each new exercise in CLion when you press `n` (requires the `clion` launcher
script on PATH).

## 5. Runner command line

```
clings                       watch mode (default)
clings run [NAME]            check one exercise (current if NAME omitted) and print output
clings hint [NAME]           print the hint
clings reset NAME            restore the original exercise file (asks y/n unless --yes)
clings list                  print all exercises with DONE/PENDING and the current marker
clings check-all             check every exercise, update the state file, report pending
clings dev check             author gate (section 10)
clings dev update            regenerate CMakeLists.txt block and src/embedded.c
clings --help | --version
```

Global options: `--edit-cmd CMD` (or env `CLINGS_EDIT_CMD`) runs `CMD PATH`
detached whenever the current exercise changes. Env `CC` selects the compiler
(default `cc`). Env `CLINGS_NO_SANITIZE=1` drops the sanitizer flags for
toolchains that lack them.

Exit codes: 0 success; 1 exercise pending or check failed; 2 usage or
environment error (no compiler, bad `info.toml`, no `exercises/` dir, stdout not
a terminal in watch mode).

## 6. Checking an exercise

Compile:

```
$CC -std=c11 -Wall -Wextra -Wshadow -g \
    -fsanitize=address,undefined -fno-sanitize-recover=all \
    -I include -I exercises/NN_topic \
    -o .clings-build/NAME exercises/NN_topic/NAME.c [exercises/NN_topic/SOURCE.c ...]
```

- `strict = true` in `info.toml` adds `-Werror`. Otherwise warnings are shown
  but do not fail the exercise.
- Solutions compile with the same line plus `-Werror`, with paths under
  `solutions/`. Helper `sources` and `-I` always point at the exercise
  directory, so there is exactly one copy of each helper.
- Compiler stdout and stderr are captured and shown verbatim; gcc's
  `file:line:col:` form is what CLion's terminal turns into clickable links.

Run (only if compilation succeeded):

- Working directory: the exercise directory (`exercises/NN_topic`), or the
  solution directory for solutions.
- stdin: the `stdin` string from `info.toml` if present, otherwise `/dev/null`.
- stdout and stderr merged and captured, capped at 64 KiB (then the process is
  killed and a truncation note is appended).
- Timeout: 5 seconds, then SIGKILL.
- Pass if and only if the exit status is 0. Nonzero exit, death by signal, and
  timeout are all failures with a clear red one-line reason after the output
  ("exited with status 1", "crashed with SIGSEGV", "killed after 5 seconds, is
  there an infinite loop?"). AddressSanitizer and UBSan reports appear in the
  captured output and already explain the fault.

Rendered block, same shape as rustlings:

```
Output
======
<compiler output, then program output>
<optional red failure line>
```

## 7. Exercise files and `clings.h`

Conventions copied from rustlings: an explanatory comment at the top, `// TODO`
markers where work is needed, a `main` the learner may experiment in, and for
exercises with checks a trailing block introduced by `// Don't change the
tests!`. Exercises whose failure is a compile error need no checks. Each topic
directory has a `README.md` with a few sentences on the concept and the K&R and
Beej sections to read.

`include/clings.h` is header-only and uses only `<stdio.h>`, `<string.h>`,
`<math.h>`. Every macro evaluates its arguments once and keeps going after a
failure so the learner sees every failing check.

```c
CHECK(cond)                       // prints "FAIL file:line: cond" on failure
CHECK_EQ_INT(actual, expected)    // compares as long long; prints both values
CHECK_EQ_CHAR(actual, expected)   // prints as 'c'
CHECK_EQ_STR(actual, expected)    // strcmp; NULL-safe; prints both strings
CHECK_EQ_DBL(actual, expected)    // fabs(a - e) < 1e-9
int clings_summary(void);         // prints "All N checks passed" or
                                  // "F of N checks FAILED"; returns 0 or 1
```

Typical tail of `main`:

```c
    // Don't change the tests!
    CHECK_EQ_INT(square(4), 16);
    CHECK_EQ_INT(square(-3), 9);
    return clings_summary();
```

The runner's own unit tests use the same header.

## 8. Data files

### 8.1 `info.toml`

Same shape as rustlings. The runner parses a documented TOML subset, nothing
more: `#` comments, blank lines, `key = value` pairs, `[[exercises]]` table
headers. Values: basic strings `"..."` with escapes `\\ \" \n \t`, multi-line
`"""..."""` strings (a newline immediately after the opening quotes is dropped,
as in TOML), `true`/`false`, integers, and single-line arrays of basic strings.
Anything else is a parse error reported with the line number.

```toml
format_version = 1
welcome_message = """..."""
final_message = """..."""

[[exercises]]
name = "io1"                  # unique, [A-Za-z0-9_], max 32 chars
dir = "05_io"                 # exercises/<dir>/<name>.c
hint = """..."""
strict = true                 # optional: warnings are errors
stdin = "hello\nworld\n"      # optional: fed to the program
sources = ["mathutil.c"]      # optional: extra sources in the exercise dir
skip_check_unsolved = true    # optional: dev check does not require failure
```

### 8.2 `.clings-state.txt`

Byte-for-byte rustlings' format: line 1 `DON'T EDIT THIS FILE!`, line 2 empty,
line 3 current exercise name, line 4 empty, then one done exercise name per
line. Unreadable or malformed files are treated as "no progress yet" and the
welcome message is shown. The file is rewritten in full on every change.

### 8.3 Embedded originals and `reset`

`clings dev update` regenerates `src/embedded.c`, a table of
`{ path, contents }` for every exercise `.c` file, from the pristine tree. It is
committed, so the learner's edits never change it and rebuilding the runner is
always safe. `reset` writes the embedded contents back to the path. `dev check`
fails if the table is stale.

## 9. Runner architecture (`src/`)

Each module has a header with a small interface and owns one concern.

| Module | Responsibility | Key interface |
|---|---|---|
| `buf.c` | growable byte buffer, read/write whole file, `die()` | `buf_append`, `buf_printf`, `file_read`, `file_write` |
| `info.c` | parse `info.toml` subset into `struct info_file` | `info_parse(text, &info, err, errlen)`; `exercise_info { name, dir, path, hint, strict, stdin_text, sources[], skip_check_unsolved }` |
| `state.c` | progress state, load/save state file | `state_load`, `state_save`, `state_set_current`, `state_mark_done`, `state_next_pending`, `state_find` |
| `proc.c` | run a child with argv, cwd, stdin text, timeout, output cap | `proc_run(argv, cwd, stdin_text, timeout_ms, max_out, &result)`; result has `exited/status/signal/timed_out/truncated/output` |
| `exercise.c` | paths, compile, run, pass/fail, output block | `exercise_check(info, ex, solution, werror, &output) -> bool` |
| `embedded.c` | generated originals table | `embedded_lookup(path) -> const char *` |
| `term.c` | ANSI colours, clear, progress bar, file link, raw mode, width | `term_raw_on/off`, `term_progress_bar`, `term_file_link` |
| `watch.c` | watch loop | `watch_run(state, opts)` |
| `list.c` | printed list and jump prompt | `list_print`, `list_prompt_jump` |
| `dev.c` | `dev check`, `dev update` (CMake block, embedded table) | `dev_check`, `dev_update` |
| `main.c` | argument parsing, dispatch, welcome/final messages | |

Watch loop: put the terminal in raw mode (ICANON and ECHO off), restore on exit
and in a SIGINT/SIGTERM handler. Use `poll()` on stdin with a 250 ms timeout;
on timeout, `stat` the current exercise file and its helper sources and rerun
when any mtime changed. Keys: `n` next (only when done; reruns and, when all are
done, runs check-all as rustlings does), `h` hint, `l` list then jump prompt,
`x` reset after y/n confirmation, `r` rerun now, `q` quit. Input is ignored
while an exercise is being checked. Render after every event: output block,
hint if requested, "Exercise done" line with solution path when done, progress
bar sized to the terminal width, current path as an OSC 8 file link plus plain
text, then the key prompt.

List mode: print every exercise as `  N  DONE|PENDING  path` with `>` marking
the current one, then prompt `Jump to (name or number, Enter to cancel):`. A
valid answer sets the current exercise and the watcher reruns it.

## 10. Dev tooling

`clings dev check` (the author's gate, run before any "done" claim):

1. `info.toml` parses; names unique and well-formed; every exercise and
   solution file exists; every helper source exists; every `.c` under
   `exercises/` is either a listed exercise or a listed helper.
2. For every exercise, the unsolved file fails (unless `skip_check_unsolved`).
3. For every exercise, the solution passes with `-Werror`.
4. The generated block in `CMakeLists.txt` and `src/embedded.c` match what
   `dev update` would produce.

Progress is printed per exercise; the command exits 1 on the first category of
failure after listing all failures in that category.

`clings dev update` rewrites the region of `CMakeLists.txt` between
`# --- BEGIN GENERATED` and `# --- END GENERATED` with one
`clings_exercise(NAME exercises/DIR/NAME.c [helpers...])` line per exercise and
a matching `clings_solution(...)` line, and regenerates `src/embedded.c`.

## 11. CMake, Makefile, and CLion

`CMakeLists.txt`:

- `project(clings C)`, C11, `enable_testing()`.
- Runner target `clings` from `src/*.c`.
- `function(clings_exercise name src ...)`: `add_executable(name EXCLUDE_FROM_ALL ...)`,
  include dirs `include` and the exercise dir, the same warning and sanitizer
  flags as section 6, binaries under `${CMAKE_BINARY_DIR}/exercises`.
  `EXCLUDE_FROM_ALL` keeps "Build All" from compiling exercises that are
  supposed to fail, while CLion still indexes them and offers Run/Debug.
- `option(CLINGS_SOLUTIONS "Add solution targets" OFF)`; `clings_solution` is a
  no-op unless it is on, so the run configuration list stays short.
- Unit tests and the integration script registered with `add_test`.

`Makefile`: `clings` (default; `cc -std=c11 -O2 -Wall -Wextra src/*.c`),
`test` (build and run unit tests, then the integration script), `check`
(`./clings dev check`), `clean`. It exists so the terminal path is one command
and does not depend on CMake's build directory.

README notes for CLion: exercises run from CLion use the build directory as
working directory by default, so for the file I/O topic set the run
configuration's working directory to the exercise folder, or just use the
terminal runner.

## 12. Curriculum

Name, concept, and how the unsolved file fails. "strict" means `-Werror` for
that exercise. "ASan"/"UBSan"/"LSan" mean the sanitizer report makes the run
exit nonzero. "test" means the CHECK block fails.

### 00_intro (K&R 1.1; Beej 2)
- intro1: welcome banner; compiles and passes; `skip_check_unsolved`; teaches `n` and that saving reruns.
- intro2: `printf` used without `#include <stdio.h>`; compile error, gcc suggests the fix.

### 01_variables (K&R 1.2, 2.1-2.4; Beej 3.1)
- variables1: `x = 5;` with no declaration; compile error.
- variables2: `int x;` read before assignment; strict (`-Wuninitialized`).
- variables3: `int ratio = 3.0 / 4.0;` expected 0.75; test; fix the type.
- variables4: assignment to a `const int`; compile error; fix without removing `const`.
- variables5: `char letter = "a";`; compile error (`-Wint-conversion` is an error in gcc 14+); fix `'a'`.

### 02_functions (K&R 1.7-1.8, 4.1-4.2; Beej 4)
- functions1: call before definition with no prototype; compile error.
- functions2: `ring()` called with too few arguments; compile error.
- functions3: `int square(int n) { n * n; }` has no `return`; strict (`-Wreturn-type`); test.
- functions4: `void increment(int x)` cannot change the caller's variable (call by value); test; fix by returning the value.

### 03_if (K&R 3.2-3.3; Beej 3.3.1)
- if1: implement `bigger(a, b)`; test.
- if2: `if (x = 5)`; strict (`-Wparentheses`); test.
- if3: second statement after a brace-less `if` always runs; strict (`-Wmisleading-indentation`); test.

### 04_loops (K&R 1.3, 3.5-3.7; Beej 3.3.2-3.3.4)
- loops1: `sum_to(n)` off by one; test.
- loops2: `count_digits` while loop never updates `n`; timeout.
- loops3: `count_digits(0)` must be 1; convert to do-while; test.
- loops4: find the first number divisible by 7 and 11; missing `break` returns the last; test.

### 05_io (K&R 1.5; Beej 9.4) all with `stdin` in `info.toml`
- io1: implement `count_chars()` with a `getchar` loop (`int c`, not `char`); test.
- io2: implement `count_lines` and `count_words` with the in/out-of-word state technique; test.
- io3: `scanf("%d %d", a, b)` missing `&`; strict (`-Wformat`), otherwise crashes; test.

### 06_switch (K&R 3.4; Beej 3.3.5)
- switch1: missing `break` falls through; strict (`-Wimplicit-fallthrough`); test.
- switch2: implement `is_vowel` with deliberate stacked cases and `default`; test.

### 07_types (K&R 2.2-2.3, 2.7; Beej 14, 15)
- types1: `printf("%d", sizeof(int))`; strict (`-Wformat`); fix `%zu`; plus compute bytes for ten doubles; test.
- types2: `INT_MAX + 1` in an `int`; UBSan signed overflow; fix with `long long`; test.
- types3: chars are small integers: implement `next_letter` (z wraps to a) and `to_upper_char` with arithmetic; test.
- types4: `for (unsigned i = 10; i >= 0; i--)`; strict (`-Wtype-limits`), otherwise infinite; test.
- types5: `3 / 4 * 100` into a double needs a cast; `0.1 + 0.2 == 0.3` is false, implement `approx_equal`; test.

### 08_operators (K&R 2.5-2.12; Beej 3.2)
- operators1: `j = i++` vs `j = ++i`; test.
- operators2: `a + b / 2` and `x & 1 == 0` precedence; strict (`-Wparentheses`); test.
- operators3: implement `clamp` with `?:` and a running total with `+=`; test.

### quizzes/quiz1 (01-08)
- quiz1: `price_of_apples` (rustlings homage), `is_leap_year` (K&R), `gcd` with a loop; test.

### 09_arrays (K&R 1.6, 5.7; Beej 6)
- arrays1: declare `int squares[5]`, fill in a loop; wrong size and index; test.
- arrays2: `sizeof(a) / sizeof(a[0])` in `main` works; the same inside a function does not; strict (`-Wsizeof-array-argument`); fix by passing the length; test.
- arrays3: `i <= 5` writes past `int a[5]`; ASan stack-buffer-overflow; test.
- arrays4: `int grid[3][4]`; implement `sum_row(int grid[][4], int row)` and `sum_all`; test.

### 10_strings (K&R 1.9, 5.5, 7.8.1; Beej 7, 21) array notation only; pointers come next
- strings1: `char name[5] = "Hello"` has no room for the NUL; `strlen` reads past it; ASan; test.
- strings2: implement `my_strlen(const char s[])`; test.
- strings3: `strcpy` of a long literal into `char buf[8]`; ASan; fix buffer size or `snprintf`; test.
- strings4: `s == "yes"` compares addresses; strict (`-Waddress`); fix `strcmp`; test.
- strings5: implement in-place `reverse(char s[])` and `to_upper(char s[])`; test.

### 11_pointers (K&R 5.1-5.4; Beej 5, 11, 23)
- pointers1: `int *p = x;`; compile error; fix `&x`; then `*p = 20` changes `x`; test.
- pointers2: implement `swap(int *a, int *b)`; test (callback to functions4).
- pointers3: `first_or(const int *p, int fallback)` dereferences NULL; ASan SEGV; test.
- pointers4: pointer arithmetic: implement `sum(const int *begin, const int *end)`; `*(p + 2) == p[2]`; test.
- pointers5: pointer to pointer: implement `swap_strings(const char **a, const char **b)`; test.

### 12_structs (K&R 6.1-6.4; Beej 8, 20)
- structs1: `struct Point` missing fields; compile error; designated initializers; test.
- structs2: `p.x` through a pointer; compile error; fix `->`; test.
- structs3: `birthday(struct Person p)` by value changes nothing; implement `make_point` returning a struct; test.
- structs4: nested `struct Rect { struct Point tl, br; }` area; `total_age(const struct Person people[], int n)`; test.

### 13_enums_typedef (K&R 6.7; Beej 10, 22)
- enums1: `enum Color` missing an enumerator; `switch` without that case; strict (`-Wswitch`); test.
- enums2: explicit values `enum HttpStatus { OK = 200, ... }`; implement `is_error`; test.
- typedef1: `typedef struct { ... } Point;` then `struct Point p;`; compile error; `typedef unsigned char byte;`; test.

### quizzes/quiz2 (09-13)
- quiz2: `struct Student[]` with `highest_score` and `average_score`; `count_char(const char *s, char c)`; `trim_newline(char *s)`; test.

### 14_memory (K&R 7.8.5; Beej 12)
- memory1: `malloc` an int array, fill, never `free`; LSan; test.
- memory2: `malloc` memory assumed zeroed (ASan fills it with 0xbe); test; fix `calloc`.
- memory3: read after `free`; ASan heap-use-after-free; test.
- memory4: implement `IntVec` push with `realloc` (capacity doubling); leak checked; test.
- memory5: `my_strdup` allocates `strlen(s)` bytes, no NUL; ASan heap-buffer-overflow; caller frees; test.

### 15_scope (K&R 4.3-4.8; Beej 13, 16)
- scope1: variable declared inside an `if` block used after it; compile error.
- scope2: `next_id()` resets its counter every call; test expects 1, 2, 3; fix `static`.
- scope3: a local shadows the global `counter`; strict (`-Wshadow`); test.

### 16_preprocessor (K&R 4.11; Beej 19)
- preprocessor1: `#define MAX 100;` trailing semicolon; compile error.
- preprocessor2: `#define SQUARE(x) x * x` with `SQUARE(a + 1)`; test; fix parentheses.
- preprocessor3: `#ifndef GREETING` default and `#if MAX_USERS > 100` / `#error`; define `MAX_USERS`; test.
- preprocessor4: two-statement macro breaks `if`/`else`; compile error; fix `do { } while (0)`; test.

### 17_multifile (K&R 4.5; Beej 17) with `sources` and `-I` on the exercise dir
- multifile1: calls `add`/`multiply` from `mathutil.c` without `#include "mathutil.h"`; compile error; test. `mathutil.h` shows an include guard.
- multifile2: `counter.c` has `static int helper(void)` and a global `call_count`; calling `helper` is a link error, reading `call_count` needs `extern`; test.

### 18_file_io (K&R 7.5-7.7; Beej 9) data files in the exercise dir, cwd is the exercise dir
- fileio1: implement `count_lines("fileio1.txt")` with `fopen`/`fgets`/`fclose`; test.
- fileio2: write 1..5 with `fprintf` to `fileio2_out.txt`, read back with `fscanf`; test.
- fileio3: `sum_numbers(path)` must return -1 when `fopen` fails; unsolved crashes on a missing file; ASan SEGV; test.

### 19_cmdline (K&R 5.10; Beej 18) tests pass a fake `argv`
- cmdline1: implement `count_args` and `join_args(int argc, char **argv, char *out, size_t cap)`; test.
- cmdline2: implement `parse_and_sum(argc, argv, long *out)` with `strtol` and `endptr` checks, returning `EXIT_FAILURE` on bad input; test.

### 20_function_pointers (K&R 5.11; Beej 23.7)
- funcptr1: `int *op(int, int) = add;`; compile error; fix `int (*op)(int, int)`; test.
- funcptr2: implement `apply(int *a, int n, int (*f)(int))` and a `typedef`; test.
- funcptr3: `qsort` comparator forgets to dereference; test.

### 21_recursion (K&R 4.10)
- recursion1: `factorial` without a base case; ASan stack-overflow; test.
- recursion2: implement recursive `power(base, exp)` and `sum_digits(n)`; test.

### 22_bits (K&R 2.9, 6.9; Beej 24)
- bits1: implement `is_odd`, `set_bit`, `clear_bit` with `& | ~`; test.
- bits2: implement `get_bit`; `1 << 31` on `int` is UBSan; fix `1u`; test.
- bits3: flag enum `PERM_READ = 1 << 0` etc.; implement `has_flag` and `toggle_flag`; test.

### 23_linked_list (K&R 6.5; Beej 20.3)
- linkedlist1: `struct Node`; implement `push_front(struct Node **head, int v)` and `length`; leak checked; test.
- linkedlist2: implement `append` and `sum`; test.
- linkedlist3: implement `remove_first(struct Node **head, int v)` and `free_list`; use-after-free and leak checked; test.

### 24_text (K&R 2.7, 5.5, 7.8.2; Beej 15.1, 21)
- text1: `<ctype.h>`: implement `count_vowels`, `count_digits`, `capitalize_words`; test.
- text2: implement `my_atoi` (K&R 2.7) handling leading spaces and a sign; test.
- text3: `sprintf` into a small buffer; ASan; fix `snprintf`; implement `split_words` with `strtok`; test.

### quizzes/quiz3 (14-24)
- quiz3: `struct Stack` on a dynamic array (push, pop, free) and `char *join(const char *const *words, int n, const char *sep)` returning malloc'd memory; leak checked; test.

Total: 88 exercises.

## 13. Error handling

- No compiler: "No C compiler found. Install gcc (`sudo apt install build-essential`) or set `CC`." exit 2.
- `info.toml` errors: `info.toml:LINE: message`, exit 2.
- Unknown exercise name in `run`/`hint`/`reset`: list the closest matches, exit 2.
- Watch mode without a TTY: exit 2 with a message to use `clings run`.
- Raw mode is always restored, including on Ctrl-C and on error paths.
- Child output is truncated at 64 KiB with a note; timeouts, signals, and
  nonzero exits each get their own one-line explanation.
- `reset` of an exercise with no embedded original (should not happen in a
  consistent tree) reports it instead of writing an empty file.

## 14. Testing

Built test-first, each piece with a failing test before the code.

- Unit tests (`tests/unit/`, run by `make test` and `ctest`), each a `main`
  using `include/clings.h`:
  - `test_buf.c`: append, printf, file round trip.
  - `test_info.c`: the TOML subset: every value kind, multi-line strings,
    arrays, defaults, every error path with the right line number.
  - `test_state.c`: load/save round trip, malformed file treated as fresh,
    next pending, mark done, set current by name.
  - `test_proc.c`: exit status, signal, timeout, stdin text delivered, output cap.
  - `test_exercise.c`: compile failure, run failure, strict warning handling,
    helper sources, cwd, against tiny C files written to a temp dir.
- Integration (`tests/integration/test_cli.sh`): copies `info.toml`,
  `include/`, `exercises/`, `solutions/` to a temp dir and drives the built
  binary: `run intro1` passes; `run variables1` fails with exit 1 and shows
  the compiler error; `hint`; `list` shows the current marker; editing a file
  then `run` passes and the state file records it; `reset` restores the
  original; `check-all` reports the pending count; `dev check` on a
  deliberately broken copy fails with a useful message.
- `clings dev check` across the real tree is the content gate and is run
  before completion.

## 15. README outline

What clings is; prerequisites (gcc, make, cmake, CLion); terminal setup;
CLion setup and the working-directory note; the watch-mode screen and keys;
commands; how an exercise is checked, with a short guide to reading an
AddressSanitizer report and what the timeout means; exercise-to-chapter table
for K&R and Beej; troubleshooting; authoring notes (`dev check`, `dev update`,
`info.toml` fields).
