#include "servos.h"

// --===================== PRIVATE =====================--

Servo servoQ2, servoQ3, servoQ4, servoQ5;
int s_target[] = {800, 500, 800, 1250};
int s_delay[] = {0, 0, 0, 0};
int s_delayCount[] = {0, 0, 0, 0};
int servos_finish = 3;

void moveTo(Servo *servo, int pos) {
    if (pos < 250 || pos > 2000)
        return; // Safety

    int actual = servo->readMicroseconds();

    if (actual == pos)
        return;

    if (actual < pos) {
        // Tengo que girar en sentido antihorario hasta la posición

        while (actual < pos) {
            actual += STEP;
            servo->writeMicroseconds(actual);
            delay(STEP_DELAY);
        }
    } else {
        // Tengo que girar en sentido horario hasta la posición
        while (actual > pos) {
            actual -= STEP;
            servo->writeMicroseconds(actual);
            delay(STEP_DELAY);
        }
    }

    servo->writeMicroseconds(pos);
}

/**
 * Mueve el servo 1us hacia su posición final.
 * Retorna 1 si llegó al objetivo
 */
int updateServo(Servo *servo, int t) {
    int actual = servo->readMicroseconds();

    if (actual == t)
        return 1;

    actual += (actual < t) - (actual > t); // if actual < t actual++ else if actual > t actual--
    servo->writeMicroseconds(actual);
    return 0;
}

void setDelays() {
    float dif2 = abs(s_target[0] - servoQ2.readMicroseconds()),
          dif3 = abs(s_target[1] - servoQ3.readMicroseconds()),
          dif4 = abs(s_target[2] - servoQ4.readMicroseconds()),
          dif5 = abs(s_target[3] - servoQ5.readMicroseconds());

    float dif = max(max(dif2, dif3), max(dif4, dif5));

    s_delay[0] = dif2 ? floor(dif / dif2) : 0;
    s_delay[1] = dif3 ? floor(dif / dif3) : 0;
    s_delay[2] = dif4 ? floor(dif / dif4) : 0;
    s_delay[3] = dif5 ? floor(dif / dif5) : 0;

    s_delayCount[0] = s_delay[0];
    s_delayCount[1] = s_delay[1];
    s_delayCount[2] = s_delay[2];
    s_delayCount[3] = s_delay[3];
}

// --===================== PUBLIC =====================--

void ServosSetup() {
    servoQ2.attach(Q2, 300, 2000, 540);
    delay(500);
    servoQ3.attach(Q3, 300, 2000, 703);
    delay(500);
    servoQ4.attach(Q4, 300, 2000, 1325);
    delay(500);
    servoQ5.attach(Q5, 300, 2000, 1150);
    delay(500);
}

void ServosHome() {
    moveTo(&servoQ2, 540);
    moveTo(&servoQ3, 703);
    moveTo(&servoQ4, 1325);
    moveTo(&servoQ5, 1150);
}

void ServosUpdate() {
    // Mueve los servos hasta el objetivo
    if (servos_finish == 3)
        return;

    servos_finish = 0;
    if (s_delayCount[0]-- == 0) {
        servos_finish += updateServo(&servoQ2, s_target[0]);
        s_delayCount[0] = s_delay[0];
    }
    if (s_delayCount[1]-- == 0) {
        servos_finish += updateServo(&servoQ3, s_target[1]);
        s_delayCount[1] = s_delay[1];
    }
    if (s_delayCount[2]-- == 0) {
        servos_finish += updateServo(&servoQ4, s_target[2]);
        s_delayCount[2] = s_delay[2];
    }
    /*if (s_delayCount[3]-- == 0) {
        servos_finish += updateServo(&servoQ5, s_target[3]);
        s_delayCount[3] = s_delay[3];
    }*/
    float q2 = (1253.2 - servoQ2.readMicroseconds()) / 587.69;
    float q3 = (128.21 - servoQ3.readMicroseconds()) / 628.21;
    float q4 = (340.35 - servoQ4.readMicroseconds()) / 596.71;
    servoQ5.writeMicroseconds(constrain(1020.7 - 603.8 * (-PI * .5 - q2 - q3 - q4), 300, 2000));
}

void ServosSetAngles(float q2, float q3, float q4, float q5) {
    s_target[0] = constrain(1253.2 - 587.69 * q2, 300, 2000);
    s_target[1] = constrain(128.21 - 628.21 * q3, 300, 2000);
    //s_target[2] = constrain(308.73 - 592.40 * q4, 300, 2000);
    s_target[2] = constrain(340.35 - 596.71 * q4, 300, 2000);
    s_target[3] = constrain(1020.7 - 603.8 * q5, 300, 2000);

    setDelays();

    servos_finish = 0;
}

bool ServosFinished() {
    return servos_finish == 3;
}

void ServosMove(int servo, int d) {
    Servo *s = servo == 2 ? &servoQ2
             : servo == 3 ? &servoQ3
             : servo == 4 ? &servoQ4
                          : &servoQ5;
    s->writeMicroseconds(s->readMicroseconds() + d);
    Serial.println("S" + String(servo) + ":" + String(s->readMicroseconds()));
}