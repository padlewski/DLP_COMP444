#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

struct TimerState {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
    #ifdef _CALC_STATS_
    unsigned int size;
    unsigned int i;
    unsigned int *statsDeltaUs;
    unsigned int *statsDeltaMs;
    #endif
};

TimerState initTimerState(unsigned int size){
    return TimerState {
        0, 0, 0, 0
        #ifdef _CALC_STATS_
        , size, 0,
        (unsigned int*) calloc(size, sizeof(unsigned int)),
        (unsigned int*) calloc(size, sizeof(unsigned int))
        #endif
    };
}

#ifdef _CALC_STATS_
void calcStats(TimerState *ts){
    ++ts->i;
    if(ts->i >= ts->size) ts->i = 0;
    ts->statsDeltaMs[ts->i] = ts->deltaMs;
    ts->statsDeltaUs[ts->i] = ts->deltaUs;
}
#endif

void tick(TimerState *ts){
    unsigned long timeMs = millis();
    unsigned long timeUs = micros();
    // TODO: Handle overflow
    ts->deltaMs = (unsigned int)(timeMs - ts->previousMs);
    ts->deltaUs = (unsigned int)(timeUs - ts->previousUs);
    ts->previousMs = timeMs;
    ts->previousUs = timeUs;
    #ifdef _CALC_STATS_
    calcStats(ts);  
    #endif
}

#ifdef _CALC_STATS_
unsigned int getRate(unsigned int vals[], const int cnt, unsigned int def) {
    if(not cnt) return def;
    unsigned int avg;
    for(unsigned int i = 0 ; i < cnt ; ++i) avg += vals[i];
    return avg / cnt; 
}

unsigned int getRateUs(TimerState *ts){
    return getRate(ts->statsDeltaUs, ts->i, ts->deltaUs);
}

unsigned int getRateMs(TimerState *ts){
    return getRate(ts->statsDeltaMs, ts->i, ts->deltaMs);
}
#endif // _CALC_STATS_

#endif // TIMER_H