#include "tuttes.h"


//ZAMINIĆ graph->num_nodes


int tuttes_algorithm(Graph *graph) {
    // 1.Stworzenie listy sąsiedstwa

    //Inicjalizacja potrzebnych narzędzi
    int idx = 0;
    int dfs_res [graph->num_nodes]; 
    uint **adj_list = (uint **)malloc(graph->num_nodes * sizeof(uint *));
    int *deg = (int *)malloc(graph->num_nodes * sizeof(int));
    for (int i = 0; i < graph->num_nodes; i++)
        adj_list[i] = (uint *)malloc(graph->num_nodes * sizeof(uint));

    //Sprawdzenie działania allocacji
    if (adj_list == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    if (deg == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    
    //Budowanie listy sąsiedstwa
    int result_adj_list = build_adj_list(graph, adj_list, deg);
    if (result_adj_list != 0) {
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listy sąsiedztwa.\n");
        return ERR_ADJ_LIST;
    }
    //Wypisywanie listy sąsiedstwa
    print_adj_list(graph, adj_list, deg);

    //Znaleznienie dowolnego cykłu wierzchołków grafu
    find_outer_face(graph, adj_list, deg, dfs_res, &idx);

    //Wypisywanie cykłu
    print_outer_face(dfs_res, idx);
   
    //Zwolnienie zaalokowanej pamięci
    free_adj_list(graph, adj_list);
    free_deg(deg);

    return EXIT_SUCCESS;
}