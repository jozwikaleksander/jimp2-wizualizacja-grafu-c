#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "stdio.h"
#include "stdlib.h"

typedef struct {
    double x;
    double y;
} Vector;

Vector add_vectors (Vector a, Vector b); //zwraca a 
double count_vector_length(Vector a); // liczy po formule żeby porównywać vector siły z MINIMUM_FORCE
double distance (Vector a, Vector b); //Euclidean distance(odłegłość)
Vector vector_from_points(Vector a, Vector b); //daje vector z 2 punktów
Vector mult_by_num(Vector a, double n);//mnoży przez n i zwraca zmieniony n;