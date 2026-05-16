#ifndef ONBOARD_LED_H
#define ONBOARD_LED_H

enum class WifiStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};

void onboard_led_setup();
void onboard_led_set(WifiStatus status);
void onboard_led_loop();

#endif