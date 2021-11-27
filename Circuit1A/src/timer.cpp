#include <Arduino.h>
#include "Timer.hpp"

Timer::Timer():
previousMs(0), previousUs(0), deltaUs(0), deltaMs(0), size(30), i(0),
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
    ++i;
    if(i >= size) i = 0;
    statsDeltaMs[i] = deltaMs;
    statsDeltaUs[i] = deltaUs;
    for(unsigned int x = 0 ; x < size; ++x){
        statsDeltaMs[x] = 0;
        statsDeltaUs[x] = 0;
    }
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
    unsigned int avg;
    for(unsigned int i = 0 ; i < size ; ++i) {
        avg += statsDeltaUs[i];
    }
    return avg / size; 
}

unsigned int Timer::getRateMs(){
    unsigned int avg;
    for(unsigned int i = 0 ; i < size ; ++i) {
        avg += statsDeltaMs[i];
    }
    return avg / size; 
}
