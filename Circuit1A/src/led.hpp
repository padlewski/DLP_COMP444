#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>

class led {
    byte pin;
    bool ledOn;
    unsigned int timeLapsed;
    unsigned int intervalOn;
    unsigned int intervalOff;
    unsigned int interval;

public:
    led(byte pin, unsigned int interval);
    led(byte pin, unsigned int intervalOn, unsigned int intervalOff);
    ~led() = default;

    void update(const unsigned int);
};

#endif  // LED_HPP
