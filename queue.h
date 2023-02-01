#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct Node Node;
typedef struct Queue Queue;

struct Node {
	void *data;
	Node *next;
	Node *prev;
};

struct Queue {
	size_t type_size;
	Node *first;
	Node *end;
};

void queue_new(Queue **Q, size_t type_size);
void queue_enqueue(Queue *Q, void *data);
void queue_dequeue(Queue *Q, void *data);
void queue_destroy(Queue *Q);

bool queue_is_empty(Queue *Q);

#endif
