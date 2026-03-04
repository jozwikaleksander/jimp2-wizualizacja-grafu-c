#include "eades.h"
#include "graph.h"

void eades_algorithm(Graph *graph){
    int t = 0;
    double max_force = 0; 
    int c = 2; //zmieniamy przez linię??????!!!!!-------------------------
    while(t<MAX_ITERATIONS && max_force>MINIMUM_FORCE){
    for(int i = 0; i<graph->num_nodes; i++){
        for(int j = i+1; j<graph->num_nodes;j++){
            Vector u = graph->nodes[i];
            Vector v = graph->nodes[j];
            
            



        }
        len_of_node = count_vector_length(graph->nodes[i].force);
        if(len_of_node>max_force){
            max_force = len_of_node;
        }
        
        }
    }

}

void compute_repulive(Node u, Node v){
    Vector repulsive; //to będzie  vector siły
    double d=distance(u,v);
    double rep_num_part = c/pow(d,2); //go część numeryczna
    Vector unit_vect = vector_from_points(u,v); //go część vectorowa
    Vector rep_unit_part.x =u.x/d ;
    Vector rep_unit_part.y =u.y/d ;
    repulsive = mult_by_num(rep_unit_part, rep_num_part); // final repulsive
    u.force = add_vectors(u.force, repulsive);
    v.force = add_vectors(v.force, -repulsive); //?
}
void compute_attract(Node u, Node v){}