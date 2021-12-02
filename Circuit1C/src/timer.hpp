#ifndef TIMER_HPP
#define TIMER_HPP

#include <Arduino.h>
#include "writer.hpp"

// Modifed to update stats only if statsResolution is provided
class Timer {
    unsigned long previousUs;
    unsigned long previousMs;
    unsigned int deltaUs;
    unsigned int deltaMs;
    unsigned int size;
    unsigned int i;
    unsigned int *statsDeltaUs;
    unsigned int *statsDeltaMs;

public:
    Timer():
    previousUs(0), previousMs(0), deltaUs(0), deltaMs(0), size(0), i(0),
    statsDeltaUs(new unsigned int[1]()),
    statsDeltaMs(new unsigned int[1]()) {}

    Timer(unsigned int statsResolution):
    previousUs(0), previousMs(0), deltaUs(0), deltaMs(0), size(statsResolution), i(0),
    statsDeltaUs(new unsigned int[this->size]()),
    statsDeltaMs(new unsigned int[this->size]()) {}

    ~Timer() {
        delete[] statsDeltaUs;
        delete[] statsDeltaMs;
    }

    void tick() {
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

    const unsigned int &tickUs() {
        tick();
        return deltaUs;
    }

    const unsigned int &tickMs() {
        tick();
        return deltaMs;
    }

    const unsigned int &getDeltaUs() { return deltaUs; }

    const unsigned int &getDeltaMs() { return deltaMs; }

    unsigned int getRateUs() {
        if(not size) return deltaUs;
        unsigned int avg;
        for(unsigned int i = 0 ; i < size ; ++i) {
            avg += statsDeltaUs[i];
        }
        return avg / size; 
    }

    unsigned int getRateMs() {
        if(not size) return deltaMs;
        unsigned int avg;
        for(unsigned int i = 0 ; i < size ; ++i) {
            avg += statsDeltaMs[i];
        }
        return avg / size; 
    }
};

/*
 * A class to print out information related to the 
 * timer.
 */
class TimerPrinter: public Printer {
    Timer& timer;
public:
    TimerPrinter(Timer& timer): timer(timer) {}
    ~TimerPrinter() = default;
    void print() override {
        Serial.print("Timer deltaUs: ");
        Serial.print(timer.getRateUs());
        Serial.print(" | Timer deltaMs: ");
        Serial.println(timer.getRateMs());
    }
};

#endif //TIMER_HPP
