
#ifndef _EADES_H_
#define _EADES_H_

#include <math.h>

#include "graph.h"
#include "vector.h"

// #define MINIMUM_FORCE 0.1 // Jeżeli maksymalna siłą spośród wszystkich jest 
// mniejsza niż ta wartość algorytm się zatrzymuje
// #define MAX_ITERATIONS 10000
// #define IDEAL_LEN 5 //Długość do której dążą sprężyny

/**
 * @brief Funkcja algorytmu Eades do zmiany pozycji wierzchołków
 * @param graph - wskaznik na graf
 * @param minimum_force - minimalna dopuszczalna wartość siły
 * @param max_iterations - maksymalna ilość iteracji
 * @param ideal_len - do tej długości będą dążyć krawędzie
 * @param spring_const -  używane do obliczenia sił przyciągania
 * @param c - używana do obliczenia sił odpychania
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwolnia pracę sił
*/
void eades_algorithm(Graph *graph, double minimum_force, int max_iterations, double ideal_len, double spring_const, int c, double cooling);

/**
 * @brief Funkcja podliczenia sił odchyłenia
 * @param u - wskaźnik na pierwszy wierzchołek
 * @param v - wskaźnik na drugi wierzchołek
 * @param c - używana do obliczenia sił odpychania
*/
void compute_repulive(Node *u, Node *v, int c);

/**
 * @brief Funkcja podliczenia sił przyciągania
 * @param graph - wskaźnik na graf
 * @param e - wskaźnik na krawędź
 * @param spring_const -  używane do obliczenia sił przyciągania
 * @param ideal_len - długość do której dążą krawędzie
*/
void compute_attract(Graph *graph, Edge *e, double spring_const, double ideal_len);

#endif
