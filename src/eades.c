#include "eades.h"
#include "graph.h"

void eades_algorithm(Graph *graph){
    int t = 0;
    
    double max_force =1;
    while(t<MAX_ITERATIONS && max_force>MINIMUM_FORCE){
        max_force = 0; 
        for(int i = 0; i<graph->num_nodes; i++){
            for(int j = i+1; j<graph->num_nodes;j++){
                Node *u = graph->nodes[i];
                Node *v = graph->nodes[j];
                compute_repulive(u,v);
                
                



            }

            len_of_node = count_vector_length(graph->nodes[i].force);
            if(len_of_node>max_force){
                max_force = len_of_node;
            }
            
        }
        t++;
    }

}

void compute_repulive(Node *u, Node *v){
    Vector repulsive; //to będzie  vector siły
    double d=distance(u->position,v->position);
    double rep_num_part = c/pow(d,2); //go część numeryczna
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector rep_unit_part;
    rep_unit_part.x =unit_vect.x/d ;//go część vectorowa
    rep_unit_part.y =unit_vect.y/d ;
    //Vector direction = vector_from_points(u,v);
    repulsive = mult_by_num(rep_unit_part, rep_num_part); // final repulsive
    u.force = add_vectors(u.force, repulsive);
    v.force = add_vectors(v.force, negative(repulsive)); //?
}
void compute_attract(Node u, Node v){}