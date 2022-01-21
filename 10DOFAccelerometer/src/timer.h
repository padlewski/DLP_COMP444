#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

struct TimerState {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
} timerState;

void tick(){
    unsigned long timeMs = millis();
    unsigned long timeUs = micros();
    // TODO: Handle overflow
    timerState.deltaMs = (unsigned int)(timeMs - timerState.previousMs);
    timerState.deltaUs = (unsigned int)(timeUs - timerState.previousUs);
    timerState.previousMs = timeMs;
    timerState.previousUs = timeUs;
}

#endif // TIMER_H