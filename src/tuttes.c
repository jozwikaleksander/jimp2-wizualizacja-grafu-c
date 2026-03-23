#include "tuttes.h"




int tuttes_algorithm(Graph *graph) {
    // 1.Stworzenie listy sąsiedstwa

    //Inicjalizacja potrzebnych narzędzi
    int n = graph->num_nodes;
    int idx = 0;
    int dfs_res [n]; 
    uint **adj_list = (uint **)malloc(n * sizeof(uint *));
    int *deg = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        adj_list[i] = (uint *)malloc(n * sizeof(uint));

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

    //2.Znaleznienie dowolnego cykłu wierzchołków grafu
    find_outer_face(graph, adj_list, deg, dfs_res, &idx);

    //Wypisywanie cykłu
    print_outer_face(dfs_res, idx);


    //3.======================Część obliczeniowa======================
    int is_fixed[n];
    for (int i = 0; i<n; i++){
        is_fixed[i] = 0;

    }

    for (int i = 0; i<idx; i++){
        
        is_fixed[dfs_res[i]] = 1;

    }
    
    // print_list(is_fixed, n);

    Vector center = get_center(graph);
    printf("Srodek:(%f, %f)\n",center.x, center.y);
    // for (int i = 0; i<n; i++){
    //     printf("[%d] - (%f,%f) \n", i,graph->nodes[i].position.x, graph->nodes[i].position.y  );

    // }
    
    place_on_circle(dfs_res,graph, idx, center);
    
   
    // for (int i = 0; i<n; i++){
    //     printf("[%d] - (%f,%f) \n", i,graph->nodes[i].position.x, graph->nodes[i].position.y  );

    // }
    // printf("\n");

    


   












   
    //Zwolnienie zaalokowanej pamięci
    free_adj_list(graph, adj_list);
    free_deg(deg);

    return EXIT_SUCCESS;
}