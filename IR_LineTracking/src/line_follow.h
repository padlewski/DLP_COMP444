#ifndef LINE_FILLOW_H
#define LINE_FILLOW_H

#include "ir_line.h"
#include "timer.h"
#include "mecanum.h"

// Forward declarations
void calibrate(IrLineSensor *sensor);
bool calibrateIr(IrLineSensor *sensor);

struct FollowState{

};

void FollowUpdate(FollowState *state, IrLineSensor *sensor) {
    
}

void calibrate(IrLineSensor *sensor){
    if(!calibrateIr(sensor)){
        // should do something
    }
}

void copyVals(int src[IR_NUM_SENSORS], int dest[IR_NUM_SENSORS][IR_NUM_SENSORS], int row) {
    ITERATE_IR dest[row][IR_i] = src[IR_i];
}

int findOn(IrLineSensor *state) {
    int on;
    uint8_t mask = 1;
    ITERATE_IR {
        if(state->raw[IR_i] & mask & state->hiMode) on = IR_i;
        mask = mask << 1;
    }
    return on;
}

void shiftTo(uint8_t ir, IrLineSensor *state) {
    updateIrLineSensor(state);
    int on = findOn(state);
    while(on != ir) {
        if(on > ir) M_move(LEFT, 100);
        else M_move(RIGHT, 100);
    }
    M_move(FORWARD, (int)0); // stop we are over the ir 
}

bool calibrateIr(IrLineSensor *sensor){
    // create a matrix to store some calibrations
    int measurements[IR_NUM_SENSORS][IR_NUM_SENSORS];
    updateIrLineSensor(sensor);
    for(int i = 0 ; i < IR_NUM_SENSORS ; ++i) {
        shiftTo(i, sensor);
        updateIrLineSensor(sensor);
        copyVals(sensor->raw, measurements, i);
    }
    return true;
}

#endif // LINE_FILLOW_H