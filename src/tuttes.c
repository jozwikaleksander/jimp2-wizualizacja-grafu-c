#include "tuttes.h"
#include "vector.h"
#include "graph.h"



#include <stdio.h>
#include <stdlib.h>


int tuttes_algorithm(Graph *graph){
    //1.Stworzenie listy sąsiedstwa

    uint** adj_list = (uint**)malloc(graph->num_nodes * sizeof(uint*));
    int* deg = (int*)malloc(graph->num_nodes * sizeof(int));
    if (adj_list == NULL){
        fprintf(stderr, "BLAD: Błąd alokacji pamięci dla listy sąsiedstwa");
        return ERR_MEMORY_ALLOC;
    }
    if (deg == NULL){
        fprintf(stderr, "BLAD: Błąd alokacji pamięci dla listy ilości sąsiedi");
        return ERR_MEMORY_ALLOC;
    }
    for (int i = 0; i < graph->num_nodes; i++)
        adj_list[i] = (uint*)malloc(graph->num_nodes * sizeof(uint));

    int result_adj_list= build_adj_list(graph, adj_list, deg);
    if (result_adj_list != 0){
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listę sąsiedstwa ");
    }
    print_adj_list(graph, adj_list, deg);


    free_adj_list(graph, adj_list);
    free_deg(graph, deg);

    return EXIT_SUCCESS;


}