#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "void_stack.h"

void void_stack_new(VoidStack *s, int element_size)
{
    s->element_size = element_size;
    s->loglength = 0;
    s->allocated_length = 4;
    s->elements = malloc((size_t) 4 * element_size);
}

static void stack_grow(VoidStack *s)
{
    s->allocated_length *= 2;
    s->elements = realloc(s->elements, (size_t) s->allocated_length * s->element_size);
}

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

void void_stack_pop(VoidStack *s, void *element_address)
{
    void *source;

    s->loglength--;
    source = (char *) s->elements + (s->loglength) * s->element_size;
    memcpy(element_address, source, (size_t) s->element_size);

}

void void_stack_remove(VoidStack *s)
{
    s->loglength--;
}


void void_stack_dispose(VoidStack *s)
{
    free(s->elements);
}