#include "proc.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After the child is killed or has exited, wait at most this long for the
 * output pipe to reach EOF (a surviving grandchild may hold it open). */
#define DRAIN_AFTER_KILL_MS 500

/* While the child runs, check this often whether it has exited. */
#define REAP_POLL_MS 50

static void ignore_sigpipe(void) {
    static bool done;
    if (done) return;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPIPE, &sa, NULL);
    done = true;
}

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void close_fd(int *fd) {
    if (*fd >= 0) {
        close(*fd);
        *fd = -1;
    }
}

static void write_all(int fd, const char *p, size_t n) {
    while (n > 0) {
        ssize_t w = write(fd, p, n);
        if (w < 0) {
            if (errno == EINTR) continue;
            return; /* EPIPE etc.: child stopped reading */
        }
        p += w;
        n -= (size_t)w;
    }
}

/* In a forked child: report why we are giving up, then exit 127. */
static void child_fail(const char *what, const char *arg) {
    char msg[1024];
    int n = snprintf(msg, sizeof msg, "clings: %s %s: %s\n", what, arg,
                     strerror(errno));
    if (n > 0) {
        size_t len = (size_t)n < sizeof msg ? (size_t)n : sizeof msg - 1;
        write_all(STDERR_FILENO, msg, len);
    }
    _exit(127);
}

static void child_default_sigpipe(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPIPE, &sa, NULL);
}

bool proc_run(const char *const argv[], const char *cwd,
              const char *stdin_text, int timeout_ms, size_t max_output,
              struct proc_result *out) {
    memset(out, 0, sizeof *out);
    buf_init(&out->output);
    ignore_sigpipe();

    int opipe[2] = {-1, -1}, ipipe[2] = {-1, -1};
    if (pipe(opipe) != 0) return false;
    if (stdin_text && pipe(ipipe) != 0) {
        close_fd(&opipe[0]);
        close_fd(&opipe[1]);
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close_fd(&opipe[0]);
        close_fd(&opipe[1]);
        close_fd(&ipipe[0]);
        close_fd(&ipipe[1]);
        return false;
    }

    if (pid == 0) {
        child_default_sigpipe();
        close_fd(&opipe[0]);
        close_fd(&ipipe[1]);
        int in_fd = ipipe[0];
        if (in_fd < 0) in_fd = open("/dev/null", O_RDONLY);
        if (in_fd >= 0 && in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        dup2(opipe[1], STDOUT_FILENO);
        dup2(opipe[1], STDERR_FILENO);
        if (opipe[1] != STDOUT_FILENO && opipe[1] != STDERR_FILENO)
            close(opipe[1]);
        if (in_fd < 0) child_fail("cannot open", "/dev/null");
        if (cwd && chdir(cwd) != 0) child_fail("cannot chdir to", cwd);
        execvp(argv[0], (char *const *)argv);
        child_fail("cannot run", argv[0]);
    }

    /* parent */
    close_fd(&opipe[1]);
    close_fd(&ipipe[0]);
    if (stdin_text) {
        write_all(ipipe[1], stdin_text, strlen(stdin_text));
        close_fd(&ipipe[1]);
    }

    long long deadline = timeout_ms > 0 ? now_ms() + timeout_ms : -1;
    long long drain_deadline = -1; /* set once the child is dead or killed */
    bool killed = false;  /* we sent SIGKILL */
    bool discard = false; /* stop collecting output (killed or capped) */
    bool reaped = false;  /* waitpid already collected the child */
    int st = 0;
    char chunk[4096];

    for (;;) {
        long long t = now_ms();
        if (!reaped && !killed && deadline >= 0 && t >= deadline) {
            kill(pid, SIGKILL);
            killed = discard = true;
            out->timed_out = true;
            drain_deadline = t + DRAIN_AFTER_KILL_MS;
        }
        if (drain_deadline >= 0 && t >= drain_deadline) break;

        /* While the child may be alive, wake up periodically to reap it:
         * a background descendant can hold the pipe open after it exits. */
        long long limit = -1;
        if (!reaped) limit = t + REAP_POLL_MS;
        if (!killed && deadline >= 0 && (limit < 0 || deadline < limit))
            limit = deadline;
        if (drain_deadline >= 0 && (limit < 0 || drain_deadline < limit))
            limit = drain_deadline;
        int wait_ms = limit < 0 ? -1 : (int)(limit > t ? limit - t : 0);

        struct pollfd pfd = {.fd = opipe[0], .events = POLLIN};
        int pr = poll(&pfd, 1, wait_ms);
        if (pr < 0 && errno != EINTR) break;
        if (pr > 0) {
            ssize_t n = read(opipe[0], chunk, sizeof chunk);
            if (n == 0) break; /* EOF */
            if (n < 0 && errno != EINTR && errno != EAGAIN) break;
            if (n > 0 && !discard) {
                size_t room = max_output - out->output.len;
                if ((size_t)n > room) {
                    buf_append(&out->output, chunk, room);
                    out->truncated = true;
                    discard = true;
                    if (!reaped) {
                        kill(pid, SIGKILL);
                        killed = true;
                    }
                    if (drain_deadline < 0)
                        drain_deadline = now_ms() + DRAIN_AFTER_KILL_MS;
                } else {
                    buf_append(&out->output, chunk, (size_t)n);
                }
            }
        }
        if (!reaped && waitpid(pid, &st, WNOHANG) == pid) {
            reaped = true;
            if (drain_deadline < 0)
                drain_deadline = now_ms() + DRAIN_AFTER_KILL_MS;
        }
    }
    close_fd(&opipe[0]);

    while (!reaped && waitpid(pid, &st, 0) < 0) {
        if (errno != EINTR) return true; /* cannot happen: pid is ours */
    }
    if (WIFEXITED(st)) {
        out->exited = true;
        out->status = WEXITSTATUS(st);
    } else if (WIFSIGNALED(st)) {
        out->signal = WTERMSIG(st);
    }
    return true;
}

void proc_result_free(struct proc_result *r) {
    buf_free(&r->output);
    memset(r, 0, sizeof *r);
}

bool proc_spawn_detached(const char *const argv[]) {
    pid_t pid = fork();
    if (pid < 0) return false;
    if (pid == 0) {
        setsid(); /* detach from the terminal's signals (e.g. Ctrl-C) */
        pid_t gpid = fork();
        if (gpid < 0) _exit(1);
        if (gpid > 0) _exit(0);
        child_default_sigpipe();
        int fd = open("/dev/null", O_RDWR);
        if (fd >= 0) {
            dup2(fd, STDIN_FILENO);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            if (fd > STDERR_FILENO) close(fd);
        }
        execvp(argv[0], (char *const *)argv);
        _exit(127);
    }
    int st;
    while (waitpid(pid, &st, 0) < 0) {
        if (errno != EINTR) return false;
    }
    return WIFEXITED(st) && WEXITSTATUS(st) == 0;
}
