#include "pomodoro.h"
#include "light.h"

constexpr unsigned long FOCUS_MS   = 20UL * 60UL * 1000UL;
constexpr unsigned long WRAP_UP_MS =  5UL * 60UL * 1000UL;
constexpr unsigned long BREAK_MS   =  5UL * 60UL * 1000UL;

static PomodoroPhase current_phase = PomodoroPhase::IDLE;
static unsigned long phase_started_at = 0;

static unsigned long duration_for(PomodoroPhase phase) {
    switch (phase) {
        case PomodoroPhase::FOCUS:   return FOCUS_MS;
        case PomodoroPhase::WRAP_UP: return WRAP_UP_MS;
        case PomodoroPhase::BREAK:   return BREAK_MS;
        default:                     return 0;
    }
}

static void enter_phase(PomodoroPhase phase) {
    current_phase = phase;
    phase_started_at = millis();
    switch (phase) {
        case PomodoroPhase::FOCUS:   light_set(LightState::RED);    break;
        case PomodoroPhase::WRAP_UP: light_set(LightState::YELLOW); break;
        case PomodoroPhase::BREAK:   light_set(LightState::GREEN);  break;
        case PomodoroPhase::IDLE:    light_set(LightState::OFF);    break;
    }
}

void pomodoro_setup() {
    current_phase = PomodoroPhase::IDLE;
}

void pomodoro_start() {
    enter_phase(PomodoroPhase::FOCUS);
}

void pomodoro_stop() {
    enter_phase(PomodoroPhase::IDLE);
}

bool pomodoro_is_running() {
    return current_phase != PomodoroPhase::IDLE;
}

PomodoroPhase pomodoro_current_phase() {
    return current_phase;
}

unsigned long pomodoro_remaining_seconds() {
    if (current_phase == PomodoroPhase::IDLE) return 0;
    unsigned long elapsed = millis() - phase_started_at;
    unsigned long total = duration_for(current_phase);
    if (elapsed >= total) return 0;
    return (total - elapsed) / 1000UL;
}

const char* pomodoro_phase_name(PomodoroPhase phase) {
    switch (phase) {
        case PomodoroPhase::FOCUS:   return "focus";
        case PomodoroPhase::WRAP_UP: return "wrap_up";
        case PomodoroPhase::BREAK:   return "break";
        case PomodoroPhase::IDLE:
        default:                     return "idle";
    }
}

void pomodoro_loop() {
    if (current_phase == PomodoroPhase::IDLE) return;

    unsigned long elapsed = millis() - phase_started_at;
    if (elapsed < duration_for(current_phase)) return;

    switch (current_phase) {
        case PomodoroPhase::FOCUS:   enter_phase(PomodoroPhase::WRAP_UP); break;
        case PomodoroPhase::WRAP_UP: enter_phase(PomodoroPhase::BREAK);   break;
        case PomodoroPhase::BREAK:   enter_phase(PomodoroPhase::IDLE);    break;
        case PomodoroPhase::IDLE:    break;
    }
}