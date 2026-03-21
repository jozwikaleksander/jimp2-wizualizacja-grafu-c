#ifndef _VALIDATION_H_
#define _VALIDATION_H_

#include <stdbool.h>
#include <planarity/graph.h>

#include "graph.h"
#include "error.h"

/**
 * @brief Funkcja, sprawdza czy podany graf jest planarny i spójny.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli graf planarny i spójny
 */
int is_graph_valid(Graph *graph);

#endif // _VALIDATION_H_
