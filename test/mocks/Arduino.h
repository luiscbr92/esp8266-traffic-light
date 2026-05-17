#pragma once
#include <cstdint>

using uint8_t  = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;
using int8_t   = std::int8_t;

namespace mock {
    inline unsigned long& millis_ref() {
        static unsigned long t = 0;
        return t;
    }
    inline void set_millis(unsigned long t) { millis_ref() = t; }
}

inline unsigned long millis() { return mock::millis_ref(); }
inline void delay(unsigned long) {}
