#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

enum class LightState {
    OFF,
    RED,
    YELLOW,
    GREEN
};

void light_setup();
void light_set(LightState state);
LightState light_get();
const char* light_state_name(LightState state);

#endif