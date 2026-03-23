#include "graph.h"
#include <math.h>
#include "vector.h"

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
 * @brief Funkcja zwalnia miejsca zaalokowane dla struktury grafu
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
/**
 * @brief Funkcja drukuje listę sąsiedstwa na stdout
 * @param graph - wskaznik do pliku
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 */

void print_adj_list(Graph *graph, uint **adj_list, int *deg) {
    for (int u = 0; u < graph->num_nodes; u++) {
        printf("Sąsiedzi dla %d: ", u + 1);
        for (int i = 0; i < deg[u]; i++)
            printf("%d ", adj_list[u][i] + 1);
        printf("\n");
    }
}
/**
 * @brief Funkcja zwalnia listę sąsiedstwa
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 */
void free_adj_list(Graph *graph, uint **adj_list) {
    for (int i = 0; i < graph->num_nodes; i++)
        free(adj_list[i]);
    free(adj_list);
}
/**
 * @brief Funkcja zwalnia podaną listę ilości sąsiadów
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 */
void free_deg(int *deg) {
    if (!deg)
        return;
    free(deg);
}
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
void dfs_rec(Graph *graph, uint **adj_list, int *deg, int *idx, int start,
             int visited[], int dfs_res[]) {
    //Lista typu: 
    //[0]:1
    //[1]:1
    //[2]:0
    //gdzie [x] wieszchołek pod indeksem x 0 - nie zwiedzony; 1- zwiedzony
    visited[start] = 1;

    //Lista przechowywająca indeksy zwiedonych wieszchołków - przyjmuje znaczenie start
    dfs_res[(*idx)++] = start;
    
    //Iteracja po wieszchołkach w adj_list
    for (int i = 0; i < deg[start]; i++) {
        // Jeden z sąsiadów i-tego wierzchołku
        int neighbor = adj_list[start][i];
        //Jeśli sąsiad nie zwidzony to rekurencyjnie wywylamy funkcje, jako start podajemy idx
        if (visited[neighbor] == 0)
            dfs_rec(graph, adj_list, deg, idx, neighbor, visited, dfs_res);
    }
}
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
                  int visited[], int parent[], int cycle_res[], int *cycle_idx, int *found) {
    //Jeśli już znalezliśmy cykl to 
    if (*found) return EXIT_SUCCESS; 

    visited[current] = 1; 
    //printf("DFS teraz w  %d\n", current); pokazuje terazniejszą pozycję algorytmu, nie równo temu co w dfs_res
    

    //Iteracja po wieszchołkach w adj_list
    for (int i = 0; i < deg[current]; i++) {
        int neighbor = adj_list[current][i];
        //Jeśli sąsiad jescze nie zwiedzony - przechodzimy dalej i rekurencyjnie zamykamy funkcję
        if (visited[neighbor] == 0) { 
            parent[neighbor] = current;
            dfs_rec_face(graph, adj_list, deg, idx, neighbor, visited, parent, cycle_res, cycle_idx, found);
            if (*found) return EXIT_SUCCESS;
        } 
        //Jeśli sąsiad nie przodek i już zwiedzony, to znalezliśmy cykł i wracamy do początku
        else if (neighbor != parent[current] && visited[neighbor] == 1) {
            *found = 1;
            int v = current;
            while (v != neighbor && v != -1) {
                cycle_res[(*cycle_idx)++] = v;
                //Wracanie po poprzednikach 
                v = parent[v];
                //printf("%d ",v); Pokaże  po jakich wieszchołkach wrócamy się, to samo co dfs res, tylko naodwrót
                //printf("\n");             
                }
            cycle_res[(*cycle_idx)++] = neighbor;
            return EXIT_SUCCESS;
        }
    }
    visited[current] = 2; 
    return EXIT_SUCCESS;
    
}
/**
 * @brief Funkcja dla znaleznienia zewnętrznego poligonu(dfs)
 * @param graph - wskaznik na strutkure grafu
 * @param adj_list - wskaznik na listę sąsiedstwa do wydrukowania 
 * @param deg - lista z ilościami sąsiadów dla każdego wieszchołku
 * @param cycle_res - ilość zwiedzonych wierzchołków w cyklu zwracania
 * @param cycle_idx - terazniejsza pozycja w cyklu zwracania

 */
void find_outer_face(Graph *graph, uint **adj_list, int *deg, int cycle_res[], int *cycle_idx) {
    //Inicjalizacja narzędzi
    int n = graph->num_nodes;
    int visited[n];
    int parent[n];
    int found = 0;
    int start = 0;

    //Zapełniamy zerami visited[], czyli "nic nie zwiedzono"; -1 w parent[] znaczy że ten wieszchołek nie ma rodzica
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }
    *cycle_idx = 0;


    dfs_rec_face(graph, adj_list, deg, &start, 0, visited, parent, cycle_res, cycle_idx, &found);
}
/**
 * @brief Funkcja dla drukowania indeksów wierzchołków zewnętrznego poligonu
 * @param dfs_res - list z resultatem dfs
 * @param dfs_res_size - pomiar tej listy

 */
void print_outer_face( int dfs_res[], int dfs_res_size){

    for (int i = 0; i<dfs_res_size; i++){
        printf( "%d", dfs_res[i]);
        printf("->");
    }
    printf( "%d", dfs_res[0]);
    printf("\n");



}

/**
 * @brief Znajduje środek na przestrzeni 
 * @param graph - graph na przestrzeni którego szukamy środek
 * @return center - Vector z współżędnymi środku
 */
Vector get_center (Graph *graph){
    Vector center;
    center.x = round((graph->width)/2);
    center.y = round((graph->height)/2);
    return center;


}
/**
 * @brief Funkcja dla rozłożenia wierzchołków po okręgu odnośnie podanego centrum
 * @param outer_faces - lista wirzchołków z zewnętrnego poligonu
 * @param graph - wskaznik na strutkure grafu
 * @param k - ilość wierzchołków do rozstawienia
 * @param center - vector z współżędnymi środku

 */

void place_on_circle(int outer_faces[], Graph *graph,int k, Vector center){
    //margin to odległość krańca okręga od granicy przestrzeni
    int margin = 20;
    //radius- promień domyślego okręga na którym rozskładamy wierzchołki
    double radius = fmin(graph->width, graph->height) / 2.0 - margin;

    //Iteracja po wierzchołkach zewnętrznego poligonu
    for (int i = 0; i<k; i++){
        
        //Kąt pomiędzy kolejnymi wierzchołkami
        double angle = 2.0 * M_PI * i / k;

        //Zmiana pozycji wieszchołków
        graph->nodes[outer_faces[i]].position.x = center.x + radius * cos(angle);
        graph->nodes[outer_faces[i]].position.y = center.y + radius * sin(angle);


    }
    

}

