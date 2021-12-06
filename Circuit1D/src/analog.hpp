#ifndef ANALOG_HPP
#define ANALOG_HPP

#include <Arduino.h>
#include <writer.hpp>

class Analog {
    const int pin;
    String type;
    int* value;
    unsigned int interval;
    unsigned int timeLapsed;

public:
    Analog(int pin, unsigned int interval, String type): 
    pin(pin), interval(interval), type(type), value(new int()) {}
    
    ~Analog() = default;

    void update(unsigned int time) {
        timeLapsed += time;
        if(timeLapsed < interval) return;
        timeLapsed = 0;
        read();
    }

    int read(){
        *value = analogRead(pin);
        return *value;
    }

    // Gets a pointer to the current value in the photoresistor
    const int * getValue() { return value; }
    const String &getType() { return type; }
};

/*
 * A class to print out information related to the photoresistor.
 */
class AnalogPrinter: public Printer {
    Analog &analog;
public:
    AnalogPrinter(Analog &analog): 
    analog(analog) {}

    ~AnalogPrinter() = default;
    
    void print() override {
        Serial.print(analog.getType() + " Value: ");
        Serial.println(*analog.getValue());
    }
};

#endif // ANALOG_HPP