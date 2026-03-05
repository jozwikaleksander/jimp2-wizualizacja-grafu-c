#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "stdio.h"
#include "stdlib.h"

#include "vector.h"

typedef struct {
    int u, v;
    double weight;
    char *name;
} Edge;

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

/**
 * @brief Funkcja tworzy graf na podstawie wczytanego pliku
 * @param graph_file - wskaznik do pliku
 * @return wskaznik na stworzony graf
*/
Graph *load_graph(FILE *graph_file);

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph);

#endif