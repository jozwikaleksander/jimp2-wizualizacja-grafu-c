#include "graph.h"
#include <string.h>
#include <stdio.h>
#define EDGELIST_SIZE 16

EdgeList *create_edge_list() {
    EdgeList *list = malloc(sizeof(EdgeList));
    if(!list) return NULL;

    list->num_edges = 0;
    list->capacity = EDGELIST_SIZE;
    list->edges = malloc(EDGELIST_SIZE * sizeof(Edge));
    return list;
}

void add_edge(EdgeList *list, int u, int v, double weight, char *name) {
    // Sprawdzenie, czy należy powiększyć miejsce
    if(list->num_edges >= list->capacity) {
        list->capacity *= 2;
        list->edges = realloc(list->edges,list->capacity * sizeof(Edge));

        if(list->edges == NULL) {
            fprintf(stderr, "BŁĄD: Nie udało się zrealokowac pamięci.\n");
            exit(1);
        }
    }

    // Tworzenie nowej krawędzi
    list->edges[list->num_edges].u = u;
    list->edges[list->num_edges].v = v;
    list->edges[list->num_edges].weight = weight;
    list->edges[list->num_edges].name = strdup(name);
    list->num_edges++;
}

void free_graph(Graph *graph) {
    if(graph) {
        for(int i = 0; i < graph->edge_list->num_edges; i++) {
            free(graph->edge_list->edges[i].name);
        } 

        free(graph->edge_list);
        free(graph);
    }
}

Graph *load_graph(FILE *graph_file) {
    if(!graph_file) return NULL;
    
    Graph *graph = malloc(sizeof(Graph));
    if(!graph) return NULL;

    graph->edge_list = create_edge_list();

    if(!graph->edge_list){
        free(graph);
        return NULL;
    }

    char buff[256];
    int u = 0;
    int v = 0;
    double weight = 0.0;
    while(fscanf(graph_file, "%s %d %d %lf", buff, &u, &v, &weight) == 1) {
        add_edge(graph->edge_list, u, v, weight, buff);
    }
    return graph;
}