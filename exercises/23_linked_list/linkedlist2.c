/*
 * linkedlist2
 *
 * `push_front` is cheap, but it builds the list backwards. Write `append`,
 * which adds a new node at the END: walk to the last node (the one whose
 * `next` is NULL) and link the new node after it. An empty list has no
 * last node, so then the new node becomes the head. Also write `sum`.
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

void append(struct Node **head, int value) {
    // TODO: add a node holding `value` at the end of the list.
    (void)head;
    (void)value;
}

int sum(const struct Node *head) {
    // TODO: add up the values.
    (void)head;
    return 0;
}

int main(void) {
    struct Node *head = NULL;
    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    const struct Node *last = head;
    while (last != NULL && last->next != NULL) {
        last = last->next;
    }

    // Don't change the tests!
    CHECK(head != NULL);
    if (head != NULL) {
        CHECK_EQ_INT(head->value, 1);
        CHECK_EQ_INT(last->value, 3);
    }
    CHECK_EQ_INT(sum(head), 6);
    CHECK_EQ_INT(length(head), 3);
    free_list(head);
    return clings_summary();
}
