#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

Node *create_node(int data)
{
	Node *temp_node = malloc(sizeof(Node));

	if(!temp_node)
	{
        fprintf(stderr, "Out of memory!\n");
        EXIT_FAILURE;
    }

    temp_node->data = data;
    temp_node->next = NULL;

	return temp_node;
}

Stack *stack_new()
{
	Stack *stack = malloc(sizeof(Stack));
	if(stack == NULL)
	{
        fprintf(stderr, "Out of memory!\n");
        EXIT_FAILURE;
	}
    else

	stack->head = NULL;
	stack->size = 0;

    return stack;
}

void stack_push(Stack *stack, int data)
{
    Node *newNode = create_node(data);

    if(newNode == NULL)
    {
        fprintf(stderr, "Out of memory!\n");
        EXIT_FAILURE;
    }
    else
    {
        if(stack->head == NULL)
        {
            newNode->next = NULL;
        }
        else
        {
            newNode->next = stack->head;
        }
        stack->head = newNode;
        stack->size++;
    }

}

int stack_pop(Stack *stack)
{
    int data;

    if(stack->head == NULL)
    {
        printf("\nStack is Empty!!!\n");
    }
    else
    {
        Node *temp = stack->head;
        data = temp->data;
        stack->head = temp->next;
        stack->size--;
        free(temp);
    }

    return data;
}

void stack_display(Stack *stack)
{
    if(stack->head == NULL)
    {
        printf("\nStack is Empty!!!\n");
    }
    else
    {
        Node *temp = stack->head;
        while(temp->next != NULL)
        {
            printf("%d--->",temp->data);
            temp = temp -> next;
        }
        printf("%d--->NULL",temp->data);
    }
}

int stack_empty(Stack *stack)
{
    if(stack->size == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
