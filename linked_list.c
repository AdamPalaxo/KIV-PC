#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/* Prints the whole list. If the 'head' is null the list is empty. */
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
        printf("%s\n", current->text);
        current = current->next;
    }
}

/* Adds a new element into the list. */
void add(List **head, char *text, int length, int metric)
{
    List *new_node, *current;

    new_node = malloc(sizeof(List));

    if (new_node != NULL)
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
        exit(1);
    }

    if (*head == NULL || (*head)->length > new_node->length || ((*head)->length == new_node->length && (*head)->metric > new_node->metric))
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
            if (current->next->length == new_node->length && current->next->metric > new_node->metric)
            {
                break;
            }
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

}

/* Deletes the whole list and frees allocated memory. */
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
