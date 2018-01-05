#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "stack.h"
#include "void_stack.h"
#include "linked_list.h"

#define DATE_LENGTH 11
#define SECONDS_IN_DAY 86400

int max_length;

void loop(Graph *graph)
{
	Vertex *cursor = graph->vertices;
    Edge *temp;

    printf("G = (%d, %d)\n", graph->V, graph->E);
	
	while(cursor != NULL)
	{
        printf("Vertex %d -> ", cursor->label);

        temp = cursor->edges;

        while(temp != NULL)
        {
            printf("| %d | - %s ", temp->connected_to->label, temp->weight);
            temp = temp->next;
        }

		cursor = cursor->next;
        printf("\n");
	}
}

Vertex *find_vertex(Graph *graph, int label)
{
    Vertex *temp = graph->vertices;

    while(temp != NULL)
    {
        if(temp->label == label)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

void free_graph(Graph *graph)
{
    Vertex *head = graph->vertices;
    Vertex *cursor;

    while((cursor = head) != NULL)
    {
        Edge *edgeHead = cursor->edges;
        Edge *temp;

        while((temp = edgeHead) != NULL)
        {
            edgeHead = edgeHead->next;
            free(temp);
        }

        head = head->next;
        free(cursor);
    }
}

Graph *create_graph()
{
	Graph *graph = (Graph *) malloc(sizeof(Graph));
	if (graph)
	{
		graph->V = 0;
		graph->E = 0;

        graph->vertices = NULL;
	}
	else
	{
		fprintf(stderr, "Out of memory!\n");
        EXIT_FAILURE;
	}

	return graph;
}

void add_vertex(Graph *graph, int label)
{
    Vertex *temp = graph->vertices;

	Vertex *vertex = (Vertex *) malloc(sizeof(Vertex));
	
	if (vertex)
	{
		vertex->id = graph->V;
        vertex->label = label;
        vertex->edges = NULL;
		vertex->next = NULL;		
	}
	else
	{
		fprintf(stderr, "Out of memory!\n");
		EXIT_FAILURE;
	}

    if (temp)
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = vertex;
    }
    else
    {
        graph->vertices = vertex;
    }

    graph->V++;


}

/*
 * Add new edge to the graph.
 */
void add_edge(Graph *graph, int source, int destination, char *weight)
{
    /* head of edge */
    Vertex *sV = find_vertex(graph, source);
    Edge *temp = sV->edges;


    Edge *edge = (Edge *) malloc(sizeof(Edge));
	if(edge)
	{
	    edge->connected_to = find_vertex(graph, destination);
        strcpy(edge->weight, weight);
		edge->next = NULL;
	}
	else
	{
		fprintf(stderr, "Out of memory!\n");
		EXIT_FAILURE;
	}

    if (temp)
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = edge;
    }
    else
    {
        sV->edges = edge;
    }

    graph->E++;

    /* loop(graph); */
}

int vertex_not_exist(Graph *graph, int id)
{

    Vertex *temp = graph->vertices;

	while(temp != NULL)
	{
		if(temp->label == id)
		{
			return 0;
		}
        temp = temp->next;
	}

	return 1;
}

void print_paths(Stack *path, char *buffer)
{
    Stack *reverse = stack_new();

    int n;

    /* create reverse stack to have path in correct order*/
    Node *temp = path->head;
    while(temp != NULL)
    {
        stack_push(reverse, temp->data);
        temp = temp->next;
    }

    if(reverse->size >= 1)
    {
        n = stack_pop(reverse);
        sprintf(buffer, "%d", n);
    }
    while(stack_empty(reverse) == 0)
    {
        n = stack_pop(reverse);
        sprintf(buffer + strlen(buffer), "-%d", n);
    }
    sprintf(buffer + strlen(buffer), ";");
    free(reverse);
}

int print_edges(VoidStack *edges, char *buffer)
{
    char weight[DATE_LENGTH] = "";
    char min_weight[DATE_LENGTH] = "", max_weight[DATE_LENGTH] = "";

    int metric;

    VoidStack *reverse = malloc(sizeof(VoidStack));
    void_stack_new(reverse, DATE_LENGTH);

    /* create reverse stack to have edges in correct order*/
    while (edges->loglength != 0)
    {
        void_stack_pop(edges, weight);
        void_stack_push(reverse, weight);
    }

    if (reverse->loglength >= 1)
    {
        void_stack_pop(reverse, weight);

        strcpy(min_weight, weight);
        strcpy(max_weight, weight);

        sprintf(buffer + strlen(buffer), "%s", weight);

        void_stack_push(edges, weight);
    }

    while (reverse->loglength != 0)
    {
        void_stack_pop(reverse, weight);

        if (compare_dates(weight, max_weight) == 1)
        {
            strcpy(max_weight, weight);
        }
        if (compare_dates(weight, min_weight) == -1)
        {
            strcpy(min_weight, weight);
        }

        sprintf(buffer + strlen(buffer), ",%s", weight);

        void_stack_push(edges, weight);
    }

    metric = (int) compute_metric(min_weight, max_weight);
    sprintf(buffer + strlen(buffer), ";%d", metric);

    void_stack_dispose(reverse);
    free(reverse);

    return metric;
}

double compute_metric(char *min_weight, char *max_weight)
{
    double seconds = difftime(parse_date(min_weight), parse_date(max_weight));

    return seconds / SECONDS_IN_DAY;
}

int compare_dates(char *a, char *b)
{
    if (difftime(parse_date(a), parse_date(b)) < 0)
    {
        return 1;
    }
    else if (difftime(parse_date(a), parse_date(b)) > 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

time_t parse_date(char *date)
{
    time_t raw_time;

    struct tm *tm;
    int year, mon, day;
    char *temp, temp_date[DATE_LENGTH];

    strcpy(temp_date, date);

    time(&raw_time);
    tm = localtime(&raw_time);

    year = (int) strtol(strtok(temp_date , "-"), &temp, 10);
    mon = (int) strtol(strtok(NULL, "-"), &temp, 10);
    day = (int) strtol(strtok(NULL, "-"), &temp, 10);

    tm->tm_year = year - 1900;
    tm->tm_mon = mon;
    tm->tm_mday = day;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;

    return mktime(tm);
}

void all_paths(Graph *graph, Vertex *s, Vertex *v, Stack *path, List **paths, VoidStack *edges, int on_path[], int length)
{
    length++;

    if(length > max_length + 1)
    {
        return;
    }

    stack_push(path, s->label);
    on_path[s->id] = 1;


    if(s->id == v->id)
    {
        char buffer[1000] = "";
        int metric;
        print_paths(path, buffer);
        metric = print_edges(edges, buffer);
        add(paths, buffer, length, metric);
    }
    else
    {
        Edge *temp = s->edges;

        while(temp != NULL)
        {
            Vertex *w = temp->connected_to;

            if(on_path[w->id] == 0)
            {
                void_stack_push(edges, temp->weight);
                all_paths(graph, w, v, path, paths, edges, on_path, length);
            }
            if(edges->loglength >= length)
            {
                void_stack_remove(edges);
            }
            temp = temp->next;
        }
    }
    stack_pop(path);
    on_path[s->id] = 0;
    length--;
}

void find_all_paths(Graph *graph, int start_vertex, int end_vertex)
{
    Stack *stack = stack_new();
    VoidStack *edges = malloc(sizeof(VoidStack));
    List *paths = NULL;
    Vertex *s, *v;

    int i, length = 0;
    int *on_path = malloc(sizeof(int) * graph->V);

    for (i = 0; i < graph->V; i++)
    {
        on_path[i] = 0;
    }

    void_stack_new(edges, DATE_LENGTH);

    s = find_vertex(graph, start_vertex);
    v = find_vertex(graph, end_vertex);

    all_paths(graph, s, v, stack, &paths, edges, on_path, length);

    print_list(&paths);
    delete_list(&paths);
    free(paths);

    free(stack);
    free(on_path);

    void_stack_dispose(edges);
    free(edges);
}

void parse_file(char *line, int i, Graph *graph)
{
    int id, id2;
    char *weight;

    /* Trim \n escape sequence from line. */
    line[strcspn(line, "\n")] = '\0';

    i++;

    /* printf("Hrana %d => %s\n", i, line); */

    id = atoi(strtok(line , ";"));

    if(vertex_not_exist(graph, id))
    {
        add_vertex(graph, id);
    }

    id2 = atoi(strtok(NULL, ";"));

    if(vertex_not_exist(graph, id2))
    {
        add_vertex(graph, id2);
    }

    weight = strtok(NULL, ";");

    add_edge(graph, id, id2, weight);
}

int main(int argc, char const *argv[])
{
	FILE *file;
	char line[256];
	int i = 0, start_vertex, end_vertex, length;

	Graph *graph;

	if (argc < 5)
    {
        fprintf(stderr, "Please run application in this format dfs.exe <graph-file> <id1> <id2> <maxD>\n");
		return EXIT_FAILURE;
	}
    else
    {
        file = fopen(argv[1], "r");

        if(file == NULL)
        {
            perror("The file does not exist!");
            return EXIT_FAILURE;
        }
        else
        {
            char *temp;

            start_vertex = (int) strtol(argv[2], &temp, 10);
            if (*temp != '\0')
            {
                fprintf(stderr, "First vertex is not a valid number!\n");
                return EXIT_FAILURE;
            }
            end_vertex = (int) strtol(argv[3], &temp, 10);
            if (*temp != '\0')
            {
                fprintf(stderr, "Second vertex is not a valid number!\n");
                return EXIT_FAILURE;
            }
            length = (int) strtol(argv[4], &temp, 10);
            if (*temp != '\0')
            {
                fprintf(stderr, "Length is not a valid number!\n");
                return EXIT_FAILURE;
            }
            max_length = length;

            graph = create_graph();

            while(fgets(line, sizeof(line), file))
            {
                parse_file(line, i++, graph);
            }

            fclose(file);

            find_all_paths(graph, start_vertex, end_vertex);

            free_graph(graph);
            free(graph);
        }
    }

	return 0;
}



