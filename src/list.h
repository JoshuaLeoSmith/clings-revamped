#ifndef CLINGS_LIST_H
#define CLINGS_LIST_H

#include <stdbool.h>
#include "buf.h"
#include "state.h"

/* Append the numbered exercise list; "> " marks the current exercise. */
void list_render(const struct app_state *st, struct buf *out);

/* Ask for an exercise name or 1-based number on stdin (cooked mode, one
 * line). Empty input cancels; unknown input prints a notice and cancels.
 * Returns true if the current exercise changed. */
bool list_prompt_jump(struct app_state *st);

#endif
