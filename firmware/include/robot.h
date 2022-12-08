#include "maths.h"
#include <Arduino.h>

// Longitudes de los eslabones (links) del brazo en cm
#define L1 5.0
#define L2 5.0
#define L3 5.0
#define L4 5.0
#define L5 3.7

// Offset lateral del lapiz respecto al centro del brazo
#define D2 -2.7

// Altura que debe levantar al lápiz
#define Z0 2.0

// Término usado en Damped Least Squares
#define lambdaSq 50.0

// limit es la distancia mínima al cuadrado.
// Cuando el EE y el target están a una distancia menor a este límite, se detiene el algoritmo
#define limit 0.25

// Si el algoritmo no llega a converger, se detiene forzosamente tras esta cantidad de iteraciones
#define MAX_ITERATIONS 1000

/**
 * Obtiene una solución a partir de aplicar iterativamente Damped Least Squares
 * @param {Number} x Posición X del target
 * @param {Number} y Posición Y del target
 * @param {Number} z Posición Z del target
 */
void RobotSolveFor(double x, double y, double z);

String RobotGetDimensions();

// Retorna una cadena de texto que representa las posiciones angulares de las articulaciones
String anglesToStr();

void RobotMoveArm();

void RobotHome();