#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/* Print the whole list. If the 'head' is null the list is empty. */
void print_list(List **head)
{
    List *current = *head;

    if (current == NULL)
    {
        printf("The list is empty.");
        return;
    }

    while (current != NULL)
    {
        printf("%s\r\n", current->text);
        current = current->next;
    }
}

/* Add a new person into the list. */
void add(List **head, char *text, int length, int metric)
{
    List *new_node, *current;

    /* allocate node */
    new_node = malloc(sizeof(List));

    if (new_node)
    {
        new_node->text = malloc(strlen(text) + 1);
        strcpy(new_node->text, text);
        new_node->length = length;
        new_node->metric = metric;
        new_node->next = NULL;
    }
    else
    {
        fprintf(stderr, "Out of memory!\n");
        EXIT_FAILURE;
    }

    if (*head == NULL || (*head)->length > new_node->length)
    {
        new_node->next = *head;
        *head = new_node;
    }
    else
    {
        /* If the 'head' exists locate the node before the point of insertion in the list. */
        current = *head;
        while ((current->next != NULL && current->next->length <= new_node->length))
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

}

/* Delete the whole list. */
void delete_list(List **head)
{
    List *current = *head;
    List *next;

    while(current != NULL)
    {
        next = current->next;
        free(current->text);
        free(current);
        current = next;
    }

    *head = NULL;
}
