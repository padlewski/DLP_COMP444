#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

struct TimerState {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
};

TimerState initTimerState(unsigned int size){
    return TimerState {
        0, 0, 0, 0
    };
}

void tick(TimerState *ts){
    unsigned long timeMs = millis();
    unsigned long timeUs = micros();
    // TODO: Handle overflow
    ts->deltaMs = (unsigned int)(timeMs - ts->previousMs);
    ts->deltaUs = (unsigned int)(timeUs - ts->previousUs);
    ts->previousMs = timeMs;
    ts->previousUs = timeUs;
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