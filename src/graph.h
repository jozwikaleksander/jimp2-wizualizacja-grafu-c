#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "stdio.h"
#include "stdlib.h"

typedef struct {
    int u, v; // Indeksy wierzchołków
    double weight; // Waga krawędzi
    char *name; // Nazwa krawędzi
} Edge;

typedef struct {
    double x;
    double y;
} Vector;

typedef struct {
    Vector position;
    Vector force;
} Node;

typedef struct{
    int num_nodes;
    Node *nodes;
    
    Edge *edges;
    int num_edges;
    int capacity_edges;
} Graph;

Graph *load_graph(FILE *graph_file);
void free_graph(Graph *graph);

#endif