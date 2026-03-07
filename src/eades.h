
#ifndef _EADES_H_
#define _EADES_H_

#include "math.h"

#include "graph.h"
#include "vector.h"

// #define MINIMUM_FORCE 0.1 //miensz za nia->stop
// #define MAX_ITERATIONS 10000
// #define IDEAL_LEN 5 //do tej długości będą dążyć sprzężyny
/**
 * @brief funkcja algorytmu Eades do zmiany pozycji wierzchołków
 * @param *graph - wskaznik na graf
 * @param minimum_force - drugi wektor
 * @param max_iterations - granica ilości iteracji
 * @param ideal_len - do tej długości będą dąrzyć krawiędzi
 * @param spring_const -  używane dla podliczenia sił przyciągania
 * @param c - używana dla podliczenia sił odpychania
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwolnia pracę sił
 * @return nic nie zwraca
*/
void eades_algorithm(Graph *graph, double minimum_force, int max_iterations, double ideal_len, double spring_const, int c, double cooling);

/**
 * @brief Funkcja podliczenia sił odchyłenia
 * @param u - pierwszy wierzchołek
 * @param v - drugi wierzchołek
 * @param c - używana dla podliczenia sił odpychania
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwolnia pracę sił 
 * @return nic nie zwraca
*/
void compute_repulive(Node *u, Node *v, int c, double cooling);

/**
 * @brief Funkcja podliczenia sił przyciągania
 * @param *graph - wskaznik na graf
 * @param *e - wskaznik na krawiędz
 * @param spring_const -  używane dla podliczenia sił przyciągania
 * @param ideal_len - do tej długości będą dąrzyć krawiędzi
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwolnia pracę sił
 * @return nic nie zwraca
*/
void compute_attract(Graph *graph, Edge *e, double spring_const, double ideal_len, double cooling);

#endif
