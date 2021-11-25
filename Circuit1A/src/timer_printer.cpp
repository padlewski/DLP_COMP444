#include <Arduino.h>
#include "timer.hpp"
#include "writer.hpp"

/*
 * A class to 
 */
class timer_printer: public printer {
    timer& _timer;
public:
    timer_printer(timer& timer): _timer(timer) {}
    ~timer_printer() = default;
    void print() {
        Serial.print("Timer deltaUs: ");
        Serial.println(_timer.getDeltaUs());
    }
};
