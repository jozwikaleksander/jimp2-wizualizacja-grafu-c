#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "eades.h"
#include "graph.h"
#include "output.h"
#include "tuttes.h"
#include "error.h"
#include "validation.h"

#define EADES_ALGORITHM 0
#define TUTTES_ALGORITHM 1

int main(int argc, char **argv) {
    // Zmienne dla parametrow wykonania
    int algorithm = 0;
    int output_type = OUTPUT_TXT;
    char *output_name = NULL;
    int opt;
    FILE *graph_file = NULL;
    
    int seed = 0;
    int wasSeedProvided = 0;
    int width = 100;
    int height = 100;

    // Parametry algorytmu Eadesa z wartościami domyślnymi
    double minimum_force = 0.1;
    int max_iterations = 10000;
    double ideal_len = 10.0;
    double repulsion_const = 2.0;
    double spring_const = 1.0;
    double cooling = 0.97;
    
    // ----------------------------

    // Wczytanie parametrow wykonania
    while((opt = getopt(argc, argv, ":a:t:s:o:w:h:m:i:l:k:c:C:")) != -1) 
    {
        switch(opt) 
            { 
                // Wybor algorytmu
                case 'a':
                    if (strcmp(optarg, "t") == 0 || strcmp(optarg, "tutte") == 0) {
                        algorithm = TUTTES_ALGORITHM; // 1 for Tutte
                    } else if (strcmp(optarg, "e") == 0 || strcmp(optarg, "eades") == 0) {
                        algorithm = EADES_ALGORITHM; // 0 for Eades
                    } else {
                        fprintf(stderr, "BŁĄD: Podano nieprawidłowy algorytm.\n");
                        algorithm = EADES_ALGORITHM;
                    }
                    break;
                case 't':
                    if(strcmp(optarg, "b") == 0 || strcmp(optarg, "bin") == 0) {
                        output_type = OUTPUT_BIN;
                    } else if(strcmp(optarg, "t") == 0 || strcmp(optarg, "txt") == 0){
                        output_type = OUTPUT_TXT;
                    } else {
                        fprintf(stderr, "BŁĄD: Podano nieprawidłowy typ pliku wyjściowego.\n");
                        output_type = OUTPUT_TXT;
                    }
                    break;
                case 'o':
                    output_name = strdup(optarg);
                    break;
                case 's':
                    
                    seed = atoi(optarg);
                    if(seed < 0)
                        fprintf(stderr,
                            "BŁĄD: Podano niepoprawne ziarno. Ziarno musi być większe od 0.\n");
                    else
                        wasSeedProvided = 1;
                    break;
                case 'w':
                            
                    width = atoi(optarg);
                    if(width <= 0) {
                        fprintf(stderr,
                            "BŁĄD: Podano nieprawidłową szerokość obszaru (wartość musi być większa od 0). Wybieram domyślną wartość = 100.\n");
                        width = 100;
                    }
                    break;
                case 'h':
                    height = atoi(optarg);
                    if(height <= 0) {
                        fprintf(stderr,
                            "BŁĄD: Podano nieprawidłową wysokość obszaru (wartość musi być większa od 0). Wybieram domyślną wartość = 100.\n");
                        height = 100;
                    }
                    break;
                // Parametry Eadesa
                case 'm':
                    minimum_force = atof(optarg);
                    break;
                case 'i':
                    max_iterations = atoi(optarg);
                    break;
                case 'l':
                    ideal_len = atof(optarg);
                    break;
                case 'k':
                    spring_const = atof(optarg);
                    break;
                case 'c':
                    repulsion_const = atof(optarg);
                    break;
                case 'C':
                    cooling = atof(optarg);
                    break;
                case ':': 
                    fprintf(stderr,"BŁĄD: Opcja -%c wymaga podania wartości!\n", optopt);
                    return EXIT_FAILURE;
                    break; 
                case '?': 
                    fprintf(stderr,"BŁĄD: Nieznana opcja -%c\n", optopt);
                    break; 
        } 
    }

    // Wczytanie pliku o nazwie wprowadzonej z linii poleceń
    if (optind < argc) {
        graph_file = fopen(argv[optind], "r");
    }

    if (graph_file == NULL) {
        fprintf(stderr,"BŁĄD: Nie udało się otworzyć pliku.\n");
        return ERR_FILE_OPEN;
    }

    // Sprawdzenie czy ziarno zostało podano jako argument linii poleceń
    if(wasSeedProvided == 1)
        srand(seed);
    else
        srand(time(NULL));

    // Stworzenie grafu na podstawie pliku
    int load_graph_status = 0;
    Graph *graph = load_graph(graph_file, width, height, &load_graph_status);
    if(load_graph_status == ERR_GRAPH_LOAD) {
        fprintf(stderr,"BŁĄD: Nie udało się wczytać grafu.\n");
        fclose(graph_file);
        free(output_name);
        return ERR_GRAPH_LOAD;
    }
    else if (load_graph_status == ERR_MEMORY_ALLOC) {
        fprintf(stderr,"BŁĄD: Nie udało się wczytać grafu.\n");
        fclose(graph_file);
        free(output_name);
        return ERR_MEMORY_ALLOC;
    }

    int validation_result = is_graph_valid(graph);
    switch(validation_result) {
        case ERR_GRAPH_NOT_CONNECTED:
            fprintf(stderr,"BŁĄD: Graf nie jest spójny.\n");
            return validation_result;
        case ERR_GRAPH_NOT_PLANAR:
            fprintf(stderr,"BŁĄD: Graf nie jest planarny.\n");
            return validation_result;
        case ERR_MEMORY_ALLOC:
            fprintf(stderr, "BŁĄD: Błąd alokacji pamięci podczas walidacji grafu.\n");
            return validation_result;
        default:
            break;
    }
    
    // Obsluga wyboru algorytmu z linii polecen
    switch(algorithm) {
        case EADES_ALGORITHM:
            eades_algorithm(graph, minimum_force, max_iterations, ideal_len, spring_const, repulsion_const, cooling);
            break;
        case TUTTES_ALGORITHM:
            tuttes_algorithm(graph);
            break;
        default:
            eades_algorithm(graph, minimum_force, max_iterations, ideal_len, spring_const, repulsion_const, cooling);
            break;
    }

    // Obsluga zapisu do wybranego typu pliku
    switch(output_type) {
        case OUTPUT_TXT:
            if(save_to_txt(graph, output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku tekstowego.\n");
                return ERR_SAVE_TEXT_FAILED;
            }
            break;
        case OUTPUT_BIN:
            if(save_to_bin(graph, output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku binarnego.\n");
                return ERR_SAVE_BINARY_FAILED;
            }
            break;
    }

    free(output_name);
    fclose(graph_file);

    printf("Pomyślnie zapisano informacje o %d wierzcholkach do pliku.\n", graph->num_nodes);
    free_graph(graph);

    return EXIT_SUCCESS;
}