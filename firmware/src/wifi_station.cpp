/**
 * @file Este módulo se encarga del manejo de la estación WiFi
*/
#include "wifi_station.h"
#include "robot.h"
#include "servos.h"

ESP8266WebServer server(80);

int pointsLength = 0;
Point points[MAX_POINTS] = {};

// Muestra la página principal
void handleRoot() {
    server.send(200, "text/html", "Servidor HTTP en funcionamiento");
}

void handleDimensions() {
    server.send(200, "text/plain", RobotGetDimensions());
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
    float z = Z0;
    
    // Cargo el arreglo de puntos con los valores enviados por el usuario
    loadPoints(server.arg(1));

    String pointsStr = "";
    for (int i = 0; i < pointsLength; i++) {
        z = points[i].up ? Z0 : 0.0;
        pointsStr += "\n{x:" + String(points[i].x, 4) + ", y:" + String(points[i].y, 4) + ", z:" + String(z, 4) + "} => ";
        RobotSolveFor(points[i].x, points[i].y, z);
        pointsStr += anglesToStr();
        RobotMoveArm();
    }

    RobotHome();    

    server.send(200, "text/plain", "Draw: [" + String(pointsLength) + "]:" + pointsStr);
}

void WiFiSetup() {
    Serial.print("Configurando Access Point...");

    // Conectar a la red WiFi
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) { 
        delay(100); Serial.print("_");
        delay(100); Serial.print(".");
        delay(100); Serial.print("~");
        delay(100); Serial.print(".");
        delay(100); Serial.print("_");
    }

    // Éxito
    Serial.println("");
    Serial.println("WiFi conectado :D");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());

    // Manejadores de HTTP requests
    server.on("/",     handleRoot);
    server.on("/dim",  handleDimensions);
    server.on("/draw", handleDraw);
    server.on("/connect", []() {
        server.send(200, "text/plain", "OK");
    });

    // DEBUG
    server.on("/m2p", []() {
        ServosMove(2, 10);
    });
    server.on("/m2n", []() {
        ServosMove(2, -10);
    });
    server.on("/m3p", []() {
        ServosMove(3, 10);
    });
    server.on("/m3n", []() {
        ServosMove(3, -10);
    });
    server.on("/m4p", []() {
        ServosMove(4, 10);
    });
    server.on("/m4n", []() {
        ServosMove(4, -10);
    });
    server.on("/m5p", []() {
        ServosMove(5, 10);
    });
    server.on("/m5n", []() {
        ServosMove(5, -10);
    });

    // Inicia el servidor
    server.begin();
    Serial.println ( "Servidor HTTP escuchando" );
}

void WiFiUpdate() {
    server.handleClient();
}