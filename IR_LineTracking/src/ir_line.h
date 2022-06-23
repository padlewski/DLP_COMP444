#ifndef IR_LINE_H
#define IR_LINE_H

#include <Arduino.h>
/*
The code for the 5 channel ir line tracking sensor
https://www.waveshare.com/wiki/Tracker_Sensor
*/

static const uint8_t IR_NUM_SENSORS = 5;
static const uint8_t IR_STATUS_MASK = B00011111;
static int IR_i;

#define ITERATE_IR for(IR_i = 0 ; IR_i < IR_NUM_SENSORS ; ++IR_i)

struct IrLineSensor {
    uint8_t *irIn;
    int raw[IR_NUM_SENSORS];
    int normalized[IR_NUM_SENSORS];
    int min[IR_NUM_SENSORS];
    int max[IR_NUM_SENSORS];
    int background[IR_NUM_SENSORS];
    int threshold[IR_NUM_SENSORS];
    int backlash;
    bool hiMode; // true if light background (dark line)
    uint8_t status;
};

void IR_coldStart(IrLineSensor*);
void IR_read(IrLineSensor*);

IrLineSensor IR_init(uint8_t *irIn, bool mode = true) {
    IrLineSensor sensor;
    sensor.irIn = irIn;
    sensor.hiMode = mode;
    ITERATE_IR {
        pinMode(irIn[IR_i], INPUT);
        sensor.max[IR_i] = 0;
        sensor.min[IR_i] = 1024;
        sensor.threshold[IR_i] = 0;
        sensor.backlash = 800;
    }
    return sensor;
}

void IR_normalize(IrLineSensor *sensor, int ir) {
    if(not (sensor->max[ir] && sensor->min[ir])) {
        // we haven't calibrated, just use raw
        sensor->normalized[ir] = sensor->raw[ir];
        return;
    }
    sensor->normalized[ir] = map(sensor->raw[ir], 0, 1023, sensor->min[ir], sensor->max[ir]);
    sensor->threshold[ir] = map(sensor->backlash, 0, 1023, sensor->min[ir], sensor->max[ir]);
}

uint8_t IR_update(IrLineSensor *sensor) {
    sensor->status = 0;
    for(IR_i = 4; IR_i > -1 ; --IR_i){
        sensor->raw[IR_i] = analogRead(sensor->irIn[IR_i]);
        // update high and low
        if(sensor->raw[IR_i] > sensor->max[IR_i]) sensor->max[IR_i] = sensor->raw[IR_i];
        if(sensor->raw[IR_i] < sensor->min[IR_i]) sensor->min[IR_i] = sensor->raw[IR_i];
        IR_normalize(sensor, IR_i);
        sensor->status = (sensor->status << 1) | (sensor->raw[IR_i] < sensor->threshold[IR_i]);// & sensor->hiMode );
    }
    if(!sensor->hiMode) sensor->status = ~sensor->status;
    return sensor->status;
}

void IR_sample(IrLineSensor *sensor) {
    ITERATE_IR {
        sensor->raw[IR_i] = analogRead(sensor->irIn[IR_i]);
        if(sensor->raw[IR_i] > sensor->max[IR_i]) sensor->max[IR_i] = sensor->raw[IR_i];
        if(sensor->raw[IR_i] < sensor->min[IR_i]) sensor->min[IR_i] = sensor->raw[IR_i];
    }
}

void IR_coldStart(IrLineSensor *sensor) {
    for (int i = 0 ; i < 10 ; ++i) IR_sample(sensor);
    int minV = 1024;
    int maxV = 0;
    ITERATE_IR {
        if(maxV < sensor->raw[IR_i]){
            maxV = sensor->raw[IR_i];
        }
        if(minV > sensor->raw[IR_i]){
            minV = sensor->raw[IR_i];
        }
    }
}

void IR_read(IrLineSensor *sensor) {
    ITERATE_IR sensor->raw[IR_i] = analogRead(sensor->irIn[IR_i]);
}

static const uint8_t IR_IS_CENTERED = B00010100;

// Use weighted average to get position
// if result = 20 we are centered, < 20 right of center 
// > 20 left of center
// adapted from https://www.waveshare.com/wiki/Tracker_Sensor
uint8_t IR_leftOrRight(uint8_t *state) {
    uint8_t m = B00000001;
    uint8_t r = 0;
    uint8_t c = 0;
    ITERATE_IR {
        if(m & *state) { 
            r += IR_i * 10;
            ++c;
        }
        m = m << 1;
    }
    return r / c;
}

bool IR_isCentered(uint8_t *state) {
    return IR_leftOrRight(state) == IR_IS_CENTERED;
}


#endif // IR_LINE_H