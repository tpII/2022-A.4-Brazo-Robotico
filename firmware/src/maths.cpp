/**
 * @file Este archivo incluye todas las operaciones matriciales necesarias para el proyecto
 */

/**
 * Aclaración:
 * Las matrices empleadas en este proyecto se implementan como un arreglo de double de la matriz ordenada por filas.
 * Si la matriz es de dimensión MxN (M filas y N columnas) para acceder a la posición (i, j) (fila i, columna j)
 * se debe acceder con el índice: Matriz[i * N + j], con 0 <= i < M, 0 <= j < N
 */

#include "maths.h"

/**
 * Esta función obtiene la transpuesta de una matriz
 * @param {Matrix} mat Matriz fuente de mxn
 * @param {Matrix} t Matriz transpuesta de "mat" de nxm
 * @param {Integer} m Número de filas de la matriz "mat"
 * @param {Integer} n Número de columnas de la matriz "mat"
 */
void transposeMxN(double* mat, double* t, int m, int n) {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            t[j * m + i] = mat[i * n + j];
}

/**
 * Esta función recibe dos matrices A y B, y deja el resultado de su multiplicación matricial en la matriz C
 * @param {Matrix} A Matriz de mxn
 * @param {Matrix} B Matriz de nxg
 * @param {Matrix} C Matriz con resultado AxB, de mxg
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A y filas de B
 * @param {Integer} g Número de columnas de B. Si no se define, por defecto es m
 */
void matMultiplication(double* A, double* B, double* C, int m, int n, int g) {
    int i, j, k;
    if (!g) g = m;
    for (i = 0; i < m; i++) {
        for (j = 0; j < g; j++) {
            C[i * g + j] = 0;
        }
    }
    for (i = 0; i < m; i++) {
        for (j = 0; j < g; j++) {
            for (k = 0; k < n; k++) {
                C[i * g + j] += A[i * n + k] * B[k * g + j];
            }
        }
    }
}

/**
 * Esta función obtiene la matriz inversa de una matriz dada.
 * El algoritmo es el descrito en Ahmad, Farooq & Khan, Hamid. (2010). "An Efficient and Simple Algorithm for Matrix Inversion"
 * Disponible en: https://www.researchgate.net/publication/220337322_An_Efficient_and_Simple_Algorithm_for_Matrix_Inversion
 * @param {Matrix} A Matriz cuadrada de la que se obtiene su inversa
 * @param {Integer} size Tamaño de la matriz. Considerar que la matriz debe ser cuadrada
 * @returns Retorna el determinante de la matriz. Si el determinante es 0, la matriz es singular y no tiene inversa
 */
double inverseMat(double* A, int size) {
    double pivot, det = 1.0;
    int i, j, p;

    for (p = 0; p < size; p++) {
        pivot = A[p * size + p];
        det *= pivot;

        if (pivot < 1e-5) return 0;

        for (i = 0; i < size; i++)
            if (i != p) {
                A[p * size + i] /= -pivot;
            }

        for (i = 0; i < size; i++)
            if (i != p)
                for (j = 0; j < size; j++)
                    if (j != p)
                        A[j * size + i] += A[j * size + p] * A[p * size + i];

        for (i = 0; i < size; i++)
            A[i * size + p] /= pivot;

        A[p * size + p] = 1 / pivot;
    }

    return det;
}

/**
 * Esta función devuelve los valores de la matriz en formato HTML para mostrarlo en la pantalla
 * @param {Matrix} mat Matriz a imprimir
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A
 */
String printMatrix(double* mat, int m, int n) {
    String str = "";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            str += String(mat[i * n + j], 4) + " ";
        str += "<br>";
    }
    return str;
}

/**
 * Esta función imprime los valores de la matriz por medio de la interfaz serie
 * @param {Matrix} mat Matriz a imprimir
 * @param {Integer} m Número de filas de A
 * @param {Integer} n Número de columnas de A
 */
void sprintMatrix(double* mat, int m, int n) {
    String str = "";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            str += String(mat[j * n + i], 4) + " ";
        str += "\n";
    }
    Serial.print(str);
}