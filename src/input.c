#include "input.h"
#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/**
* @brief Funkcja tworzy nową strukturę Parameters i ustawia domyślne parametry programu.
* @return wskaźnik na strukturę Parameters
*/
Parameters create_params() {
    Parameters params;
    params.algorithm = EADES_ALGORITHM;
    params.output_type = OUTPUT_TXT;
    params.output_name = NULL;
    params.graph_file = NULL;
    
    params.seed = 0;
    params.wasSeedProvided = 0;
    params.width = 100;
    params.height = 100;

    // Parametry algorytmu Eadesa z wartościami domyślnymi
    params.minimum_force = 0.1;
    params.max_iterations = 10000;
    params.ideal_len = 10.0;
    params.repulsion_const = 2.0;
    params.spring_const = 1.0;
    params.cooling = 0.97;

    return params;
}

/**
* @brief Funkcja pobiera parametry podane z linii poleceń i umieszcza je w strukturze Parameters.
* @param argc - ilość parametrów
* @param argv - tablica parametrów
* @param params - wskaźnik na strukturę Parameters
* @return dla sukcesu EXIT_SUCCESS, dla błędu ERR_MISSING wyboru opcji
*/
int load_params(int argc, char **argv, Parameters *params) {
    int opt;
    // Wczytanie parametrow wykonania
    while((opt = getopt(argc, argv, ":a:t:s:o:w:h:m:i:l:k:c:C:")) != -1) 
    {
        switch(opt) 
            { 
                // Wybor algorytmu
                case 'a':
                    if (strcmp(optarg, "t") == 0 || strcmp(optarg, "tutte") == 0) {
                        params->algorithm = TUTTES_ALGORITHM; // 1 for Tutte
                    } else if (strcmp(optarg, "e") == 0 || strcmp(optarg, "eades") == 0) {
                        params->algorithm = EADES_ALGORITHM; // 0 for Eades
                    } else {
                        fprintf(stderr, "BŁĄD: Podano nieprawidłowy algorytm.\n");
                        params->algorithm = EADES_ALGORITHM;
                    }
                    break;
                case 't':
                    if(strcmp(optarg, "b") == 0 || strcmp(optarg, "bin") == 0) {
                        params->output_type = OUTPUT_BIN;
                    } else if(strcmp(optarg, "t") == 0 || strcmp(optarg, "txt") == 0){
                        params->output_type = OUTPUT_TXT;
                    } else {
                        fprintf(stderr, "BŁĄD: Podano nieprawidłowy typ pliku wyjściowego.\n");
                        params->output_type = OUTPUT_TXT;
                    }
                    break;
                case 'o':
                    params->output_name = strdup(optarg);
                    break;
                case 's':
                    
                    params->seed = atoi(optarg);
                    if(params->seed < 0)
                        fprintf(stderr,
                            "BŁĄD: Podano niepoprawne ziarno. Ziarno musi być większe od 0.\n");
                    else
                        params->wasSeedProvided = 1;
                    break;
                case 'w':
                            
                    params->width = atoi(optarg);
                    if(params->width <= 0) {
                        fprintf(stderr,
                            "BŁĄD: Podano nieprawidłową szerokość obszaru (wartość musi być większa od 0). Wybieram domyślną wartość = 100.\n");
                        params->width = 100;
                    }
                    break;
                case 'h':
                    params->height = atoi(optarg);
                    if(params->height <= 0) {
                        fprintf(stderr,
                            "BŁĄD: Podano nieprawidłową wysokość obszaru (wartość musi być większa od 0). Wybieram domyślną wartość = 100.\n");
                        params->height = 100;
                    }
                    break;
                // Parametry Eadesa
                case 'm':
                    params->minimum_force = atof(optarg);
                    break;
                case 'i':
                    params->max_iterations = atoi(optarg);
                    break;
                case 'l':
                    params->ideal_len = atof(optarg);
                    break;
                case 'k':
                    params->spring_const = atof(optarg);
                    break;
                case 'c':
                    params->repulsion_const = atof(optarg);
                    break;
                case 'C':
                    params->cooling = atof(optarg);
                    break;
                case ':': 
                    fprintf(stderr,"BŁĄD: Opcja -%c wymaga podania wartości!\n", optopt);
                    return ERR_MISSING_VALUE;
                    break; 
                case '?': 
                    fprintf(stderr,"BŁĄD: Nieznana opcja -%c\n", optopt);
                    break; 
        } 
    }
    // Wczytanie pliku o nazwie wprowadzonej z linii poleceń
    if (optind < argc) {
        params->graph_file = fopen(argv[optind], "r");
    }
    return EXIT_SUCCESS;
}