#include "light.h"

static LightState current = LightState::OFF;
LightState mock_last_light_state = LightState::OFF;

void  light_setup() {}
void  light_set(LightState s) { current = s; mock_last_light_state = s; }
LightState light_get() { return current; }
const char* light_state_name(LightState) { return ""; }
