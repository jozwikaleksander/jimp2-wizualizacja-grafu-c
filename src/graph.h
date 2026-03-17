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
    uint id;
} Node;

typedef struct{
    Node *nodes;
    int num_nodes;
    int capacity_nodes;
    
    Edge *edges;
    int num_edges;
    int capacity_edges;
} Graph;

/**
 * @brief Funkcja zwieksza rozmiar dynamicznej tablicy wierzcholkow
 * @param graph - wskaznik na strukture grafu
 * @param max_index - maksymalny indeks wierzcholka
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
*/
Graph *load_graph(FILE *graph_file, int width, int height);

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph);

#endif