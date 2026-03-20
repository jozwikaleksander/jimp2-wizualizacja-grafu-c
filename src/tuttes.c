#include "tuttes.h"
#include "vector.h"
#include "graph.h"



#include <stdio.h>
#include <stdlib.h>


void tuttes_algorythm(Graph *graph){
    //1.Stworzenie listy sąsiedstwa

    uint** adj_list = (uint**)malloc(graph->num_nodes * sizeof(uint*));
    int* deg = (int*)malloc(graph->num_nodes * sizeof(int));

    for (int i = 0; i < graph->num_nodes; i++)
        adj_list[i] = (uint*)malloc(graph->num_nodes * sizeof(uint));



}