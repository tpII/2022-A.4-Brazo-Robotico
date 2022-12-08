/*
 Step motor turns CCW               Step motor turns CW, 
 Array tab1 used:                  Array tab2 used:
 IN1 IN2 IN3 IN4                   IN1 IN2 IN3 IN4
 1000                              1000
 1100                              1001
 0100                              0001
 0110                              0011
 0010                              0010
 0011                              0110
 0001                              0100
 1001                              1100
*/

#include "base_motor.h"

#define isSet(a,b) (a & b) == b
#define STOP 0
#define TO_CCW 1
#define TO_CW 2

const char CCW[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};
const char CW[]  = {0x01, 0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03};

// --===================== PRIVATE =====================--
String input;
int target = 505;
int direction = STOP;

void moveCW(uint8 timedelay) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(IN1, isSet(CW[i], 1));
        digitalWrite(IN2, isSet(CW[i], 2));
        digitalWrite(IN3, isSet(CW[i], 4));
        digitalWrite(IN4, isSet(CW[i], 8));
        
        delay(timedelay);

        if (analogRead(POTE) <= target)
            return;
    }
}

void moveCCW(uint8 timedelay) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(IN1, isSet(CCW[i], 1));
        digitalWrite(IN2, isSet(CCW[i], 2));
        digitalWrite(IN3, isSet(CCW[i], 4));
        digitalWrite(IN4, isSet(CCW[i], 8));

        delay(timedelay);

        if (analogRead(POTE) >= target)
            return;
    }
}

void moveTo(uint16 pos) {
    if (pos > 650 || pos < 300) return; // Security

    uint16 actual = analogRead(POTE);

    if (actual == pos) return;

    
    if (actual < pos) {
        // Tengo que girar en sentido antihorario hasta la posición

        while (actual < pos) {
            moveCCW(5);
            actual = analogRead(POTE);
        }

    } else {
        // Tengo que girar en sentido horario hasta la posición
        while (actual > pos) {
            moveCW(5);
            actual = analogRead(POTE);
        }
    }

    BaseMotorStop();
}

// --===================== PUBLIC =====================--

void BaseMotorSetup() {
    pinMode(IN1, OUTPUT); // Set the IN1 pin as OUTPUT
    pinMode(IN2, OUTPUT); // Set the IN2 pin as OUTPUT
    pinMode(IN3, OUTPUT); // Set the IN3 pin as OUTPUT
    pinMode(IN4, OUTPUT); // Set the IN4 pin as OUTPUT

    BaseMotorStop();
}


void BaseMotorUpdate() {
    if (direction == STOP) {
        return;
    } else if (direction == TO_CCW) {
        uint16 actual = analogRead(POTE);

        if (actual >= target) {
            direction = STOP;
        } else {
            moveCCW(10);
        }
    } else if (direction == TO_CW) {
        uint16 actual = analogRead(POTE);

        if (actual <= target) {
            direction = STOP;
        } else {
            moveCW(10);
        }
    }
    BaseMotorStop();
}

void BaseMotorSetAngle(float angle) {
    target = 508.35 + 214.51 * angle;
    target = constrain(target, MIN_POTE, MAX_POTE);

    uint16 actual = analogRead(POTE);
    if (actual < target)
        direction = TO_CCW;
    else if (actual > target)
        direction = TO_CW;
}

void BaseMotorStop() {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
}

bool BaseMotorFinished() {
    return direction == STOP;
}

void BaseMotorHome() {
    Serial.println("Base Motor Home");
    BaseMotorStop();
    target = 505;
    moveTo(505);
}