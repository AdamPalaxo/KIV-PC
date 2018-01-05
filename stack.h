#ifndef __STACK_H__
#define __STACK_H__

typedef struct node_t
{
	int data;
	struct node_t *next;

} Node;

typedef struct stack_t
{
	size_t size;
	Node *head;

} Stack;

Stack *stack_new();
void stack_push(Stack *stack, int data);
int stack_pop(Stack *stack);
void stack_dispose(Stack *stack);
int stack_empty(Stack *stack);
void stack_display(Stack *stack);

#endif