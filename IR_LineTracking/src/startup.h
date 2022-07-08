#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "actions.h"
#include "ir_line.h"

static const byte STARTUP_STATE_COMPLETE = B00000111;
static const String STARTUP_STATE_NAME = "STARTUP";
static const int STARTUP_SPEEDS[M_N] = {65, 65, 65, 65};

// actions
void SUA_updateIr(void);
void SUA_updateMag(void);
void SUA_checkState(void);
void SUA_onLineCallback(void);

struct StartupState {
    byte lastKnownLinePosition;
    // setup some memory for movement
    byte mSize = 3;
    byte directions[3] = {0,0,0};
    int speeds[3][M_N] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    unsigned int times[3] = {0,0,0};
    byte mode;
    TimedActionMs *current;
} SU_State;

struct TimedActionMs SU_ActionCheckState = TMR_buildActionMs("check state", 50, &SUA_checkState, true);

// initialize setup and return our action head
TimedActionMs* SU_init(void) {
    SU_State.mode = 0; // Mode 0 start
    // setup the timed move action
    actionMoveSequenceState.size = SU_State.mSize;
    actionMoveSequenceState.directions =  SU_State.directions;
    actionMoveSequenceState.speeds = SU_State.speeds;
    actionMoveSequenceState.times = SU_State.times;
    // setup the until action (for reorienting the bot)
    actionMoveUntilState.check = &checkIsIrCentered;
    actionMoveUntilState.run = &SUA_onLineCallback;
    actionMoveUntilState.until = &untilNoop;
    return &SU_ActionCheckState;
}

// setup and start ir calibration
void SU_setIrCal(void) {
    SU_State.directions[0] = Left;
    SU_State.directions[1] = Right;
    SU_State.directions[2] = Forward;
    memcpy(SU_State.speeds[0], STARTUP_SPEEDS, SIZE_SPEEDS);
    memcpy(SU_State.speeds[1], STARTUP_SPEEDS, SIZE_SPEEDS);
    // leave the last speed as a stop
    SU_State.times[0] = 1500;
    SU_State.times[1] = 2500;
    SU_State.times[2] = 10000;
    SU_State.mode = 1; // Mode 1 calibrate line sensor
    doMoveSequence.delta = actionMoveSequenceState.times[0];
    doMoveSequence.active = true;
}

// check the state of the ir calibration
void SU_checkIrCal(void) {
    if(actionMoveSequenceState.count < actionMoveSequenceState.size) return;
    ++SU_State.mode; // set the next state (mode 2 re-orient on the line)
    // re-orientate back onto the line
    doMoveSequence.active = false; // disable movement
    M_stop();
    if(IR_leftOrRight(&lineSensor.status) == IR_IS_CENTERED) { 
        ++SU_State.mode; // Mode 3 On line
        return; // we are centered
    }
    memcpy(actionMoveUntilState.speed, STARTUP_SPEEDS, SIZE_SPEEDS);
    actionMoveUntilState.direction = IR_leftOrRight(&SU_State.lastKnownLinePosition) < IR_IS_CENTERED ? Left : Right;
    actionMoveUntilState.until = &untilCenteredStrafe;
    doMoveUntil.active = true;
}

// setup and start Magnetometer calibration
void SU_setMagCal(void) {
    SU_State.directions[0] = Clockwise;
    SU_State.directions[1] = Counter;
    SU_State.directions[2] = Forward;
    // Leave speeds as is
    SU_State.times[0] = 12000;
    SU_State.times[1] = 12000;
    actionMoveSequenceState.count = 0;
    actionMoveSequenceState.index = 0;
    doMoveSequence.delta = actionMoveSequenceState.times[0];
    ++SU_State.mode; // mode 4
    doMoveSequence.active = true;
}

void SU_checkMagCal(void) {
    if(actionMoveSequenceState.count < actionMoveSequenceState.size) return;
    ++SU_State.mode; // Mode 5
    doMoveSequence.active = false; // disable movement
    M_stop();
    if(IR_isCentered(&lineSensor.status)) { 
        ++SU_State.mode; // Mode 6 On line
        return; // we are centered
    }
    actionMoveUntilState.direction = IR_leftOrRight(&SU_State.lastKnownLinePosition) < IR_IS_CENTERED ? Counter : Clockwise;
    actionMoveUntilState.until = &untilCenteredRotate;
    doMoveUntil.active = true;
}

// we have finished the setup 
void SU_setComplete(void) {
    ++SU_State.mode; // Mode 7 all done
    SU_ActionCheckState.active = false;
}

void SUA_checkState(void) {
    if(IR_STATUS_MASK & lineSensor.status) 
        SU_State.lastKnownLinePosition = lineSensor.status;
    switch(SU_State.mode) {
        case 0:
            SU_setIrCal();
            break;
        case 1:
            SU_checkIrCal();
            break;
        case 3:
            SU_setMagCal();
            break;
        case 4:
            SU_checkMagCal();
            break;
        case 6:
            SU_setComplete();
            break;
    }
}

void SUA_onLineCallback(void) {
    doMoveUntil.active = false;
    M_stop();
    ++SU_State.mode;
}

#endif // STARTUP_H
