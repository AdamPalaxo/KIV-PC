#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "void_stack.h"

/* Creates new stack with given element size, allocates memory */
void void_stack_new(VoidStack *s, int element_size)
{
    s->element_size = element_size;
    s->loglength = 0;
    s->allocated_length = 4;
    s->elements = malloc((size_t) 4 * element_size);
}

/* Grows stack in case when maximal length of stack would be exceeded */
static void stack_grow(VoidStack *s)
{
    s->allocated_length *= 2;
    s->elements = realloc(s->elements, (size_t) s->allocated_length * s->element_size);
}

/* Pushes new element to stack on given address */
void void_stack_push(VoidStack *s, void *element_address)
{
    void *target;

    if(s->loglength == s->allocated_length)
    {
        stack_grow(s);
    }

    target = (char *)s->elements + s->loglength * s->element_size;
    memcpy(target, element_address, (size_t) s->element_size);
    s->loglength++;
}

/* Pops element from top of stack to given address */
void void_stack_pop(VoidStack *s, void *element_address)
{
    void *source;

    s->loglength--;
    source = (char *) s->elements + (s->loglength) * s->element_size;
    memcpy(element_address, source, (size_t) s->element_size);

}

/* Removes element from top of stack respectively decrease its size */
void void_stack_remove(VoidStack *s)
{
    s->loglength--;
}

/* Disposes stack and frees memory */
void void_stack_dispose(VoidStack *s)
{
    free(s->elements);
}