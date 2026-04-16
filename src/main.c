#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "input.h"
#include "eades.h"
#include "graph.h"
#include "output.h"
#include "tuttes.h"
#include "error.h"
#include "validation.h"

int main(int argc, char **argv) {
    // Pobranie parametrów wykonania programu z linii poleceń
    Parameters params = create_params();
    if(load_params(argc,argv,&params) == EXIT_FAILURE)
        return EXIT_FAILURE;

    // Wczytanie pliku o nazwie wprowadzonej z linii poleceń
    if (optind < argc) {
        params.graph_file = fopen(argv[optind], "r");
    }

    if (params.graph_file == NULL) {
        fprintf(stderr,"BŁĄD: Nie udało się otworzyć pliku.\n");
        return ERR_FILE_OPEN;
    }

    // Sprawdzenie czy ziarno zostało podano jako argument linii poleceń
    if(params.wasSeedProvided == 1)
        srand(params.seed);
    else
        srand(time(NULL));

    // Stworzenie grafu na podstawie pliku
    int load_graph_status = 0;
    Graph *graph = load_graph(params.graph_file, params.width, params.height, &load_graph_status);
    if(load_graph_status == ERR_GRAPH_LOAD) {
        fprintf(stderr,"BŁĄD: Nie udało się wczytać grafu.\n");
        fclose(params.graph_file);
        free(params.output_name);
        return ERR_GRAPH_LOAD;
    }
    else if (load_graph_status == ERR_MEMORY_ALLOC) {
        fprintf(stderr,"BŁĄD: Nie udało się wczytać grafu.\n");
        fclose(params.graph_file);
        free(params.output_name);
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
    switch(params.algorithm) {
        case EADES_ALGORITHM:
            eades_algorithm(graph, params.minimum_force, params.max_iterations, params.ideal_len, params.spring_const, 
                params.repulsion_const, params.cooling);
            break;
        case TUTTES_ALGORITHM:
            tuttes_algorithm(graph, params.max_iterations);
            break;
        default:
            eades_algorithm(graph, params.minimum_force, params.max_iterations, params.ideal_len, params.spring_const, params.repulsion_const, params.cooling);
            break;
    }

    // Obsluga zapisu do wybranego typu pliku
    switch(params.output_type) {
        case OUTPUT_TXT:
            if(save_to_txt(graph, params.output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku tekstowego.\n");
                return ERR_SAVE_TEXT_FAILED;
            }
            break;
        case OUTPUT_BIN:
            if(save_to_bin(graph, params.output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku binarnego.\n");
                return ERR_SAVE_BINARY_FAILED;
            }
            break;
    }
    free(params.output_name);
    fclose(params.graph_file);

    printf("Pomyślnie zapisano informacje o %d wierzcholkach do pliku.\n", graph->num_nodes);
    free_graph(graph);

    return EXIT_SUCCESS;
}