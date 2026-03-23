#ifndef _TUTTES_H_
#define _TUTTES_H_

#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "graph.h"

#define MINIMUM_CHANGE 0.01


/**
 * @brief Funkcja algorytmu Tutes do zmiany pozycji wierzchołków
 * @return kod pomyłki
*/
int tuttes_algorithm(Graph *graph, int max_iterations) ;
int tutte_iteration(Graph *graph,int iteration, int is_fixed[], uint** adj_list, Vector new_pos[], double sum_x, double sum_y, int deg[]);

#endif

