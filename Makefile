CC ?= cc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wshadow -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -I include -I src

SRCS := $(wildcard src/*.c)
LIB_SRCS := $(filter-out src/main.c,$(SRCS))
UNIT_SRCS := $(wildcard tests/unit/test_*.c)
UNIT_BINS := $(patsubst tests/unit/%.c,tests/unit/bin/%,$(UNIT_SRCS))

.PHONY: all unit-tests test check clean

all: clings

clings: $(SRCS) $(wildcard src/*.h) include/clings.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

tests/unit/bin/%: tests/unit/%.c $(LIB_SRCS) $(wildcard src/*.h) include/clings.h
	@mkdir -p tests/unit/bin
	$(CC) $(CFLAGS) -o $@ $< $(LIB_SRCS) -lm

unit-tests: $(UNIT_BINS)
	@set -e; for t in $(UNIT_BINS); do echo "== $$t"; ./$$t; done

test: unit-tests clings
	tests/integration/test_cli.sh ./clings

check: clings
	./clings dev check

clean:
	rm -rf clings tests/unit/bin
