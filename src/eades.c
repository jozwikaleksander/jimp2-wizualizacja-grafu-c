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
    
    double max_force =1;
    while(t<max_iterations && max_force>minimum_force){
        max_force = 0; 
        for(int i = 0; i<graph->num_nodes; i++){// iteracja po każdemu wierzchołku dla resetowania sił (żeby siły nie gromadzili się)

            graph->nodes[i].force.x = 0;
            graph->nodes[i].force.y = 0;
        }
        for(int i = 0; i<graph->num_nodes; i++){// iteracja po każdej parze wierzchołków -> obliczenie sił odpychania
            
            for(int j = i+1; j<graph->num_nodes;j++){
                Node *u = &graph->nodes[i];
                Node *v = &graph->nodes[j];
                compute_repulive(u,v,c, cooling);
                cooling = cooling*cooling; // w każdej iteracji cooling zmniesza się
                }}

        for(int i = 0;i<graph->num_edges; i++){//   iteracja po krawiędziach -> obliczenie sił przyciągania
            Edge *e = &graph->edges[i];
            cooling =0.97;
            compute_attract(graph,e, spring_const,ideal_len, cooling);
            cooling = cooling*cooling; // w każdej iteracji cooling zmniesza się
        }
        for(int i = 0; i<graph->num_nodes; i++){// // iteracja po każdemu wierzchołku -> znaleznienie maksymalnej siły
            
            double len_of_node = count_vector_length(graph->nodes[i].force);
                if(len_of_node>max_force){ 
                    max_force = len_of_node;
                }
            }
            
        
        t++;
    }

}

/**
 * @brief Funkcja podliczenia sił odchyłenia
 * @param u - wskaźnik na pierwszy wierzchołek
 * @param v - wskaźnik na drugi wierzchołek
 * @param c - używana do obliczenia sił odpychania
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwalnia pracę sił
*/
void compute_repulive(Node *u, Node *v, int c, double cooling){
    Vector repulsive; //vector siły odpychania
    double d=distance(u->position,v->position);
    if(d<0.0001){ //zapobieganie nachodzenia wierzchołków na siebie
        d= 0.0001;
    }
    double rep_num_part = c/pow(d,2); //część numeryczna siły odpychania
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector rep_unit_part;//część vectorowa siły odpychania
    rep_unit_part.x =unit_vect.x/d ;
    rep_unit_part.y =unit_vect.y/d ;
    repulsive = mult_by_num(rep_unit_part, rep_num_part); //końcowa wartość siły odpychania
    u->force = add_vectors(u->force, mult_by_num(repulsive, cooling)); // zmieniamy siłę wierzchołku siłą odpychania
    v->force = add_vectors(v->force, mult_by_num(negative(repulsive), cooling)); 
}

/**
 * @brief Funkcja podliczenia sił przyciągania
 * @param graph - wskaźnik na graf
 * @param e - wskaźnik na krawędź
 * @param spring_const -  używane do obliczenia sił przyciągania
 * @param ideal_len - długość do której dążą krawędzie
 * @param cooling - współczynnik "ochłodzenia" - stopniowo zwalnia pracę sił
*/
void compute_attract(Graph *graph, Edge *e, double spring_const, double ideal_len, double cooling){
    Node *u = &graph->nodes[e->u]; //2 wierzchołki nałeżające do tej krawiędzi
    Node *v = &graph->nodes[e->v];

    Vector attractive; // vector siły przyciągania

    double d=distance(u->position,v->position);
    if(d<0.0001){//zapobieganie nachodzenia wierzchołków na siebie
        d= 0.0001;
    }
    
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector attr_unit_part;//część vectorowa siły przyciągania
    attr_unit_part.x =unit_vect.x/d ;
    attr_unit_part.y =unit_vect.y/d ;
    double attr_num_part = spring_const*log(d/ideal_len); // numeryczna część siły przyciągania
    attractive = mult_by_num(attr_unit_part,attr_num_part);  //końcowa wartość siły przyciągania
    u->force = add_vectors(u->force, mult_by_num(attractive, cooling)); // zmieniamy siłę wierzchołku siłą przyciągania
    v->force = add_vectors(v->force, mult_by_num(negative(attractive), cooling)); 
    

}