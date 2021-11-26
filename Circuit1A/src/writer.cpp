#include <Arduino.h>
#include "Writer.hpp"

Writer::Writer(unsigned int interval, Printer& printer): 
interval(interval), timeLapsed(0), _printer(printer) {}

void Writer::update(const unsigned int time){
    timeLapsed += time;
    if(timeLapsed < interval) return;
    timeLapsed -= interval;
    _printer.print();
}
