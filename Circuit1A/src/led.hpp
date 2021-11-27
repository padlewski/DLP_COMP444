#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>

class Led {
    byte pin;
    bool ledOn;
    unsigned int timeLapsed;
    unsigned int intervalOn;
    unsigned int intervalOff;
    unsigned int interval;

public:
    Led(byte pin, unsigned int interval);
    Led(byte pin, unsigned int intervalOn, unsigned int intervalOff);
    ~Led() = default;

    void update(const unsigned int);
};

#endif  // LED_HPP
