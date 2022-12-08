#include "wifi_station.h"
#include "base_motor.h"
#include "servos.h"

void setup() {
    delay(3000);

    Serial.begin(9600);

    Serial.println();

    WiFiSetup();
    BaseMotorSetup();
    ServosSetup();

    BaseMotorHome();
}

void loop() {
    WiFiUpdate();
} 