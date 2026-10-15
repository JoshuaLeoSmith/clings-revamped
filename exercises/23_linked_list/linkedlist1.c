/*
 * linkedlist1
 *
 * A linked list is a chain of nodes on the heap, each holding a value and a
 * pointer to the next node; the last node's `next` is NULL. Write
 * `push_front`, which allocates a new node and makes it the new head, and
 * `length`, which counts the nodes. `free_list` is done for you.
 * See K&R 6.5 and Beej 20.3.
 */

#include <stdlib.h>

#include "clings.h"

struct Node {
    int value;
    struct Node *next;
};

void push_front(struct Node **head, int value) {
    // TODO: allocate a node holding `value` and put it in front of `*head`.
    (void)head;
    (void)value;
}

int length(const struct Node *head) {
    // TODO: count the nodes.
    (void)head;
    return 0;
}

void free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    struct Node *head = NULL;
    push_front(&head, 3);
    push_front(&head, 2);
    push_front(&head, 1);

    // Don't change the tests!
    CHECK_EQ_INT(length(head), 3);
    if (length(head) == 3) {
        CHECK_EQ_INT(head->value, 1);
        CHECK_EQ_INT(head->next->value, 2);
    }
    free_list(head);
    return clings_summary();
}
