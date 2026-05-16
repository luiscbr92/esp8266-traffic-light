#include <Arduino.h>
#include "wifi_setup.h"
#include "web_server.h"
#include "light.h"
#include "onboard_led.h"

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println("booting focus light");

    onboard_led_setup();
    light_setup();
    light_set(LightState::OFF);

    if (wifi_connect()) {
        web_setup();
    }
}

void loop() {
    web_loop();
    onboard_led_loop();
}