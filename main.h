#ifndef __MAIN_H__
#define __MAIN_H__

#include "stack.h"
#include "void_stack.h"
#include "linked_list.h"

typedef struct Graph Graph;
typedef struct Vertex Vertex;
typedef struct Edge Edge;

struct Graph
{
    int V;
    int E;
    Vertex *vertices;
};

struct Vertex
{
    int id;
    int label;
    Edge *edges;
    Vertex *next;
};

struct Edge
{
    Vertex *connected_to;
    char weight[11];
    Edge *next;
};

void loop(Graph *graph);

Graph *create_graph();

void free_graph(Graph *graph);

void add_vertex(Graph *graph, int label);

void add_edge(Graph *graph, int source, int destination, char *weight);

int vertex_not_exist(Graph *graph, int label);

void process_paths(Stack *path, char *buffer);

int process_edges(VoidStack *edges, char *buffer);

Vertex *find_vertex(Graph *graph, int label);

double compute_metric(char *min_weight, char *max_weight);

int compare_dates(char *a, char *b);

time_t parse_date(char *date);

void dfs(Graph *graph, Vertex *s, Vertex *v, Stack *path, List **paths, VoidStack *edges, int on_path[], int length);

void all_paths(Graph *graph, int start_vertex, int end_vertex);

void parse_file(char *line, Graph *graph);

#endif
