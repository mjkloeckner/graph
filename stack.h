#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdbool.h>

#define	STACK_INITIAL_ALLOC	10
#define	STACK_GROWTH_FACTOR	 2

typedef struct Stack Stack;

struct Stack {
	void **vector;
	size_t len, mem_alloc, mem_width;
};

void stack_new(Stack **S, size_t mem_width);
void stack_push(Stack *S, const void *elem);
void stack_peek(const Stack *S, void *data);
void stack_pop(Stack *S, void *data);
void stack_destroy(Stack *S);
bool stack_is_empty(const Stack *S);


#endif
