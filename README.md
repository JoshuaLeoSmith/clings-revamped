# clings

clings is supposed to be like
[rustlings](https://github.com/rust-lang/rustlings), but for C: small exercises
to get you used to reading and writing C. It is made for people who already
program in another language and are new to C.

> **A confession.** I set out to learn C properly, with no AI coding assistant
> doing the thinking for me. So, naturally, the first thing I did was vibe code
> an entire tool to help me do that. The irony compiles cleanly, even with
> `-Wall -Wextra`.
>
> The good news is that the runner cannot write a single line of C for you.
> Every missing semicolon and every segfault in here is yours to fix by hand.

There are 88 exercises in 25 topics, plus 3 quizzes. Each one is a short C file
that does not compile or does not pass its checks yet. You fix it, save, and
clings reruns it. When it passes, you move on.

```sh
git clone https://github.com/JoshuaLeoSmith/clings-revamped.git
cd clings-revamped
make        # build the runner
./clings    # start watch mode
```

Then open the current exercise (the path under the progress bar) in CLion or
any editor, fix it, and save.

## What you need

- Linux with `gcc` and `make` (on Ubuntu: `sudo apt install build-essential`).
  Exercises are built with AddressSanitizer and UndefinedBehaviorSanitizer,
  which gcc on Linux includes.
- Optional: CLion and CMake 3.20 or newer, to edit, run, and debug exercises
  in an IDE.
- A copy of "The C Programming Language, 2nd edition" by Kernighan and Ritchie
  (K&R). The books are not part of this repository; you need your own copy.
- Optional: "Beej's Guide to C Programming", free at
  <https://beej.us/guide/bgc/>.

Every topic folder has a `README.md` that explains the idea in a few lines and
points to the matching K&R sections and Beej chapters. The full map is in
[exercises/README.md](exercises/README.md).

## Start in the terminal

Run clings from the project root (the folder with `info.toml`):

```sh
cd clings-revamped
make
./clings
```

The first start shows a welcome message; press Enter. clings then checks the
first exercise and waits. Edit the file and save it: clings notices the save
within about a quarter of a second and checks the exercise again.

Your progress is kept in `.clings-state.txt`. Run `./clings` again later to
continue where you stopped.

## Use CLion

1. File > Open, and choose the `clings-revamped` folder. CLion loads `CMakeLists.txt`
   and creates one target per exercise.
2. Open CLion's Terminal tab and run:

   ```sh
   make && ./clings
   ```

3. Edit exercises in the editor. Save with Ctrl+S (CLion also saves when it
   loses focus); the watcher in the terminal reruns the exercise.
4. To step through an exercise, pick its target (for example `pointers3`) in
   the run configuration list and press Run or Debug. Exercise targets are
   left out of "Build All" on purpose, because many of them do not compile
   until you fix them.

Programs run from CLion start in CLion's build directory, not in the exercise
folder. The `18_file_io` exercises open their data files by relative path, so
for those set the run configuration's "Working directory" to
`exercises/18_file_io`, or just use the terminal runner.

To have clings open each exercise in CLion for you, pass an editor command:

```sh
./clings --edit-cmd clion
```

clings runs `clion PATH` when watch mode starts and each time it moves to
another exercise. This needs a `clion` command on your `PATH`. If you installed
CLion with JetBrains Toolbox, Toolbox can create this launcher as a shell
script; put Toolbox's scripts folder on your `PATH`. You can also set
`CLINGS_EDIT_CMD` instead of passing the option.

To get a run target for every reference solution too (named `NAME_sol`), add
`-DCLINGS_SOLUTIONS=ON` to Settings > Build, Execution, Deployment > CMake >
CMake options.

## Watch mode

The screen shows the output of the last check, a progress bar, the current
exercise path, and a key prompt:

```
n:next / h:hint / l:list / x:reset / r:rerun / q:quit ?
```

| Key | What it does |
|---|---|
| `n` | Go to the next pending exercise. Shown only once the exercise passes. |
| `h` | Show the hint for this exercise. |
| `l` | List all exercises, then type a name or number and Enter to jump there (Enter alone cancels). |
| `x` | Restore this exercise to its original contents, after a `y`/`n` question. |
| `r` | Check the exercise again now. |
| `q` | Quit. Your progress is saved. |

Keys act at once; you do not need to press Enter. When an exercise passes you
see "Exercise done ✓" and the path of a reference solution under `solutions/`.
Compare it with yours, then press `n`.

## Commands

From `./clings --help`:

| Command | What it does |
|---|---|
| `./clings` | Watch mode: rerun the current exercise whenever it changes |
| `./clings run [NAME]` | Run the current exercise, or NAME, once |
| `./clings hint [NAME]` | Show the hint of the current exercise, or of NAME |
| `./clings reset NAME` | Restore exercise NAME to its original contents (no question asked) |
| `./clings list` | List all exercises with their state |
| `./clings check-all` | Check every exercise and report which are pending |
| `./clings --edit-cmd CMD` | Command used to open exercises (default `$CLINGS_EDIT_CMD`) |
| `./clings --help`, `--version` | Print help or the version |

Exit status: 0 on success, 1 when an exercise fails or is pending, 2 for a
usage or setup error.

## How an exercise is checked

clings compiles the exercise with:

```sh
cc -std=c11 -Wall -Wextra -Wshadow -g \
   -fsanitize=address,undefined -fno-sanitize-recover=all \
   -I include -I exercises/DIR -o .clings-build/NAME exercises/DIR/NAME.c [exercises/DIR/HELPER.c ...] -lm
```

- Warnings are shown but do not fail the exercise, except in exercises marked
  strict, where they are errors (`-Werror`).
- If it compiles, the program runs with the exercise folder as its working
  directory.
- It passes only if it exits with status 0. Most exercises end with checks
  that print a `FAIL file:line: ...` line for each one that does not hold.
- A failed run ends with a red line such as `exited with status 1`,
  `crashed with SIGSEGV`, or `killed after 5 seconds, is there an infinite
  loop?`. Every run is stopped after 5 seconds.
- A program that prints more than 64 KiB of output is stopped and the
  exercise fails, with the red line `stopped after 64 KiB of output, is there
  an infinite loop?`.

### Reading a sanitizer report

The sanitizers catch memory and undefined-behaviour bugs while the program
runs and print a report. To read one:

1. The first `ERROR: AddressSanitizer:` line names the bug class (for example `heap-use-after-free`); the next line gives the access, `READ` or `WRITE of size N`.
2. In the stack trace below it, the first frame whose file is your exercise `.c` is where it happened (`#1 ... in main exercises/14_memory/memory3.c:25`).
3. For heap bugs, the `freed by` and `previously allocated by` sections show where the memory came from and where it was released.
4. A `LeakSanitizer` report lists allocations that were never freed, each with the call that made it.
5. UBSan lines look like `file.c:LINE:COL: runtime error: ...` and name the exact operation, such as `index 5 out of bounds for type 'int [5]'`.
6. Any of these makes the program exit nonzero, so clings marks the exercise as failed.

## Troubleshooting

- **"No C compiler found"**: install gcc (`sudo apt install build-essential`),
  or set `CC` to your compiler.
- **Sanitizer link errors** (`cannot find -lasan` or similar): your toolchain
  lacks the sanitizers. Run `CLINGS_NO_SANITIZE=1 ./clings` to check without
  them. Memory bugs then go unreported, so prefer installing them.
- **"watch mode needs a terminal"**: watch mode must run in a real terminal.
  Use `./clings run` from scripts.
- **"no exercises/ directory here"**: run clings from the project root.
- **The terminal does not show what you type** after clings was killed: type
  `reset` and press Enter (the shell command, not `./clings reset`).
- **An exercise is broken beyond repair**: `./clings reset NAME`, or `x` in
  watch mode.

## Adding or changing exercises

Each exercise is an entry in `info.toml`, in the order learners do them:

```toml
[[exercises]]
name = "io1"                  # unique; letters, digits, underscore
dir = "05_io"                 # the file is exercises/05_io/io1.c
hint = """..."""
strict = true                 # optional: warnings are errors
stdin = "hello\nworld\n"      # optional: text fed to the program
sources = ["mathutil.c"]      # optional: extra .c files in the exercise folder
skip_check_unsolved = true    # optional: the unsolved file may pass
```

The reference solution goes in `solutions/DIR/NAME.c`. After adding or editing
exercise files:

```sh
./clings dev update        # regenerate CMakeLists.txt targets and src/embedded.c
make                       # rebuild, so reset knows the new originals
./clings dev check         # every unsolved file fails, every solution passes
./clings dev check 14_memory pointers3   # or only some topics or exercises
make test                  # unit and command-line tests of the runner
```

`dev check` also fails when `dev update` has not been run since the last change.

## License

[MIT](LICENSE). Use it, change it, share it.
