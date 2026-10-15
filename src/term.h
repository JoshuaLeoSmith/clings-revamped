#ifndef CLINGS_TERM_H
#define CLINGS_TERM_H

#include <stdbool.h>
#include <stddef.h>
#include "buf.h"

enum term_color {
    TERM_RED,
    TERM_GREEN,
    TERM_CYAN,
    TERM_BLUE,
    TERM_BOLD,
    TERM_UNDERLINE,
    TERM_RESET,
};

/* True when stdout is a TTY and NO_COLOR is unset/empty, unless forced. */
bool term_colors_enabled(void);
/* Test override; wins over the TTY / NO_COLOR checks. */
void term_force_colors(bool on);
/* True when stdout is a TTY. */
bool term_is_tty(void);

/* Appends the escape for c; no-op when colours are disabled. */
void term_color(struct buf *b, enum term_color c);
/* Terminal columns of stdout, 80 when unknown. */
int term_width(void);
/* Home, clear screen, clear scrollback. */
void term_clear(struct buf *b);
void term_progress_bar(struct buf *b, size_t done, size_t total, int width);
/* OSC 8 file:// hyperlink around rel_path when colours are on. */
void term_file_link(struct buf *b, const char *rel_path);

/* Raw (non-canonical, no echo) stdin; idempotent, no-op if stdin is no TTY.
 * Restored at exit and on SIGINT/SIGTERM. */
void term_raw_on(void);
void term_raw_off(void);

/* Flushes stdout and reads stdin until newline or EOF. */
void term_press_enter(void);

#endif
