#include "vector.h"
#include <math.h>

/**
 * @brief Funkcja dodaje wektory
 * @param a - pierwszy wektor
 * @param b - drugi wektor
 * @return zmieniony pierwszy wektor
*/
Vector add_vectors (Vector a, Vector b){
    a.x += b.x;
    a.y += b.y;
    return a;
}

/**
 * @brief Funkcja oblicza dlugosc wektora
 * @param a - wektor
 * @return zwraca wartosc dlugosci wektora o typie double
*/
double count_length(Vector a){
    return sqrt(pow(a.x,2) + pow(a.y,2));
}

/**
 * @brief Funkcja oblicza odleglosc pomiedzy dwoma wektorami
 * @param a - pierwszy wektor
 * @param b - drugi wektor
 * @return zwraca wartosc odleglosci pomiedzy dwoma wektorami o typie double
*/
double distance (Vector a, Vector b){
    return sqrt(pow(b.x-a.x,2)+pow(b.y-a.y,2));
}

/** 
@TODO: OPISAĆ FUNKCJĘ
*/
Vector vector_from_points(Vector a, Vector b){ //daje vector z 2 punktów    OD A(POCZĄTEK) DO B(KONIEC)
    Vector new;
    new.x = b.x-a.x;
    new.y = b.y-a.y;
    return new;
}

/**
 * @brief Funkcja mnozy wektor przez skalar
 * @param a - wektor
 * @param n - wartosc liczbowa o typie double
 * @return zmieniony wektor
*/
Vector mult_by_num(Vector a, double n){
    a.x *=n;
    a.y *=n;
    return a;
}