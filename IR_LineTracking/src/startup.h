#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "action_move.h"
#include "ir_line.h"

static const byte STARTUP_STATE_COMPLETE = B00000111;
static const String STARTUP_STATE_NAME = "STARTUP";
static const int STARTUP_SPEEDS[M_N] = {50, 50, 50, 50};

// actions
void SUA_updateIr(void);
void SUA_updateMag(void);
void SUA_checkState(void);
bool SUA_checkLineCallback(void);
void SUA_onLineCallback(void);

struct StartupState {
    IrLineSensor *lineSensor;
    uint8_t lastKnownLinePosition;
    // setup some memory for movement
    byte mSize = 3;
    byte directions[3] = {0,0,0};
    int speeds[3][M_N] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    unsigned int times[3] = {0,0,0};
    byte mode;
    TimedActionMs *current;
} SU_State;

struct TimedActionMs SU_ActionUpdate = TMR_buildActionMs("update sensors", 25, &SUA_updateIr, true);
struct TimedActionMs SU_ActionCheckState = TMR_buildActionMs("check state", 50, &SUA_checkState, true);

// initialize setup and return our action head
TimedActionMs* SU_init(IrLineSensor *ls) {
    SU_State.mode = 0; // Mode 0 start
    SU_State.lineSensor = ls;
    // setup the timed move action
    actionMoveSequenceState.size = SU_State.mSize;
    actionMoveSequenceState.directions =  SU_State.directions;
    actionMoveSequenceState.speeds = SU_State.speeds;
    actionMoveSequenceState.times = SU_State.times;
    // setup the until action (for reorienting the bot)
    actionMoveUntilState.check = &SUA_checkLineCallback;
    actionMoveUntilState.run = &SUA_onLineCallback;
    SU_ActionUpdate.next = &actionMoveSequenceState.action;
    actionMoveSequenceState.action.next = &actionMoveUntilState.action;
    actionMoveUntilState.action.next = &SU_ActionCheckState;
    return &SU_ActionUpdate;
}

// setup and start ir calibration
void SU_setIrCal(void) {
    SU_State.directions[0] = Left;
    SU_State.directions[1] = Right;
    SU_State.directions[2] = Forward;
    memcpy(SU_State.speeds[0], STARTUP_SPEEDS, sizeof(STARTUP_SPEEDS));
    memcpy(SU_State.speeds[1], STARTUP_SPEEDS, sizeof(STARTUP_SPEEDS));
    // leave the last speed as a stop
    SU_State.times[0] = 1500;
    SU_State.times[1] = 2500;
    SU_State.times[2] = 10000;
    SU_State.mode = 1; // Mode 1 calibrate line sensor
    actionMoveSequenceState.action.interval = actionMoveSequenceState.times[0];
    actionMoveSequenceState.action.active = true;
}

// check the state of the ir calibration
void SU_checkIrCal(void) {
    if(actionMoveSequenceState.count < actionMoveSequenceState.size) return;
    ++SU_State.mode; // set the next state (mode 2 re-orient on the line)
    // re-orientate back onto the line
    actionMoveSequenceState.action.active = false; // disable movement
    M_stop();
    if(IR_leftOrRight(&SU_State.lineSensor->status) == IR_IS_CENTERED) { 
        ++SU_State.mode; // Mode 3 On line
        return; // we are centered
    }
    memcpy(actionMoveUntilState.speed, STARTUP_SPEEDS, sizeof(STARTUP_SPEEDS));
    actionMoveUntilState.direction = IR_leftOrRight(&SU_State.lastKnownLinePosition) < IR_IS_CENTERED ? Left : Right;
    actionMoveUntilState.action.active = true;
}

// setup and start Magnatrometer calibration
void SU_setMagCal(void) {
    Serial.println("Setting Mag Calculation");
    SU_State.directions[0] = Clockwise;
    SU_State.directions[1] = Counter;
    SU_State.directions[2] = Forward;
    // Leave speeds as is
    SU_State.times[0] = 12000;
    SU_State.times[1] = 12000;
    actionMoveSequenceState.action.previous = 0;
    actionMoveSequenceState.action.delta = 0;
    actionMoveSequenceState.count = 0;
    actionMoveSequenceState.index = 0;
    actionMoveSequenceState.action.interval = actionMoveSequenceState.times[0];
    ++SU_State.mode; // mode 4
    Serial.println(SU_State.mode);
    actionMoveSequenceState.action.active = true;
}

void SU_checkMagCal(void) {
    if(actionMoveSequenceState.count < actionMoveSequenceState.size) return;
    Serial.println("Reorienting after mag cal");
    ++SU_State.mode; // Mode 5
    actionMoveSequenceState.action.active = false; // disable movement
    M_stop();
    if(IR_isCentered(&SU_State.lineSensor->status)) { 
        ++SU_State.mode; // Mode 6 On line
        return; // we are centered
    }
    actionMoveUntilState.direction = IR_leftOrRight(&SU_State.lastKnownLinePosition) < IR_IS_CENTERED ? Counter : Clockwise;
    actionMoveUntilState.action.active = true;
}

// we have finished the setup 
void SU_setComplete(void) {
    ++SU_State.mode; // Mode 7 all done
    SU_ActionUpdate.active = false;
}

void SUA_checkState(void) {
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

bool SUA_checkLineCallback(void) {
    // Serial.print("Check "); Serial.print(SU_State.lineSensor->status, BIN);
    // Serial.print(" isC "); Serial.println(IR_leftOrRight(&SU_State.lineSensor->status));
    return IR_isCentered(&SU_State.lineSensor->status);
}

void SUA_onLineCallback(void) {
    actionMoveUntilState.action.active = false;
    M_stop();
    ++SU_State.mode;
    Serial.print("Complete online status: ");
    Serial.println(SU_State.mode);
}

void SUA_updateIr(void) {
    uint8_t status = IR_update(SU_State.lineSensor);
    if(IR_STATUS_MASK & status) 
        SU_State.lastKnownLinePosition = status;
    SUA_updateMag();
}

// from https://appelsiini.net/2018/calibrate-magnetometer/
void SUA_updateMag(void) {
  static int min[3] = {32767, 32767, 32767};
  static int max[3] = {-32768, -32768, -32768};
  static float avg[3] = {0,0,0};
  static float avg_delta = 0;
  MPU9250_ReadCalibrateMag();
  for(int i = 0 ; i < 3 ; ++i) {
    if(MPU9250.magnRaw[i] < min[i]) min[i] = MPU9250.magnRaw[i];
    if(MPU9250.magnRaw[i] > max[i]) max[i] = MPU9250.magnRaw[i];
    avg[i] = ((float)(max[i] - min[i]) / 2.0f);
  }
  MPU9250.magn_OffsetErr.X = (int)((min[0] + max[0]) / 2);
  MPU9250.magn_OffsetErr.X = (int)((min[1] + max[1]) / 2);
  MPU9250.magn_OffsetErr.X = (int)((min[2] + max[2]) / 2);
  avg_delta = ((float)(avg[0] + avg[1] + avg[2]) / 3.0f);
  MPU9250.magn_Corr[0] = avg_delta / avg[0];
  MPU9250.magn_Corr[1] = avg_delta / avg[1];
  MPU9250.magn_Corr[2] = avg_delta / avg[2];
}

#endif // STARTUP_H
