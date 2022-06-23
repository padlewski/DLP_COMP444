#ifndef ACTIONS_H
#define ACTIONS_H

#include <Arduino.h>
#include "timer.h"
#include "MPU9250.h"
#include "ir_line.h"
#include "mecanum.h"

void ActionMoveSequenceDo(void);
void ActionMoveUntilDo(void);
void ActionUpdateIr(void);
void ActionUpdateMpu(void);

struct TimedActionMs doUpdateIr = TMR_buildActionMs("Do IR", 20, &ActionUpdateIr, false);
struct TimedActionMs doUpdateMpu = TMR_buildActionMs("Do Mpu", 20, &ActionUpdateMpu, false);
struct TimedActionMs doMoveSequence = TMR_buildActionMs("Do MOVE_S", 25, &ActionMoveSequenceDo, false);
struct TimedActionMs doMoveUntil = TMR_buildActionMs("Do MOVE_U", 20, &ActionMoveUntilDo, false);

struct ActionMoveSequenceState{
    byte size;
    byte index;
    byte* directions;
    int (*speeds)[4];   // two dimensional array 
    unsigned int* times; // time to run
    unsigned int count;  // how many times the action as been triggered
    TimedActionMs *action = &doMoveSequence;
} actionMoveSequenceState;

void ActionMoveSequenceDo(void) {
    // create a lazy alias
    Serial.println("Setting Movement");
    static struct ActionMoveSequenceState * const _ = &actionMoveSequenceState;
    M_move(M_getDirection(_->directions[_->index]), _->speeds[_->index]);
    doMoveSequence.interval = _->times[_->index];
    //_->action.delta = 0;
    ++_->index;
    if(_->index >= _->size) _->index = 0;
    ++_->count;
}

struct ActionMoveUntilState{
    byte direction;
    int speed[4] = {0,0,0,0};
    unsigned int count;
    bool(*check)(); // The condition which end the action
    void(*run)();   // the callback to run when check returns true
    TimedActionMs *action = &doMoveUntil;
} actionMoveUntilState;

void ActionMoveUntilDo() {
    ++actionMoveUntilState.count;
    M_move(M_getDirection(actionMoveUntilState.direction), actionMoveUntilState.speed);
    if(actionMoveUntilState.check()) actionMoveUntilState.run();
}

uint8_t irInputs[IR_NUM_SENSORS] = {A0,A1,A2,A3,A4};
IrLineSensor lineSensor = IR_init(irInputs);

void ActionUpdateIr(void) {
    IR_update(&lineSensor);
}

void ActionUpdateMpu(void) {
    MPU9250_READ_MAG();
}

#endif // ACTIONS_H