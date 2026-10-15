#!/usr/bin/env bash
# End-to-end tests for the clings binary.
# Usage: tests/integration/test_cli.sh PATH/TO/clings
# Run from the project root; the tree is copied to a temp dir first.
set -u

if [ $# -ne 1 ]; then
    echo "usage: $0 PATH/TO/clings" >&2
    exit 2
fi
BIN="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

tar -C "$ROOT" -cf - \
    --exclude=./.git --exclude=./.superpowers --exclude=./.clings-build \
    --exclude=./build --exclude='./cmake-build-*' --exclude=./.clings-state.txt \
    --exclude=./clings --exclude='*.pdf' --exclude=./tests/unit/bin \
    . | tar -C "$WORK" -xf -
cd "$WORK" || exit 2

FAILS=0
PASSES=0
OUT=""
LAST=""

pass() { PASSES=$((PASSES + 1)); echo "ok   $*"; }
fail() { FAILS=$((FAILS + 1)); echo "FAIL $*"; }

# expect_exit N cmd... : run cmd, capture stdout+stderr in $OUT, check status.
expect_exit() {
    local want=$1
    shift
    LAST="$*"
    OUT="$("$@" 2>&1)"
    local got=$?
    if [ "$got" -eq "$want" ]; then
        pass "exit $want: $LAST"
    else
        fail "exit $want (got $got): $LAST"
        printf '%s\n' "$OUT" | sed 's/^/     | /'
    fi
}

expect_contains() {
    if printf '%s\n' "$OUT" | grep -qF -- "$1"; then
        pass "  contains: $1"
    else
        fail "  contains: $1  (after: $LAST)"
        printf '%s\n' "$OUT" | sed 's/^/     | /'
    fi
}

expect_matches() {
    if printf '%s\n' "$OUT" | grep -qE -- "$1"; then
        pass "  matches: $1"
    else
        fail "  matches: $1  (after: $LAST)"
        printf '%s\n' "$OUT" | sed 's/^/     | /'
    fi
}

# --version
expect_exit 0 "$BIN" --version
expect_contains "clings 0.1.0"

# run intro1: passes and moves on
expect_exit 0 "$BIN" run intro1
expect_contains "Successfully ran"
expect_contains "Next exercise: exercises/00_intro/intro2.c"

# reset restores the pristine text (after dev update embeds it)
expect_exit 0 "$BIN" dev update
expect_contains "Updated src/embedded.c"
if grep -qF 'exercises/00_intro/intro2.c' src/embedded.c; then
    pass "embedded.c lists intro2"
else
    fail "embedded.c lists intro2"
fi
cp exercises/00_intro/intro1.c "$WORK/intro1.pristine"
echo "// scribble" >> exercises/00_intro/intro1.c
expect_exit 0 "$BIN" reset intro1
expect_contains "has been reset"
if cmp -s exercises/00_intro/intro1.c "$WORK/intro1.pristine"; then
    pass "reset restored intro1.c"
else
    fail "reset restored intro1.c"
fi

# state file contents
if printf "DON'T EDIT THIS FILE!\n\nintro2\n\nintro1\n" | cmp -s - .clings-state.txt; then
    pass "state file after run intro1"
else
    fail "state file after run intro1"
    sed 's/^/     | /' .clings-state.txt
fi

# run intro2: compile error
expect_exit 1 "$BIN" run intro2
expect_contains "error:"
expect_contains "with errors"

# hint
expect_exit 0 "$BIN" hint intro2
expect_contains "Hint:"
expect_contains "names the header that is missing"

# list
expect_exit 0 "$BIN" list
line="$(printf '%s\n' "$OUT" | grep 'intro2')"
case "$line" in
    ">"*PENDING*) pass "  list: intro2 is current and PENDING" ;;
    *) fail "  list: intro2 is current and PENDING: '$line'" ;;
esac
line="$(printf '%s\n' "$OUT" | grep 'intro1')"
case "$line" in
    *DONE*) pass "  list: intro1 is DONE" ;;
    *) fail "  list: intro1 is DONE: '$line'" ;;
esac

# unknown exercise name
expect_exit 2 "$BIN" run intro9
expect_contains "intro1"

# check-all with pending exercises
expect_exit 1 "$BIN" check-all
expect_matches "exercises? pending"

# watch mode without a TTY
LAST="clings </dev/null >file"
"$BIN" </dev/null >"$WORK/watch.out" 2>&1
got=$?
OUT="$(cat "$WORK/watch.out")"
if [ "$got" -eq 2 ]; then
    pass "exit 2: $LAST"
else
    fail "exit 2 (got $got): $LAST"
fi
expect_contains "needs a terminal"

# dev check on the pristine copy
for f in CMakeLists.txt Makefile src/main.c tests/unit/test_buf.c; do
    if [ -f "$f" ]; then pass "copy includes $f"; else fail "copy includes $f"; fi
done
expect_exit 0 "$BIN" dev check
expect_contains "All 2 exercises OK"

# dev check: a solution that fails
cp solutions/00_intro/intro2.c "$WORK/intro2_sol.pristine"
echo 'int main(void) { return 1; }' > solutions/00_intro/intro2.c
expect_exit 1 "$BIN" dev check
expect_contains "intro2"
expect_contains "solution: FAILS"
cp "$WORK/intro2_sol.pristine" solutions/00_intro/intro2.c

# dev check: an exercise edited without dev update
cp exercises/00_intro/intro1.c "$WORK/intro1.pristine"
echo "// scribble" >> exercises/00_intro/intro1.c
expect_exit 1 "$BIN" dev check
expect_contains "src/embedded.c is outdated"
cp "$WORK/intro1.pristine" exercises/00_intro/intro1.c

# CMake project
if command -v cmake >/dev/null 2>&1; then
    expect_exit 0 cmake -S . -B "$WORK/build"
    expect_exit 0 cmake --build "$WORK/build" --target intro1
    expect_exit 0 "$WORK/build/exercises/intro1"
    expect_exit 0 cmake --build "$WORK/build" --target clings
    expect_exit 0 cmake --build "$WORK/build" --target test_buf
    expect_exit 0 ctest --test-dir "$WORK/build" -R test_buf
else
    echo "skip cmake not found; CMake checks skipped"
fi

echo
echo "$PASSES passed, $FAILS failed"
[ "$FAILS" -eq 0 ]
