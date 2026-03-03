#include "vector.h"
#include <string.h>
#include <stdio.h>
#include <math.h>


Vector add_vectors (Vector a, Vector b){ //dodajemy b do a i wrócamy zmieniony a
    a.x += b.x;
    a.y += b.y;
    return a;
}

double count_length(Vector a){
    return sqrt(pow(a.x,2) + pow(a.y,2));
}

double distance (Vector a, Vector b){
    return sqrt(pow(b.x-a.x,2)+pow(b.y-a.y,2));
}

Vector vector_from_points(Vector a, Vector b){ //daje vector z 2 punktów    OD A(POCZĄTEK) DO B(KONIEC)
    Vector new.x=b.x-a.x;
    Vector new.y=b.y-a.y;
    return new;
}

Vector mult_by_num(Vector a, double n){
    a.x *=n;
    a.y *=n;
    return a;
}