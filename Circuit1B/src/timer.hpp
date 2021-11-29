#ifndef TIMER_HPP
#define TIMER_HPP

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
    Timer();
    Timer(unsigned int statsResolution);
    ~Timer();

    void tick();
    const unsigned int &tickUs();
    const unsigned int &tickMs();
    const unsigned int &getDeltaUs();
    const unsigned int &getDeltaMs();
    unsigned int getRateUs();
    unsigned int getRateMs();
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
