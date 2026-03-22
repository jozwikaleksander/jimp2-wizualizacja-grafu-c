#include "graph.h"

#define EDGELIST_SIZE 16
#define NODELIST_SIZE 16

/**
 * @brief Dodaje nowa krawedz do grafu
 * @param graph - wskaznik na strukture grafu
 * @param u - indeks wierzcholka poczatkowego
 * @param v - indeks wierzcholka koncowego
 * @param weight - waga krawedzi
 * @param name - etykieta krawedzi
 */
int add_edge(Graph *graph, int u, int v, double weight, char *name) {
    // Sprawdzenie, czy należy powiększyć miejsce
    if (graph->num_edges >= graph->capacity_edges) {
        size_t new_capacity = graph->capacity_edges * 2;
        Edge *new_edges = realloc(graph->edges, new_capacity * sizeof(Edge));

        if (new_edges == NULL) {
            return ERR_MEMORY_ALLOC;
        }
        graph->edges = new_edges;
        graph->capacity_edges = new_capacity;
    }

    // Tworzenie nowej krawędzi
    Edge *new_edge = &graph->edges[graph->num_edges];
    new_edge->u = u;
    new_edge->v = v;
    new_edge->weight = weight;
    new_edge->name = strdup(name);
    graph->num_edges++;
    return 0;
}

/**
 * @brief Funkcja sprawdza czy wierzchołek o podanym identyfikatorze istnieje w
 * tablicy wierzchołków
 * @param graph  - wskaźnik na strukturę grafu
 * @param index  - identyfikator wierzchołka
 * @return int - indeks w tablicy jeżeli znaleziono, -1 jeżeli nie znaleziono
 */
int find_node(Graph *graph, int index) {
    for (int i = 0; i < graph->num_nodes; i++)
        if (graph->nodes[i].id == index)
            return i;
    return -1;
}

/**
 * @brief Funkcja dodaje wierzcholek do tablicy wierzcholkow
 * @param graph - wskaznik na strukture grafu
 * @param index - identyfikator wierzcholka
 * @param width - szerokosc obszaru, w ktorym wyswietlony bedzie graf
 * @param height - wysokosc obszaru, w ktorym wyswietlony bedzie graf
 */
int add_node(Graph *graph, uint index, int width, int height) {
    if (graph->num_nodes >= graph->capacity_nodes) {
        size_t new_capacity = graph->capacity_nodes * 2;
        Node *new_nodes = realloc(graph->nodes, new_capacity * sizeof(Node));

        if (new_nodes == NULL) {
            return ERR_MEMORY_ALLOC;
        }
        graph->nodes = new_nodes;
        graph->capacity_nodes = new_capacity;
    }

    // Tworzenie nowego wierzchołka
    Node *new_node = &graph->nodes[graph->num_nodes];
    new_node->position.x = rand() % (width + 1);
    new_node->position.y = rand() % (height + 1);
    new_node->force.x = 0;
    new_node->force.y = 0;
    new_node->id = index;
    graph->num_nodes++;
    return 0;
}

Graph *load_graph(FILE *graph_file, int width, int height, int *out_code) {
    if (!graph_file) {
        *out_code = ERR_FILE_OPEN;
        return NULL;
    }

    Graph *graph = malloc(sizeof(Graph));
    if (!graph) {
        *out_code = ERR_MEMORY_ALLOC;
        return NULL;
    };

    graph->edges = malloc(EDGELIST_SIZE * sizeof(Edge));
    graph->nodes = malloc(NODELIST_SIZE * sizeof(Node));

    if (!graph->edges || !graph->nodes) {
        free(graph->edges);
        free(graph->nodes);
        free(graph);
        *out_code = ERR_MEMORY_ALLOC;
        return NULL;
    }

    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->capacity_edges = EDGELIST_SIZE;
    graph->capacity_nodes = NODELIST_SIZE;

    char buff[256];
    int u = 0;
    int v = 0;
    double weight = 0.0;
    while (fscanf(graph_file, "%s %d %d %lf", buff, &u, &v, &weight) == 4) {
        int u_idx = find_node(graph, u);
        if (u_idx == -1) {
            if (add_node(graph, u, width, height) == ERR_MEMORY_ALLOC) {
                free_graph(graph);
                *out_code = ERR_MEMORY_ALLOC;
                return NULL;
            }
            u_idx = graph->num_nodes - 1;
        }

        int v_idx = find_node(graph, v);
        if (v_idx == -1) {
            if (add_node(graph, v, width, height) == ERR_MEMORY_ALLOC) {
                free_graph(graph);
                *out_code = ERR_MEMORY_ALLOC;
                return NULL;
            }
            v_idx = graph->num_nodes - 1;
        }

        if (add_edge(graph, u_idx, v_idx, weight, buff) == ERR_MEMORY_ALLOC) {
            free_graph(graph);
            *out_code = ERR_MEMORY_ALLOC;
            return NULL;
        }
    }
    return graph;
}

/**
 * @brief Funkcja zwalnie miejsca zaalokowane dla struktury grafuu
 * @param graph - wskaznik na strutkure grafu
 */
void free_graph(Graph *graph) {
    if (graph) {
        for (int i = 0; i < graph->num_edges; i++) {
            free(graph->edges[i].name);
        }

        free(graph->edges);
        free(graph->nodes);
        free(graph);
    }
}

/**
 * @brief Tworzy listę sąsiedstwa dla grafu
 * @param graph - wskaźnik na graf
 * @return lista sąsiedstwa
 */

int build_adj_list(Graph *graph, uint **adj_list, int *deg) {
    int n_nodes = graph->num_nodes;
    int n_edges = graph->num_edges;

    // Zapełnienie deg
    for (int i = 0; i < n_nodes; i++)
        deg[i] = 0;

    // Zapełnienie adj_list
    for (int i = 0; i < n_edges; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;

        adj_list[u][deg[u]++] = v;
        adj_list[v][deg[v]++] = u;
    }
    return EXIT_SUCCESS;
}

void print_adj_list(Graph *graph, uint **adj_list, int *deg) {
    for (int u = 0; u < graph->num_nodes; u++) {
        printf("Sąsiedzi dla %d: ", u + 1);
        for (int i = 0; i < deg[u]; i++)
            printf("%d ", adj_list[u][i] + 1);
        printf("\n");
    }
}

void free_adj_list(Graph *graph, uint **adj_list) {
    for (int i = 0; i < graph->num_nodes; i++)
        free(adj_list[i]);
    free(adj_list);
}

void free_deg(int *deg) {
    if (!deg)
        return;
    free(deg);
}

void dfs_rec(Graph *graph, uint **adj_list, int *deg, int *idx, int start,
             int visited[], int dfs_res[]) {
    visited[start] = 1;
    
    dfs_res[(*idx)++] = start;
    

    for (int i = 0; i < deg[start]; i++) {
        
        int neighbor = adj_list[start][i];
        if (visited[neighbor] == 0)
            dfs_rec(graph, adj_list, deg, idx, neighbor, visited, dfs_res);
    }
}
void dfs_rec_face(Graph *graph, uint **adj_list, int *deg, int *idx, int current,
                  int visited[], int parent[], int cycle_res[], int *cycle_idx, int *found) {
    
    if (*found) return; 

    visited[current] = 1; 
    //printf("DFS teraz w  %d\n", current); pokzuje terazniejszą pozycję algorytmu, nie równo temu co w dfs_res
    


    for (int i = 0; i < deg[current]; i++) {
        int neighbor = adj_list[current][i];

        if (visited[neighbor] == 0) { 
            parent[neighbor] = current;
            dfs_rec_face(graph, adj_list, deg, idx, neighbor, visited, parent, cycle_res, cycle_idx, found);
            if (*found) return;
        } 
        else if (neighbor != parent[current] && visited[neighbor] == 1) {
            *found = 1;
            int v = current;
            while (v != neighbor && v != -1) {
                cycle_res[(*cycle_idx)++] = v;
                v = parent[v];
                //printf("%d ",v); Pokaże  po jakich wieszchołkach wrócamy się, to samo co dfs res, tylko naodwrót
                //printf("\n");             
                }
            cycle_res[(*cycle_idx)++] = neighbor;
            return;
        }
    }
    
    visited[current] = 2; 
}

void find_outer_face(Graph *graph, uint **adj_list, int *deg, int cycle_res[], int *cycle_idx) {
    int n = graph->num_nodes;
    int visited[n];
    int parent[n];
    int found = 0;
    int dummy_idx = 0;

    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }
    *cycle_idx = 0;


    dfs_rec_face(graph, adj_list, deg, &dummy_idx, 0, visited, parent, cycle_res, cycle_idx, &found);
}

void print_outer_face( int dfs_res[], int dfs_res_size){

    for (int i = 0; i<dfs_res_size; i++){
        printf( "%d", dfs_res[i]);
        printf("->");

    }
    printf( "%d", dfs_res[0]);


    printf("\n");



}

