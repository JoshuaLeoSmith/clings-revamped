#ifndef CLINGS_EMBEDDED_H
#define CLINGS_EMBEDDED_H

#include <string.h>

struct embedded_file {
    const char *path;
    const char *contents;
};

/* Pristine text of the exercise at path (as in info.toml), or NULL. */
const char *embedded_lookup(const char *path);

#endif
