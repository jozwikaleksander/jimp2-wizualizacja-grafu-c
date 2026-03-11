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
    int width = 500;
    int height = 500;

    //te wartości dobra byłoby umieć znieniać przez linię poleceń
    double minimum_force =  0.1; //miensz za nia->stop
    int max_iterations = 10000;
    double ideal_len = 10.0; //do tej długości będą dążyć sprzężyny 
    int c = 2;
    double spring_const =1.0;
    double cooling = 0.97;
    
    // ----------------------------

    // Wczytanie parametrow wykonania
    while((opt = getopt(argc, argv, ":a:t:s:o:")) != -1) 
    {
        switch(opt) 
            { 
                // Wybor algorytmu
                case 'a':
                    algorithm = atoi(optarg);
                    if(algorithm < 0 || algorithm > 1) {
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

    // Wypisanie krawedzi grafu
    for(int i = 0; i < graph->num_edges; i++) {
        Edge *current_edge = &graph->edges[i];
        printf("Krawędź: %s, Indeks u: %d, Indeks v: %d, Waga: %lg\n",current_edge->name, current_edge->u, 
            current_edge->v, current_edge->weight);
    }
    // Wypisanie wierzcholkow grafu
    for(int i = 0; i < graph->num_nodes; i++) {
        Node *node = &graph->nodes[i];
        printf("Indeks: %d, X: %lg, Y: %lg, Siła X: %lg, Siła Y: %lg\n",i+1, node->position.x, node->position.y, 
            node->force.x, node->force.y);
    }
    
    // Obsluga wyboru algorytmu z linii polecen
    switch(algorithm) {
        case 0:
            eades_algorithm(graph, minimum_force, max_iterations, ideal_len, spring_const, c, cooling);
            break;
        default:
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

    printf("Pomyślnie zapisano informacje o %d wierzcholkach do pliku.\n", graph->num_nodes);
    free_graph(graph);

    return EXIT_SUCCESS;
}