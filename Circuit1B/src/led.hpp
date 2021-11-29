#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>
#include "writer.hpp"

class Led {
    const int pin;
    bool ledOn;
    const int *interval;
    int timeLapsed;

public:
    Led(const int pin, const int *interval):
    pin(pin), ledOn(false), interval(interval),  timeLapsed(0) {
        pinMode(this->pin, OUTPUT);
    }
    ~Led() = default;

    void update(const unsigned int time){
        timeLapsed += (int)time; 
        if(timeLapsed < *interval) return; // nothing to do
        timeLapsed = 0;
        if(ledOn) digitalWrite(pin, LOW);
        else digitalWrite(pin, HIGH);
        ledOn = !ledOn;
    }
};

class LedFlipped {
    const int pin;
    bool ledOn;
    const int *interval;
    int timeLapsed;

public:
    LedFlipped(const int pin, const int *interval):
    pin(pin), ledOn(false), interval(interval),  timeLapsed(0) {
        pinMode(this->pin, OUTPUT);
    }
    ~LedFlipped() = default;

    void update(const unsigned int time){
        // the max value of the pot is 1024, so we are flipping direction
        const unsigned int flippedInterval = 1024 - *interval; 
        timeLapsed += (int)time; 
        if(timeLapsed < flippedInterval) return; // nothing to do
        timeLapsed = 0;
        if(ledOn) digitalWrite(pin, LOW);
        else digitalWrite(pin, HIGH);
        ledOn = !ledOn;
    }
};

class DimmableLed {
    const int pin;
    const int *setting;
    bool ledOn;
    int *interval;
    const int *onInterval;
    int timeLapsed;

public:
    DimmableLed(const int pin, const int *setting):
    pin(pin), setting(setting), ledOn(false), onInterval(new int(2)), timeLapsed(0) {
        pinMode(this->pin, OUTPUT);
    }
    ~DimmableLed() {
        delete onInterval;
    }

    void update(const unsigned int time){
        timeLapsed += (int)time; 
        if(timeLapsed < (*interval * 10)) return; // nothing to do
        timeLapsed = 0;
        if(ledOn) { 
            digitalWrite(pin, LOW);
            interval = setting;
        } else {
            digitalWrite(pin, HIGH);
            interval = onInterval;
        }
        ledOn = !ledOn;
    }

    int getOffInterval() { return *setting * 10; }
};

class DimmableLedPrinter: public Printer {
    DimmableLed &led;
public:
    DimmableLedPrinter(DimmableLed &led): led(led) {}
    ~DimmableLedPrinter() = default;
    
    void print() override {
        Serial.print("Led Off Interval Us: ");
        Serial.println(led.getOffInterval());
    }
};

class DimmableLedFlipped {
    const int pin;
    const int *setting;
    bool ledOn;
    int *interval;
    const int *onInterval;
    int timeLapsed;

public:
    DimmableLedFlipped(const int pin, const int *setting):
    pin(pin), setting(setting), ledOn(false), onInterval(new int(2)), timeLapsed(0) {
        pinMode(this->pin, OUTPUT);
    }
    ~DimmableLedFlipped() {
        delete onInterval;
    }

    void update(const unsigned int time){
        const unsigned int intervalFlipped = ledOn ? *interval : (10240 - (*interval * 10));
        timeLapsed += (int)time; 
        if(timeLapsed < intervalFlipped) return; // nothing to do
        timeLapsed = 0;
        if(ledOn) { 
            digitalWrite(pin, LOW);
            interval = setting;
        } else {
            digitalWrite(pin, HIGH);
            interval = onInterval;
        }
        ledOn = !ledOn;
    }

    int getOffInterval() { return *setting * 10; }
};

#endif  // LED_HPP
