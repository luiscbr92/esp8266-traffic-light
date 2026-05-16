#include "wifi_setup.h"
#include "secrets.h"
#include "onboard_led.h"
#include <ESP8266WiFi.h>

bool wifi_connect() {
    Serial.print("connecting to wifi");
    onboard_led_set(WifiStatus::CONNECTING);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 30) {
        onboard_led_loop();
        delay(500);
        if (tries % 10 == 0) Serial.print(".");
        tries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print("connected, ip: ");
        Serial.println(WiFi.localIP());
        onboard_led_set(WifiStatus::CONNECTED);
        return true;
    } else {
        Serial.println();
        Serial.println("wifi failed");
        onboard_led_set(WifiStatus::DISCONNECTED);
        return false;
    }
}