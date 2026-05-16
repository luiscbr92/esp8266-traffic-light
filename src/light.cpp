#include "light.h"
#include <Adafruit_NeoPixel.h>

constexpr uint8_t NEOPIXEL_PIN = 4;
constexpr uint16_t NUM_PIXELS = 1;
constexpr float BRIGHTNESS = 0.3f;

static Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
static LightState current_state = LightState::OFF;

struct RGB { uint8_t r, g, b; };

static RGB color_for(LightState state) {
    switch (state) {
        case LightState::RED:    return {255, 30, 30};
        case LightState::YELLOW: return {255, 160, 0};
        case LightState::GREEN:  return {40, 200, 40};
        case LightState::OFF:
        default:                 return {0, 0, 0};
    }
}

void light_setup() {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void light_set(LightState state) {
    current_state = state;
    RGB c = color_for(state);
    uint8_t r = static_cast<uint8_t>(c.r * BRIGHTNESS);
    uint8_t g = static_cast<uint8_t>(c.g * BRIGHTNESS);
    uint8_t b = static_cast<uint8_t>(c.b * BRIGHTNESS);
    for (uint16_t i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
}

LightState light_get() {
    return current_state;
}

const char* light_state_name(LightState state) {
    switch (state) {
        case LightState::RED:    return "red";
        case LightState::YELLOW: return "yellow";
        case LightState::GREEN:  return "green";
        case LightState::OFF:
        default:                 return "off";
    }
}