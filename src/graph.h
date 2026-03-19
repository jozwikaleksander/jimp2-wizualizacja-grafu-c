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
 * @brief Funkcja tworzy graf na podstawie wczytanego pliku.
 * @param graph_ptr - wskaźnik na wskaźnik, gdzie zostanie zapisany adres nowego grafu.
 * @param graph_file - wskaźnik do otwartego pliku z danymi grafu.
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
 * @return 0 w przypadku sukcesu, lub kod błędu w przypadku niepowodzenia.
 */
int load_graph(Graph **graph_ptr, FILE *graph_file, int width, int height);

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph);

#endif