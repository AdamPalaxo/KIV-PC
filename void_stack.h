#ifndef __VOID_STACK_H__
#define __VOID_STACK_H__

typedef struct stack
{
    void *elements;
    int element_size;
    int loglength;
    int allocated_length;

} VoidStack;

void void_stack_new(VoidStack *s, int element_size);
void void_stack_dispose(VoidStack *s);
void void_stack_push(VoidStack *s, void *element_address);
void void_stack_pop(VoidStack *s, void *element_address);
void void_stack_remove(VoidStack *s);

#endif
