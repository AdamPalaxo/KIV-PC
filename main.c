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

/*
 * Global variable of maximal length of path
 */
int max_length;

/* Prints all graph's vertexes and edges into console
 * mainly for test cases */
void loop(Graph *graph)
{
	Vertex *cursor = graph->vertices;
    Edge *temp;

    printf("G = (%d, %d)\n", graph->V, graph->E);
	
	while (cursor != NULL)
	{
        printf("Vertex %d -> ", cursor->label);

        temp = cursor->edges;

        while (temp != NULL)
        {
            printf("| %d | - %s ", temp->connected_to->label, temp->weight);
            temp = temp->next;
        }

		cursor = cursor->next;
        printf("\n");
	}
}

/* Completely frees Graph structure together with vertexes and edges */
void free_graph(Graph *graph)
{
    Vertex *head = graph->vertices;
    Vertex *cursor;

    while ((cursor = head) != NULL)
    {
        Edge *edgeHead = cursor->edges;
        Edge *temp;

        while ((temp = edgeHead) != NULL)
        {
            edgeHead = edgeHead->next;
            free(temp);
        }

        head = head->next;
        free(cursor);
    }

    free(graph);
}

/* Creates new graph, allocate memory for vertices */
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
        exit(1);
	}

	return graph;
}

/* Adds new vertex to given Graph with desired label.
 * Allocates its memory */
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
		exit(1);
	}

    /* add vertex */
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
 * Add new edge to the given Graph from desired source to destination
 * with given edge weight, also allocates its memory.
 */
void add_edge(Graph *graph, int source, int destination, char *weight)
{
    /* head of edge */
    Vertex *sV = find_vertex(graph, source);
    Edge *temp = sV->edges;


    Edge *edge = (Edge *) malloc(sizeof(Edge));
	if (edge)
	{
	    edge->connected_to = find_vertex(graph, destination);
        strcpy(edge->weight, weight);
		edge->next = NULL;
	}
	else
	{
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}

    /* add edge */
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
}

/* Finds vertex in Graph structure with given label */
Vertex *find_vertex(Graph *graph, int label)
{
    Vertex *temp = graph->vertices;

    while (temp != NULL)
    {
        if (temp->label == label)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

/* Checks if vertex with given label already exits in Graph. */
int vertex_not_exist(Graph *graph, int label)
{

    Vertex *temp = graph->vertices;

	while (temp != NULL)
	{
		if (temp->label == label)
		{
			return 0;
		}
        temp = temp->next;
	}

	return 1;
}

/* Prints found path to buffer for addition use. */
void process_paths(Stack *path, char *buffer)
{
    Stack *reverse = stack_new();

    int n;

    /* Create reverse stack to have path in correct order */
    Node *temp = path->head;
    while (temp != NULL)
    {
        stack_push(reverse, temp->data);
        temp = temp->next;
    }

    if (reverse->size >= 1)
    {
        n = stack_pop(reverse);
        sprintf(buffer, "%d", n);
    }
    while (stack_empty(reverse) == 0)
    {
        n = stack_pop(reverse);
        sprintf(buffer + strlen(buffer), "-%d", n);
    }
    sprintf(buffer + strlen(buffer), ";");
    free(reverse);
}

/* Prints found edges to buffer for addition use. Also computes metric
 * of found path. */
int process_edges(VoidStack *edges, char *buffer)
{
    char weight[DATE_LENGTH] = "";
    char min_weight[DATE_LENGTH] = "", max_weight[DATE_LENGTH] = "";

    int metric;

    VoidStack *reverse = malloc(sizeof(VoidStack));
    void_stack_new(reverse, DATE_LENGTH);

    /* Create reverse stack to have edges in correct order */
    while (edges->loglength != 0)
    {
        void_stack_pop(edges, weight);
        void_stack_push(reverse, weight);
    }

    if (reverse->loglength >= 1)
    {
        void_stack_pop(reverse, weight);

        strcpy(max_weight, weight);
        strcpy(min_weight, weight);

        sprintf(buffer + strlen(buffer), "%s", weight);

        void_stack_push(edges, weight);
    }

    while (reverse->loglength != 0)
    {
        void_stack_pop(reverse, weight);

        if (compare_dates(weight, max_weight) ==  1)
        {
            strcpy(max_weight, weight);
        }
        else if (compare_dates(min_weight, weight) == 1)
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

/* Computes difference between minimal weight and maximal weight
 * of founded path. Also called metric. */
double compute_metric(char *min_weight, char *max_weight)
{
    double seconds = difftime(parse_date(max_weight), parse_date(min_weight));

    return seconds / SECONDS_IN_DAY;
}

/* Compares to dates if its older or newer */
int compare_dates(char *a, char *b)
{
    double difference = 0;
    difference = difftime(parse_date(a), parse_date(b));

    if (difference > 0)
    {
        return 1;
    }
    else if (difference < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/* Parses given date in format YYYY-MM-DD into time_t type*/
time_t parse_date(char *date)
{
    struct tm tm = {0};
    int year = 0, mon = 0, day = 0;
    char temp_date[DATE_LENGTH] = "";

    const char *delimiter = "-";

    strncpy(temp_date, date, strlen(date));

    year = (int) strtol(strtok(temp_date , delimiter), NULL, 0);
    mon = (int) strtol(strtok(NULL, delimiter), NULL, 0);
    day = (int) strtol(strtok(NULL, delimiter), NULL, 0);

    tm.tm_year = year - 1900;
    tm.tm_mon = mon - 1;
    tm.tm_mday = day;


    return mktime(&tm);
}

/* Finds all possible paths from source vertex s to end vertex v in given graph
 * using depth-first search technique. */
void dfs(Graph *graph, Vertex *s, Vertex *v, Stack *path, List **paths, VoidStack *edges, int on_path[], int length)
{
    length = length + 1;

    /* Skip if length of path is longer than desired length
     * One is added to max_length, because length counts vertices but max_length
     * is measured in count of edges */
    if (length > max_length + 1)
    {
        return;
    }

    stack_push(path, s->label);
    on_path[s->id] = 1;

    /* If path found, save into buffer, compute metric and save into linked list of all found paths */
    if (s->id == v->id)
    {
        int metric;
        char *buffer = malloc((2 * length * sizeof(int)) + (length * DATE_LENGTH) + sizeof(int));
        process_paths(path, buffer);
        metric = process_edges(edges, buffer);
        add(paths, buffer, length, metric);
        free(buffer);
    }
    else
    {
        Edge *temp = s->edges;

        /* Check all neighbours of current vertex also avoid cycle*/
        while(temp != NULL)
        {
            Vertex *w = temp->connected_to;

            if(on_path[w->id] == 0)
            {
                void_stack_push(edges, temp->weight);
                dfs(graph, w, v, path, paths, edges, on_path, length);
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
    length = length - 1;
}

/* Allocates memory for stack with paths, prepares data structures
 * and calls function for finding all paths */
void all_paths(Graph *graph, int start_vertex, int end_vertex)
{
    Stack *path = stack_new();
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

    if(s == NULL || v == NULL)
    {
        fprintf(stderr, "You are trying to find path between non-existing vertices!");
        exit(1);
    }

    /* Start process of finding paths */
    dfs(graph, s, v, path, &paths, edges, on_path, length);

    print_list(&paths);
    delete_list(&paths);
    free(paths);

    free(path);
    free(on_path);

    void_stack_dispose(edges);
    free(edges);
}

/* Parse line of input file and adds vertexes and edges to given Graph. */
void parse_file(char *line, Graph *graph)
{
    int id = 0, id2 = 0;
    char *weight;

    const char *delimiter = ";";

    /* Trims \n escape sequence from line */
    line[strcspn(line, "\n")] = '\0';

    id = (int) strtol(strtok(line , delimiter), NULL, 0);

    if (vertex_not_exist(graph, id))
    {
        add_vertex(graph, id);
    }

    id2 = strtol(strtok(NULL, delimiter), NULL, 0);

    if (vertex_not_exist(graph, id2))
    {
        add_vertex(graph, id2);
    }

    weight = malloc(DATE_LENGTH);
    strncpy(weight, strtok(NULL, delimiter), DATE_LENGTH);

    /* Adds edge and reverse edge */
    add_edge(graph, id, id2, weight);
    add_edge(graph, id2, id, weight);

    free(weight);
}

/* Main function that loads input file from arguments and
 * calls functions to find all paths. */
int main(int argc, char const *argv[])
{
	FILE *file;
	char line[256];
	int start_vertex, end_vertex, length;

	Graph *graph;

	if (argc < 5)
    {
        fprintf(stderr, "Please run application in this format dfs.exe <graph-file> <id1> <id2> <maxD>\n");
		return EXIT_FAILURE;
	}
    else
    {
        file = fopen(argv[1], "r");

        if (file == NULL)
        {
            fprintf(stderr, "The file does not exist!");
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
                parse_file(line, graph);
            }

            fclose(file);

            all_paths(graph, start_vertex, end_vertex);

            free_graph(graph);
        }
    }

	return 0;
}



