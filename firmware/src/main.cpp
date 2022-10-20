#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "robot.h"

/* Configuración de la red. */
const char *ssid = "SSID";// WIFI ssid
const char *password = "password"; // WIFI password

// Estructura de datos de un punto
typedef struct Point {
    float x;
    float y;
    bool up; // Si está en UP => z=1, sino z=0
} Point;

int pointsLength = 0;
Point points[256] = {};

ESP8266WebServer server(80);

// Muestra la página principal
void handleRoot() {
    server.send(
        200,
        "text/html",
"<form action=\"/LED_BUILTIN_on\" method=\"get\" id=\"form1\">\
</form><button type=\"submit\" form=\"form1\" value=\"On\">On</button>\
<form action=\"/LED_BUILTIN_off\" method=\"get\" id=\"form2\"></form>\
<button type=\"submit\" form=\"form2\" value=\"Off\">Off</button>\
<form action=\"/draw\" method=\"get\" id=\"form3\">\
<input type=\"number\" name=\"q\"/>\
<input name=\"ps\"/>\
<button type=\"submit\" form=\"form3\">Dibujar</button>\
</form>\
"
        /*"<div>" + printMatrix(J, 6, 5) + "<br>"
        "Max radius: " + String(maxRadius, 4) + ", Min radius: " + String(minRadius, 4) + "<br>" +
        "Alpha:" + String(alpha, 4) + ", Base/Width: " + String(base, 4) + ", Height: " + String(height, 4) + "</div>"*/
    );
}

/**
 * A partir de una cadena de texto que representa la parametrización de los segmentos,
 * completa el arreglo Points con las coordenadas de los puntos
 * La parametrización consiste en valores separados por ','.
 * Si el valor es DOWN significa un comienzo de segmento. Si es UP significa un fin de segmento
 * el resto de los valores son pares X,Y
 */
void loadPoints(String str) {
    String val = "";    // Va a contener el valor leído
    int p = 0;          // Índice dentro de la cadena "str"
    char c;             // Caracter leído
    bool up = true;

    for (int i = 0; i < pointsLength; i++) {
        do {
            c = str.charAt(p++);
            if (c != ',')
                val += c;
        } while (c != ',');

        // En este punto val puede ser UP, DOWN o puede contner el valor de X

        if (val == "UP") {
            points[i].x = points[i - 1].x;
            points[i].y = points[i - 1].y;
            points[i].up = true;
            up = true;
        } else if (val == "DOWN") {
            points[i].x = points[i - 1].x;
            points[i].y = points[i - 1].y;
            points[i].up = false;
            up = false;
        } else {
            points[i].x = val.toFloat();
            val = "";

            do {
                c = str.charAt(p++);
                if (c != ',')
                    val += c;
            } while (c != ',');

            points[i].y = val.toFloat();
            points[i].up = up;
        }
        val = "";
    }
}

// Espera recibir 2 parámetros
// arg0: cantidad de puntos, arg1: cadena que representa la parametrización de los puntos
void handleDraw() {
    pointsLength = server.arg(0).toInt();
    float z = 1.0;
    
    // Cargo el arreglo de puntos con los valores enviados por el usuario
    loadPoints(server.arg(1));

    String pointsStr = "";
    for (int i = 0; i < pointsLength; i++) {
        z = points[i].up ? 1.0 : 0.0;
        pointsStr += "\n{x:" + String(points[i].x, 4) + ", y:" + String(points[i].y, 4) + ", z:" + String(z, 4) + "} => ";
        solveFor(points[i].x, points[i].y, z);
        pointsStr += anglesToStr();
    }

    server.send(200, "text/plain", "Draw: [" + String(pointsLength) + "]:" + pointsStr);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    delay(3000);

    Serial.begin(9600);

    Serial.println();
    Serial.print("Configuring access point...");

    // Conectar a la red WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Éxito
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Manejadores de HTTP requests
    server.on("/",     handleRoot);
    server.on("/draw", handleDraw);
    server.on("/LED_BUILTIN_on", []() { // Deberían implementar una arrow function en Arduino para evitar esto...
        digitalWrite(LED_BUILTIN, 1);
        Serial.println("on");
        handleRoot();
    });
    server.on("/LED_BUILTIN_off", []() {
        digitalWrite(LED_BUILTIN, 0);
        Serial.println("off");
        handleRoot();
    });

    // Inicia el servidor
    server.begin();
    Serial.println ( "HTTP server started" );
}

void loop() {
    server.handleClient();
} 