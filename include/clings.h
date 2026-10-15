#ifndef CLINGS_H
#define CLINGS_H

#include <math.h>
#include <stdio.h>

static int clings_failed, clings_passed;

#if defined(__GNUC__) || defined(__clang__)
/* Under clings, stdout is a pipe and so fully buffered. Sanitizers end the
 * program with _exit, which would drop anything still in the buffer, so make
 * stdout line-buffered before main runs. */
static void __attribute__((constructor)) clings_linebuffer_stdout(void) {
    setvbuf(stdout, NULL, _IOLBF, 0);
}
#endif

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (cond) {                                                        \
            clings_passed++;                                               \
        } else {                                                           \
            clings_failed++;                                               \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);         \
            fflush(stdout);                                                \
        }                                                                  \
    } while (0)

#define CHECK_EQ_INT(actual, expected)                                     \
    do {                                                                   \
        long long clings_a = (long long)(actual);                          \
        long long clings_e = (long long)(expected);                        \
        if (clings_a == clings_e) {                                        \
            clings_passed++;                                               \
        } else {                                                           \
            clings_failed++;                                               \
            printf("FAIL %s:%d: %s is %lld, expected %lld\n", __FILE__,    \
                   __LINE__, #actual, clings_a, clings_e);                 \
            fflush(stdout);                                                \
        }                                                                  \
    } while (0)

#define CHECK_EQ_CHAR(actual, expected)                                    \
    do {                                                                   \
        char clings_a = (char)(actual);                                    \
        char clings_e = (char)(expected);                                  \
        if (clings_a == clings_e) {                                        \
            clings_passed++;                                               \
        } else {                                                           \
            clings_failed++;                                               \
            printf("FAIL %s:%d: %s is '%c', expected '%c'\n", __FILE__,    \
                   __LINE__, #actual, clings_a, clings_e);                 \
            fflush(stdout);                                                \
        }                                                                  \
    } while (0)

#define CHECK_EQ_STR(actual, expected)                                     \
    do {                                                                   \
        const char *clings_a = (actual);                                   \
        const char *clings_e = (expected);                                 \
        if ((clings_a == NULL && clings_e == NULL) ||                      \
            (clings_a != NULL && clings_e != NULL &&                       \
             clings_str_eq(clings_a, clings_e))) {                         \
            clings_passed++;                                               \
        } else {                                                           \
            clings_failed++;                                               \
            printf("FAIL %s:%d: %s is ", __FILE__, __LINE__, #actual);     \
            clings_print_str(clings_a);                                    \
            printf(", expected ");                                         \
            clings_print_str(clings_e);                                    \
            printf("\n");                                                  \
            fflush(stdout);                                                \
        }                                                                  \
    } while (0)

#define CHECK_EQ_DBL(actual, expected)                                     \
    do {                                                                   \
        double clings_a = (double)(actual);                                \
        double clings_e = (double)(expected);                              \
        if (fabs(clings_a - clings_e) < 1e-9) {                            \
            clings_passed++;                                               \
        } else {                                                           \
            clings_failed++;                                               \
            printf("FAIL %s:%d: %s is %g, expected %g\n", __FILE__,        \
                   __LINE__, #actual, clings_a, clings_e);                 \
            fflush(stdout);                                                \
        }                                                                  \
    } while (0)

static inline int clings_str_eq(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

static inline void clings_print_str(const char *s) {
    if (s)
        printf("\"%s\"", s);
    else
        printf("NULL");
}

static inline int clings_summary(void) {
    int total = clings_passed + clings_failed;
    if (clings_failed == 0) {
        printf("All %d checks passed\n", total);
        fflush(stdout);
        return 0;
    }
    printf("%d of %d checks FAILED\n", clings_failed, total);
    fflush(stdout);
    return 1;
}

#endif
