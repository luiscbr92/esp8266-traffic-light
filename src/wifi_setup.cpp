#include "wifi_setup.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

bool wifi_connect() {
    Serial.print("connecting to wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 30) {
        delay(500);
        Serial.print(".");
        tries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print("connected, ip: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println();
        Serial.println("wifi failed");
        return false;
    }
}