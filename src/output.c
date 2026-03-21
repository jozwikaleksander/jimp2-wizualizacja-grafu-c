#include "output.h"

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
    
    uint32_t num_nodes = (uint32_t) graph->num_nodes;
    fwrite(&num_nodes,sizeof(uint32_t),1,file);
    for(int i = 0; i < graph->num_nodes; i++) {
        uint32_t index = (uint32_t) graph->nodes[i].id;
        float x = (float) graph->nodes[i].position.x;
        float y = (float) graph->nodes[i].position.y;

        fwrite(&index,sizeof(uint32_t),1,file);
        fwrite(&x,sizeof(float),1,file);
        fwrite(&y,sizeof(float),1,file);
    }
    fclose(file);
    return 0;
}