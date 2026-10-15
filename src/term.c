#include "term.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static int forced_colors = -1; /* -1: auto, 0: off, 1: on */

bool term_is_tty(void) { return isatty(STDOUT_FILENO) == 1; }

void term_force_colors(bool on) { forced_colors = on ? 1 : 0; }

bool term_colors_enabled(void) {
    if (forced_colors >= 0) return forced_colors == 1;
    const char *nc = getenv("NO_COLOR");
    if (nc && *nc) return false;
    return term_is_tty();
}

void term_color(struct buf *b, enum term_color c) {
    if (!term_colors_enabled()) return;
    const char *seq = "";
    switch (c) {
    case TERM_RED: seq = "\x1b[31m"; break;
    case TERM_GREEN: seq = "\x1b[32m"; break;
    case TERM_CYAN: seq = "\x1b[36m"; break;
    case TERM_BLUE: seq = "\x1b[34m"; break;
    case TERM_BOLD: seq = "\x1b[1m"; break;
    case TERM_UNDERLINE: seq = "\x1b[4m"; break;
    case TERM_RESET: seq = "\x1b[0m"; break;
    }
    buf_appends(b, seq);
}

int term_width(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_col == 0) return 80;
    return ws.ws_col;
}

void term_clear(struct buf *b) { buf_appends(b, "\x1b[H\x1b[2J\x1b[3J"); }

static void repeat(struct buf *b, char ch, size_t n) {
    for (size_t i = 0; i < n; i++) buf_append(b, &ch, 1);
}

/* Algorithm from rustlings term.rs: "Progress: [" (11) + "] xxx/xxx" (9). */
void term_progress_bar(struct buf *b, size_t done, size_t total, int width) {
    if (width < 24) {
        buf_printf(b, "Progress: %zu/%zu", done, total);
        return;
    }
    size_t bar = (size_t)width - 20;
    size_t filled = total ? bar * done / total : 0;
    if (filled > bar) filled = bar;

    buf_appends(b, "Progress: [");
    term_color(b, TERM_GREEN);
    repeat(b, '#', filled);
    if (filled < bar) {
        buf_appends(b, ">");
        if (bar - filled > 1) {
            term_color(b, TERM_RED);
            repeat(b, '-', bar - filled - 1);
        }
    }
    term_color(b, TERM_RESET);
    buf_printf(b, "] %3zu/%zu", done, total);
}

void term_file_link(struct buf *b, const char *rel_path) {
    if (!term_colors_enabled()) {
        buf_appends(b, rel_path);
        return;
    }
    char *abs = realpath(rel_path, NULL);
    if (!abs) {
        buf_appends(b, rel_path);
        return;
    }
    buf_appends(b, "\x1b]8;;file://");
    buf_appends(b, abs);
    buf_appends(b, "\x1b\\");
    buf_appends(b, rel_path);
    buf_appends(b, "\x1b]8;;\x1b\\");
    free(abs);
}

/* Raw mode state: the saved settings and whether they are applied. */
static struct termios saved_termios;
static volatile sig_atomic_t raw_active;
static bool hooks_installed;

static void restore_on_signal(int sig) {
    if (raw_active) {
        tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
        raw_active = 0;
        static const char nl = '\n';
        ssize_t r = write(STDOUT_FILENO, &nl, 1);
        (void)r;
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(sig, &sa, NULL);
    raise(sig); /* delivered once this handler returns */
}

static void install_hooks(void) {
    if (hooks_installed) return;
    hooks_installed = true;
    atexit(term_raw_off);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = restore_on_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void term_raw_on(void) {
    if (raw_active || !isatty(STDIN_FILENO)) return;
    struct termios t;
    if (tcgetattr(STDIN_FILENO, &t) != 0) return;
    saved_termios = t;
    install_hooks();
    t.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == 0) raw_active = 1;
}

void term_raw_off(void) {
    if (!raw_active) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
    raw_active = 0;
}

void term_press_enter(void) {
    fflush(stdout);
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
    }
}
