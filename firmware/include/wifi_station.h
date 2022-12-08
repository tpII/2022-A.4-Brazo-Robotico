#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* Configuración de la red. */
#define SSID "alumnosInfo" // "alumnosInfo"
#define PASSWORD "Informatica2019" // "Informatica2019"
#define MAX_POINTS 512

/**
 * Inicializa el WiFi del ESP8266 con la configuración por defecto
*/
void WiFiSetup();

/**
 * Atiende a los clientes de la red
*/
void WiFiUpdate();