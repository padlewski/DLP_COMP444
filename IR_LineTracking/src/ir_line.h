#ifndef IR_LINE_H
#define IR_LINE_H

#include <Arduino.h>
/*
The code for the 5 channel ir line tracking sensor
https://www.waveshare.com/wiki/Tracker_Sensor
*/

static const byte IR_NUM_SENSORS = 5;
static const byte IR_STATUS_MASK = B00011111;
static const byte IR_RIGHT_MASK = B00011100;
static const byte IR_LEFT_MASK =B111;
static int IR_i;

#define ITERATE_IR for(IR_i = 0 ; IR_i < IR_NUM_SENSORS ; ++IR_i)

struct IrLineSensor {
    byte irIn[IR_NUM_SENSORS] = {A0,A1,A2,A3,A4};
    int raw[IR_NUM_SENSORS];
    int normalized[IR_NUM_SENSORS];
    int min[IR_NUM_SENSORS];
    int max[IR_NUM_SENSORS];
    int background[IR_NUM_SENSORS];
    int threshold[IR_NUM_SENSORS];
    int backlash;
    bool hiMode; // true if light background (dark line)
    byte status;
} lineSensor;

void IR_coldStart(IrLineSensor*);
void IR_read(IrLineSensor*);

void IR_init(void) {
    lineSensor.hiMode = true;
    ITERATE_IR {
        pinMode(lineSensor.irIn[IR_i], INPUT);
        lineSensor.max[IR_i] = 0;
        lineSensor.min[IR_i] = 1024;
        lineSensor.threshold[IR_i] = 0;
        lineSensor.backlash = 500;
    }
}

void IR_normalize(int ir) {
    lineSensor.normalized[ir] = map(lineSensor.raw[ir], 0, 1023, lineSensor.min[ir], lineSensor.max[ir]);
    lineSensor.threshold[ir] = map(lineSensor.backlash, 0, 1023, lineSensor.min[ir], lineSensor.max[ir]);
}

byte IR_update(void) {
    lineSensor.status = 0;
    for(IR_i = 4; IR_i > -1 ; --IR_i){
        lineSensor.raw[IR_i] = analogRead(lineSensor.irIn[IR_i]);
        // update high and low
        if(lineSensor.raw[IR_i] > lineSensor.max[IR_i]) lineSensor.max[IR_i] = lineSensor.raw[IR_i];
        if(lineSensor.raw[IR_i] < lineSensor.min[IR_i]) lineSensor.min[IR_i] = lineSensor.raw[IR_i];
        IR_normalize(IR_i);
        lineSensor.status = (lineSensor.status << 1) | (lineSensor.raw[IR_i] < lineSensor.threshold[IR_i]);// & sensor->hiMode );
    }
    if(!lineSensor.hiMode) lineSensor.status = ~lineSensor.status;
    return lineSensor.status;
}

void IR_sample(void) {
    ITERATE_IR {
        lineSensor.raw[IR_i] = analogRead(lineSensor.irIn[IR_i]);
        if(lineSensor.raw[IR_i] > lineSensor.max[IR_i]) lineSensor.max[IR_i] = lineSensor.raw[IR_i];
        if(lineSensor.raw[IR_i] < lineSensor.min[IR_i]) lineSensor.min[IR_i] = lineSensor.raw[IR_i];
    }
}

void IR_coldStart() {
    for (int i = 0 ; i < 10 ; ++i) IR_sample();
    int minV = 1024;
    int maxV = 0;
    ITERATE_IR {
        if(maxV < lineSensor.raw[IR_i]){
            maxV = lineSensor.raw[IR_i];
        }
        if(minV > lineSensor.raw[IR_i]){
            minV = lineSensor.raw[IR_i];
        }
    }
}

void IR_read(void) {
    ITERATE_IR lineSensor.raw[IR_i] = analogRead(lineSensor.irIn[IR_i]);
}

static const uint8_t IR_IS_CENTERED = B00011110; //30

// Use weighted average to get position
// if result = 30 we are centered, < 30 Left of center 
// > 30 Right of center
// adapted from https://www.waveshare.com/wiki/Tracker_Sensor
uint8_t IR_leftOrRight(uint8_t *state) {
    uint8_t m = B00000001;
    uint8_t r = 0;
    uint8_t c = 0;
    ITERATE_IR {
        if(m & *state) { 
            r += (IR_i + 1) * 10;
            ++c;
        }
        m = m << 1;
    }
    return c ? r / c : 0;
}

bool IR_isCentered(byte *state) {
    return IR_leftOrRight(state) == IR_IS_CENTERED;
}

bool IR_isOffLine(byte *state) {
    return (*state & IR_STATUS_MASK) == 0;
}

bool IR_isIntersection(byte *state) {
    return ((*state & IR_LEFT_MASK) == IR_LEFT_MASK) || ((*state & IR_RIGHT_MASK) == IR_RIGHT_MASK);
}

#endif // IR_LINE_H