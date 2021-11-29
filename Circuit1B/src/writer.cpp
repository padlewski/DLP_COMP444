#include <Arduino.h>
#include "Writer.hpp"

Writer::Writer(unsigned int interval, Printer** printers, int size): 
timeLapsed(0), interval(interval), printers(printers), size(size) {}

void Writer::update(const unsigned int time){
    timeLapsed += time;
    if(timeLapsed < interval) return;
    timeLapsed = 0;
    for(int i = 0 ; i < size ; ++i) printers[i]->print();
}
