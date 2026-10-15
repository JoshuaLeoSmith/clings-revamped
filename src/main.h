#ifndef CLINGS_MAIN_H
#define CLINGS_MAIN_H

/* Command-line options shared with the commands that need them. */
struct cli {
    const char *edit_cmd; /* --edit-cmd or $CLINGS_EDIT_CMD; NULL when unset */
};

#endif
