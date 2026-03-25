#ifndef _TUTTES_H_
#define _TUTTES_H_

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "vector.h"

#define MINIMUM_CHANGE 0.01

/**
 * @brief Główna funkcja implementująca algorytm Tutte'a (Barycentric Method).
 * @param graph Wskaźnik na strukturę grafu.
 * @param max_iterations Maksymalna liczba iteracji algorytmu.
 * @return EXIT_SUCCESS lub kod błędu.
 */
int tuttes_algorithm(Graph *graph, int max_iterations) ;

/**
 * @brief Oblicza nową pozycję pojedynczego wierzchołka w oparciu o pozycje jego sąsiadów.
 * @param graph Wskaźnik na strukturę grafu.
 * @param iteration Indeks aktualnie przetwarzanego wierzchołka.
 * @param is_fixed Tablica określająca, czy wierzchołek jest unieruchomiony.
 * @param adj_list Lista sąsiedztwa grafu.
 * @param new_pos Tablica wynikowa dla nowych pozycji.
 * @param deg Tablica stopni wierzchołków.
 * @param center Środek obszaru rysowania (używany dla wierzchołków stopnia 1).
 * @return EXIT_SUCCESS w przypadku powodzenia.
 */
int tutte_iteration(Graph *graph,int iteration, int is_fixed[], uint** adj_list, Vector new_pos[],  int deg[], Vector center);
/**
 * @brief Sprawdza, czy dwa punkty znajdują się w tym samym miejscu z określoną dokładnością.
 * @param a Pierwszy wektor pozycji.
 * @param b Drugi wektor pozycji.
 * @return 1 jeśli pozycje są identyczne (w granicach błędu), 0 w przeciwnym razie.
 */
int have_same_pos(Vector a, Vector b);

/**
 * @brief Rozsuwa dwa wierzchołki o zadany dystans w losowym kierunku.
 * @param new_pos Tablica z pozycjami wierzchołków.
 * @param iteration Indeks pierwszego wierzchołka.
 * @param jiteration Indeks drugiego wierzchołka.
 * @param difference Dystans, o jaki punkty mają zostać rozunięte.
 */
void offset(Vector new_pos[], int iteration,int jiteration, double difference);

#endif
