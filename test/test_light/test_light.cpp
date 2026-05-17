#include <unity.h>
#include "light.h"

// Include light implementation directly. Adafruit_NeoPixel calls become no-ops
// via the mock header in test/mocks/.
#include "../../src/light.cpp"

void setUp()    { light_setup(); }
void tearDown() {}

void test_state_names() {
    TEST_ASSERT_EQUAL_STRING("off",    light_state_name(LightState::OFF));
    TEST_ASSERT_EQUAL_STRING("red",    light_state_name(LightState::RED));
    TEST_ASSERT_EQUAL_STRING("yellow", light_state_name(LightState::YELLOW));
    TEST_ASSERT_EQUAL_STRING("green",  light_state_name(LightState::GREEN));
}

void test_set_and_get_roundtrip() {
    light_set(LightState::RED);
    TEST_ASSERT_EQUAL((int)LightState::RED, (int)light_get());

    light_set(LightState::YELLOW);
    TEST_ASSERT_EQUAL((int)LightState::YELLOW, (int)light_get());

    light_set(LightState::GREEN);
    TEST_ASSERT_EQUAL((int)LightState::GREEN, (int)light_get());

    light_set(LightState::OFF);
    TEST_ASSERT_EQUAL((int)LightState::OFF, (int)light_get());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_state_names);
    RUN_TEST(test_set_and_get_roundtrip);
    return UNITY_END();
}
