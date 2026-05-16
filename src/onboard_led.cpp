#include "onboard_led.h"
#include <Arduino.h>

constexpr uint8_t ONBOARD_LED_PIN = 5;
constexpr unsigned long BLINK_INTERVAL_MS = 500;

static WifiStatus current_status = WifiStatus::DISCONNECTED;
static unsigned long last_toggle = 0;
static bool blink_state = false;

void onboard_led_setup() {
    pinMode(ONBOARD_LED_PIN, OUTPUT);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
}

void onboard_led_set(WifiStatus status) {
    current_status = status;
    // Onboard LED light up on LOW
    if (status == WifiStatus::CONNECTED) {
        digitalWrite(ONBOARD_LED_PIN, LOW);
    } else if (status == WifiStatus::DISCONNECTED) {
        digitalWrite(ONBOARD_LED_PIN, HIGH);
    }
}

void onboard_led_loop() {
    if (current_status != WifiStatus::CONNECTING) return;

    unsigned long now = millis();
    if (now - last_toggle >= BLINK_INTERVAL_MS) {
        last_toggle = now;
        blink_state = !blink_state;
        digitalWrite(ONBOARD_LED_PIN, blink_state ? LOW : HIGH);
    }
}