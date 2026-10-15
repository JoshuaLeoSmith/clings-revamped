# Linked lists

A linked list is a chain of `struct`s that each point to the next one: `struct Node { int value; struct Node *next; };` is a self-referential struct (a struct may contain a pointer to its own type, though not a copy of itself), and the last node's `next` is `NULL`.
Every node is a separate `malloc` block, so every node needs its own `free`; walking a list is a loop of `p = p->next` until `p` is `NULL`.
A function that may change which node is first (push to the front, append to an empty list, remove the head) takes a `struct Node **head`: C passes arguments by value, so a plain `struct Node *` parameter would only be a copy, and assigning to it would leave the caller's head unchanged, while `*head = node;` writes to the caller's variable.
When freeing, save `next` before calling `free`: after `free(p)` the node no longer belongs to you, and reading `p->next` is a use-after-free.

## Further information

- K&R 6.5 Self-referential Structures
- Beej's Guide to C, section 20.3 (Self-Referential Structs)
