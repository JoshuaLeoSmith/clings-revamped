#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "buf.h"
#include "commands.h"
#include "dev.h"
#include "exercise.h"
#include "info.h"
#include "state.h"
#include "term.h"
#include "watch.h"

#define VERSION "0.1.0"
#define INFO_PATH "info.toml"
#define STATE_PATH ".clings-state.txt"

static const char usage_text[] =
    "Usage: clings [OPTIONS] [COMMAND]\n"
    "\n"
    "Small exercises to get you used to reading and writing C code.\n"
    "Run it from the clings project root.\n"
    "\n"
    "Commands:\n"
    "  (none)          Watch mode: rerun the current exercise whenever it changes\n"
    "  run [NAME]      Run the current exercise, or NAME, once\n"
    "  hint [NAME]     Show the hint of the current exercise, or of NAME\n"
    "  reset NAME      Restore exercise NAME to its original contents\n"
    "  list            List all exercises with their state\n"
    "  check-all       Check every exercise and report which are pending\n"
    "  dev check [NAME|DIR...]\n"
    "                  Verify the exercise set, compiling only the selected\n"
    "                  exercises (for exercise authors)\n"
    "  dev update      Regenerate CMakeLists.txt and the embedded originals (authors)\n"
    "\n"
    "Options:\n"
    "  --edit-cmd CMD  Command used to open exercises (default $CLINGS_EDIT_CMD)\n"
    "  --help          Print this help and exit\n"
    "  --version       Print the version and exit\n";

static int usage_error(const char *fmt, const char *arg) {
    fputs("clings: ", stderr);
    fprintf(stderr, fmt, arg);
    fputs("\n\n", stderr);
    fputs(usage_text, stderr);
    return 2;
}

static bool is_dir(const char *path) {
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

/* First start: show the welcome message and wait for Enter. */
static void show_welcome(const struct info_file *info) {
    struct buf out;
    buf_init(&out);
    term_clear(&out);
    append_trimmed(&out, info->welcome_message);
    buf_appends(&out, "\n\nPress ENTER to continue ");
    flush_out(&out);
    term_press_enter();
    term_clear(&out);
    flush_out(&out);
    buf_free(&out);
}

int main(int argc, char **argv) {
    struct cli cli = {getenv("CLINGS_EDIT_CMD")};
    if (cli.edit_cmd && !*cli.edit_cmd) cli.edit_cmd = NULL;

    /* Command and its operands; NULL-padded so args[1] and args[2] always exist. */
    const char **args = xmalloc((size_t)(argc + 2) * sizeof *args);
    for (int i = 0; i < argc + 2; i++) args[i] = NULL;
    int nargs = 0;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--help") || !strcmp(a, "-h")) {
            fputs(usage_text, stdout);
            return 0;
        } else if (!strcmp(a, "--version") || !strcmp(a, "-V")) {
            puts("clings " VERSION);
            return 0;
        } else if (!strcmp(a, "--edit-cmd")) {
            if (i + 1 >= argc) return usage_error("%s needs an argument", a);
            cli.edit_cmd = argv[++i];
        } else if (a[0] == '-' && a[1]) {
            return usage_error("unknown option %s", a);
        } else {
            args[nargs++] = a;
        }
    }

    const char *cmd = args[0];
    /* Validate the command line before touching the project. */
    if (!cmd) {
        if (!term_is_tty() || !isatty(STDIN_FILENO)) {
            fputs("clings: watch mode needs a terminal; use 'clings run' instead\n", stderr);
            return 2;
        }
    } else if (!strcmp(cmd, "run") || !strcmp(cmd, "hint")) {
        if (nargs > 2) return usage_error("unexpected argument %s", args[2]);
    } else if (!strcmp(cmd, "reset")) {
        if (nargs < 2) return usage_error("%s needs an exercise name", cmd);
        if (nargs > 2) return usage_error("unexpected argument %s", args[2]);
    } else if (!strcmp(cmd, "list") || !strcmp(cmd, "check-all")) {
        if (nargs > 1) return usage_error("unexpected argument %s", args[1]);
    } else if (!strcmp(cmd, "dev")) {
        if (nargs < 2 || (strcmp(args[1], "check") && strcmp(args[1], "update")))
            return usage_error("%s needs 'check' or 'update'", cmd);
        if (!strcmp(args[1], "update") && nargs > 2)
            return usage_error("unexpected argument %s", args[2]);
    } else {
        return usage_error("unknown command %s", cmd);
    }

    if (!is_dir("exercises")) {
        fputs("clings: no exercises/ directory here; run clings from the clings project root\n",
              stderr);
        return 2;
    }
    if (!exercise_compiler_available()) {
        fputs("No C compiler found. Install gcc (sudo apt install build-essential) or set CC.\n",
              stderr);
        return 2;
    }

    struct info_file info;
    char err[512];
    if (!info_load(INFO_PATH, &info, err, sizeof err)) {
        info_free(&info);
        die("%s", err);
    }

    struct app_state st;
    bool fresh;
    state_load(&st, &info, STATE_PATH, &fresh);

    int rc;
    if (!cmd) {
        if (fresh) show_welcome(&info);
        rc = watch_run(&st, &cli);
    } else if (!strcmp(cmd, "run")) {
        rc = cmd_run(&st, args[1]);
    } else if (!strcmp(cmd, "hint")) {
        rc = cmd_hint(&st, args[1]);
    } else if (!strcmp(cmd, "list")) {
        rc = cmd_list(&st);
    } else if (!strcmp(cmd, "check-all")) {
        rc = cmd_check_all(&st);
    } else if (!strcmp(cmd, "reset")) {
        rc = cmd_reset(&st, args[1]);
    } else if (!strcmp(cmd, "dev") && !strcmp(args[1], "update")) {
        rc = dev_update(&info);
    } else if (!strcmp(cmd, "dev") && !strcmp(args[1], "check")) {
        rc = dev_check(&info, args + 2, (size_t)(nargs - 2));
    } else {
        fprintf(stderr, "clings: %s not implemented yet\n", cmd);
        rc = 2;
    }

    state_free(&st);
    info_free(&info);
    free(args);
    return rc;
}
