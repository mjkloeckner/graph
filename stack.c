/* Stack implementation using a vector to store the elements */

#include "stack.h"

#include <stdio.h>
#include <string.h>

void stack_new(Stack **S, size_t mem_width) {
	(*S) = malloc(sizeof(Stack));
	(*S)->vector = malloc(sizeof(Stack *) * STACK_INITIAL_ALLOC);
	(*S)->mem_width = mem_width;
	(*S)->mem_alloc = STACK_INITIAL_ALLOC;
	(*S)->len = 0;
}

/* generates a copy of the element an push it into stack
 * does not check if elem is valid type */
void stack_push(Stack *S, const void *elem) {
	if(S->mem_alloc == S->len)
		S->vector = realloc(S->vector, 
				(S->mem_alloc *= STACK_GROWTH_FACTOR) * sizeof(Stack *));

	S->vector[S->len] = malloc(S->mem_width);
	memcpy(S->vector[S->len++], elem, S->mem_width);
}

void stack_peek(const Stack *S, void *data) {
	data = S->len ? S->vector[S->len - 1] : NULL;
}

void stack_pop(Stack *S, void *data) {
	memcpy(data, S->vector[--S->len], S->mem_width);
	free(S->vector[S->len]);
}

bool stack_is_empty(const Stack *S) {
	return S->len;
}

void stack_destroy(Stack *S) {
	free(S->vector);
	free(S);
}

