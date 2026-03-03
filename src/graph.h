#ifndef _MAP_H_
#define _MAP_H_

#include "stdio.h"
#include "stdlib.h"

typedef struct {
    int u, v; // Indeksy wierzchołków
    double weight; // Waga krawędzi
    char *name; // Nazwa krawędzi
} Edge;

typedef struct {
    Edge *edges;
    int num_edges;
    int capacity;
} EdgeList;

typedef struct{
    EdgeList *edge_list;
} Graph;

Graph *load_graph(FILE *graph_file);
void free_graph(Graph *graph);

#endif