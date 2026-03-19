#include "validation.h"
#include <stdbool.h>
#include "error.h"
#include <planarity/graph.h>

/**
 * @brief Funkcja, sprawdza czy podany graf spełnia warunek konieczny planarności.
 * @param graph - wskaźnik na strukturę grafu
 * @return int - 1 jeżeli spełnia warunek konieczny
 */
int is_potentially_planar(Graph *graph) {
    if (graph->num_nodes < 3) return 1;
    return graph->num_edges <= 3 * graph->num_nodes - 6;
}

/**
 * @brief Funkcja znajdująca korzeń drzewa
 * @param parent 
 * @param i 
 * @return int 
 */
int find_root(int *parent, int i) {
    if(parent[i] == i) { // Jeżeli znaleźliśmy korzeń to go zwracamy
        return i;
    }
    return parent[i] = find_root(parent, parent[i]); // Jeżeli nie to szukamy dalej idąc w górę
}

/**
 * @brief Łączy dwa rozłączne zbiory (wierzchołki x i y) w jeden zbiór.
 * * Funkcja wykorzystuje optymalizację "Union by Rank" (łączenie według rangi),
 * aby zminimalizować wysokość drzew w strukturze. Mniejsze drzewo jest zawsze
 * dołączane jako poddrzewo korzenia większego drzewa.
 * @param parent Tablica reprezentująca rodziców poszczególnych wierzchołków.
 * @param rank Tablica przechowująca rangi (przybliżone wysokości) drzew.
 * @param x Indeks pierwszego wierzchołka do połączenia.
 * @param y Indeks drugiego wierzchołka do połączenia.
 */
void union_sets(int *parent, int *rank, int x, int y) {
    int root_x = find_root(parent, x);
    int root_y = find_root(parent, y);

    // Łączymy tylko wtedy, gdy wierzchołki są w różnych zbiorach
    if (root_x != root_y) {
        // Dołączamy niższe drzewo pod wyższe, aby zminimalizować głębokość
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            // Jeśli rangi są równe, wybieramy jeden korzeń i zwiększamy jego rangę
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
}

/**
 * @brief Funkcja sprawdza czy graf jest spójny
 * @return int - zwraca 1 jeżeli spójny, 0 jeżeli niespójny i -1 w przypadku błędu alokacji
**/
int is_graph_connected(Graph *graph) {
    // Puste grafy lub grafy z 1 wierzchołkiem są z definicji spójne
    if(graph->num_nodes <= 1)
        return 1;

    int *parent = (int *)malloc(graph->num_nodes * sizeof(int));
    int *rank = (int *)calloc(graph->num_nodes, sizeof(int));

    if(parent == NULL || rank == NULL) {
        printf("BŁĄD: Nie udało się zaalokować pamięci.\n");
        return -1;
    }

    // Inicjalizacja. Na początku każdy wierzchołek jest niepołączoną wyspą.
    for(int i = 0; i < graph->num_nodes; i++) {
        parent[i] = i;
    }

    int successful_unions = 0;

    for(int i = 0; i < graph->num_edges; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;

        // Sprawdzanie czy indeksy wierzchołków mieszczą się w odpowiednim zakresie
        if (u >= 0 && u < graph->num_nodes && v >= 0 && v < graph->num_nodes) {
            int root_u = find_root(parent, u);
            int root_v = find_root(parent, v);

            if(root_u != root_v) {
                union_sets(parent,rank, root_u, root_v);
                successful_unions++;
            }
        }
    }
    // Dealokacja pamięci
    free(parent);
    free(rank);

    // Graf jest spójny, jeżeli udało się wykonać V-1 połączeń łączących różne zbiory.
    return successful_unions == (graph->num_nodes - 1);
}

/**
 * @brief Funkcja, sprawdza jaki indeks w tabeli nodes ma wierzchołek o podanym identyfikatorze
 * 
 * @param graph - wskaźnik na strukturę grafu
 * @param node_id - identyfikator wierzchołka
 * @return int 
 */
int find_node_index(Graph *graph, uint node_id) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i].id == node_id) {
            return i;
        }
    }
    return -1;
}

int is_graph_planar(Graph *graph) {
    graphP gp = gp_New();

    if (gp_InitGraph(gp, graph->num_nodes) != OK) {
        gp_Free(&gp);
        return false; 
    }

    // Dodawanie krawędzi
    for(int i = 0; i < graph->num_edges; i++) {
        int index_u = find_node_index(graph, graph->edges[i].u);
        int index_v = find_node_index(graph, graph->edges[i].v);

        if(index_u != -1 && index_v != -1) { // Sprawdzenie czy znaleziono indeksy
            gp_AddEdge(gp, index_u, 0, index_v, 0);        
        }
    }

    // Uruchomienia funkcji dla sprawdzanie planarności z biblioteki LibPlanarity
    int is_planar = (gp_Embed(gp, EMBEDFLAGS_PLANAR) == OK);

    // Zwalnianie pamięci
    gp_Free(&gp);
    return is_planar;
}

int is_graph_valid(Graph *graph){
    if(is_potentially_planar(graph) == 0)
        return ERR_GRAPH_NOT_PLANAR;
    else if(is_graph_connected(graph) == 0){
        return ERR_GRAPH_NOT_CONNECTED;
    }
    else if(is_graph_planar(graph) == 0){
        return ERR_GRAPH_NOT_PLANAR;
    }
    return 0;
}