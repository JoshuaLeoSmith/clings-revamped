#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "buf.h"
#include "commands.h"
#include "dev.h"
#include "exercise.h"
#include "info.h"
#include "state.h"
#include "term.h"

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
    "  dev check       Verify the exercise set (for exercise authors)\n"
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

int main(int argc, char **argv) {
    struct cli cli = {getenv("CLINGS_EDIT_CMD")};
    if (cli.edit_cmd && !*cli.edit_cmd) cli.edit_cmd = NULL;

    const char *args[3] = {NULL, NULL, NULL}; /* command and up to two operands */
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
            if (nargs == 3) return usage_error("unexpected argument %s", a);
            args[nargs++] = a;
        }
    }

    const char *cmd = args[0];
    /* Validate the command line before touching the project. */
    if (!cmd) {
        if (!term_is_tty()) {
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
        if (nargs != 2 || (strcmp(args[1], "check") && strcmp(args[1], "update")))
            return usage_error("%s needs 'check' or 'update'", cmd);
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
        fputs("clings: watch mode not implemented yet\n", stderr);
        rc = 2;
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
    } else {
        fprintf(stderr, "clings: %s not implemented yet\n", cmd);
        rc = 2;
    }
    (void)cli;

    state_free(&st);
    info_free(&info);
    return rc;
}
