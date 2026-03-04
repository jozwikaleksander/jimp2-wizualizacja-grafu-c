#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "stdio.h"
#include "stdlib.h"

typedef struct {
    double x;
    double y;
} Vector;

/**
 * @brief Funkcja dodaje wektory
 * @param a - pierwszy wektor
 * @param b - drugi wektor
 * @return zmieniony pierwszy wektor
*/
Vector add_vectors (Vector a, Vector b);

/**
 * @brief Funkcja oblicza dlugosc wektora
 * @param a - wektor
 * @return zwraca wartosc dlugosci wektora o typie double
*/
double count_vector_length(Vector a);

/**
 * @brief Funkcja oblicza odleglosc pomiedzy dwoma wektorami
 * @param a - pierwszy wektor
 * @param b - drugi wektor
 * @return zwraca wartosc odleglosci pomiedzy dwoma wektorami o typie double
*/
double distance (Vector a, Vector b); //Euclidean distance(odłegłość)

/** 
@TODO: OPISAĆ FUNKCJĘ
*/
Vector vector_from_points(Vector a, Vector b);

/**
 * @brief Funkcja mnozy wektor przez skalar
 * @param a - wektor
 * @param n - wartosc liczbowa o typie double
 * @return zmieniony wektor
*/
Vector mult_by_num(Vector a, double n);

#endif