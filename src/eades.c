#include "eades.h"
#include "graph.h"

/**
 * @brief Funkcja algorytmu Eades do zmiany pozycji wierzchołków
 * @param graph - wskaznik na graf
 * @param minimum_force - minimalna dopuszczalna wartość siły
 * @param max_iterations - maksymalna ilość iteracji
 * @param ideal_len - do tej długości będą dążyć krawędzie
 * @param spring_const -  używane do obliczenia sił przyciągania
 * @param c - używana do obliczenia sił odpychania
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwolnia pracę sił
*/
void eades_algorithm(Graph *graph, 
    double minimum_force, int max_iterations, 
    double ideal_len, double spring_const, int c, double cooling)
{
    int t = 0;
    double max_force_magnitude = 1.0;
    double temperature = 1.0; // Współczynnik kroku, który maleje w czasie

    while(t < max_iterations && max_force_magnitude > minimum_force) {
        max_force_magnitude = 0.0;

        // 1. Wyzeruj siły dla bieżącej iteracji
        for(int i = 0; i < graph->num_nodes; i++) {
            graph->nodes[i].force.x = 0;
            graph->nodes[i].force.y = 0;
        }

        // 2. Oblicz siły odpychania między wszystkimi parami wierzchołków
        for(int i = 0; i < graph->num_nodes; i++) {
            for(int j = i + 1; j < graph->num_nodes; j++) {
                compute_repulive(&graph->nodes[i], &graph->nodes[j], c);
            }
        }

        // 3. Oblicz siły przyciągania wzdłuż krawędzi
        for(int i = 0; i < graph->num_edges; i++) {
            compute_attract(graph, &graph->edges[i], spring_const, ideal_len);
        }

        // 4. Zaktualizuj pozycje wierzchołków i znajdź maksymalną siłę
        for(int i = 0; i < graph->num_nodes; i++) {
            Node *node = &graph->nodes[i];
            double current_force_magnitude = count_vector_length(node->force);

            if(current_force_magnitude > max_force_magnitude) {
                max_force_magnitude = current_force_magnitude;
            }

            // Zastosuj siłę do pozycji wierzchołka, skalując przez temperaturę
            Vector displacement = mult_by_num(node->force, temperature);
            node->position = add_vectors(node->position, displacement);
        }

        // 5. "Schładzaj" system
        temperature *= cooling;
        t++;
    }
}

/**
 * @brief Funkcja do obliczenia sił odchyłenia
 * @param u - wskaźnik na pierwszy wierzchołek
 * @param v - wskaźnik na drugi wierzchołek
 * @param c - używana do obliczenia sił odpychania
 */
void compute_repulive(Node *u, Node *v, int c) {
    Vector repulsive; //vector siły odpychania
    double d = distance(u->position, v->position);
    if(d < 0.0001) { //zapobieganie dzieleniu przez zero
        d = 0.0001;
    }
    double rep_num_part = c / (d * d); //część numeryczna siły odpychania
    Vector direction_uv = vector_from_points(u->position, v->position); // Wektor v - u
    Vector unit_vec; // Wektor jednostkowy w kierunku od u do v
    unit_vec.x = direction_uv.x / d;
    unit_vec.y = direction_uv.y / d;
    repulsive = mult_by_num(unit_vec, rep_num_part); //końcowa wartość siły odpychania

    // Siła odpychająca na 'u' działa w kierunku przeciwnym do 'v'
    u->force = add_vectors(u->force, negative(repulsive));
    // Siła odpychająca na 'v' działa w kierunku przeciwnym do 'u'
    v->force = add_vectors(v->force, repulsive);
}

/**
 * @brief Funkcja obliczająca siłę przyciągania
 * @param graph - wskaźnik na graf
 * @param e - wskaźnik na krawędź
 * @param spring_const -  używane do obliczenia sił przyciągania
 * @param ideal_len - długość do której dążą krawędzie
 */
void compute_attract(Graph *graph, Edge *e, double spring_const, double ideal_len) {
    Node *u = &graph->nodes[e->u]; //2 wierzchołki nałeżające do tej krawiędzi
    Node *v = &graph->nodes[e->v];

    Vector attractive; // vector siły przyciągania

    double d = distance(u->position, v->position);
    if(d < 0.0001) { //zapobieganie dzieleniu przez zero
        d = 0.0001;
    }
    
    Vector direction_uv = vector_from_points(u->position, v->position); // Wektor v - u
    Vector unit_vec; // Wektor jednostkowy w kierunku od u do v
    unit_vec.x = direction_uv.x / d;
    unit_vec.y = direction_uv.y / d;
    double attr_num_part = spring_const * log(d / ideal_len); // numeryczna część siły przyciągania
    attractive = mult_by_num(unit_vec, attr_num_part);  //końcowa wartość siły przyciągania
    // Siła przyciągająca na 'u' działa w kierunku 'v'
    u->force = add_vectors(u->force, attractive);
    // Siła przyciągająca na 'v' działa w kierunku 'u'
    v->force = add_vectors(v->force, negative(attractive));
}