
#ifndef _EADES_H_
#define _EADES_H_

#include "graph.h"

#define MINIMUM_FORCE 0.1 //miensz za nia->stop
#define MAX_ITERATIONS 10000
#define IDEAL_LEN 5 //do tej długości będą dążyć sprzężyny

void eades_algorithm(Graph *graph);
void compute_repulive(Node *u, Node *v);
void compute_attract(Node *u, Node *v);

#endif
