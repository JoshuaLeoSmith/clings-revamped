# clings backlog (deferred from the 2026-10-15 build reviews)

Items the task reviews and the final whole-branch review judged real but not worth blocking the first merge. None affects whether an exercise passes unsolved or a solution fails. Grouped by area; each line names the file.

## Runner

- `src/proc.c`: a child that closes stdout/stderr but keeps running makes the final `waitpid` block with no deadline. Poll with `WNOHANG` until the deadline, then `SIGKILL`.
- `src/proc.c`: `SIGPIPE` is ignored process-wide, so `./clings check-all | head -3` keeps checking all 88 exercises after `head` exits. Have `flush_out` check `ferror(stdout)` and stop.
- `src/proc.c`: stdin text is written before the read loop (fine for the small texts in `info.toml`); document the size assumption in `proc.h`. Pipe fds are not `CLOEXEC`.
- `src/exercise.c`: compiler diagnostics and sanitizer reports are uncoloured because gcc/ASan only colour on a tty. Add `-fdiagnostics-color=always` and `ASAN_OPTIONS=color=always` when colours are enabled, or run the child on a pty.
- `src/exercise.c`: `timeout_ms / 1000` truncates (1500 ms → "1 seconds"); the run path builds `../../` paths, which assumes a one-segment `dir` (add a `dev check` rule); the compile step has no timeout.
- `src/term.c`: the OSC 8 `file://` link is not percent-encoded, so a checkout under a path with spaces gives a broken link. `raw_active` is set after `tcsetattr`, leaving a tiny signal window.
- `src/watch.c`: `--edit-cmd` splits on spaces only (spec says whitespace); raw mode is not reapplied after Ctrl-Z then `fg`; a file missing at snapshot time that stays missing is never treated as changed (accepted; add the unit test); exercise names starting with a digit could not be chosen in the jump prompt (none exist).
- `src/watch.c` / `src/commands.c`: the one-shot `run` all-done path clears the screen (rustlings prints a newline there); `cmd_run` returns 0 even when the final check-all finds pending exercises (rustlings parity).
- `src/main.c`: `CC="gcc -m32"` fails `execvp` and is reported as "No C compiler found"; document that `CC` must be a bare command. `args` is not freed on early returns.
- `src/dev.c`: a name leaks when `lstat` fails in the directory walk; symlinked stray `.c` files are not flagged; `1 problem` singular wording.
- `src/info.c`: a `"""` string whose content ends in `"` closes early; duplicate-key rejection covers only `name`/`dir`/`hint`; the `key[0]=='s'` field dispatch is cryptic.
- `src/state.c`: calling `state_load` twice leaks the old `done` array (called once by design); CRLF or trailing whitespace on names is not stripped.
- `src/buf.c`: `mkdir_p("")` reads out of bounds (only ever called with a constant); `buf_printf`/`die` lack printf format attributes.
- Linux only: `src/watch.c` uses `st_mtim`; macOS needs `st_mtimespec` behind an `#ifdef`.
- `src/main.c`: with a fresh state file, the welcome "Press ENTER" prompt blocks on a closed stdin (only reachable under a pty harness such as `script`; the integration test never starts from a fresh state).

- `src/watch.c` `drain_stdin`: treats `read() == -1` with `EINTR` as stdin closed (the main `read_key` path retries); retry on `EINTR` before setting the closed flag. In the `x` reset confirmation, Ctrl-D cancels instead of quitting.

## Tests

- `tests/unit/test_clings_h.c` and the `leak.c` case in `test_exercise.c` pass because of the line-buffering constructor alone; nothing tests the `fflush` calls on their own, and `find(&out, "FAIL")` also matches `checks FAILED`.

- No unit test pins the "file missing at snapshot, still missing → unchanged" watch rule, the all-done path of `run`, `state_set_current`'s return when the save fails, or several `info.toml` parse edges (trailing backslash, empty array, key with no value, CRLF).
- `tests/unit/test_term.c` depends on the working directory being the repo root; `test_exercise.c` unsets `CC`/`CLINGS_NO_SANITIZE` instead of restoring them; the proc timeout test's margin (0.7 s of 1.0 s) may flake under load.
- `tests/integration/test_cli.sh`: `expect_contains "intro2"` in the broken-solution case is weak (the paired `solution: FAILS` carries the proof).

## Content

- Alternative valid fixes that skip the intended lesson (the checks cannot tell): `types5` (`3 * 100 / 4`), `enums1` (`default:` silences `-Wswitch`), `preprocessor3` (define `MAX_USERS` after the `#if`), `text3` (bigger buffer with `sprintf`), `bits2` (`1LL << shift`). Tighten the TODO wording or hints to name the misconception.
- Wording: `02_functions/README.md` overstates the explicit-return rule and equates declaration with prototype; `04_loops/README.md` "ends only when its condition becomes false" contradicts `break`; `07_types/README.md` "fixed sizes that depend on the machine"; `09_arrays` "overwrites whatever lies after" should say undefined behaviour first; `18_file_io/README.md` "may never reach the disk" (only abnormal termination loses buffered output); `fileio3` "crashes" → undefined behaviour; `variables5` note assumes ASCII; `functions3` note says "garbage"; `bits3` header treats the mask `p` as a bit index; `quiz2` header and hint cite different Beej chapters; `05_io/README.md` Beej 9.4 lacks its title.
- Solutions: `strings3` uses `char buf[32]` without a comment (and could show `snprintf`); `loops2` narrows a `long` counter to `int` without a note (`long long` would be portable); `cmdline2`'s running sum can overflow with real arguments; `quiz3`'s `join` uses quadratic `strcat`; `memory4` silently drops the value when `realloc` fails.
- Hints close to the answer: `functions4`, `switch2`.
- Learner-visible noise: `functions1` shows a follow-on `conflicting types` error; `structs2` shows an extra unused-but-set-parameter warning; `strings1`'s ASan frame names `puts` for the `printf`; `recursion1` could mention gcc's infinite-recursion warning.
- `exercises/quizzes/quiz1.c` header is about 15 lines (deliberate: it states three functions' rules).

## Documentation

- `README.md`: add a `git clone` step and git to prerequisites; the ASan guide's "freed by / previously allocated by" applies to use-after-free and double free only, and the READ/WRITE line differs for SEGV; the "No C compiler" message only appears once `make` has succeeded; CLion's working directory is safer as `$ProjectFileDir$/exercises/18_file_io`; the key table could say `h` disappears once the hint is shown; the authoring example pairs `sources` with a dir that has no such file.
- `include/clings.h` has no usage comment for learners, and its helpers are defined after the macros that use them.
