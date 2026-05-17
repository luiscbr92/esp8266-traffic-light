#pragma once
#include <cstdint>

constexpr int NEO_GRB    = 0;
constexpr int NEO_KHZ800 = 0;

class Adafruit_NeoPixel {
public:
    Adafruit_NeoPixel(uint16_t, uint8_t, int) {}
    void     begin() {}
    void     clear() {}
    void     show()  {}
    void     setPixelColor(uint16_t, uint32_t) {}
    uint32_t Color(uint8_t, uint8_t, uint8_t) { return 0; }
};
