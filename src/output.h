#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "graph.h"

#define OUTPUT_NAME_TXT "output.txt"
#define OUTPUT_NAME_BIN "output.bin"
#define OUTPUT_TXT 0
#define OUTPUT_BIN 1

/**
 * @brief Zapisuje informacje na temat wierzcholkow grafu do pliku tekstowego
 * @param graph - wskaznik na strukture grafu
 * @param output_name - wskaznik na nazwe pliku
 * @return 0 jezeli udalo sie zapisac, 1 jezeli nie udalo sie
*/
int save_to_txt(Graph *graph, char *output_name);

/**
 * @brief Zapisuje informacje na temat wierzcholkow grafu do pliku binarnego
 * @param graph - wskaznik na strukture grafu
 * @param output_name - wskaznik na nazwe pliku
 * @return 0 jezeli udalo sie zapisac, 1 jezeli nie udalo sie
*/
int save_to_bin(Graph *graph, char *output_name);

#endif