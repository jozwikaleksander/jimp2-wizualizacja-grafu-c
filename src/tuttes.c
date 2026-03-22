#include "tuttes.h"


//ZAMINIĆ graph->num_nodes


int tuttes_algorithm(Graph *graph) {
    // 1.Stworzenie listy sąsiedstwa

    //1.1 Inicjalizacja potrzebnych narzędzi


   
    
    

    uint **adj_list = (uint **)malloc(graph->num_nodes * sizeof(uint *));
    int *deg = (int *)malloc(graph->num_nodes * sizeof(int));
    if (adj_list == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    if (deg == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    for (int i = 0; i < graph->num_nodes; i++)
        adj_list[i] = (uint *)malloc(graph->num_nodes * sizeof(uint));

    int result_adj_list = build_adj_list(graph, adj_list, deg);
    if (result_adj_list != 0) {
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listy sąsiedztwa.\n");
        return ERR_ADJ_LIST;
    }
    print_adj_list(graph, adj_list, deg);

    int idx = 0;
    int dfs_res [graph->num_nodes]; 
    find_outer_face(graph, adj_list, deg, dfs_res, &idx);
    print_outer_face(dfs_res, idx);
    //print_outer_face(cycle_res, cycle_idx);

    free_adj_list(graph, adj_list);
    free_deg(deg);

    return EXIT_SUCCESS;
}