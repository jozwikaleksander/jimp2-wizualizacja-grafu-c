#include "vector.h"

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
double count_vector_length(Vector a){
    return sqrt(a.x * a.x + a.y * a.y);
}

/**
 * @brief Funkcja oblicza odleglosc pomiedzy dwoma wektorami
 * @param a - pierwszy wektor
 * @param b - drugi wektor
 * @return wartosc odleglosci pomiedzy dwoma wektorami o typie double
*/
double distance (Vector a, Vector b){
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

/** 
 * @brief Funkcja tworzy wektor na podstawie dwoch punktow pobranych z 2 innych wektorow
 * @param a - wektor poczatkowy
 * @param b - wektor koncowy
 * @return utworzony wektor
*/
Vector vector_from_points(Vector a, Vector b){
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


/**
 * @brief Funkcja zwraca wektor z wartościami pomnożonymi przez -1
 * @param a - wektor
 * @return nowy zmieniony wektor
*/

Vector negative(Vector a){
    Vector new;
    new.x = -a.x;
    new.y= -a.y;
    return new;
}