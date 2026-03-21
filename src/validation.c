#include "validation.h"
#include <stdbool.h>
#include "error.h"
#include "graph.h"
#include <planarity/graph.h>

/**
 * @brief Funkcja, sprawdza czy podany graf spełnia warunek konieczny planarności.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli spełnia warunek konieczny
 */
int is_potentially_planar(Graph *graph) {
    if (graph->num_nodes < 3) return 1;
    return graph->num_edges <= 3 * graph->num_nodes - 6;
}

/**
 * @brief Funkcja, sprawdza czy graf jest spójny stosując algorytm DFS.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli graf jest spójny, 0 jeżeli nie jest
 */
int is_graph_connected(Graph *graph) {
    if(graph->num_nodes == 0) return 1;

    // Alokoacja pamięci
    int *visited = (int*)calloc(graph->num_nodes, sizeof(int));
    int *dfs_res = (int*)malloc(graph->num_nodes * sizeof(int));
    uint **adj_list = (uint**)malloc(graph->num_nodes * sizeof(uint*));
    int *deg = (int*)malloc(graph->num_nodes * sizeof(int));
    int idx = 0;
    int is_connected = 0;

    // Sprawdzenie czy udało się zaalokować pamięć
    if (visited == NULL || dfs_res == NULL || adj_list == NULL || deg == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        if (visited) free(visited);
        if (dfs_res) free(dfs_res);
        if (adj_list) free(adj_list);
        if (deg) free(deg);
        return ERR_MEMORY_ALLOC;
    }
    
    // Alokacja pamieci dla elementów listy sąsiedztwa
    for (int i = 0; i < graph->num_nodes; i++)
        adj_list[i] = (uint*)malloc(graph->num_nodes * sizeof(uint));

    // Zbudowanie listy sąsiedztwa
    int result_adj_list = build_adj_list(graph, adj_list, deg);
    
    // Sprawdzenie czy udało się zbudować
    if (result_adj_list != 0){
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listy sąsiedztwa.\n");
    } else {
        // Uruchomienie DFS
        dfs_rec(graph, adj_list, deg, &idx, 0, visited,dfs_res);
        // Jeżeli indeks po wykonaniu DFS jest równy ilości wierzchołków to graf jest spójny
        if(idx == graph->num_nodes)
            is_connected = 1;
    }

    // Zwolnienie pamięci
    free_adj_list(graph, adj_list);
    free_deg(deg);
    free(visited);
    free(dfs_res);

    return is_connected;
}

/**
 * @brief Funkcja, sprawdza jaki indeks w tabeli nodes ma wierzchołek o podanym identyfikatorze
 * 
 * @param graph - wskaźnik na strukturę grafu
 * @param node_id - identyfikator wierzchołka
 * @return int 
 */
int find_node_index(Graph *graph, uint node_id) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i].id == node_id) {
            return i;
        }
    }
    return -1;
}

int is_graph_planar(Graph *graph) {
    graphP gp = gp_New();

    if (gp_InitGraph(gp, graph->num_nodes) != OK) {
        gp_Free(&gp);
        return false; 
    }

    // Dodawanie krawędzi
    for(int i = 0; i < graph->num_edges; i++) {
        int index_u = find_node_index(graph, graph->edges[i].u);
        int index_v = find_node_index(graph, graph->edges[i].v);

        if(index_u != -1 && index_v != -1) { // Sprawdzenie czy znaleziono indeksy
            gp_AddEdge(gp, index_u, 0, index_v, 0);        
        }
    }

    // Uruchomienia funkcji dla sprawdzanie planarności z biblioteki LibPlanarity
    int is_planar = (gp_Embed(gp, EMBEDFLAGS_PLANAR) == OK);

    // Zwalnianie pamięci
    gp_Free(&gp);
    return is_planar;
}

int is_graph_valid(Graph *graph){
    if(is_potentially_planar(graph) == 0)
        return ERR_GRAPH_NOT_PLANAR;
    else if(is_graph_connected(graph) == 0){
        return ERR_GRAPH_NOT_CONNECTED;
    }
    else if(is_graph_planar(graph) == 0){
        return ERR_GRAPH_NOT_PLANAR;
    }
    return 0;
}