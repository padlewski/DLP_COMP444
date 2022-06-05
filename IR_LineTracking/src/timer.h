#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

static const unsigned long TMR_MAX_ULONG = 0xFFFFFFFF;

struct TimedActionMs {
    String name;
    bool active;
    unsigned long previous;
    unsigned int delta;
    unsigned int interval;
    void(*action)();
    TimedActionMs *next;
};

struct TimedActionMs buildActionMs(String name, unsigned int interval, void(*a)(), bool active) {
    struct TimedActionMs action;
    action.name = name;
    action.active = active;
    action.interval = interval;
    action.action = a;
    return action;
}


struct TimerState {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
    TimedActionMs *actions;
} timerState;

TimedActionMs* TMR_update(TimedActionMs *action) {
    if(not action->active) return action->next;
    unsigned long timeMs = millis();
    if(timeMs < action->previous) 
        action->delta += (unsigned int)(timeMs + (TMR_MAX_ULONG - action->previous));
    else action->delta += (unsigned int)(timeMs - action->previous);
    action->previous = timeMs;
    if(action->delta < action->interval) return action->next;
    action->delta -= action->interval;
    action->action();
    return action->next;
}

void TMR_tick() {
    unsigned long timeMs = millis();
    unsigned long timeUs = micros();
    if(timeMs < timerState.previousMs) 
        timerState.deltaMs = (unsigned int)(timeMs + (TMR_MAX_ULONG - timerState.previousMs));
    else timerState.deltaMs = (unsigned int)(timeMs - timerState.previousMs);
    if(timeUs < timerState.previousUs) 
        timerState.deltaUs = (unsigned int)(timeUs + (TMR_MAX_ULONG - timerState.previousUs));
    else timerState.deltaUs = (unsigned int)(timeUs - timerState.previousUs);
    timerState.previousMs = timeMs;
    timerState.previousUs = timeUs;
    TimedActionMs *action = timerState.actions;
    while(action) action = TMR_update(action);
}

#endif // TIMER_H