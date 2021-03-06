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

long getSeconds() {
    return millis() / 1000;
}

String formatHHMMSS(long timeMs) {
    const long SH = 3600;
    const long SM = 60;
    const long MS = 1000;

    timeMs = timeMs / MS;
    long HH = timeMs / SH;
    long MM = (timeMs -= HH * SH) / SM;
    long SS = timeMs - MM * SM;
    char buff[16];
    snprintf(buff, 16, "%9.01ld:%02ld:%02ld", HH, MM, SS);
    return String(buff);
}

#endif // TIMER_H