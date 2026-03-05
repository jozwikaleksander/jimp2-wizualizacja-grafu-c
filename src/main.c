#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "graph.h"

int main(int argc, char **argv) {
    //te wartości dobra byłoby umieć znieniać


    srand(time(NULL));

    int opt;
    FILE *graph_file = NULL;
    FILE *output_file = NULL;
    int algorithm = 0;
    // ----------------------------

    // Wczytanie parametrow wykonania
    while((opt = getopt(argc, argv, ":p:i:c:n:s:r:")) != -1) 
    {
        switch(opt) 
            { 
                // Wybor algorytmu
                case 'a':
                    algorithm = atoi(optarg);
                    if(algorithm < 0 || algorithm > 1) {
                        fprintf(stderr, "BŁĄD: Podano nie prawidłowy algorytm.\n");
                        return EXIT_FAILURE;
                    }
                    break;
                case ':': 
                    printf("Opcja -%c wymaga podania wartości!\n", optopt); 
                    break; 
                case '?': 
                    printf("Nieznana opcja: -%c\n", optopt);
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

    // Stworzenie grafu na podstawie pliku
    Graph *graph = load_graph(graph_file);

    // Wypisanie krawedzi grafu
    for(int i = 0; i < graph->num_edges; i++) {
        Edge *current_edge = &graph->edges[i];
        printf("Krawędź: %s, Indeks u: %d, Indeks v: %d, Waga: %lg\n",current_edge->name, current_edge->u, current_edge->v, current_edge->weight);
    }
    // Wypisanie wierzcholkow grafu
    for(int i = 0; i < graph->num_nodes; i++) {
        Node *node = &graph->nodes[i];
        printf("Indeks: %d, X: %lg, Y: %lg, Siła X: %lg, Siła Y: %lg\n",i, node->position.x, node->position.y, node->force.x, node->force.y);
    }
    free_graph(graph);

    return 0;
}