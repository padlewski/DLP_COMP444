#ifndef POTENTIOMETER_HPP
#define POTENTIOMETER_HPP

#include <Arduino.h>
#include <writer.hpp>

class Potentiometer {
    const int pin;
    int* value;

public:
    Potentiometer(int pin): pin(pin), value(new int()) {}

    int read(){
        *value = analogRead(pin);
        return *value;
    }

    const int * getValue() {
        return value;
    }
};

/*
 * A class to print out information related to the potentiometer.
 */
class PotPrinter: public Printer {
    Potentiometer &pot;
public:
    PotPrinter(Potentiometer &pot): pot(pot) {}
    ~PotPrinter() = default;
    
    void print() override {
        Serial.print("Potentiometer Value: ");
        Serial.println(*pot.getValue());
    }
};

#endif // POTENTIOMETER_HPP