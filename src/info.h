#ifndef CLINGS_INFO_H
#define CLINGS_INFO_H

#include <stdbool.h>
#include <stddef.h>
#include "buf.h"

struct exercise_info {
    char *name, *dir, *path, *hint, *stdin_text;
    char **sources;
    size_t n_sources;
    bool strict, skip_check_unsolved;
};

struct info_file {
    int format_version;
    char *welcome_message, *final_message;
    struct exercise_info *exercises;
    size_t n;
};

/* Parse the info.toml subset from a NUL-terminated string (not modified).
 * On failure returns false and writes "LINE: message" to err. */
bool info_parse(const char *text, struct info_file *out, char *err, size_t errlen);

/* Read and parse a file; errors are prefixed with "info.toml:". */
bool info_load(const char *path, struct info_file *out, char *err, size_t errlen);

/* Frees everything and zeroes the struct; safe after a failed parse. */
void info_free(struct info_file *f);

/* Index of the exercise called name, or -1. */
long info_find(const struct info_file *f, const char *name);

/* Append up to 3 best-matching exercise names, one per line. */
void info_closest(const struct info_file *f, const char *name, struct buf *out);

#endif
