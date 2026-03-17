#ifndef _TUTTES_H_
#define _TUTTES_H_

#include "graph.h"

/**
 * @brief Funkcja, sprawdza czy podany graf jest planarny i spójny.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli graf planarny i spójny
 */
int is_graph_planar(Graph *graph);

#endif
