#include "graph.h"
#include <string.h>
#include <stdio.h>
#define EDGELIST_SIZE 16

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
    new_edge.u = u;
    new_edge.v = v;
    new_edge.weight = weight;
    new_edge.name = strdup(name);
    graph->num_edges++;
}

void free_graph(Graph *graph) {
    if(graph) {
        for(int i = 0; i < graph->num_edges; i++) {
            free(graph->edges[i].name);
        } 

        free(graph->edges);
        free(graph);
    }
}

Graph *load_graph(FILE *graph_file) {
    if(!graph_file) return NULL;
    
    Graph *graph = malloc(sizeof(Graph));
    if(!graph) return NULL;

    graph->edges = malloc(EDGELIST_SIZE * sizeof(Edge));

    if(!graph->edges){
        free(graph);
        return NULL;
    }

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
        add_edge(graph, u, v, weight, buff);
    }
    return graph;
}