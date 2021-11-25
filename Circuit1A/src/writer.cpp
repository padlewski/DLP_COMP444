#include <Arduino.h>
#include "writer.hpp"

writer::writer(unsigned int interval, printer& printer): 
interval(interval), timeLapsed(0), _printer(printer) {}

void writer::update(const unsigned int time){
    timeLapsed += time;
    if(timeLapsed < interval) return;
    timeLapsed -= interval;
    _printer.print();
}
