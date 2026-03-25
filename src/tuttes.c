#include "tuttes.h"

int tuttes_algorithm(Graph *graph, int max_iterations) {
    // 1.Stworzenie listy sąsiedstwa

    // Inicjalizacja potrzebnych narzędzi
    int n = graph->num_nodes;
    int idx = 0;
    int dfs_res[n];
    uint **adj_list = (uint **)malloc(n * sizeof(uint *));
    int *deg = (int *)malloc(n * sizeof(int));
    Vector new_pos[n];
    for (int i = 0; i < n; i++) {
        adj_list[i] = (uint *)malloc(n * sizeof(uint));
    }

    double max_change;

    // Sprawdzenie działania allocacji
    if (adj_list == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    if (deg == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }

    // Budowanie listy sąsiedstwa
    int result_adj_list = build_adj_list(graph, adj_list, deg);
    if (result_adj_list != 0) {
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listy sąsiedztwa.\n");
        return ERR_ADJ_LIST;
    }
    // Wypisywanie listy sąsiedstwa
    // print_adj_list(graph, adj_list, deg);

    // 2.Znaleznienie dowolnego cykłu wierzchołków grafu
    find_outer_face(graph, adj_list, deg, dfs_res, &idx);

    // Wypisywanie cykłu
    // print_outer_face(dfs_res, idx);

    // 3.======================Część obliczeniowa======================
    int is_fixed[n];
    for (int i = 0; i < n; i++) {
        is_fixed[i] = 0;
    }
    for (int i = 0; i < idx; i++) {
        is_fixed[dfs_res[i]] = 1;
    }
    Vector center = get_center(graph);
    printf("Srodek:(%f, %f)\n", center.x, center.y);
    print_nodes_pos(graph, is_fixed);
    place_on_circle(dfs_res, graph, idx, center);
    print_nodes_pos(graph, is_fixed);

    int t = 0;
    max_change = 10.0;
    while (t < max_iterations && max_change > MINIMUM_CHANGE) {

        t++;
        max_change = 0.0;

        for (int i = 0; i < graph->num_nodes; i++) {
            tutte_iteration(graph, i, is_fixed, adj_list, new_pos, deg);
        }

        for (int i = 0; i < graph->num_nodes; i++) {
            if (is_fixed[i] != 1) {
                double change = distance(new_pos[i], graph->nodes[i].position);
                if (change > max_change) {
                    max_change = change;
                }

                graph->nodes[i].position.x = new_pos[i].x;
                graph->nodes[i].position.y = new_pos[i].y;
            }
        }
        print_nodes_pos(graph, is_fixed);
    }
    // Zwolnienie zaalokowanej pamięci
    free_adj_list(graph, adj_list);
    free_deg(deg);
    // printf("%d", t);

    return EXIT_SUCCESS;
}

int tutte_iteration(Graph *graph, int iteration, int is_fixed[],
                    uint **adj_list, Vector new_pos[], int deg[]) {
    double sum_x = 0.0;
    double sum_y = 0.0;
    if (is_fixed[iteration] != 1) {
        for (int j = 0; j < deg[iteration]; j++) {
            int neighbor = adj_list[iteration][j];
            sum_x += graph->nodes[neighbor].position.x;
            sum_y += graph->nodes[neighbor].position.y;
        }
        new_pos[iteration].x = sum_x / deg[iteration];
        new_pos[iteration].y = sum_y / deg[iteration];
    }

    return EXIT_SUCCESS;
}
