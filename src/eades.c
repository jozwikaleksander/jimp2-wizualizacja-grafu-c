#include "eades.h"
#include "graph.h"

void eades_algorithm(Graph *graph, 
    double minimum_force, int max_iterations, 
    double ideal_len, double spring_const, int c, double cooling)
{
    int t = 0;
    
    double max_force =1;
    while(t<max_iterations && max_force>minimum_force){
        max_force = 0; 
        for(int i = 0; i<graph->num_nodes; i++){// ITERACJA PO każdemu

            graph->nodes[i].force.x = 0;
            graph->nodes[i].force.y = 0;
        }
        for(int i = 0; i<graph->num_nodes; i++){// ITERACJA PO PARACH
            
            for(int j = i+1; j<graph->num_nodes;j++){
                Node *u = &graph->nodes[i];
                Node *v = &graph->nodes[j];
                compute_repulive(u,v,c, cooling);
                cooling = cooling*cooling;
                }}

        for(int i = 0;i<graph->num_edges; i++){//   ITERACJA PO EDGES
            Edge *e = &graph->edges[i];
            cooling =0.97;
            compute_attract(graph,e, spring_const,ideal_len, cooling);
            cooling = cooling*cooling;
        }
        for(int i = 0; i<graph->num_nodes; i++){// ITERACJA PO KAŻDEMU
            
            double len_of_node = count_vector_length(graph->nodes[i].force);
                if(len_of_node>max_force){ 
                    max_force = len_of_node;
                }
            }
            
        
        t++;
    }

}

void compute_repulive(Node *u, Node *v, int c, double cooling){
    Vector repulsive; //to będzie  vector siły
    double d=distance(u->position,v->position);
    if(d<0.0001){
        d= 0.0001;
    }
    double rep_num_part = c/pow(d,2); //go część numeryczna
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector rep_unit_part;//go część vectorowa
    rep_unit_part.x =unit_vect.x/d ;
    rep_unit_part.y =unit_vect.y/d ;
    //Vector direction = vector_from_points(u,v);
    repulsive = mult_by_num(rep_unit_part, rep_num_part); // final repulsive
    u->force = add_vectors(u->force, mult_by_num(repulsive, cooling));
    v->force = add_vectors(v->force, mult_by_num(negative(repulsive), cooling)); 
}


void compute_attract(Graph *graph, Edge *e, double spring_const, double ideal_len, double cooling){
    Node *u = &graph->nodes[e->u];
    Node *v = &graph->nodes[e->v];

    Vector attractive;

    double d=distance(u->position,v->position);
    if(d<0.0001){
        d= 0.0001;
    }
    
    Vector unit_vect = vector_from_points(u->position,v->position); 
    Vector attr_unit_part;//go część vectorowa
    attr_unit_part.x =unit_vect.x/d ;
    attr_unit_part.y =unit_vect.y/d ;
    double attr_num_part = spring_const*log(d/ideal_len);
    attractive = mult_by_num(attr_unit_part,attr_num_part);
    u->force = add_vectors(u->force, mult_by_num(attractive, cooling));
    v->force = add_vectors(v->force, mult_by_num(negative(attractive), cooling)); 
    

}