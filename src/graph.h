#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "stdio.h"
#include "stdlib.h"

#include "vector.h"
#include "error.h"

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
 * @brief Funkcja tworzy graf na podstawie wczytanego pliku
 * @param graph_file - wskaznik do pliku
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param out_code - kod błedu
 * @return Wskaźnik na strukturę grafu
 */
Graph *load_graph(FILE *graph_file, int width, int height, int *out_code);

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph);

/**
 * @brief Tworzy list sąsiedstwa dla grafu
 * @param graph - wskaźnik na hraf
 * @return lista sąsiedstwa - przechowywa indeksy wierzchołków w graph->nodes
 */

int build_adj_list(Graph* graph, uint** adj_list, int* deg);
void print_adj_list ( Graph *graph,uint** adj_list, int* deg );
void free_adj_list(Graph *graph, uint** adj_list);
void free_deg(Graph *graph, int* deg);
int dfs_res(Graph *graph, uint** adj_list, int *idx, int start, int visited[], int dfs_res);

#endif