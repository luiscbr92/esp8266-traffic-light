#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>
#include "light.h"

enum class PomodoroPhase {
    IDLE,
    FOCUS,
    WRAP_UP,
    BREAK
};

void pomodoro_setup();
void pomodoro_loop();
void pomodoro_start();
void pomodoro_stop();
bool pomodoro_is_running();
PomodoroPhase pomodoro_current_phase();
unsigned long pomodoro_remaining_seconds();
const char* pomodoro_phase_name(PomodoroPhase phase);

#endif