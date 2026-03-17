#include "output.h"
#include <stdio.h>

/**
 * @brief Zapisuje informacje na temat wierzcholkow grafu do pliku tekstowego
 * @param graph - wskaznik na strukture grafu
 * @param output_name - wskaznik na nazwe pliku
 * @return 0 jezeli udalo sie zapisac, 1 jezeli nie udalo sie
*/
int save_to_txt(Graph *graph, char *output_name) {
    if(!graph) return 1;

    char *final_name = output_name ? output_name : OUTPUT_NAME_TXT;

    FILE *file = fopen(final_name,"w");
    if(!file) return 1;

    for(int i = 0; i < graph->num_nodes; i++) {
        fprintf(file, "%d %lf %lf\n", graph->nodes[i].id, graph->nodes[i].position.x, graph->nodes[i].position.y);
    }
    fclose(file);
    return 0;
}

/**
 * @brief Zapisuje informacje na temat wierzcholkow grafu do pliku binarnego
 * @param graph - wskaznik na strukture grafu
 * @param output_name - wskaznik na nazwe pliku
 * @return 0 jezeli udalo sie zapisac, 1 jezeli nie udalo sie
*/
int save_to_bin(Graph *graph, char *output_name) {
    if(!graph) return 1;

    char *final_name = output_name ? output_name : OUTPUT_NAME_BIN;

    FILE *file = fopen(final_name,"wb");
    if(!file) return 1;

    fwrite(&graph->num_nodes,sizeof(int),1,file);
    for(int i = 0; i < graph->num_nodes; i++) {
        int index = graph->nodes[i].id;
        fwrite(&(index),sizeof(int),1,file);
        fwrite(&graph->nodes[i].position.x,sizeof(double),1,file);
        fwrite(&graph->nodes[i].position.y,sizeof(double),1,file);
    }
    fclose(file);
    return 0;
}