
#ifndef _EADES_H_
#define _EADES_H_

#include "math.h"

#include "graph.h"
#include "vector.h"

// #define MINIMUM_FORCE 0.1 //miensz za nia->stop
// #define MAX_ITERATIONS 10000
// #define IDEAL_LEN 5 //do tej długości będą dążyć sprzężyny

void eades_algorithm(Graph *graph, double minimum_force, int max_iterations, int ideal_len, double spring_const, int c);
void compute_repulive(Node *u, Node *v, int c);
void compute_attract(Graph *graph, Edge *e,double spring_const, double ideal_len);

#endif
