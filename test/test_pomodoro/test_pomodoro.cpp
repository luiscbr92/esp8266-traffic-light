#include <unity.h>
#include "pomodoro.h"
#include "light.h"

// Provided by light_mock.cpp — tracks last light_set() call
extern LightState mock_last_light_state;

// Include pomodoro implementation directly into this translation unit.
// light_set() and friends are resolved from light_mock.cpp at link time.
#include "../../src/pomodoro.cpp"

void setUp() {
    mock::set_millis(0);
    mock_last_light_state = LightState::OFF;
    pomodoro_setup();
}

void tearDown() {}

void test_initial_state_is_idle() {
    TEST_ASSERT_FALSE(pomodoro_is_running());
    TEST_ASSERT_EQUAL((int)PomodoroPhase::IDLE, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL(0UL, pomodoro_remaining_seconds());
}

void test_start_enters_focus_and_sets_red_light() {
    pomodoro_start();
    TEST_ASSERT_TRUE(pomodoro_is_running());
    TEST_ASSERT_EQUAL((int)PomodoroPhase::FOCUS, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL((int)LightState::RED, (int)mock_last_light_state);
}

void test_remaining_decrements_during_focus() {
    pomodoro_start();
    mock::set_millis(60UL * 1000UL); // 1 minute elapsed
    TEST_ASSERT_EQUAL(19UL * 60UL, pomodoro_remaining_seconds());
}

void test_remaining_is_zero_when_idle() {
    TEST_ASSERT_EQUAL(0UL, pomodoro_remaining_seconds());
}

void test_loop_noop_when_idle() {
    pomodoro_loop();
    TEST_ASSERT_FALSE(pomodoro_is_running());
}

void test_focus_transitions_to_wrap_up_at_20min() {
    pomodoro_start();
    mock::set_millis(20UL * 60UL * 1000UL);
    pomodoro_loop();
    TEST_ASSERT_EQUAL((int)PomodoroPhase::WRAP_UP, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL((int)LightState::YELLOW, (int)mock_last_light_state);
}

void test_wrap_up_transitions_to_break_at_5min() {
    pomodoro_start();
    mock::set_millis(20UL * 60UL * 1000UL);
    pomodoro_loop(); // FOCUS -> WRAP_UP; phase_started_at reset to 20min mark
    mock::set_millis(25UL * 60UL * 1000UL);
    pomodoro_loop(); // WRAP_UP -> BREAK
    TEST_ASSERT_EQUAL((int)PomodoroPhase::BREAK, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL((int)LightState::GREEN, (int)mock_last_light_state);
}

void test_break_transitions_to_idle_at_5min() {
    pomodoro_start();
    mock::set_millis(20UL * 60UL * 1000UL);
    pomodoro_loop(); // -> WRAP_UP
    mock::set_millis(25UL * 60UL * 1000UL);
    pomodoro_loop(); // -> BREAK
    mock::set_millis(30UL * 60UL * 1000UL);
    pomodoro_loop(); // -> IDLE
    TEST_ASSERT_FALSE(pomodoro_is_running());
    TEST_ASSERT_EQUAL((int)PomodoroPhase::IDLE, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL((int)LightState::OFF, (int)mock_last_light_state);
}

void test_stop_returns_to_idle_and_turns_off_light() {
    pomodoro_start();
    pomodoro_stop();
    TEST_ASSERT_FALSE(pomodoro_is_running());
    TEST_ASSERT_EQUAL((int)PomodoroPhase::IDLE, (int)pomodoro_current_phase());
    TEST_ASSERT_EQUAL((int)LightState::OFF, (int)mock_last_light_state);
}

void test_phase_names() {
    TEST_ASSERT_EQUAL_STRING("idle",    pomodoro_phase_name(PomodoroPhase::IDLE));
    TEST_ASSERT_EQUAL_STRING("focus",   pomodoro_phase_name(PomodoroPhase::FOCUS));
    TEST_ASSERT_EQUAL_STRING("wrap_up", pomodoro_phase_name(PomodoroPhase::WRAP_UP));
    TEST_ASSERT_EQUAL_STRING("break",   pomodoro_phase_name(PomodoroPhase::BREAK));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_idle);
    RUN_TEST(test_start_enters_focus_and_sets_red_light);
    RUN_TEST(test_remaining_decrements_during_focus);
    RUN_TEST(test_remaining_is_zero_when_idle);
    RUN_TEST(test_loop_noop_when_idle);
    RUN_TEST(test_focus_transitions_to_wrap_up_at_20min);
    RUN_TEST(test_wrap_up_transitions_to_break_at_5min);
    RUN_TEST(test_break_transitions_to_idle_at_5min);
    RUN_TEST(test_stop_returns_to_idle_and_turns_off_light);
    RUN_TEST(test_phase_names);
    return UNITY_END();
}
