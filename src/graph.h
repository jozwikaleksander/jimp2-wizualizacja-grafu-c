#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "error.h"

typedef struct {
    int u, v;
    double weight;
    char *name;
} Edge;

typedef struct {
    Vector position;
    Vector force;
    uint id;
} Node;

typedef struct{
    Node *nodes;
    int num_nodes;
    int capacity_nodes;
    
    Edge *edges;
    int num_edges;
    int capacity_edges;
    int width;
    int height;
} Graph;

/**
 * @brief Funkcja tworzy graf na podstawie wczytanego pliku
 * @param graph_file - wskaznik do pliku
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param out_code - kod błedu
 * @return Wskaźnik na strukturę grafu
 */
Graph *load_graph(FILE *graph_file, int width, int height, int *out_code);

/**
 * @brief Funkcja zwalnia miejsca zaalokowane dla struktury grafu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph);

/**
 * @brief Tworzy list sąsiedstwa dla grafu
 * @param graph - wskaźnik na hraf
 * @return lista sąsiedstwa - przechowywa indeksy wierzchołków w graph->nodes
 */

int build_adj_list(Graph* graph, uint** adj_list, int* deg);
/**
 * @brief Funkcja drukuje listę sąsiedstwa na stdout
 * @param graph - wskaznik do pliku
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 */
void print_adj_list ( Graph *graph,uint** adj_list, int* deg );
/**
 * @brief Funkcja zwalnia listę sąsiedstwa
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 */
void free_adj_list(Graph *graph, uint** adj_list);
/**
 * @brief Funkcja zwalnia podaną listę ilości sąsiadów
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 */
void free_deg(int* deg);
/**
 * @brief Funkcja dla znaleznienia ścieżki w grafie
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 * @param idx - ilość zwiedzonych wierzchołków
 * @param start - terazniejsza pozycja
 * @param visited - lista poprzednie zwiedzonych wierzchołków
 * @param dfs_res - lista przechowująca końcową ścieżkę 
 */
void dfs_rec(Graph *graph, uint **adj_list, int *deg, int *idx, int start, int visited[],
             int dfs_res[]);
/**
 * @brief Funkcja dla znaleznienia zewnętrznego poligonu(dfs)
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 * @param cycle_res - ilość zwiedzonych wierzchołków w cyklu zwracania
 * @param cycle_idx - terazniejsza pozycja w cyklu zwracania

 */
void find_outer_face(Graph *graph, uint **adj_list, int *deg, int cycle_res[], int *cycle_idx);
/**
 * @brief Funkcja dla drukowania indeksów wierzchołków zewnętrznego poligonu
 * @param dfs_res - wskaznik na strutkure grafu
 * @param dfs_res_size - wskaznik na listę sąsiedstwa do wydrukowania 

 */
void print_outer_face( int dfs_res[], int dfs_res_size);
/**
 * @brief Funkcja dla znaleznienia ścieżki w grafie
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 * @param idx - ilość zwiedzonych wierzchołków
 * @param current - terazniejsza pozycja
 * @param visited - lista poprzednie zwiedzonych wierzchołków
 * @param parent - lista przechowująca końcową ścieżkę 
 * @param cycle_res - lista poprzednie zwiedzonych wierzchołków
 * @param cycle_idx - terazniejszy indeks w cycle_res
 * @param found - flaga pokazująca czy znalezlismy rozwiązanie 
 */
int dfs_rec_face(Graph *graph, uint **adj_list, int *deg, int *idx, int current,
                  int visited[], int parent[], int cycle_res[], int *cycle_idx, int *found);


/**
 * @brief Znajduje środek na przestrzeni 
 * @param graph - graph na przestrzeni którego szukamy środek
 * @return center - Vector z współżędnymi środku
 */
Vector get_center (Graph *graph);

void place_on_circle(int outer_faces[], Graph *graph, int k, Vector center);

#endif