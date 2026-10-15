// Fix: each node is its own `malloc` block; the new node's `next` points at
// the old head, and writing through `head` (a pointer to the caller's head
// pointer) makes the new node the caller's head.
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
