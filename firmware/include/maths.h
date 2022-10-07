#include <Arduino.h>

#define SQRT2 1.4142135623730951 // Raíz de 2

/**
 * Esta función obtiene la transpuesta de una matriz
 * @param {Matrix} mat Matriz fuente de mxn
 * @param {Matrix} t Matriz transpuesta de "mat" de nxm
 * @param {Integer} m Número de filas de la matriz "mat"
 * @param {Integer} n Número de columnas de la matriz "mat"
 */
void transposeMxN(double* mat, double* t, int m, int n);

/**
 * Esta función recibe dos matrices A y B, y deja el resultado de su multiplicación matricial en la matriz C
 * @param {Matrix} A Matriz de mxn
 * @param {Matrix} B Matriz de nxg
 * @param {Matrix} C Matriz con resultado AxB, de mxg
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A y filas de B
 * @param {Integer} g Número de columnas de B. Si no se define, por defecto es m
 */
void matMultiplication(double* A, double* B, double* C, int m, int n, int g = 0);

/**
 * Esta función obtiene la matriz inversa de una matriz dada.
 * El algoritmo es el descrito en Ahmad, Farooq & Khan, Hamid. (2010). "An Efficient and Simple Algorithm for Matrix Inversion"
 * Disponible en: https://www.researchgate.net/publication/220337322_An_Efficient_and_Simple_Algorithm_for_Matrix_Inversion
 * @param {Matrix} A Matriz cuadrada de la que se obtiene su inversa
 * @param {Integer} size Tamaño de la matriz. Considerar que la matriz debe ser cuadrada
 * @returns Retorna el determinante de la matriz. Si el determinante es 0, la matriz es singular y no tiene inversa
 */
double inverseMat(double* A, int size);

/**
 * Esta función devuelve los valores de la matriz en formato HTML para mostrarlo en la pantalla
 * @param {Matrix} mat Matriz a imprimir
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A
 */
String printMatrix(double* mat, int m, int n);

/**
 * Esta función imprime los valores de la matriz por medio de la interfaz serie
 * @param {Matrix} mat Matriz a imprimir
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A
 */
void sprintMatrix(double* mat, int m, int n);