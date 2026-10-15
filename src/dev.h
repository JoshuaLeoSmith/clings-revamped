#ifndef CLINGS_DEV_H
#define CLINGS_DEV_H

#include <stddef.h>
#include "buf.h"
#include "info.h"

/* Append line[0..n) to out as the inside of a C string literal. */
void embedded_escape_line(const char *line, size_t n, struct buf *out);

/* Emit the text of src/embedded.c, reading every exercise file from disk
 * (paths relative to the current directory). Exits via die() on read errors. */
void dev_generate_embedded(const struct info_file *info, struct buf *out);

/* Regenerate generated files. Returns 0, or 2 after reporting an error. */
int dev_update(const struct info_file *info);

#endif
