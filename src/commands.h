#ifndef CLINGS_COMMANDS_H
#define CLINGS_COMMANDS_H

#include "buf.h"
#include "info.h"
#include "state.h"

/* Each returns the process exit code: 0 success, 1 pending/failed, 2 error.
 * name may be NULL to use the current exercise. */
int cmd_run(struct app_state *st, const char *name);
int cmd_hint(struct app_state *st, const char *name);
int cmd_list(struct app_state *st);
int cmd_check_all(struct app_state *st);
/* Rewrite the exercise file from its embedded original; name is required. */
int cmd_reset(struct app_state *st, const char *name);

/* Append the finish banner and the trimmed final message. */
void render_final_message(const struct info_file *info, struct buf *out);

#endif
