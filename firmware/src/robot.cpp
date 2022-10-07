#include "robot.h"

// Máximo radio en cm de dibujo que puede alcanzar el brazo.
// Se obtiene considerando la máxima extensión del mismo mientras el EE toque la hoja
double maxRadius = sqrt((L2+L3+L4)*(L2+L3+L4) - (L5-L1)*(L5-L1));

// Mínimo radio en cm de dibujo que puede alcanzar el brazo.
double minRadius = 3.3;

// Dimensiones del área del dibujo del brazo
// Garantizan la máxima área posible dentro de los límites del brazo
double alpha = acos(SQRT2 * minRadius/maxRadius) - PI * 0.25;
double width  = sin(alpha) * 2 * maxRadius,
       height = cos(alpha) * maxRadius - minRadius;


/* ---------- VARIABLES DEL BRAZO ---------- */

// Ángulos (en radianes) correspondientes a cada articulación (joint) del brazo
double q1 = 0.6, q2 = 1.0, q3 = -0.4, q4 = -0.6, q5 = -1.0;

// Posición tridimensional del End Effector (EE) (la punta del lápiz)
double ex = 0.0, ey = 0.0, ez = 0.0;

// Posición tridimensional del Target, es decir, donde queremos que esté la punta del lápiz
double tx = 0.0, ty = 4.0, tz = 0.0;

// Vector error. Diferencia entre la posición del target y la del EE
double e[] = {0, 0, 0, 0, 0, 0};

// J: Jacobiano, Jt: Transpuesta del Jacobiano
double J[30] = {}, Jt[30] = {};
// JJt = JxJt,  J_1: matriz pseudoinversa obtenida por Damped Least Squares
double JJt[36] = {}, J_1[30] = {};

// Variación en radianes que sufren las articulaciones tras cada iteración. Tal que q(i+1) = q(i) + deltaTheta
double deltaTheta[6] = {};

/**
 * Esta función realiza la cinemática directa.
 * Obtiene la posición tridimensional del EE a partir de los ángulos de las articulaciones
 */
void setEndEffector() {
    double r =
        L2 * cos(q2) +
        L3 * cos(q2 + q3) +
        L4 * cos(q2 + q3 + q4) +
        L5 * cos(q2 + q3 + q4 + q5);
    ex = cos(q1) * r;
    ey = sin(q1) * r;
    ez = L1 + L2 * sin(q2) + L3 * sin(q2 + q3) + L4 * sin(q2 + q3 + q4) + L5 * sin(q2 + q3 + q4 + q5);
}

/**
 * Establece los valores del vector "e", tal que e = target - EE
 */
void setEVector() {
    setEndEffector();
    e[0] = tx - ex;
    e[1] = ty - ey;
    e[2] = tz - ez;
}

/**
 * Obtiene el Jacobiano del brazo
 */
void setJacobian() {
    double s1 = sin(q1), c1 = cos(q1);
    double s2 = sin(q2), c2 = cos(q2);
    double c23   = cos(q2+q3),       s23   = sin(q2+q3);
    double c234  = cos(q2+q3+q4),    s234  = sin(q2+q3+q4);
    double c2345 = cos(q2+q3+q4+q5), s2345 = sin(q2+q3+q4+q5);
    double b1 = (L2*c2 + L3*c23 + L4*c234 + L5*c2345);
    double b2 = (L3*c23 + L4*c234 + L5*c2345);
    double b3 = (L4*c234 + L5*c2345);
    double b4 = (L5*c2345);
    double a2 = (L2*s2 + L3*s23 + L4*s234 + L5*s2345);
    double a3 = (L3*s23 + L4*s234 + L5*s2345);
    double a4 = (L4*s234 + L5*s2345);
    double a5 = (L5*s2345);
    J[0] = -s1 * b1;
    J[1] = -c1 * a2;
    J[2] = -c1 * a3;
    J[3] = -c1 * a4;
    J[4] = -c1 * a5;
    J[5] =  c1 * b1;
    J[6] = -s1 * a2;
    J[7] = -s1 * a3;
    J[8] = -s1 * a4;
    J[9] = -s1 * a5;
    J[10] = 0;
    J[11] = b1;
    J[12] = b2;
    J[13] = b3;
    J[14] = b4;
    J[15] = 0;
    J[16] = -s1;
    J[17] = -s1;
    J[18] = -s1;
    J[19] = -s1;
    J[20] = 0;
    J[21] = c1;
    J[22] = c1;
    J[23] = c1;
    J[24] = c1;
    J[25] = 1;
    J[26] = 0;
    J[27] = 0;
    J[28] = 0;
    J[29] = 0;
}

/**
 * Realiza la serie de cálculos necesarios para obtener la pseudoinversa de la matriz Jacobiana
 */
void getInvJacobian() {
    setJacobian();                              // Inicializo la matriz Jacobiana de la posición actual
    //sprintMatrix(J, 6, 5);
    transposeMxN(J, Jt, 6, 5);                  // Jt = transpuesta(J)
    matMultiplication(J, Jt, JJt, 6, 5);        // JJt = J x Jt
    // printMatrix(Jt, 6, 5)
    for (int i = 0; i < 6; i++)                 // JJt = JJt + lambdaSq * I
        JJt[i * 6 + i] += lambdaSq;
    inverseMat(JJt, 6);                         // JJt = inv(JJt)
    matMultiplication(Jt, JJt, J_1, 5, 6, 6);   // J_1 = Jt x JJt
}

/**
 * Aplica el cambio "deltaTheta" a las articulaciones del brazo
 */
void addDeltaTheta() {
    q1 += deltaTheta[0];
    q2 += deltaTheta[1];
    q3 += deltaTheta[2];
    q4 += deltaTheta[3];
    q5 = -PI * .5 - q2 - q3 - q4; // Esta restricción garantiza que el lápiz se encuentra perpendicular a la hoja
}

/**
 * Algoritmo de Damped Least Squares
 * Produce un cambio (delta theta) en los ángulos de las articulaciones
 * @returns Retorna true si la solución converge
 */
bool DampedLeastSquares() {
    setEVector();

    // Si el end effector está dentro de los límites, acepto la solución
    if (e[0] * e[0] + e[1] * e[1] < limit && e[2] < 0.1)
        return true;

    getInvJacobian();
    matMultiplication(J_1, e, deltaTheta, 5, 6, 1); // deltaTheta = J_1 x e
    addDeltaTheta();                                // q(i+1) = q(i) + delatTheta

    // La solución todavía no converge
    return false;
}

/**
 * Obtiene una solución a partir de aplicar iterativamente Damped Least Squares
 * @param {Number} x Posición X del target
 * @param {Number} y Posición Y del target
 * @param {Number} z Posición Z del target
 */
void solveFor(double x, double y, double z) {
    // Inicializo el vector target
    tx = x; ty = y; tz = z;

    // La máxima cantidad de iteraciones es MAX_ITERATIONS
    int i = MAX_ITERATIONS;

    //Serial.println("Max radius: " + String(maxRadius, 4) + ", Min radius: " + String(minRadius, 4));
    //Serial.println("Alpha:" + String(alpha, 4) + ", Width: " + String(width, 4) + ", Height: " + String(height, 4));
    
    // Bucle iterativo que va convergiendo a la solución
    while (i--) {
        // Si la solución convergió, detengo las iteraciones
        if (DampedLeastSquares()) {
            //Serial.println("take: " + ((MAX_ITERATIONS - i)));
            //Serial.println(anglesToStr());
            return;
        }
    }

    // Error. Cuando el algoritmo no logra converger
    Serial.println("El brazo no pudo alcanzar la posicion en " + String(MAX_ITERATIONS));
}

// Retorna una cadena de texto que representa las posiciones angulares de las articulaciones
String anglesToStr() {
    return "q1: " + String(q1, 4) + ", q2: " + String(q2, 4) + ", q3: " + String(q3, 4) + ", q4: " + String(q4, 4) + ", q5: " + String(q5, 4);
}