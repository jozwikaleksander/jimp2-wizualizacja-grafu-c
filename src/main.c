#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "eades.h"
#include "graph.h"
#include "output.h"
#include "tuttes.h"

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

    //te wartości dobra byłoby umieć znieniać przez linię poleceń
    // TO DO: Te zmienne trzeba opisać, ponieważ nie wiadomo jak je wytłumaczyć w argumentach linii poleceń
    double minimum_force =  0.1; //miensz za nia->stop
    int max_iterations = 10000;
    double ideal_len = 10.0; //do tej długości będą dążyć sprzężyny 
    int c = 2;
    double spring_const =1.0;
    double cooling = 0.97;
    
    // ----------------------------

    // Wczytanie parametrow wykonania
    while((opt = getopt(argc, argv, ":a:t:s:o:w:h:")) != -1) 
    {
        switch(opt) 
            { 
                // Wybor algorytmu
                case 'a':
                    if (strcmp(optarg, "t") == 0 || strcmp(optarg, "tutte") == 0) {
                        algorithm = 1; // 1 for Tutte
                    } else if (strcmp(optarg, "e") == 0 || strcmp(optarg, "eades") == 0) {
                        algorithm = 0; // 0 for Eades
                    } else {
                        fprintf(stderr, "BŁĄD: Podano nieprawidłowy algorytm.\n");
                        algorithm = 0;
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
        return EXIT_FAILURE;
    }

    // Sprawdzenie czy ziarno zostalo podano jako argument linii polecen
    if(wasSeedProvided == 1)
        srand(seed);
    else
        srand(time(NULL));

    // Stworzenie grafu na podstawie pliku
    Graph *graph = load_graph(graph_file, width, height);
    if(graph == NULL) {
        fprintf(stderr,"BŁĄD: Nie udało się wczytać grafu.\n");
    }
    
    // Obsluga wyboru algorytmu z linii polecen
    switch(algorithm) {
        case 0:
            eades_algorithm(graph, minimum_force, max_iterations, ideal_len, spring_const, c, cooling);
            break;
        default:
            // TODO: Wykonać funkcje dla tw Tuttego
            eades_algorithm(graph, minimum_force, max_iterations, ideal_len, spring_const, c, cooling);
            break;
    }

    // Obsluga zapisu do wybranego typu pliku
    switch(output_type) {
        case OUTPUT_TXT:
            if(save_to_txt(graph, output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku tekstowego.\n");
                return EXIT_FAILURE;
            }
            break;
        case OUTPUT_BIN:
            if(save_to_bin(graph, output_name) == 1){
                fprintf(stderr, "BŁĄD: Nie udało się zapisać do pliku binarnego.\n");
                return EXIT_FAILURE;
            }
            break;
    }

    free(output_name);
    fclose(graph_file);

    printf("Pomyślnie zapisano informacje o %d wierzcholkach do pliku.\n", graph->num_nodes);
    free_graph(graph);

    return EXIT_SUCCESS;
}