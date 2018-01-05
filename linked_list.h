#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct List
{
    char *text;
    int length;
    int metric;
    struct List *next;
} List;

void print_list(List **head);

void add(List **head, char *text, int length, int metric);

void delete_list(List **head);

#endif
