#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

static const unsigned long TMR_MAX_ULONG = 0xFFFFFFFF;

struct TimedActionMs {
    String name;
    bool active;
    unsigned int delta;
    unsigned int interval;
    void(*action)();
    TimedActionMs *next;
};

struct TimedActionMs TMR_buildActionMs(String name, unsigned int interval, void(*a)(), bool active) {
    struct TimedActionMs action;
    action.name = name;
    action.active = active;
    action.interval = interval;
    action.action = a;
    return action;
}


struct TimerState {
    unsigned long previousMs;
    unsigned int deltaMs;
    TimedActionMs *actions;
} timerState;

TimedActionMs* TMR_update(TimedActionMs *action) {
    if(not action->active) return action->next;
    action->delta += timerState.deltaMs;
    if(action->delta < action->interval) return action->next;
    action->action();
    action->delta = 0;
    return action->next;
}

void TMR_tick() {
    unsigned long timeMs = millis();
    if(timeMs < timerState.previousMs) 
        timerState.deltaMs = (unsigned int)(timeMs + (TMR_MAX_ULONG - timerState.previousMs));
    else timerState.deltaMs = (unsigned int)(timeMs - timerState.previousMs);
    timerState.previousMs = timeMs;
    TimedActionMs *action = timerState.actions;
    while(action) action = TMR_update(action);
}

#endif // TIMER_H