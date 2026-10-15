/*
 * linkedlist3
 *
 * Write `remove_first`: unlink the first node holding `value`, free it and
 * return 1, or return 0 if no node holds it. Removing the head changes the
 * caller's head pointer. Then write `free_list`, which must free every node
 * without touching a node after it is freed. The sanitizers catch leaks and
 * use-after-free. See K&R 6.5 and Beej 20.3.
 */

#include <stdlib.h>

#include "clings.h"

struct Node {
    int value;
    struct Node *next;
};

void push_front(struct Node **head, int value) {
    struct Node *node = malloc(sizeof *node);
    if (node == NULL) {
        return;
    }
    node->value = value;
    node->next = *head;
    *head = node;
}

int length(const struct Node *head) {
    int count = 0;
    for (const struct Node *p = head; p != NULL; p = p->next) {
        count++;
    }
    return count;
}

int remove_first(struct Node **head, int value) {
    // TODO: unlink and free the first node holding `value`.
    (void)head;
    (void)value;
    return 0;
}

void free_list(struct Node *head) {
    // TODO: free every node.
    (void)head;
}

int main(void) {
    struct Node *head = NULL;
    push_front(&head, 2);
    push_front(&head, 3);
    push_front(&head, 2);
    push_front(&head, 1);

    // Don't change the tests!
    CHECK_EQ_INT(remove_first(&head, 2), 1);
    CHECK_EQ_INT(length(head), 3);
    if (length(head) == 3) {
        CHECK_EQ_INT(head->value, 1);
        CHECK_EQ_INT(head->next->value, 3);
        CHECK_EQ_INT(head->next->next->value, 2);
    }
    CHECK_EQ_INT(remove_first(&head, 9), 0);
    CHECK_EQ_INT(remove_first(&head, 1), 1);
    CHECK(head != NULL);
    if (head != NULL) {
        CHECK_EQ_INT(head->value, 3);
    }
    free_list(head);
    return clings_summary();
}
