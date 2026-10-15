#ifndef CLINGS_DEV_H
#define CLINGS_DEV_H

#include <stdbool.h>
#include <stddef.h>
#include "buf.h"
#include "info.h"

/* Append line[0..n) to out as the inside of a C string literal. */
void embedded_escape_line(const char *line, size_t n, struct buf *out);

/* Emit the text of src/embedded.c, reading every exercise file from disk
 * (paths relative to the current directory). Exits via die() on read errors. */
void dev_generate_embedded(const struct info_file *info, struct buf *out);

/* Emit the generated CMake lines: per exercise, in order,
 * "clings_exercise(NAME DIR [helper.c ...])" then "clings_solution(...)". */
void dev_generate_cmake_block(const struct info_file *info, struct buf *out);

/* Append cmake_text to out with everything between the line starting
 * "# --- BEGIN GENERATED" and the next line starting "# --- END GENERATED"
 * replaced by block (both marker lines kept). False if a marker is missing. */
bool dev_replace_cmake_block(const char *cmake_text, const char *block, struct buf *out);

/* Verify the exercise set; filters (names or dirs) select which exercises are
 * compiled. Returns 0 when everything is fine, 1 on problems, 2 on bad use. */
int dev_check(const struct info_file *info, const char *const *filters, size_t nfilters);

/* Regenerate CMakeLists.txt and src/embedded.c. Returns 0, or 2 after reporting an error. */
int dev_update(const struct info_file *info);

#endif
