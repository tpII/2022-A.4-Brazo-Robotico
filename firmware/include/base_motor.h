#include <Arduino.h>

#define IN1 16 // Pin IN1 del driver conectado al GPIO16 (D0)
#define IN2  5 // Pin IN2 del driver conectado al GPIO16 (D1)
#define IN3  4 // Pin IN3 del driver conectado al GPIO16 (D2)
#define IN4  0 // Pin IN4 del driver conectado al GPIO16 (D3)
#define POTE A0 // Potenciómetro conectado a la entrada analógica

// Límites del ángulo de la base expresados en mediciones del potenciómetro
#define MIN_POTE 220
#define MAX_POTE 650

void BaseMotorSetup();

void BaseMotorUpdate();

void BaseMotorSetAngle(float angle);

void BaseMotorStop();

bool BaseMotorFinished();

void BaseMotorHome();