#include "Arduino.h"
#include <Servo.h>

#define Q2 15 // Pin S del Servo Q2 conectado al GPIO15 (D8)
#define Q3 13 // Pin S del Servo Q3 conectado al GPIO13 (D7)
#define Q4 12 // Pin S del Servo Q4 conectado al GPIO12 (D6)
#define Q5 14 // Pin S del Servo Q5 conectado al GPIO14 (D5)

#define STEP 1 // Paso en us para mover el servomotor
#define STEP_DELAY 2 // Esperar 2 ms entre pasos de servo

void ServosSetup();

void ServosUpdate();

void ServosSetAngles(float q2, float q3, float q4, float q5);

bool ServosFinished();

void ServosHome();

// DEBUG

void ServosMove(int servo, int d);