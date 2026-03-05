#include "eades.h"
#include "graph.h"

void eades_algorithm(Graph *graph, 
    double minimum_force, int max_iterations, 
    int ideal_len, int c)
{
    int t = 0;
    
    double max_force =1;
    while(t<max_iterations && max_force>minimum_force){
        max_force = 0; 
        for(int i = 0; i<graph->num_nodes; i++){// ITERACJA PO PARACH
            for(int j = i+1; j<graph->num_nodes;j++){
                Node *u = graph->nodes[i];
                Node *v = graph->nodes[j];
                compute_repulive(u,v);
                }}




        for(int i = 0;i<graph->num_edges; i++){//   ITERACJA PO EDGES
            Edge *e = graph->edges[i];

            compute_attract(graph,e ,ideal_len);
        }
        for(int i = 0; i<graph->num_nodes; i++){// ITERACJA PO KAŻDEMU
            
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
    Vector rep_unit_part;//go część vectorowa
    rep_unit_part.x =unit_vect.x/d ;
    rep_unit_part.y =unit_vect.y/d ;
    //Vector direction = vector_from_points(u,v);
    repulsive = mult_by_num(rep_unit_part, rep_num_part); // final repulsive
    u.force = add_vectors(u.force, repulsive);
    v.force = add_vectors(v.force, negative(repulsive)); 
}


void compute_attract(Graph *graph, Edge *e,double spring_const, double ideal_len){
    Node *u = graph->nodes[e->u];
    Node *v = graph->nodes[e->v];

    Vector attractive;

    double d=distance(u->position,v->position);
    
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector attr_unit_part;//go część vectorowa
    attr_unit_part.x =unit_vect.x/d ;
    attr_unit_part.y =unit_vect.y/d ;
    double attr_num_part = spring_const*log(d/ideal_len);
    attractive = mult_by_num(attr_unit_part,attr_num_part);
    u.force = add_vectors(u.force, attractive);
    v.force = add_vectors(v.force, negative(attractive)); 
    

}