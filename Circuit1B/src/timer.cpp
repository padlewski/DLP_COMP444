#include <Arduino.h>
#include "Timer.hpp"

Timer::Timer():
previousUs(0), previousMs(0), deltaUs(0), deltaMs(0), size(0), i(0),
statsDeltaUs(new unsigned int[1]()),
statsDeltaMs(new unsigned int[1]()) {}

Timer::Timer(unsigned int statsResolution):
previousUs(0), previousMs(0), deltaUs(0), deltaMs(0), size(statsResolution), i(0),
statsDeltaUs(new unsigned int[this->size]()),
statsDeltaMs(new unsigned int[this->size]()) {}

Timer::~Timer(){
    delete[] statsDeltaUs;
    delete[] statsDeltaMs;
}

void Timer::tick(){
    unsigned long timeMs = millis();
    unsigned long timeUs = micros();
    // TODO: Handle overflow
    deltaMs = (unsigned int)(timeMs - previousMs);
    deltaUs = (unsigned int)(timeUs - previousUs);
    previousMs = timeMs;
    previousUs = timeUs;
    if(not size) return; // We don't need to update stats 
    ++i;
    if(i >= size) i = 0;
    statsDeltaMs[i] = deltaMs;
    statsDeltaUs[i] = deltaUs;
}

const unsigned int &Timer::tickMs() {
    tick();
    return deltaMs;
}

const unsigned int &Timer::tickUs() {
    tick();
    return deltaUs;
}

const unsigned int &Timer::getDeltaUs(){
    return deltaUs;
}

const unsigned int &Timer::getDeltaMs(){
    return deltaMs;
}

unsigned int Timer::getRateUs(){
    if(not size) return deltaUs;
    unsigned int avg;
    for(unsigned int i = 0 ; i < size ; ++i) {
        avg += statsDeltaUs[i];
    }
    return avg / size; 
}

unsigned int Timer::getRateMs(){
    if(not size) return deltaMs;
    unsigned int avg;
    for(unsigned int i = 0 ; i < size ; ++i) {
        avg += statsDeltaMs[i];
    }
    return avg / size; 
}
