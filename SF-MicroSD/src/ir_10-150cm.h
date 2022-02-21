#ifndef IR_10_150CM.H
#define IR_10_150CM.H

#include <Arduino.h>

struct IR {
    const byte pin;
    const byte vcc;
    double volts;
    double cm;
};

const double const_5v = 64.106;
const double pow_5v = -1.603;

IR init(byte pin, byte vcc) {
    return (IR) {pin, vcc, 0, 0 };
}

void read(IR *ir) {
    ir->volts = 0;
    for(int i = 0 ; i < 10 ; ++i){
        ir->volts += analogRead(ir->pin);
    }
    ir->volts /= 10.0;
    ir->volts = ir->volts * (5.0f / 1023.0);
    ir->cm = const_5v * pow(ir->volts, pow_5v);
}

#endif // IR_10_150CM.H