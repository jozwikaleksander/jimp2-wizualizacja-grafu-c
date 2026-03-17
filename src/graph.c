#include "graph.h"
#include <string.h>
#include <stdio.h>
#define EDGELIST_SIZE 16
#define NODELIST_SIZE 16

int is_potentially_planar(Graph *graph) {
    if (graph->num_nodes < 3) return 0;
    return graph->num_edges <= 3 * graph->num_nodes - 6;
}

/** 
* @brief Dodaje nowa krawedz do grafu
* @param graph - wskaznik na strukture grafu
* @param u - indeks wierzcholka poczatkowego
* @param v - indeks wierzcholka koncowego
* @param weight - waga krawedzi
* @param name - etykieta krawedzi
*/
void add_edge(Graph *graph, int u, int v, double weight, char *name) {
    // Sprawdzenie, czy należy powiększyć miejsce
    if(graph->num_edges >= graph->capacity_edges) {
        graph->capacity_edges *= 2;
        graph->edges = realloc(graph->edges,graph->capacity_edges * sizeof(Edge));

        if(graph->edges == NULL) {
            fprintf(stderr, "BŁĄD: Nie udało się zrealokować pamięci.\n");
            exit(7);
        }
    }

    // Tworzenie nowej krawędzi
    Edge *new_edge = &graph->edges[graph->num_edges];
    new_edge->u = u;
    new_edge->v = v;
    new_edge->weight = weight;
    new_edge->name = strdup(name);
    graph->num_edges++;
}

/**
 * @brief Funkcja sprawdza czy wierzchołek o podanym identyfikatorze istnieje w tablicy wierzchołków
 * @param graph  - wskaźnik na strukturę grafu
 * @param index  - indeks wierzchołka
 * @return int - 1 jeżeli znaleziono, 0 jeżeli nie znaleziono
 */
int find_node(Graph *graph, int index) {
    for(int i = 0; i < graph->num_nodes; i++)
        if(graph->nodes[i].id == index)
            return 1;
    return 0;
}

/**
 * @brief Funkcja dodaje wierzcholek do tablicy wierzcholkow
 * @param graph - wskaznik na strukture grafu
 * @param index - indeks wierzcholka
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
*/
void add_node(Graph *graph, int index, int width, int height){
    if(graph->num_nodes >= graph->capacity_nodes) {
        graph->capacity_nodes *= 2;
        graph->nodes = realloc(graph->nodes,graph->capacity_nodes * sizeof(Node));

        if(graph->nodes == NULL) {
            fprintf(stderr, "BŁĄD: Nie udało się zrealokować pamięci.\n");
            exit(7);
        }
    }

    // Tworzenie nowego wierzchołka
    Node *new_node = &graph->nodes[graph->num_nodes];
    new_node->position.x = rand() % (width + 1);
    new_node->position.y = rand() % (height + 1);
    new_node->force.x = 0;
    new_node->force.y = 0;
    new_node->id = index;
    graph->num_nodes++;
}

/**
 * @brief Funkcja tworzy graf na podstawie wczytanego pliku
 * @param graph_file - wskaznik do pliku
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
 * @return wskaznik na stworzony graf
*/
Graph *load_graph(FILE *graph_file, int width, int height) {
    if(!graph_file) return NULL;
    
    Graph *graph = malloc(sizeof(Graph));
    if(!graph) return NULL;

    graph->edges = malloc(EDGELIST_SIZE * sizeof(Edge));
    graph->nodes = malloc(NODELIST_SIZE * sizeof(Node));

    if(!graph->edges){
        free(graph);
        return NULL;
    }

    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->capacity_edges = EDGELIST_SIZE;
    graph->capacity_nodes = NODELIST_SIZE;

    if(!graph->edges){
        free(graph);
        return NULL;
    }

    char buff[256];
    int u = 0;
    int v = 0;
    double weight = 0.0;
    while(fscanf(graph_file, "%s %d %d %lf", buff, &u, &v, &weight) == 4) {
        if(find_node(graph, u) == 0)
            add_node(graph, u, width, height);
        if(find_node(graph, v) == 0)
            add_node(graph, v, width, height);

        add_edge(graph, u, v, weight, buff);
    }

    if(is_potentially_planar(graph) != 0) {
        printf("BŁĄD: Graf nie jest planarny.\n");
        free_graph(graph);
        exit(5);
        return NULL;
    }

    return graph;
}

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
*/
void free_graph(Graph *graph) {
    if(graph) {
        for(int i = 0; i < graph->num_edges; i++) {
            free(graph->edges[i].name);
        } 

        free(graph->edges);
        free(graph->nodes);
        free(graph);
    }
}