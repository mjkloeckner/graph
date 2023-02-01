#include "queue.h"

/* FIFO: First In First Out */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void queue_new(Queue **Q, size_t type_size) {
	(*Q) = malloc(sizeof(Queue));
	(*Q)->first = NULL;
	(*Q)->end = NULL;
	(*Q)->type_size = type_size;
}

void queue_enqueue(Queue *Q, void *data) {
	Node *new = malloc(sizeof(Node));
	new->data = malloc(Q->type_size);
	memcpy(new->data, data, Q->type_size);

	if(!Q->first) Q->first = new;

	if(!Q->end) Q->end = new;
	else Q->end->next = new;

	new->prev = Q->end;
	new->next = NULL;
	Q->end = new;
}

void queue_dequeue(Queue *Q, void *data) {
	assert(Q->first);
	assert(Q->end);

	memcpy(data, Q->first->data, Q->type_size);

	Q->first->prev = Q->first;
	Q->first = Q->first->next;
	if(Q->first == NULL) return;

	free(Q->first->prev->data);
	free(Q->first->prev);
	Q->first->prev = NULL;
}

bool queue_is_empty(Queue *Q) {
	return Q->first;
}

void queue_destroy(Queue *Q) {
	for(Node *n = Q->first; n; n = n->next)
		free(n->prev);

	if(Q->end)
		free(Q->end->data);

	free(Q->end);
	if(Q->first)
		free(Q->first->data);

	free(Q->first);
	free(Q);
}
