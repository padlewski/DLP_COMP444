#include <Arduino.h>
#include "Timer.hpp"
#include "Writer.hpp"

/*
 * A class to print out information related to the 
 * timer.
 */
class TimerPrinter: public Printer {
    Timer& _timer;
public:
    TimerPrinter(Timer& timer): _timer(timer) {}
    ~TimerPrinter() = default;
    void print() {
        Serial.print("Timer deltaUs: ");
        Serial.println(_timer.getDeltaUs());
    }
};
