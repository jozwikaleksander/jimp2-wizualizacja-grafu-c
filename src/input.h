#ifndef _INPUT_H_
#define _INPUT_H_

#include "stdio.h"
#include "output.h"

#define EADES_ALGORITHM 0
#define TUTTES_ALGORITHM 1

typedef struct {
    int algorithm;
    int output_type;
    char *output_name;
    FILE *graph_file;

    int seed;
    int wasSeedProvided;
    int width;
    int height;

    // Parametry algorytmu Eadesa z wartościami domyślnymi
    double minimum_force;
    int max_iterations;
    double ideal_len;
    double repulsion_const;
    double spring_const;
    double cooling;
} Parameters;

/**
* @brief Funkcja pobiera parametry podane z linii poleceń i umieszcza je w strukturze Parameters.
* @param argc - ilość parametrów
* @param argv - tablica parametrów
* @param params - wskaźnik na strukturę Parameters
* @return dla sukcesu EXIT_SUCCESS, dla błędu EXIT_FAILURE
*/
int load_params(int argc, char **argv, Parameters *params);

/**
* @brief Funkcja tworzy nową strukturę Parameters i ustawia domyślne parametry programu.
* @return wskaźnik na strukturę Parameters
*/
Parameters create_params();


#endif