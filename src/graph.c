#include "graph.h"
#include <string.h>
#include <stdio.h>
#define EDGELIST_SIZE 16


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
            fprintf(stderr, "BŁĄD: Nie udało się zrealokowac pamięci.\n");
            exit(1);
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
 * @brief Funkcja zwieksza rozmiar dynamicznej tablicy wierzcholkow
 * @param graph - wskaznik na strukture grafu
 * @param max_index - maksymalny indeks wierzcholka
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
*/
void resize_nodes(Graph *graph, int max_index, int width, int height) {
    if(max_index >= graph->num_nodes){
        int old_amount = graph->num_nodes;

        graph->num_nodes = max_index + 1;
        graph->nodes = realloc(graph->nodes, graph->num_nodes * sizeof(Node));

        for(int i = old_amount; i < graph->num_nodes; i++) {
            graph->nodes[i].position.x = rand() % (width + 1);
            graph->nodes[i].position.y = rand() % (height + 1);;
            graph->nodes[i].force.x = 0;
            graph->nodes[i].force.y = 0;
        }
    }
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

    if(!graph->edges){
        free(graph);
        return NULL;
    }

    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->capacity_edges = EDGELIST_SIZE;

    if(!graph->edges){
        free(graph);
        return NULL;
    }

    char buff[256];
    int u = 0;
    int v = 0;
    double weight = 0.0;
    while(fscanf(graph_file, "%s %d %d %lf", buff, &u, &v, &weight) == 4) {
        int max_index = (u > v) ? u : v;

        resize_nodes(graph, max_index-1, width ,height);

        add_edge(graph, u, v, weight, buff);
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