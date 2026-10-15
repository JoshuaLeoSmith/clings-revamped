#ifndef CLINGS_STATE_H
#define CLINGS_STATE_H

#include <stdbool.h>
#include <stddef.h>
#include "info.h"

struct app_state {
    const struct info_file *info;
    const char *path; /* borrowed; caller keeps it alive */
    size_t current;
    bool *done;       /* info->n entries */
    size_t n_done;
};

/* Never fails. *fresh is true when the file is missing or malformed. */
void state_load(struct app_state *st, const struct info_file *info,
                const char *path, bool *fresh);
bool state_save(const struct app_state *st);
bool state_set_current(struct app_state *st, size_t idx);
void state_mark_done(struct app_state *st, size_t idx);
void state_mark_pending(struct app_state *st, size_t idx);
/* First pending exercise after current, wrapping; never current itself. */
bool state_next_pending(const struct app_state *st, size_t *idx);
void state_free(struct app_state *st);

#endif
