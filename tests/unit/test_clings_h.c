#include <stdio.h>
#include "clings.h"

int main(void) {
    CHECK_EQ_INT(1, 1);
    CHECK_EQ_STR("a", "a");
    CHECK_EQ_DBL(0.1 + 0.2, 0.3);
    CHECK_EQ_CHAR('x', 'x');
    CHECK_EQ_INT(1, 2);
    CHECK_EQ_STR(NULL, "a");
    if (clings_failed != 2) {
        printf("expected 2 failures, got %d\n", clings_failed);
        return 1;
    }
    clings_failed = 0;
    int n = 0;
    CHECK_EQ_INT(n++, 0);
    CHECK_EQ_INT(n, 1);
    return clings_summary();
}
