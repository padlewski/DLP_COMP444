#include <Arduino.h>
#include "led.hpp"

led::led(byte pin, unsigned int interval):
pin(pin), intervalOn(interval), intervalOff(interval), 
ledOn(false), interval(intervalOff), timeLapsed(0) {
    pinMode(this->pin, OUTPUT);
}

led::led(byte pin, unsigned int intervalOn, unsigned int intervalOff):
pin(pin), intervalOn(intervalOn), intervalOff(intervalOff), 
ledOn(false), interval(intervalOff), timeLapsed(0) {
    pinMode(this->pin, OUTPUT);
}

void led::update(const unsigned int time) {
    timeLapsed += time;
    if(timeLapsed < interval) return; // nothing to do
    timeLapsed = 0;
    if(ledOn) {
        digitalWrite(pin, LOW);
        interval = intervalOff;
    } else {
        digitalWrite(pin, HIGH);
        interval = intervalOn;
    }
    ledOn = !ledOn;
}
