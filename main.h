#ifndef DFS_MAIN_H
#define DFS_MAIN_H

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

double compute_metric(char *min_weight, char *max_weight);

int compare_dates(char *a, char *b);

time_t parse_date(char *date);

#endif
