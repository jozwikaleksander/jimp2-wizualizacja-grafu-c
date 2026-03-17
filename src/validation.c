#include "validation.h"

/**
 * @brief Funkcja, sprawdza czy podany graf spełnia warunek konieczny planarności.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli spełnia warunek konieczny
 */
int is_potentially_planar(Graph *graph) {
    if (graph->num_nodes < 3) return 1;
    return graph->num_edges <= 3 * graph->num_nodes - 6;
}

int is_graph_planar(Graph *graph){
    // TODO: Napisać funkcję, która spradza czy graf jest planarny i spójny
    return is_potentially_planar(graph);
}