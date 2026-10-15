// Fix: a singly linked list only knows its head, so appending walks to the
// node whose `next` is NULL. The empty list is the one case that changes the
// caller's head, which is why `append` takes a `struct Node **`.
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
    struct Node *node = malloc(sizeof *node);
    if (node == NULL) {
        return;
    }
    node->value = value;
    node->next = NULL;
    if (*head == NULL) {
        *head = node;
        return;
    }
    struct Node *last = *head;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = node;
}

int sum(const struct Node *head) {
    int total = 0;
    for (const struct Node *p = head; p != NULL; p = p->next) {
        total += p->value;
    }
    return total;
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
